#include <iostream>

#include "AST.hpp"
#include "Parser.hpp"

int main( int argc, char** argv )
{
  Parser p( argv[ 1 ] );

  p.pass_first();
}