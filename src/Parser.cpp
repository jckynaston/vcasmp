#include <algorithm>
#include <cstring>
#include <format>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>

#include "Parser.hpp"
#include "string_utils.hpp"

Parser::Parser( const std::string file_name )
{
  read_file( file_name );
  lines.resize( lines_str.size() );
  line_cur_str = lines_str[ 0 ];
}

void Parser::throw_error()
{
  err_msg = std::format( "\nAssembler found error on line {}\n\n  {} | {}\n\n",
                         line_cur_id,
                         line_cur_id,
                         line_cur_str )
            + err_msg;
  std::cerr << err_msg << "\n" << std::endl;
  exit( EXIT_FAILURE );
}

int Parser::read_file( const std::string file_name )
{
  std::ifstream in_file( file_name );
  for( std::string line; getline( in_file, line ); ) {
    std::transform( line.begin(), line.end(), line.begin(), tolower );
    lines_str.push_back( trim( line ) );
  }
  return true;
}

int Parser::read_line()
{
  line_cur_id++;
  if( line_cur_id == lines_str.size() )
    return false;
  else {
    line_cur_str = lines_str[ line_cur_id ];
    return true;
  }
}


/// @brief Constructs an AST for the input file
/// @return Returns true on success. On failure, exit program and print error to `STDERR`.
int Parser::pass_first()
{
  while( read_line() ) {
    parse_instruction( line_cur_str, lines[ line_cur_id ] );
  }

  for( auto& inst : lines ) {
    if( inst.line_type == type_dec ) {
      std::string symbol = inst.op1.as_string();
      uint16_t    value  = inst.op2.as_int();
      auto        loc    = symbol_table.find( symbol );
      if( loc != symbol_table.end() ) {
        std::cerr << std::format(
          "identifier \x1B[31m{}\033[0m on line {} already defined", symbol, inst.line_no )
                  << std::endl;
        exit( EXIT_FAILURE );
      }
      else {
        symbol_table.insert( { inst.op1.as_string(), inst.op2.as_int() } );
      }
    };
  }
  return true;
}

int Parser::parse_instruction( const std::string in_str, Instruction& inst )
{
  // set line number in AST instruction
  inst.line_no = line_cur_id;

  // parse anything containing `=` or `equ` as declaration
  if( ( in_str.find( "=" ) != std::string::npos )
      || ( in_str.find( "equ" ) != std::string::npos ) )
    parse_declaration( in_str, inst );
  return true;
}

int Parser::parse_declaration( const std::string in_str, Instruction& inst )
{
  std::regex  decl( R"(^([\S+][\s\S]*[\S+])\s*(?:=|equ)\s*([\S]+[\s\S]*[\S]+)$)" );
  std::cmatch cm;
  if( std::regex_match( in_str.data(), cm, decl ) ) {
    std::string _identifier = cm[ 1 ];
    std::string _value      = cm[ 2 ];
    operand_t   op_identifier, op_value;
    if( !parse_symbol( _identifier, op_identifier ) | !parse_value( _value, op_value ) )
      goto error;
    else {
      inst.line_no   = line_cur_id;
      inst.line_type = type_dec;
      inst.mnemonic  = "equ";
      inst.op1       = op_identifier;
      inst.op2       = op_value;
    }
  }
  else {
    goto error;
  }

  if( FLAG_VERBOSE ) {
    std::cout << "Line: " << line_cur_id << " | Found symbol [" << inst.op1
              << "] with value [0x" << inst.op2 << "]" << std::endl;
  }

  return true;

error:
  err_msg = "Invalid declaration: " + err_msg;
  throw_error();
  return false;
}

int Parser::parse_symbol( const std::string in_str, operand_t& op )
{
  // check for valid alphanumeric identifier
  std::regex sym( R"(^[a-z][a-z0-9]*$)" );
  if( !std::regex_match( in_str.data(), sym ) ) {
    err_msg += "Bad identifier: " + in_str;
    return false;
  }
  else {
    op = in_str;
    return true;
  }
}

int Parser::parse_value( const std::string in_str, operand_t& op )
{
  // regexes to check for valid numeric input
  std::regex rehex( R"(^(?:\$|0x)[a-f0-9]+$)" );
  std::regex redec( R"(^[0-9]+$)" );
  std::regex rebin( R"(^(?:\%|0b)[0-1]+$)" );
  bool       ishex = std::regex_match( in_str.data(), rehex );
  bool       isdec = std::regex_match( in_str.data(), redec );
  bool       isbin = std::regex_match( in_str.data(), rebin );

  // get first two chars of input
  std::string head = in_str.substr( 0, 2 );

  // allocs
  int _val;

  // throw error if string is not a valid numeric value
  if( !ishex && !isdec && !isbin ) {
    err_msg += "Invalid numeric value: ";
    goto error;
  }

  // read head to determine base and parse value, throwing error if parsed value overflows
  // a uint16_t
  if( head[ 0 ] == '$' )
    if( ( _val = std::stoi( in_str.substr( 1 ), 0, 16 ) ) <= 0xFFFF )
      op = (uint16_t)_val;
    else {
      err_msg += "Integer overflow: ";
      goto error;
    }
  else if( head == "0x" )
    op = (uint16_t)std::stoi( in_str.substr( 2 ), 0, 16 );
  else if( head[ 0 ] == '%' )
    op = (uint16_t)std::stoi( in_str.substr( 1 ), 0, 2 );
  else if( head == "0b" )
    op = (uint16_t)std::stoi( in_str.substr( 2 ), 0, 2 );
  else
    op = (uint16_t)std::stoi( in_str, 0, 10 );

  return true;

error:
  err_msg += in_str;
  return false;
}
