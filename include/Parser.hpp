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
  size_t                                      line_cur_id = -1;
  std::string                                 line_cur_str;
  std::unordered_map< std::string, uint16_t > symbol_table;
  std::string                                 err_msg    = "";
  bool                                        is_verbose = true;

  Parser() = default;
  Parser( const std::string file_name );

  void throw_error();
  int  read_file( const std::string file_name );
  int  read_line();
  int  pass_first();
  int  parse_line( const std::string in_str, Instruction& inst );
  int  parse_label( const std::string in_str, Instruction& inst );
  int  parse_directive( const std::string in_str, Instruction& inst );
  int  parse_declaration( const std::string in_str, Instruction& inst );
  int  parse_operand( const std::string in_str, operand_t& op );
  int  parse_symbol( const std::string in_str, operand_t& op );
  int  parse_value( const std::string in_str, operand_t& op );
};

#endif