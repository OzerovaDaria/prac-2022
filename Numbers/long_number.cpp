#include "long_number.h"
#include "../parsing/parsing.h"


// convert string to number with base
nat_num string_to_num(const std::string& num)
{
    return strtoull(num.c_str(), nullptr, long_number::get_base());
}

// getting subpart of number
nat_num long_number::get_subpart(nat_num cur_pos) const {

    // part len is number of digits to cut
    nat_num part_len = cur_pos > BASE_NUM ? BASE_NUM : cur_pos;

    // convert subpart to number
    return string_to_num(number.substr(cur_pos >= BASE_NUM ? cur_pos - BASE_NUM : 0, part_len));
}

numConvert long_number::convert_number(nat_num number, nat_num neededZeros = BASE_NUM) {

    // num_add is number to transfer to next rank
    nat_num num_add = number/BASE_VAL;

    // number is result digits in number
    number %= BASE_VAL;

    // added_num is a string to concatenate with result string
    std::string added_num = std::to_string(number);

    // zero_num is a number of possible zeros to add if rank not full
    auto zero_num = neededZeros >= BASE_NUM ? BASE_NUM : neededZeros;

    // zeros_added is true number of added zeros
    nat_num zeros_added = zero_num >= added_num.length() ? zero_num - added_num.length() : BASE_NUM - added_num.length();

    // forming a res string with needed zeros to form a full rank
    std::string res_string = std::string(zeros_added, '0') + std::to_string(number);

    // returning a result string and number to transfer
    return numConvert(res_string, num_add);
}

std::string long_number::remove_zeros(std::string str_zeros) {
    // remove all unnecessary zeros
    nat_num zero_pos = 0;
    auto zero_check = str_zeros.begin();
    for (; (*zero_check == '0') && (zero_check != str_zeros.end()); zero_check++, zero_pos++);
    auto res_string = str_zeros.substr(zero_pos, str_zeros.length());
    return res_string.length() > 0 ? res_string : "0";
}


long_number long_number::operator+(const long_number &num) const {

    // first term is always bigger than the second

    if (num > *this)
        return num + *this;
    std::string res_string;

    nat_num num_add = 0, first_cur_pos = get_length(), second_cur_pos = num.get_length();

    // if only one of the operands is negative, use the subtraction
    if (is_negative ^ num.is_negative)
        return is_negative ? num - *this : *this - (-num);

    // cycle for both of numbers from right to left
    for (; (first_cur_pos > 0) && (second_cur_pos > 0);
           first_cur_pos = get_next_pos(first_cur_pos),
           second_cur_pos = get_next_pos(second_cur_pos))
    {
        nat_num first_part = get_subpart(first_cur_pos);
        nat_num second_part = num.get_subpart(second_cur_pos);

        // summing subpart of number
        nat_num new_sum = first_part + second_part + num_add;

        // forming a full rank
        auto res = convert_number(new_sum, first_cur_pos);

        // adding a rank to result string
        res_string = res.first.append(res_string);
        num_add = res.second;
    }

    // condition for forming a long number if second number has ended
    if ((first_cur_pos != 0) || num_add != 0)
    {
        while ((num_add != 0) && (first_cur_pos != 0)) {

            nat_num sub_part = get_subpart(first_cur_pos);
            auto sub_res = convert_number(sub_part+num_add, first_cur_pos);

            res_string = sub_res.first.append(res_string);
            num_add = sub_res.second;

            first_cur_pos = get_next_pos(first_cur_pos);
        }
        // forming a final res string!!!
        res_string = (num_add != 0 ? std::to_string(num_add) : number.substr(0, first_cur_pos)).append(res_string);
    }

    // removing unnecessary zeros if they exist
    long_number res_num(remove_zeros(res_string));
    // defining a sign
    res_num.is_negative = is_negative;

    return res_num;
}

long_number long_number::operator-(const long_number &num) const {

    if (num.is_negative)
        return *this + -num;

    if (num > *this)
        return -(num-*this);

    std::string res_string;
    nat_num num_sub = 0, first_cur_pos = get_length(), second_cur_pos = num.get_length();
    bool sub_needed = false;

    for (; (first_cur_pos > 0) && (second_cur_pos > 0);
           first_cur_pos = get_next_pos(first_cur_pos),
           second_cur_pos = get_next_pos(second_cur_pos))
    {
        nat_num first_part = get_subpart(first_cur_pos);
        nat_num second_part = num.get_subpart(second_cur_pos);

        second_part += nat_num(sub_needed);
        sub_needed = first_part < second_part;

        num_sub = sub_needed ? BASE_VAL+first_part-second_part : first_part-second_part;

        auto res = convert_number(num_sub, first_cur_pos);
        res_string = res.first.append(res_string);
    }
    while ((first_cur_pos != 0) && (sub_needed)) {

        nat_num sub_part = get_subpart(first_cur_pos);
        sub_needed = sub_part < 1;

        num_sub = sub_needed ? BASE_VAL+sub_part-1 : sub_part-1;
        auto sub_res = convert_number(num_sub, first_cur_pos);

        first_cur_pos = get_next_pos(first_cur_pos);
        res_string = sub_res.first.append(res_string);
    }

    if (first_cur_pos != 0)
        res_string = number.substr(0, first_cur_pos).append(res_string);

    return long_number(remove_zeros(res_string));
}

// unary minus
long_number long_number::operator-() const {
    long_number new_num(*this);
    new_num.is_negative = !is_negative;
    return new_num;
}


long_number long_number::operator*(const long_number &num) const {

    if (num > *this)
        return num * *this;

    std::string res_string;
    long_number res_num;

    nat_num num_add = 0, first_cur_pos = get_length(), second_cur_pos = num.get_length();
    nat_num shift = 0, big_shift = 0;

    for (; (first_cur_pos > 0); first_cur_pos = get_next_pos(first_cur_pos))
    {
        nat_num first_part = get_subpart(first_cur_pos);
        for (; (second_cur_pos > 0); second_cur_pos = get_next_pos(second_cur_pos)) {

            nat_num second_part = num.get_subpart(second_cur_pos);
            num_add = first_part*second_part;

            long_number num_to_add(std::to_string(num_add)+std::string(big_shift+shift, '0'));
            res_num = res_num + num_to_add;

            shift += BASE_NUM;
        }

        // 53
        //*47
        //_____
        //350 + 21 = 371

        shift = 0;
        big_shift += BASE_NUM;
        second_cur_pos = num.get_length();
    }

    res_num.is_negative = is_negative^num.is_negative;

    return res_num;
}

RatNum long_number::get_div_mod(const long_number &num) const {

    if (num == (long_number)0) {
        throw zero_division_error(RatNum(*this, num));
    }

    if (num > *this)
        return RatNum(0, *this);

    std::vector<long_number> mul_res = {num};

    for (int i = 2; i <= BASE; i++)
        mul_res.push_back(num * long_number(std::to_string(i)));

    nat_num cur_pos = 0, digit_number = num.get_length();;
    std::string res_num, remained;

    // 1243|25
    // 124/25 => 49
    //   243
    // cycle for dividend (while not at the end)
    while (cur_pos + digit_number <= number.length()) {

        std::string cur_str = remained + number.substr(cur_pos, digit_number);
        long_number cur_num(remove_zeros(cur_str));


        if (mul_res.at(0) > cur_num) {
            digit_number++;
            res_num += "0";
            continue;
        }
        nat_num div_res = 0;

        for (nat_num i = 0; i < BASE; i++) {

            auto cur_number = mul_res.at(i);

            if (cur_number == cur_num)
            {
                remained = "0";
                div_res = i+1;
                break;
            }
            else if (cur_number > cur_num) {
                remained = (cur_num - mul_res.at(i-1)).get_number();
                div_res = i;
                break;
            }
        }

        res_num += std::to_string(div_res);
        cur_pos += digit_number;
        digit_number = 1;
    }

    auto remains_string = remove_zeros(remained + number.substr(cur_pos, digit_number));

    long_number divisor(remove_zeros(res_num)), rems(remains_string);

    divisor.is_negative = is_negative^num.is_negative;
    rems.is_negative = divisor.is_negative;

    return RatNum(divisor, rems);
}

long_number long_number::operator/(const long_number &num) const
{
    return get_div_mod(num).first;
}

long_number long_number::operator%(const long_number &num) const
{
    return get_div_mod(num).second;
}

bool long_number::operator>(const long_number &num) const {

    if (is_negative ^ num.is_negative)
        return num.is_negative;
    else if (is_negative)
        return !(-*this > -num) && (*this != num);

    if (get_length() != num.get_length())
        return get_length() > num.get_length();

    for (nat_num i = 0; i < get_length(); i++)
    {
        if (number[i] != num.number[i])
            return number[i] > num.number[i];
    }

    return false;
}

bool long_number::operator==(const long_number &num) const {

    if ((is_negative ^ num.is_negative) || (get_length() != num.get_length()))
        return false;

    for (nat_num i = 0; i < get_length(); i++)
    {
        if (number[i] != num.number[i])
            return false;
    }

    return true;
}

bool long_number::operator>=(const long_number &num) const {
    return (*this > num) || (*this == num);
}

bool long_number::operator<(const long_number &num) const {
    return !(*this >= num);
}

bool long_number::operator<=(const long_number &num) const {
    return !(*this > num);
}

bool long_number::operator!=(const long_number &num) const {
    return !(*this == num);
}


long_number::long_number(const std::string& num) {

    if (num.empty()) {
        number = "0";
        return;
    }

    auto res_num = string_strip(only_one_space(num));

    if (!(is_number(res_num)) || (res_num.find('.') != std::string::npos))
        throw not_a_number_error(res_num);

    if (num.at(0) == '-')
        is_negative = true;
    number = res_num.substr(is_negative);
}

long_number long_number::gcd(long_number num) const{

    // greatest common divisor

    long_number zero(0);

    if (*this < zero)
        return (-(*this)).gcd(num);

    if (num < zero)
        return gcd(-num);

    if (num > *this)
        return num.gcd(*this);

    long_number box, one(number);

    while (num != zero)
    {
        box = num;
        num = one % num;
        one = box;
    }

    return one;
}

long_number long_number::lcm(const long_number& num) const {
    // least common multiple
    long_number gcd_num = gcd(num), zero("0");
    return gcd_num != zero ? (*this/gcd_num*num) : zero;
}

long_number long_number::abs() const{
    long_number new_num(*this);
    new_num.is_negative = false;
    return new_num;
}

long_number long_number::operator++(int) &{
    long_number old_val(*this);
    *this = old_val + 1;
    return old_val;
}

long_number long_number::operator--(int) &{
    long_number old_val(*this);
    *this = old_val - 1;
    return old_val;
}



std::ostream& operator<<(std::ostream& os, const long_number& number)
{
    os << (number.get_sign() ? "-" : "") << number.get_number();
    return os;
}

// check if string is number
bool is_number(const std::string& number)
{
    return number.empty() ||
    ((number.substr((number.at(0) == '-')).find_first_not_of( "0123456789." ) ==  std::string::npos)
    && (std::count(number.begin(), number.end(), '.') <= 1));
}


const char *not_a_number_error::what() const noexcept {
    return error_msg.c_str();
}

not_a_number_error::not_a_number_error(const std::string& input) : input_str(input) {
    error_msg += "Input " + input + " is not a number";
}


const char *zero_division_error::what() const noexcept {
    return error_msg.c_str();
}

zero_division_error::zero_division_error(const RatNum& num) : div_nums(num) {
    if (num.second == 0)
        error_msg += ": " + num.first.get_number() + " is divided by 0";
}

