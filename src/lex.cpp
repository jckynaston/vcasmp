// TODO add boolean arguments to is_* functions that indicate whether any errors within
// can be considered as recoverable; otherwise, throw an exception when encountered

#include "lex.hpp"
#include "Parser.hpp"
#include "optables.hpp"

using namespace std;

// regexes for use in lexing routines
const regex re_hexval( R"(^(?:\$|0x)([a-f0-9]+)$)" );
const regex re_decval( R"(^([1-9][0-9]*)$)" );
const regex re_binval( R"(^(?:\%|0b)([0-1]+)$)" );
const regex re_identf( R"(^[a-z][a-z0-9]+$)" );
const regex re_declar( R"(^(\S*)\s(?:=|equ)\s(\S*)$)" );
const regex re_instrc( R"(^([a-z][a-z0-9]*|)(?::|)\s*([a-z]{3})\s*(\S*)$)" );

// addressing mode identification regexes
const static set< string > mnems_directives = { "org" };

int lexer::is_hexval( const string& in, uint16_t& val )
{
  cmatch cm;
  bool   is_valid = regex_match( in.data(), cm, re_hexval );
  int    _val;

  if( is_valid ) {
    _val = stoi( cm[ 1 ], 0, 16 );
    if( _val > 0xFFFF )
      throw lexer::lexing_error( "integer overflow" );
    val = _val;
  }

  return is_valid;
}

int lexer::is_decval( const string& in, uint16_t& val )
{
  cmatch cm;
  bool   is_valid = regex_match( in.data(), cm, re_decval );
  int    _val;

  if( is_valid ) {
    _val = stoi( cm[ 1 ], 0, 10 );
    if( _val > 0xFFFF )
      throw lexer::lexing_error( "integer overflow" );
    val = _val;
  }

  return is_valid;
}

int lexer::is_binval( const string& in, uint16_t& val )
{
  cmatch cm;
  bool   is_valid = regex_match( in.data(), cm, re_binval );
  int    _val;

  if( is_valid ) {
    _val = stoi( cm[ 1 ], 0, 2 );
    if( _val > 0xFFFF )
      throw lexer::lexing_error( "integer overflow" );
    val = _val;
  }
  return is_valid;
}

int lexer::is_declaration( const string& in, string& symbol, uint16_t& value )
{
  cmatch cm;
  if( regex_match( in.data(), cm, re_declar ) ) {
    symbol = cm[ 1 ];
    if( is_identifier( cm[ 1 ] ) && is_value( cm[ 2 ], value ) ) {
      return true;
    }
  }
  return false;
}

int lexer::is_instruction( const string& in,
                           string&       label,
                           string&       mnemonic,
                           string&       operand )
{
  cmatch cm;
  if( regex_match( in.data(), cm, re_instrc )
      && mnems_instructions.contains( cm[ 2 ] ) ) {
    label    = cm[ 1 ];
    mnemonic = cm[ 2 ];
    operand  = cm[ 3 ];
    return true;
  }
  return false;
}

int lexer::is_identifier( const string& in )
{
  return regex_match( in, re_identf );
}

int lexer::is_value( const string& in, uint16_t& val )
{
  if( is_hexval( in, val ) )
    return true;
  else if( is_decval( in, val ) )
    return true;
  else if( is_binval( in, val ) )
    return true;
  else
    return false;
}

int lexer::is_indexed_operand( const std::string& raw_op,
                               operand_t&         op,
                               addr_mode_id&      mode )
{
  std::regex  re_izy( R"(^\((\S+)\),y$)" );
  std::regex  re_izx( R"(^\((\S+),x\)$)" );
  std::regex  re_x( R"(^(\S+),x$)" );
  std::regex  re_y( R"(^(\S+),y$)" );
  std::cmatch cm;

  bool     is_x_indexed, is_indirect;
  uint16_t _val;

  if( std::regex_match( raw_op.data(), cm, re_izy ) ) {
    mode = addr_izy;
    if( lexer::is_value( cm[ 1 ], _val ) && _val <= 0xFF ) {
      op = _val;
    }
    else if( lexer::is_identifier( cm[ 1 ] ) ) {
      op = cm[ 1 ];
    }
    else {
      // TODO addr_izy error handling
      throw std::runtime_error( "invalid izy instruction" );
    }
    return true;
  }
  else if( std::regex_match( raw_op.data(), cm, re_izx ) ) {
    mode = addr_izx;
    if( lexer::is_value( cm[ 1 ], _val ) && _val <= 0xFF ) {
      op = _val;
    }
    else if( lexer::is_identifier( cm[ 1 ] ) ) {
      op = cm[ 1 ];
    }
    else {
      // TODO addr_izx error handling
      throw std::runtime_error( "invalid izx instruction" );
    }
    return true;
  }
  else if( std::regex_match( raw_op.data(), cm, re_x ) ) {
    if( lexer::is_value( cm[ 1 ], _val ) ) {
      op = _val;
      if( _val <= 0xFF )
        mode = addr_zpx;
      else
        mode = addr_abx;
    }
    else if( lexer::is_identifier( cm[ 1 ] ) ) {
      mode = addr_abx;
      op   = cm[ 1 ];
    }
    else {
      // TODO addr_zpx & addr_abx error handling
      throw std::runtime_error( "invalid x-indexed instruction" );
    }
    return true;
  }
  else if( std::regex_match( raw_op.data(), cm, re_y ) ) {
    if( lexer::is_value( cm[ 1 ], _val ) ) {
      op = _val;
      if( _val <= 0xFF )
        mode = addr_zpy;
      else
        mode = addr_aby;
    }
    else if( lexer::is_identifier( cm[ 1 ] ) ) {
      mode = addr_aby;
      op   = cm[ 1 ];
    }
    else {
      // TODO addr_zpy & addr_aby error handling
      throw std::runtime_error( "invalid y-indexed instruction" );
    }
    return true;
  }
  else {
    return false;
  }
}