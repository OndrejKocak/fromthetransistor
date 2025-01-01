#include <iostream>
#include <bits/stdc++.h>
using namespace std;

#define UNDERLINE_ON "\033[4m"
#define UNDERLINE_OFF "\033[24m"
#define PC_INCREMENT 4
#define _START 32768 //0x8000
#define MAX_REG_NUM 15


const map<string, int> COND_MAP = {{"eq", 0},{ "ne", 1}, {"cs", 2}, {"cc", 3}, {"mi", 4}, {"pl", 5}, {"vs", 6}, {"vc", 7}, {"hi", 8}, {"ls", 9}, {"ge", 10}, {"lt", 11}, {"gt", 12}, {"le", 13}, {"al", 14}};
const set<string> OPERATION_SET = {{"and", 0}, {"eor", 1}, {"sub", 2}, {"rsb", 3}, {"add", 4}, {"adc", 5}, {"sbc", 6}, {"rsc", 7}, 
                                   {"tst", 8}, {"teq", 9}, {"cmp", 10}, {"cmn", 11}, {"orr", 12}, {"mov", 13}, {"bic", 14}, {"mvn", 15}};

enum Shift{
    COND = 28,
    IMMEDIATE_OPERAND = 24,
    OP_CODE = 21,
    S = 20,
    RN = 16,
    RD = 12,
    L = 24
};

enum MnemonicType{
    BLOCK,
    BX,
    B,
};

enum OperandType{
    R = 'r', //register
    Expr = '#', //expression
};

class Instruction;

class SyntaxError : public std::exception {
 public:
    SyntaxError(const string& message, const unsigned int &lineNum, const string& line) {
        message_ = "Error: " + message + " on line: " + to_string(lineNum);
        line_ = line;
        lineNum_ = lineNum;
    }
  const char* what() const throw() { return message_.c_str(); }
  const char* line() const throw() { return line_.c_str();}
  const unsigned int lineNum() const throw() { return lineNum_; }

 private:
    string message_;
    string line_;
    unsigned int lineNum_;
};

class Instruction {
    public:
        int address;
        MnemonicType mnemonicType;
        string line;
        unsigned int lineNum;
        string mnemonic;
        vector<string> operands;
        bitset<32> cond;
        bitset<32> binary;
    
        Instruction(string line, unsigned int &lineNum, unsigned int &PC){
            this->line = line;
            this->lineNum = lineNum;
            this->address = PC;
            PC += PC_INCREMENT;
            //mnemonic
            this->mnemonic = strtok(&line[0], " ,");
            //operands
            char* operand = strtok(NULL, " ,");
            while(operand != NULL){
                this->operands.push_back(operand);
                operand = strtok(NULL, " ,");
            }
            this->initMnemonicType(PC);
        }

        bitset<32> getBinary(map<string, int> &blocks){
            if(this->binary == bitset<32>()){
                this->initBinary(blocks);
            }
            return this->binary;
        }
        MnemonicType getType() const{
            return this->mnemonicType;
        }
        string getMnemonic() const{
            return this->mnemonic;
        }
        unsigned int getAddress() const{
            return this->address;
        }
        string getLine() const{
            return this->line;
        }
        unsigned int getLineNum() const{
            return this->lineNum;
        }

    private:
        void initMnemonicType(unsigned int &PC){
            //cout << this->mnemonic.compare(0, 2, "bx") << " | " << line << endl;
            if(this->mnemonic[this->mnemonic.length()-1] == ':' && operands.empty()) {
                this->mnemonicType = MnemonicType::BLOCK;
                PC-=4;
                return;
            } 
            if(this->mnemonic.compare(0, 2, "bx") == 0) {
                if(this->mnemonic.size() != 2 && this->mnemonic.size() != 4) throw SyntaxError("Bad instruction", this->lineNum, this->line);
                this->mnemonicType = MnemonicType::BX;
                this->initCond((this->mnemonic.size() == 2));
                return;
            }
            if(this->mnemonic.compare(0, 1, "b") == 0) {
                this->mnemonicType = MnemonicType::B;
                this->initCond((this->mnemonic.size() <= 2));
                return;
            }
        }

        void initCond(bool empty=false){
            string cond = "al";
            if(!empty) cond = this->mnemonic.substr(this->mnemonic.length()-2,2);
            if(!COND_MAP.count(cond)) throw SyntaxError("Wrong condition", this->lineNum, this->line);
            this->cond |= bitset<32>(COND_MAP.at(cond) << Shift::COND);
            this->mnemonic = this->mnemonic.substr(0, this->mnemonic.length()-2);
        }

        void initBinary(map<string, int> &blocks){
            //cout << this->line << " | " << this->mnemonicType << endl;
            switch (this->mnemonicType)
            {
            case MnemonicType::BX:
                this->binary = branchAndExchange();
                break;
            case MnemonicType::B:
                this->binary = branch(blocks);
                break;
            
            default:
                break;
            }
        }
        
        bitset<32> procesOperand(const string &operand, const OperandType operandType){
            if(operand[0] != operandType) {
                throw SyntaxError("Wrong operand", this->lineNum, this->line);
            }
            if(operandType == OperandType::R){
                int registerNum = stoi(operand.substr(1), nullptr);
                if(registerNum < 0 || registerNum > MAX_REG_NUM) throw SyntaxError("Register number out of range", this->lineNum, this->line);
                bitset<32> val(registerNum);
                return val;
            }
            return {}; 
        }

        bitset<32> branchAndExchange() {
            if(this->operands.size() != 1) throw SyntaxError("Missing operand", this->lineNum, this->line);
            bitset<32> base("00000010001011111111111100010000");
            base |= this->cond;
            base |= procesOperand(this->operands[0], OperandType::R);
            return base;
        }

        bitset<32> calculateBranchOffset(map<string, int> &blocks){
            bitset<32> delta((blocks.at(this->operands[0])-this->address-8)/4);
            bitset<32> offset("11111111111111111111111111");
            offset &= delta;
            return offset;
        }

        bitset<32> branch(map<string, int> &blocks) {
            if(this->operands.size() != 1) throw SyntaxError("Missing operand", this->lineNum, this->line);
            if(!blocks.count(this->operands[0])) throw SyntaxError("Undefined reference", this->lineNum, this->line);
            bitset<32> base("00001010000000000000000000000000");
            
            base |= calculateBranchOffset(blocks);
            base |= this->cond;
            
            if(this->mnemonic[this->mnemonic.size()-1] == 'l'){
                base |= bitset<32>(1 << Shift::L);
            }
            return base;
        }
};


    //1110 00 0 1101 0 0000 1111 00000000 0111 addne r15, r7
    //1110 00 0 0100 1 1111 1111 00000000 0111 ADDS R15, R7
    //1110 00 0 0100 0 1111 1111 00000000 0111 ADD R15, R7
    //1110 00 0 0010 0 0101 0100 00100011 0111 SUB R4,R5,R7,LSR R2
    //0110 00 0 0010 1 0101 0100 00100011 0111

int main(int argc, char* argv[]) {
    if(argc < 2){
        cerr << "File path missing" << endl;
        return -1;
    }
    unsigned int PC = _START;
    map<string, int> blocks = map<string, int>();
    queue<Instruction> instructions = queue<Instruction>();
    string line;
    unsigned int lineNum = 1;
    
    ifstream asmFile(argv[1]);
    if(!asmFile.good()){
        cerr << "Assebler file does not exist" << endl;
        return -1;
    }
    try{
        while(getline(asmFile, line)){
            if(line.empty() || line.find_first_not_of(' ') == std::string::npos) continue;
            std::transform(line.begin(), line.end(), line.begin(), [](unsigned char c){ return std::tolower(c); });
                Instruction instruction = Instruction(line, lineNum, PC);
                if(instruction.getType() == MnemonicType::BLOCK){
                    blocks.insert({instruction.getMnemonic().substr(0, instruction.getMnemonic().size()-1), instruction.getAddress()});
                }
                else{
                    instructions.push(instruction);
                }
            lineNum++;
        }

        while(!instructions.empty()){
            Instruction instruction = instructions.front();
            instructions.pop();
            cout << instruction.getBinary(blocks) << endl;
        }
    }
    catch(const SyntaxError& e){
        cerr << e.what()<< endl;
        cerr << e.lineNum() << " |    " UNDERLINE_ON << e.line() << UNDERLINE_OFF <<endl;
        return -1;
    }
    return 0;
}
