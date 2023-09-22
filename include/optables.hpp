/// @file global utility variables containing information 

#ifndef OPTABLES_HPP
#define OPTABLES_HPP

#include <set>
#include <string>
#include <unordered_map>

#include "AST.hpp"

using optable
  = std::unordered_map< std::string, std::unordered_map< addr_mode_id, uint8_t > >;

extern optable opc6502;

/// @brief set containing instructions that accept `implied` addressing mode
const static std::set< std::string > mode_imp_instructions
  = { "tax", "tay", "tsx", "txa", "txs", "tya", "pha", "php", "pla",
      "plp", "dex", "dey", "inx", "iny", "brk", "rti", "rts", "clc",
      "cld", "cli", "clv", "sec", "sed", "sei", "nop" };

/// @brief set containg full list of valid opcode mnemonics
const static std::set< std::string > mnems_instructions
  = { "adc", "and", "asl", "bcc", "bcs", "beq", "bit", "bmi", "bne", "bpl", "brk",
      "bvc", "bvs", "clc", "cld", "cli", "clv", "cmp", "cpx", "cpy", "dec", "dex",
      "dey", "eor", "inc", "inx", "iny", "jmp", "jsr", "lda", "ldx", "ldy", "lsr",
      "nop", "ora", "pha", "php", "pla", "plp", "rol", "ror", "rti", "sbc", "sed",
      "sei", "sta", "stx", "sty", "tax", "tay", "tsx", "txa", "txs", "tya" };

#endif