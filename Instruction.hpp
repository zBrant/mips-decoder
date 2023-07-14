#include "Decoder.hpp"

#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

#include <string>
#include <map>
#include <vector>


class Instruction {
    public:
        Instruction(std::string &inputUser);
        std::string to_bin(std::string& var);
        std::map<std::string, std::string> define_fields(std::string& bits, char& type);
        std::string find_name(char& type, std::string& bits);
        std::vector<std::string> find_mnemonic(std::string& name);

        std::string getBits() const;
        int getOpcode() const;
        char getType() const;
        std::map<std::string, std::string> getInstructionFields() const;
        std::string getName() const;
        std::vector<std::string> getMnemonic() const;


    private:
        std::string bits;
        int opcode;
        char type;
        std::map<std::string, std::string> instruction_fields;
        std::string name;
        std::vector<std::string> mnemonic;
};

#endif
