#ifndef AST_HPP
#define AST_HPP

#include <cstdint>
#include <iostream>
#include <string>
#include <variant>

class operand_t
{
protected:
  std::variant< uint16_t, std::string > data;

public:
  std::string as_string() const
  {
    return std::get< std::string >( data );
  }
  uint16_t as_int() const
  {
    return std::get< uint16_t >( data );
  }
  void operator=( std::string in )
  {
    data = in;
  }
  void operator=( uint16_t in )
  {
    data = in;
  }

  friend std::ostream& operator<<( std::ostream& os, const operand_t& op );
};

inline std::ostream& operator<<( std::ostream& os, const operand_t& op )
{
  if( op.data.index() == 0 )
    os << std::hex << op.as_int();
  else
    os << op.as_string();
  return os;
}

enum addr_mode_id
{
  addr_imp = 0,
  addr_imm = 1,
  addr_zpg = 2,
  addr_zpx = 3,
  addr_zpy = 4,
  addr_izx = 5,
  addr_izy = 6,
  addr_abs = 7,
  addr_abx = 8,
  addr_aby = 9,
  addr_ind = 10,
  addr_rel = 11
};

enum line_type_t
{
  type_null = 0,
  type_dec,
  type_ins,
  type_dir,
  type_lab
};

struct Instruction
{
  uint16_t     line_no;
  line_type_t  line_type = type_null;
  std::string  label;
  std::string  mnemonic;
  addr_mode_id addr_mode;
  operand_t    op1, op2;
};

#endif