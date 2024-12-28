#include <iostream>
#include <bits/stdc++.h>
using namespace std;

#define underline_on "\033[4m"
#define underline_off "\033[24m"

//1110 00 0 1101 0 0000 1111 00000000 0111
char testInput[] = "BX R4";//" adDne r15, r7";
//1110 00 0 0100 1 1111 1111 00000000 0111 ADDS R15, R7
//1110 00 0 0100 0 1111 1111 00000000 0111 ADD R15, R7
//1110 00 0 0010 0 0101 0100 00100011 0111 SUB R4,R5,R7,LSR R2
//0110 00 0 0010 1 0101 0100 00100011 0111
const map<string, int> condMap = {{"eq", 0},{ "ne", 1}, {"cs", 2}, {"cc", 3}, {"mi", 4}, {"pl", 5}, {"vs", 6}, {"vc", 7}, {"hi", 8}, {"ls", 9}, {"ge", 10}, {"lt", 11}, {"gt", 12}, {"le", 13}, {"al", 14}};
const set<string> opSet = {"and", "eor", "sub", "rsb", "add", "adc", "sbc", "rsc", "tst", "teq","cmp", "cmn", "orr", "mov", "bic", "mvn"};
enum Shift{
    COND = 28,
    IMMEDIATE_OPERAND = 24,
    OP_CODE = 21,
    S = 20,
    RN = 16,
    RD = 12,
};

enum MnemonicType{
    BX,
    B,
};

enum ParamType{
    R = 'r', //register
    Expr = '#', //expression
};

class Instruction {
    public:
        MnemonicType mnemonicType;
        string mnemonic;
        vector<string> operands;
        bitset<32> cond;
    
        Instruction(char* line, int lineNum){
            char lineCpy[sizeof(line)];
            strcpy(lineCpy, line);
            //mnemonic
            while(this->mnemonic.empty()) this->mnemonic = strtok(lineCpy, " ,");
           
            this->setMnemonicType();
            //operands
            char* operand = strtok(NULL, " ,");
            while(operand != NULL){
                this->operands.push_back(operand);
                operand = strtok(NULL, " ,");
            }

        }

    private:
        void setMnemonicType(){
            if(this->mnemonic.compare(0, 1, "bx")) {
                this->mnemonicType = MnemonicType::BX;
                this->setCond(!(this->mnemonic.size() == 4));
                return;
            }
            if(this->mnemonic.compare(0, 1, "b")) {
                this->mnemonicType = MnemonicType::BX;
                return;
            } 
        }

        void setCond(bool empty=false){
            string cond = "al";
            if(!empty)  string cond = this->mnemonic.substr(this->mnemonic.length()-2,2);
            if(!condMap.count(cond)) throw runtime_error("Wrong condition");
            this->cond |= bitset<32>(condMap.at(cond));
            this->cond <<=Shift::COND;
        }

};

bitset<32> procesOperand(string &operand, ParamType paramType){
    if(operand[0] != paramType){
        throw runtime_error("Wrong operand");
    }
    if(paramType == ParamType::R && operand[0]){
        bitset<32> val(stoi(operand.substr(1), nullptr));
        return val;
    }
    return {};
}

bitset<32> BranchAndExchange(Instruction &instruction) {
    if(instruction.operands.size() != 1) throw runtime_error("Missing operand");
    bitset<32> base("00000010001011111111111100010000");
    base |= instruction.cond;
    auto rn = procesOperand(instruction.operands[0], ParamType::R);
    base |= rn;
    cout << base.to_string() << endl; 
    return base;
}



int main() {
    char* line = testInput;
    std::transform(line, line+sizeof(line), line, [](unsigned char c){ return std::tolower(c); });
    int lineNum = 1;
    try{
        Instruction instruction = Instruction(line, lineNum);
        BranchAndExchange(instruction);
    }
    catch(const exception& e){
        cerr << "Error: " << e.what() << " on line: " << lineNum << endl;
        cerr << lineNum << " |    " underline_on << line << underline_off <<endl;
    }
    
    return 0;
}
