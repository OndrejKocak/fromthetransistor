#include <iostream>
#include <bits/stdc++.h>
using namespace std;

#define UNDERLINE_ON "\033[4m"
#define UNDERLINE_OFF "\033[24m"
#define PC_INCREASE 4
#define MAX_REG_NUM 15

const map<string, int> COND_MAP = {{"eq", 0},{ "ne", 1}, {"cs", 2}, {"cc", 3}, {"mi", 4}, {"pl", 5}, {"vs", 6}, {"vc", 7}, {"hi", 8}, {"ls", 9}, {"ge", 10}, {"lt", 11}, {"gt", 12}, {"le", 13}, {"al", 14}};
const set<string> OPERATION_SET = {{"and", 0}, {"eor", 1}, {"sub", 2}, {"rsb", 3}, {"add", 4}, {"adc", 5}, {"sbc", 6}, {"rsc", 7}, 
                                   {"tst", 8}, {"teq", 9}, {"cmp", 10}, {"cmn", 11}, {"orr", 12}, {"mov", 13}, {"bic", 14}, {"mvn", 15}};
unsigned int PC = 32768; //0x8000

enum Shift{
    COND = 28,
    IMMEDIATE_OPERAND = 24,
    OP_CODE = 21,
    S = 20,
    RN = 16,
    RD = 12,
};

enum MnemonicType{
    BLOCK,
    BX,
    B,
};

enum ParamType{
    R = 'r', //register
    Expr = '#', //expression
};

class Instruction {
    public:
        unsigned int address;
        MnemonicType mnemonicType;
        string mnemonic;
        vector<string> operands;
        bitset<32> cond;
        bitset<32> binary;
    
        Instruction(char* line, int lineNum){
            this->address = PC;
            PC += PC_INCREASE;
            char lineCpy[sizeof(line)];
            strcpy(lineCpy, line);

            //mnemonic
            while(this->mnemonic.empty()) this->mnemonic = strtok(lineCpy, " ,");
            //operands
            char* operand = strtok(NULL, " ,");
            while(operand != NULL){
                this->operands.push_back(operand);
                operand = strtok(NULL, " ,");
            }
            this->initMnemonicType();
            this->initBinary();
        }

        bitset<32> getBinary(){
            return this->binary;
        }

    private:
        void initMnemonicType(){
            if(this->mnemonic[this->mnemonic.length()-1] == ':' && operands.empty()) {
                this->mnemonicType = MnemonicType::BLOCK;
                PC-=4;
                return;
            } 
            if(this->mnemonic.compare(0, 1, "bx")) {
                this->mnemonicType = MnemonicType::BX;
                this->initCond(!(this->mnemonic.size() == 4));
                return;
            }
            if(this->mnemonic.compare(0, 1, "b")) {
                this->mnemonicType = MnemonicType::BX;
                return;
            } 
        }

        void initCond(bool empty=false){
            string cond = "al";
            if(!empty)  string cond = this->mnemonic.substr(this->mnemonic.length()-2,2);
            if(!COND_MAP.count(cond)) throw runtime_error("Wrong condition");
            this->cond |= bitset<32>(COND_MAP.at(cond));
            this->cond <<=Shift::COND;
        }

        void initBinary(){
            switch (this->mnemonicType)
            {
            case MnemonicType::BX:
                this->binary = BranchAndExchange();
                break;
            
            default:
                break;
            }
        }
        
        bitset<32> procesOperand(string &operand, ParamType paramType){
            if(operand[0] != paramType) throw runtime_error("Wrong operand");
            if(paramType == ParamType::R){
                int registerNum = stoi(operand.substr(1), nullptr);
                if(registerNum < 0 || registerNum > MAX_REG_NUM) throw runtime_error ("Register number out of range");
                bitset<32> val(registerNum);
                return val;
            }
            return {};
        }

        bitset<32> BranchAndExchange() {
            if(this->operands.size() != 1) throw runtime_error("Missing operand");
            bitset<32> base("00000010001011111111111100010000");
            base |= this->cond;
            base |= procesOperand(this->operands[0], ParamType::R);
            return base;
        }
};







int main() {
    //1110 00 0 1101 0 0000 1111 00000000 0111 addne r15, r7
    //1110 00 0 0100 1 1111 1111 00000000 0111 ADDS R15, R7
    //1110 00 0 0100 0 1111 1111 00000000 0111 ADD R15, R7
    //1110 00 0 0010 0 0101 0100 00100011 0111 SUB R4,R5,R7,LSR R2
    //0110 00 0 0010 1 0101 0100 00100011 0111
    char line[] = "BX R17";
    std::transform(line, line+sizeof(line), line, [](unsigned char c){ return std::tolower(c); });
    int lineNum = 1;
    try{
        Instruction instruction = Instruction(line, lineNum);
        cout << instruction.getBinary() << endl;
    }
    catch(const exception& e){
        cerr << "Error: " << e.what() << " on line: " << lineNum << endl;
        cerr << lineNum << " |    " UNDERLINE_ON << line << UNDERLINE_OFF <<endl;
    }
    
    return 0;
}
