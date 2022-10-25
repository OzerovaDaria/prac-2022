#include <climits>
#include <utility>
#include <stdexcept>
#include <iostream>
#include <string>
#include <numeric>
#include <cmath>
#include <vector>

#ifndef SALNIKOV_TASK_LONG_NUMBER_H
#define SALNIKOV_TASK_LONG_NUMBER_H

class long_number;


// custom types
using num = long long;
using nat_num = unsigned long long;
using numConvert =  std::pair<std::string, nat_num>;
using RatNum = std::pair<long_number, long_number>;

nat_num string_to_num(const std::string& num);

// function for number power
template <typename T>
constexpr nat_num ipow(T num, unsigned int pow)
{
    return pow == 0 ? 1 : num * ipow(num, pow-1);
}

class long_number
{
    std::string number = "0";
    static constexpr unsigned short BASE_NUM = 9, BASE = 10;
    // BASE_NUM is number of digits used in operations

    static constexpr nat_num BASE_VAL = ipow(BASE, BASE_NUM);
    // BASE_VAL is number base of system (currently 10^9)

    // sign of number
    bool is_negative = false;

public:

    [[nodiscard]] std::string get_number() const {return number;}

    [[nodiscard]] nat_num get_length() const {return number.length();}

    long_number() = default;
    explicit long_number(const std::string& num);

    template <typename T> requires (std::is_integral_v<T>)
    long_number(T num)
    {is_negative = num < 0;
    number = std::to_string(num > 0 ? num : -num);}

    static unsigned short get_base_num() {return BASE_NUM;}
    static unsigned short get_base() {return BASE;}
    static nat_num get_base_val() {return BASE_VAL;}

    [[nodiscard]] bool get_sign() const {return is_negative;}
    [[nodiscard]] std::string to_string() const {return (is_negative ? "-" : "") + number;}

    void set_number(std::string num) {number = std::move(num);}
    static numConvert convert_number(nat_num number, nat_num neededZeros);

    static std::string remove_zeros(std::string str_zeros);

    // right corner for number in cycle
    static nat_num get_next_pos(nat_num pos) {return pos > BASE_NUM ? pos - BASE_NUM : 0;}


    nat_num get_subpart(nat_num cur_pos) const;

    [[nodiscard]] RatNum get_div_mod(const long_number &num) const;

    long_number& operator =(const long_number& num) = default;
    long_number operator-() const;

    long_number operator+(const long_number& num) const;
    long_number operator*(const long_number& num) const;
    long_number operator-(const long_number& num) const;
    long_number operator/(const long_number& num) const;
    long_number operator%(const long_number &num) const;

    long_number& operator++() {return *this = *this + 1;}
    long_number operator++(int)&;
    long_number& operator--() {return *this = *this - 1;}
    long_number operator--(int)&;

    [[nodiscard]] long_number abs() const;

    long_number& operator +=(const long_number& num) {return *this = *this + num;}
    long_number& operator -=(const long_number& num) {return *this = *this - num;}
    long_number& operator *=(const long_number& num) {return *this = *this * num;}
    long_number& operator /=(const long_number& num) {return *this = *this / num;}
    long_number& operator %=(const long_number& num) {return *this = *this % num;}

    long_number round() {return long_number(*this);}
    long_number ceil() {return long_number(*this);}
    long_number floor() {return long_number(*this);}

    bool operator>(const long_number& num) const;
    bool operator==(const long_number& num) const;
    bool operator>=(const long_number& num) const;
    bool operator<(const long_number& num) const;
    bool operator <= (const long_number& num) const;
    bool operator != (const long_number& num) const;

    [[nodiscard]] long_number gcd(long_number num) const;
    [[nodiscard]] long_number lcm(const long_number& num) const;


    // cast from long number to one of integer type
    template <typename T> requires (std::is_integral_v<T>)
    explicit operator T()
    {
        if ((*this > std::numeric_limits<T>::max()) || (*this < std::numeric_limits<T>::min()))
            throw std::out_of_range(("Number " + number + " is out of range of type " + std::string(typeid(T).name())).c_str());
        return (T) (is_negative ? std::stoll("-" + number) : std::stoull(number));
    }

};

class not_a_number_error : public std::exception
{
    std::string error_msg;
    std::string input_str;

public:
    not_a_number_error() = default;
    explicit not_a_number_error(const std::string& input);

    [[nodiscard]] const char* what() const noexcept override;

    std::string get_input() {return input_str;}
};

bool is_number(const std::string& s);


class zero_division_error : public std::exception
{
    std::string error_msg = "Zero division detected";
    RatNum div_nums;
public:
    zero_division_error() = default;
    zero_division_error(const RatNum& num);

    [[nodiscard]] const char * what () const noexcept override;
    RatNum get_numbers() {return div_nums;}

};

std::ostream& operator<<(std::ostream& os, const long_number& number);
#endif //SALNIKOV_TASK_RATIONAL_H
