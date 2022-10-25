//!  File parsing.
/*!
  Functions for editing a line read from a file are implemented here.
*/

#include <iostream>
#include "parsing.h"

//! function to delete comments.
/*!
  \param comment line read from file.
*/
std::string no_comment(const std::string& comment){
    return comment.substr(0, (comment.find('#')));
}

//! function to delete double spaces.
/*!
  \param many_spaces line read from file.
*/
std::string only_one_space(std::string many_spaces)
{
    std::string one_space = std::move(many_spaces);

    auto two_spaces = [](unsigned char const a)
    {
        static bool pred_space = false;
        bool cur_space = std::isspace(a);
        auto ret_value = cur_space && pred_space;
        pred_space = cur_space;
        return ret_value;
    };

    one_space.erase(std::remove_if(one_space.begin(),
                                   one_space.end(),
                                   two_spaces), one_space.end());

    return one_space;
}

//! function to delete extra spaces.
/*!
  \param last_space line read from file.
*/
std::string string_strip(std::string last_space)
{
    auto non_space_char = std::find_if_not(last_space.begin(), last_space.end(),
                                           [](unsigned char const a) {return std::isspace(a);}) - last_space.begin();
    std::string no_space = last_space.substr(non_space_char);
    no_space.erase(std::remove_if(std::prev(no_space.end()),
                                  no_space.end(),
                                  [](unsigned char const a) {return std::isspace(a);}),
                   no_space.end());
    return no_space;
}

//! function to clean string from extra spaces and comments.
/*!
  \param not_cleaned line read from file.
*/
std::string clean_string(const std::string& not_cleaned)
{
    return string_strip(only_one_space(no_comment(not_cleaned)));
}

//! function to get natural number from a string.
/*!
  \param num_string line read from file.
*/
num_pos extract_nat_num(const std::string& num_string)
{
    std::string::size_type pos = num_string.find_first_not_of("0123456789");
    auto first_part = num_string.substr(0, pos), second_part = num_string.substr(pos+1);
    return num_pos(std::stoull(first_part), pos);
}

