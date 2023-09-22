#include <algorithm>
#include <cstring>
#include <format>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>

#include "Parser.hpp"
#include "lex.hpp"
#include "optables.hpp"
#include "string_utils.hpp"

Parser::Parser( const std::string file_name )
{
  read_file( file_name );
  lines.resize( lines_str.size() );
  line_cur_str = lines_str[ 0 ];
}

int Parser::read_file( const std::string file_name )
{
  std::ifstream in_file( file_name );
  for( std::string line; getline( in_file, line ); ) {
    std::transform( line.begin(), line.end(), line.begin(), tolower );
    lines_str.push_back( reduce( line ) );
  }
  return true;
}

int Parser::read_line()
{
  id_current_line++;
  if( id_current_line == lines_str.size() )
    return false;
  else {
    line_cur_str = lines_str[ id_current_line ];
    return true;
  }
}

/// @brief Constructs an AST for the input file
/// @return Returns true on success. On failure, exit program and print error to `STDERR`.
int Parser::pass_first()
{
  while( read_line() ) {
    std::cout << "parsing line: " << line_cur_str << std::endl;
    parse_line( line_cur_str, lines[ id_current_line ] );
  }

  return true;
}

int Parser::parse_line( const std::string in_str, Instruction& inst )
{
  // set line number in AST instruction
  inst.line_no = id_current_line;

  std::string symbol;
  std::string label;
  std::string mnem;
  std::string operand;
  uint16_t    value;
  uint8_t     opcode;

  if( lexer::is_declaration( in_str, symbol, value ) ) {
    symbol_table.insert( { symbol, value } );
  }
  else if( lexer::is_instruction( in_str, label, mnem, operand ) ) {
    // parse_instruction( mnem, operand, opcode, value, symbol);
  }
  return true;
}

int parse_instruction( const std::string& str_mn,
                       const std::string& str_op,
                       uint8_t&           op_code,
                       uint16_t&          op_value,
                       std::string&       op_symbol )
{
  if (str_mn == "lda")
  {

  }
  return true;
}