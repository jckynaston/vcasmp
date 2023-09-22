/// @file
/// @author Joshua C. Kynaston
/// @version v0.0.1
///
/// @section LICENSE
///
/// This program is free software; you can redistribute it and/or
/// modify it under the terms of the GNU General Public License as
/// published by the Free Software Foundation; either version 2 of
/// the License, or (at your option) any later version.
///
/// @section DESCRIPTION
///
/// This file contains functions related to input lexing. Most of this is accomplished via
/// the `std::regex` library.

#ifndef LEXER_HPP
#define LEXER_HPP

#include <array>
#include <exception>
#include <regex>
#include <set>
#include <string>

#include "Parser.hpp"

namespace lexer
{
  using namespace std;

  class lexing_error : public exception
  {
  public:
    string message;

    lexing_error( std::string str )
    {
      message = str;
    }

    char* what()
    {
      return message.data();
    }
  };

  /// @brief Determines whether string contains valid 16-bit hex value
  /// @param in String to be tested
  /// @param val Value contained in string
  /// @return Returns `true` on success and updates `val` to contain the found value. On
  /// failure, returns `false` - in this case, the value contained in `val` is not
  /// altered.
  int is_hexval( const string& in, uint16_t& val );

  /// @brief Determines whether string contains valid 16-bit decimal value
  /// @param in String to be tested
  /// @param val Value contained in string
  /// @return Returns `true` on success and updates `val` to contain the found value. On
  /// failure, returns `false` - in this case, the value contained in `val` is not
  /// altered.
  int is_decval( const string& in, uint16_t& val );

  /// @brief Determines whether string contains valid 16-bit binary value
  /// @param in String to be tested
  /// @param val Value contained in string
  /// @return Returns `true` on success and updates `val` to contain the found value. On
  /// failure, returns `false` - in this case, the value contained in `val` is not
  /// altered.
  int is_binval( const string& in, uint16_t& val );

  int is_declaration( const string& in, string& symbol, uint16_t& value );

  int is_instruction( const string& in,
                      string&       label,
                      string&       mnemonic,
                      string&       operand );

  int is_identifier( const string& in );

  int is_value( const string& in, uint16_t& val );

  int is_indexed_operand( const string& raw_op, operand_t& op, addr_mode_id& mode );
};  // namespace lexer

#endif