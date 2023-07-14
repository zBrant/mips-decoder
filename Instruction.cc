#include "Instruction.hpp"

#include <bitset>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>

using namespace std;

Instruction::Instruction(string &inputUser){
    this->bits = to_bin(inputUser);
    this->opcode = stoi(bits.substr(0, 6), nullptr, 2);
    this->type = (opcode == 0) ? 'R' : (opcode == 10 || opcode == 11) ? 'J' : 'I'; 
    this->instruction_fields = define_fields(bits, type);
    this->name = find_name(type, bits);
    this->mnemonic = find_mnemonic(name);
}


/* transforma a entrada do usuario em binario de 32 bits e a retorna no formato de string */
string Instruction::to_bin(string &var)
{
    
    stringstream ss;
    unsigned n;

    if (var.substr(0, 2) == "0x")
    {
        ss << hex << var;
    }
    else
    {
        n = stoi(var, nullptr, 10);
        ss << dec << var;
    }

    ss >> n;
    bitset<32> b(n);
    return b.to_string();
}


/* define os campos dado o tipo da instrucao */
map<string, string> Instruction::define_fields(string &bits, char &type){

    map<string, string> fields;

    if(type == 'J'){
        fields.insert({"label", to_string(stoi(bits.substr(6, 26),nullptr, 2))});
    }else{
        fields.insert({"rs", to_string(stoi(bits.substr(6, 5), nullptr, 2))});
        fields.insert({"rt", to_string(stoi(bits.substr(11, 5), nullptr, 2))});

        if(type == 'R'){
            fields.insert({"rd"  , to_string(stoi(bits.substr(16, 5), nullptr, 2))});
            fields.insert({"shamt", to_string(stoi(bits.substr(21, 5), nullptr, 2))});
        }else{
            fields.insert({"IMM", to_string(stoi(bits.substr(16, 16), nullptr, 2))});
        }
    }

    return fields;
}


/*  
    retorna o nome da instrucao dependendo do tipo tera que escolher o campo 
    funct ou opcode para buscar no map correspondente
*/
string Instruction::find_name(char &type, string &bits){
    Decoder decode;
   
    if(type == 'R' || type == 'J'){
        int funct = stoi(bits.substr(26, 5), nullptr, 2);
        return (decode.FUNCTIONS.find(funct) != decode.FUNCTIONS.end()) ? decode.FUNCTIONS[funct] : " ";
    }else{ 
        return (decode.OPCODES.find(getOpcode()) != decode.OPCODES.end()) ? decode.OPCODES[getOpcode()] : " ";
    }
}


vector<string> Instruction::find_mnemonic(string &name){
    Decoder decode;
    return decode.FIELDS[name];
}


string Instruction::getBits() const {
    return bits;
}


int Instruction::getOpcode() const {
    return opcode;
}


char Instruction::getType() const {
    return type;
}


std::map<std::string, std::string> Instruction::getInstructionFields() const {
    return instruction_fields;
}


std::string Instruction::getName() const {
    return name;
}


std::vector<std::string> Instruction::getMnemonic() const {
    return mnemonic;
}



