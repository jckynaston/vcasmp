#ifndef PARSER6502_HPP
#define PARSER6502_HPP

#define LEN_MAX_ID 256

#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "AST.hpp"

class Parser
{
public:
  std::vector< Instruction >                  lines;
  std::vector< std::string >                  lines_str;
  uint16_t                                    id_current_line = -1;
  uint16_t                                    id_current_addr = 0x00;
  std::string                                 line_cur_str;
  std::unordered_map< std::string, uint16_t > symbol_table;
  std::string                                 err_msg    = "";
  bool                                        is_verbose = true;

  Parser() = default;
  Parser( const std::string file_name );

  int  read_file( const std::string file_name );
  int  read_line();
  int  pass_first();
  int  parse_line( const std::string in_str, Instruction& inst );
};

#endif