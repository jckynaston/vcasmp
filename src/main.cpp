#include <iostream>

#include "AST.hpp"
#include "Parser.hpp"
#include "lex.hpp"

int main( int argc, char** argv )
{
  // Parser p( argv[ 1 ] );
  // p.pass_first();

  // for (auto& [sym, val]: p.symbol_table)
  // {
  //   std::cout <<  "found " << sym << " with value " << std::hex << val << std::endl;
  // }

  std::string  test_str = argv[ 1 ];
  operand_t    op;
  addr_mode_id mode;

  std::cout << lexer::is_indexed_operand( test_str, op, mode ) << std::endl;
  std::cout << "value [" << op << "] with mode [" << mode << "]\n";
}