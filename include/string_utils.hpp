#ifndef STRING_UTILS_HPP
#define STRING_UTILS_HPP

#include <string>

std::string trim( const std::string& str,
                  const std::string& whitespace = " \t" )
{
  const auto strBegin = str.find_first_not_of( whitespace );
  if( strBegin == std::string::npos )
    return "";

  const auto strEnd   = str.find_last_not_of( whitespace );
  const auto strRange = strEnd - strBegin + 1;

  return str.substr( strBegin, strRange );
}

std::string reduce( const std::string& str,
                    const std::string& fill       = " ",
                    const std::string& whitespace = " \t" )
{
  auto result = trim( str, whitespace );

  auto beginSpace = result.find_first_of( whitespace );
  while( beginSpace != std::string::npos ) {
    const auto endSpace = result.find_first_not_of( whitespace, beginSpace );
    const auto range    = endSpace - beginSpace;

    result.replace( beginSpace, range, fill );

    const auto newStart = beginSpace + fill.length();
    beginSpace          = result.find_first_of( whitespace, newStart );
  }

  return result;
}

#endif