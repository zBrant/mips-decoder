#ifndef DECODER_HPP
#define DECODER_HPP

#include <map>

#include "Instruction.hpp"

class Decoder {
    public:
        std::map<std::string, std::vector<std::string>> FIELDS;
        std::map<int, std::string> REGISTERS;
        std::map<int, std::string> OPCODES;
        std::map<int, std::string> FUNCTIONS;

        Decoder();

        std::map<std::string, int> define_signs(Instruction &instruction);
        Instruction parse_instruction(std::string &input_user);
};

#endif
