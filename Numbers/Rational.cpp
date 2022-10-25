#include "Rational.h"

#include <utility>
#include "../parsing/parsing.h"


RationalNumber::RationalNumber(const long_number& numerator, const long_number& denominator) {
    if (denominator == 0)
        throw zero_division_error(RatNum(numerator, denominator));
    number = RatNum(numerator, denominator);
    make_canonical();
}

RationalNumber::RationalNumber(const std::string &numerator, const std::string &denominator) {
    auto true_numerator = string_strip(only_one_space(numerator));
    auto true_denominator = string_strip(only_one_space(denominator));
    if (!(is_number(true_numerator)))
        throw not_a_number_error(true_numerator);
    else if (!(is_number(true_denominator)))
        throw not_a_number_error(true_denominator);
    if (long_number(denominator) == 0)
        throw zero_division_error(RatNum(numerator, denominator));
    number = RatNum(numerator, denominator);
    make_canonical();
}

RationalNumber::RationalNumber(const std::string &some_string) {

    auto good_string = string_strip(only_one_space(some_string));

    // finding if rational number is in canonical form
    auto first_pos = good_string.find('<'), end_pos = good_string.find('>');
    if (!((first_pos == std::string::npos) && (end_pos == std::string::npos))
    && (!((first_pos == 0) && (end_pos == good_string.length()-1))))
        throw std::logic_error(good_string + " is not a rational number");

    first_pos = (first_pos == std::string::npos) ? 0 : first_pos + 1;
    end_pos = (end_pos == std::string::npos) ? good_string.length() : end_pos-1;

    auto res_string = good_string.substr(first_pos, end_pos);

    auto sl_pos = res_string.find('/');
    if (sl_pos == std::string::npos)
    {
        if (!(is_number(res_string)))
            throw not_a_number_error(some_string);
        if (std::count(res_string.begin(), res_string.end(), '.') == 1)
        {
            number = RatNum(float_to_string(res_string));
            make_canonical();
            return;
        }
        number = RatNum(res_string, 1);
        return;
    }

    std::string first_part = clean_string(res_string.substr(0, sl_pos)),
    second_part = clean_string(res_string.substr(sl_pos+1));

    if (!(is_number(first_part)))
        throw not_a_number_error(first_part);
    if (!(is_number(second_part)))
        throw not_a_number_error(second_part);

    std::pair<std::string, std::string> first_res =
     (std::count(first_part.begin(), first_part.end(), '.') == 1) ?
     float_to_string(first_part) :  std::pair<std::string, std::string>(first_part, "1");

    std::pair<std::string, std::string> second_res =
            (std::count(second_part.begin(), second_part.end(), '.') == 1) ?
            float_to_string(second_part) :  std::pair<std::string, std::string>(second_part, "1");

    auto first_zeros = std::count(first_res.second.begin(), first_res.second.end(), '0');
    auto second_zeros = std::count(second_res.second.begin(), second_res.second.end(), '0');

    if (first_zeros > second_zeros)
        second_res.first += std::string(first_zeros-second_zeros, '0');
    else
        first_res.first += std::string(second_zeros-first_zeros, '0');

    second_res.first = long_number::remove_zeros(second_res.first);

    if (second_res.first == "0")
        throw zero_division_error(RatNum(first_res.first, second_res.first));

    number = RatNum(first_res.first, second_res.first);
    make_canonical();
}

RationalNumber RationalNumber::make_canonical() {
    auto gcd = number.first.gcd(number.second);
    bool sign = number.first.get_sign() ^ number.second.get_sign();
    number = RatNum((sign ? -(number.first.abs()/gcd) : number.first.abs()/gcd), number.second.abs()/gcd);
    return *this;
}

RationalNumber RationalNumber::operator+(const RationalNumber& num) const {
    long_number lcm = number.second.lcm(num.get_denominator());
    long_number first_mult = (lcm/number.second), second_mult = lcm/num.get_denominator();
    //std::cout << first_mult*number.first << std::endl;
    return RationalNumber(first_mult*number.first + second_mult*num.get_numerator(), lcm);
}

RationalNumber RationalNumber::operator-(const RationalNumber& num) const {
    return *this + -num;
}

RationalNumber RationalNumber::operator-() const {
    return RationalNumber(-number.first, number.second);
}

RationalNumber RationalNumber::operator*(const RationalNumber &num) const {
    return RationalNumber(number.first*num.get_numerator(), number.second*num.get_denominator());
}

RationalNumber RationalNumber::operator/(const RationalNumber &num) const {
    return RationalNumber(number.first*num.get_denominator(), number.second*num.get_numerator());
}

bool RationalNumber::operator<(const RationalNumber &num) const {
    long_number lcm = number.second.lcm(num.get_denominator());
    long_number first_mult = (lcm/number.second), second_mult = lcm/num.get_denominator();
    return first_mult*number.first < second_mult*num.get_numerator();
}

bool RationalNumber::operator<=(const RationalNumber &num) const {
    return (*this < num) || (*this == num);
}

bool RationalNumber::operator>(const RationalNumber &num) const {
    return !(*this <= num);
}

bool RationalNumber::operator>=(const RationalNumber &num) const {
    return !(*this < num);
}

bool RationalNumber::operator==(const RationalNumber &num) const {
    long_number lcm = number.second.lcm(num.get_denominator());
    long_number first_mult = (lcm/number.second), second_mult = lcm/num.get_denominator();
    return first_mult*number.first == second_mult*num.get_numerator();
}

bool RationalNumber::operator!=(const RationalNumber &num) const {
    return !(*this == num);
}

RationalNumber RationalNumber::operator++(int) &{
    RationalNumber old_val(*this);
    *this = old_val + 1;
    return old_val;
}

RationalNumber  RationalNumber ::operator--(int) &{
    RationalNumber old_val(*this);
    *this = old_val - 1;
    return old_val;
}

RationalNumber RationalNumber::round() const {
    RatNum vals = number.first.get_div_mod(number.second);
    bool need_round = (number.first*2 >= number.second);
    return RationalNumber(vals.first + (number.first.get_sign() ? -need_round : need_round), 1);
}

RationalNumber RationalNumber::floor() const {
    RatNum vals = number.first.get_div_mod(number.second);
    return RationalNumber(vals.first - (number.first.get_sign() && (vals.second != 0)), 1);
}

RationalNumber RationalNumber::ceil() const {
    RatNum vals = number.first.get_div_mod(number.second);
    return RationalNumber(vals.first + (!number.first.get_sign() && (vals.second != 0)), 1);
}

std::pair<std::string, std::string> RationalNumber::float_to_string(const std::string& floating_string) {
    auto sl_pos = floating_string.find('.');
    std::string first_part = floating_string.substr(0, sl_pos),
    second_part = floating_string.substr(sl_pos+1);

    std::string res_num = long_number::remove_zeros(first_part + second_part),
    denom = "1" + std::string(second_part.length(), '0');

    return std::pair<std::string, std::string>(res_num, denom);
}


std::ostream& operator<<(std::ostream &os, const RationalNumber& number) {
    os << "<" << number.get_numerator();
    auto den = number.get_denominator();
    if (den != 1)
        os << "/" << den;
    os << ">";
    return os;
}


const char *has_frac_part::what() const noexcept {
    return error_msg.c_str();
}

has_frac_part::has_frac_part(RationalNumber &num): num_to_transform(num) {
    if (num.get_denominator() != 1)
        error_msg = num.to_string() + " can not be casted to integral type: it has fractional part";
}