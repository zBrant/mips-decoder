#include "Decoder.hpp"

#include <string>
#include <map>
#include <vector>
#include <algorithm>

using namespace std;

Decoder::Decoder(){

  /* campos usados na hora de imprimir a instrucao */
  FIELDS = {
            {"addi" , {"rt", "rs", "IMM"}},
            {"addiu", {"rt", "rs", "IMM"}},
            {"andi" , {"rt", "rs", "IMM"}},
            {"beq"  , {"rs", "rt", "label"}},
            {"bgez" , {"rs", "label"}},
            {"bgtz" , {"rs", "label"}},
            {"blez" , {"rs", "label"}},
            {"bltz" , {"rs", "label"}},
            {"bne"  , {"rs", "rt", "label"}},
            {"lb"   , {"rt", "IMM","rs"}},
            {"lbu"  , {"rt", "IMM","rs"}},
            {"lh"   , {"rt", "IMM","rs"}},
            {"lhu"  , {"rt", "IMM","rs"}},
            {"lui"  , {"rt", "IMM"}},
            {"lw"   , {"rt", "IMM","rs"}},
            {"lwc1" , {"rt", "IMM","rs"}},
            {"ori"  , {"rt", "rs", "IMM"}},
            {"sb"   , {"rt", "IMM","rs"}},
            {"slti" , {"rt", "rs", "IMM"}},
            {"sltiu", {"rt", "rs", "IMM"}},
            {"sh"   , {"rt", "IMM","rs"}},
            {"sw"   , {"rt", "IMM","rs"}},
            {"swc1" , {"rt", "IMM","rs"}},
            {"xori" , {"rt", "rs", "IMM"}},
            {"add"  , {"rd", "rs", "rt"}},
            {"addu" , {"rd", "rs", "rt"}},
            {"and"  , {"rd", "rs", "rt"}},
            {"break", {}},
            {"div"  , {"rs", "rt"}},
            {"divu" , {"rs", "rt"}},
            {"jalr" , {"rd", "rs"}},
            {"jr"   , {"rs"}},
            {"mfhi" , {"rd"}},
            {"mflo" , {"rd"}},
            {"mthi" , {"rs"}},
            {"mtlo" , {"rs"}},
            {"mult" , {"rs", "rt"}},
            {"multu", {"rs", "rt"}},
            {"nor"  , {"rd", "rs", "rt"}},
            {"or"   , {"rd", "rs", "rt"}},
            {"sll"  , {"rd", "rt", "sa"}},
            {"sllv" , {"rd", "rt", "rs"}},
            {"slt"  , {"rd", "rs", "rt"}},
            {"sltu" , {"rd", "rs", "rt"}},
            {"sra"  , {"rd", "rt", "sa"}},
            {"srav" , {"rd", "rt", "rs"}},
            {"srl"  , {"rd", "rt", "sa"}},
            {"srlv" , {"rd", "rt", "rs"}},
            {"sub"  , {"rd", "rs", "rt"}},
            {"subu" , {"rd", "rs", "rt"}},
            {"syscall", {}},
            {"xor"  , {"rd", "rs", "rt"}},
            {"j"    , {"label"}},
            {"jal"  , {"label"}}
          };

  REGISTERS = {
                {0, "$zero"},
                {1, "$at"},
                {2, "$v0"},
                {3, "$v1"},
                {4, "$a0"},
                {5, "$a1"},
                {6, "$a2"},
                {7, "$a3"},
                {8, "$t0"},
                {9, "$t1"},
                {10, "$t2"},
                {11, "$t3"},
                {12, "$t4"},
                {13, "$t5"},
                {14, "$t6"},
                {15, "$t7"},
                {16, "$s0"},
                {17, "$s1"},
                {18, "$s2"},
                {19, "$s3"},
                {20, "$s4"},
                {21, "$s5"},
                {22, "$s6"},
                {23, "$s7"},
                {24, "$t8"},
                {25, "$t9"},
                {26, "$k0"},
                {27, "$k1"},
                {28, "$gp"},
                {29, "$sp"},
                {30, "$s8"},
                {31, "$ra"}
             };

  OPCODES = {
              /*Tipo I*/
              {0b000000, "[R-Type]"},
              {0b001000, "addi"},
              {0b001001, "addiu"},
              {0b001100, "andi"},
              {0b000100, "beq"},
              {0b000001, "bgez"},
              {0b000111, "bgtz"},
              {0b000110, "blez"},
              {0b000101, "bltz"},
              {0b000101, "bne"},
              {0b100000, "lb"},
              {0b100100, "lbu"},
              {0b100001, "lh"},
              {0b100101, "lhu"},
              {0b001111, "lui"},
              {0b100011, "lw"},
              {0b110001, "lwc1"},
              {0b001101, "ori"},
              {0b101000, "sb"},
              {0b001010, "slti"},
              {0b001011, "sltiu"},
              {0b101001, "sh"},
              {0b101011, "sw"},
              {0b111001, "swc1"},
              {0b001110, "xori"}
            };

  FUNCTIONS = {
                /* Tipo R */
                {0b100000, "add"},
                {0b100001, "addu"},
                {0b100100, "and"},
                {0b001101, "break"},
                {0b011010, "div"},
                {0b011011, "divu"},
                {0b001001, "jalr"},
                {0b001000, "jr"},
                {0b010000, "mfhi"},
                {0b010010, "mflo"},
                {0b010001, "mthi"},
                {0b010011, "mtlo"},
                {0b011000, "mult"},
                {0b011001, "multu"},
                {0b100111, "nor"},
                {0b100101, "or"},
                {0b000000, "sll"},
                {0b000100, "sllv"},
                {0b101010, "slt"},
                {0b101011, "sltu"},
                {0b000011, "sra"},
                {0b000111, "srav"},
                {0b000010, "srl"},
                {0b000110, "srlv"},
                {0b100010, "sub"},
                {0b100011, "subu"},
                {0b001100, "syscall"},
                {0b100110, "xor"},
                /* Tipo J */
                {0b000010, "j"},
                {0b000011, "jal"}
              };
};


map<string, int> Decoder::define_signs(Instruction &instruction){

  /* instrucoes que usam determinados sinal de controle */
  vector<string> memRead = {"lw", "lh", "lb", "lbu"};
  vector<string> memWrite = {"sw", "sh", "sb"};
  vector<string> branch = {"beq", "bne", "bltzal", "bltz", "blez", "bgtz", "bgezal", "bgez"};

  return map<string, int> {
      {"RegDst"  , (instruction.getType() == 'R') ? 1 : 0},
      {"Jump"    , (instruction.getType() == 'J') ? 1 : 0},
      {"Branch"  , (find(branch.begin(), branch.end(), instruction.getName()) != branch.end()) ? 1 : 0},
      {"MemRead" , (find(memRead.begin(), memRead.end(), instruction.getName()) != memRead.end()) ? 1 : 0},
      {"MemtoReg", (find(memRead.begin(), memRead.end(), instruction.getName()) != memRead.end()) ? 1 : 0},
      {"MemWrite", (find(memWrite.begin(), memWrite.end(), instruction.getName()) != memWrite.end()) ? 1 : 0},
      {"ALUSrc"  , (instruction.getType() == 'I') ? 1 : 0},
      {"RegWrite", (find(memWrite.begin(), memWrite.end(), instruction.getName()) != memWrite.end() ||
                    find(branch.begin(), branch.end(), instruction.getName()) != branch.end() || instruction.getType() == 'J') ? 0 : 1}
  };
}


Instruction Decoder::parse_instruction(string &input_user){
  Instruction instruc(input_user);
  return instruc;
}
