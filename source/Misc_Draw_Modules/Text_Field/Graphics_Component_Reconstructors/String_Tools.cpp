#include <Misc_Draw_Modules/Text_Field/Graphics_Component_Reconstructors/String_Tools.h>

using namespace LMD;

constexpr char new_line_symbol = '\n';
constexpr char fucking_extra_windows_new_line_symbol = '\r';


unsigned int LMD::offset_if_new_line(const std::string& _text, unsigned int _symbol_index)
{
    if(_text[_symbol_index] == new_line_symbol)
        return 1;

    if(_text[_symbol_index] == fucking_extra_windows_new_line_symbol)
    {
        if(_text.size() - 1 == _symbol_index)
            return 0;

        if(_text[_symbol_index + 1] == new_line_symbol)
            return 2;
    }

    return 0;
}

unsigned int LMD::calculate_lines_amount(const std::string& _text)
{
    unsigned int result = 1;

    for(unsigned int i=0; i<_text.size(); ++i)
    {
        unsigned int offset_if_newline = offset_if_new_line(_text, i);

        if(offset_if_newline == 0)
            continue;

        ++result;
        i += offset_if_newline - 1;
    }

    return result;
}
