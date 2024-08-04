#pragma once

#include <string>


namespace LMD
{

    unsigned int offset_if_new_line(const std::string& _text, unsigned int _symbol_index);

    unsigned int calculate_lines_amount(const std::string& _text);

}
