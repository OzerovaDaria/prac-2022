//!  File parsing.
/*!
  Functions for editing a line read from a file are declared here.
*/
#ifndef SALNIKOV_TASK_PARSING_H
#define SALNIKOV_TASK_PARSING_H

#include <string>

using num_pos = std::pair<unsigned long long, std::string::size_type>;

std::string no_comment(const std::string& comment);
std::string only_one_space(std::string many_spaces);
std::string string_strip(std::string last_space);
std::string clean_string(const std::string& not_cleaned);
num_pos extract_nat_num(const std::string& num_string);


#endif //SALNIKOV_TASK_PARSING_H
