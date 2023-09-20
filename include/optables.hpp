#ifndef OPTABLES_HPP
#define OPTABLES_HPP

#include <string>
#include <unordered_map>

#include "AST.hpp"

using optable
  = std::unordered_map< std::string,
                        std::unordered_map< addr_mode_id, uint8_t > >;

extern optable opc6502;

#endif