#include <iostream>
#include <cstring>

#include "Decoder.hpp"
#include "Instruction.hpp"

using namespace std;

void print_formated(Instruction &instruct, map<string, int> signs){
    cout << "\nInstrução: " << instruct.getName() << " ";

    Decoder reg;
    
    for (const auto& pair : instruct.getMnemonic())
    {

        if(pair != "IMM"){
            if(stoi(instruct.getInstructionFields()[pair]) == 0) break;
            cout << reg.REGISTERS[stoi(instruct.getInstructionFields()[pair])] << " ";
        }else{
            cout << instruct.getInstructionFields()[pair] << " ";
        }
    }
    

    cout << "\n- Tipo: " << instruct.getType() << endl;
    cout << "- Operandos: " << endl;
    
    for (const auto& pair : instruct.getInstructionFields())
    {
        if(pair.first != "IMM"){
            cout << "\t" << pair.first << " : " << reg.REGISTERS[stoi(pair.second)] << endl;
        }else{
            cout << "\t" << pair.first << " : " << pair.second << endl;
        }
    }
    

    cout << "- Sinais de controle: " << endl;
    for (const auto& pair: signs) {
        cout << '\t' << pair.first << " : " << pair.second << endl;
    }
}


int main()
{
    Decoder decoder;
    string input;

    cout << "\n---------| BEM VINDO AO MIPS DECODER |---------\n";
    cout << "\n[DIGITE exit PARA SAIR]  \n\n";

    while (1)
    {
        cout << "\nDIGITE O VALOR DA INSTRUÇÃO (EM DECIMAL OU HEXADECIMAL) : ";
        cin >> input;

        if(input == "exit") break;   
        
        Instruction instruction = decoder.parse_instruction(input);
        map<string, int> signs = decoder.define_signs(instruction);
        print_formated(instruction, signs);
    }

    return 0;
}