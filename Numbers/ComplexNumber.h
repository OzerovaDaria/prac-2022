#ifndef SALNIKOV_TASK_COMPLEX_H
#define SALNIKOV_TASK_COMPLEX_H

#include "Rational.h"
#include "../parsing/parsing.h"

template<typename T>
concept RatNumeric = std::is_arithmetic_v<T> || Custom<T>;

template<RatNumeric T> requires (std::is_arithmetic_v<T>)
std::string num_to_string(T num) {return std::to_string(num);}

template<Custom T>
std::string num_to_string(T num) {return num.to_string();}

template<RatNumeric T> requires (std::is_arithmetic_v<T>)
T custom_round(T num) {return round(num);}

template<Custom T>
T custom_round(T num) {return num.round();}

template<RatNumeric T> requires (std::is_arithmetic_v<T>)
T custom_floor(T num) {return floor(num);}

template<Custom T>
T custom_floor(T num) {return num.floor();}

template<RatNumeric T> requires (std::is_arithmetic_v<T>)
T custom_ceil(T num) {return ceil(num);}

template<Custom T>
T custom_ceil(T num) {return num.ceil();}

template <RatNumeric T>
constexpr unsigned int get_num_from_type()
{
    if (std::is_floating_point_v<T>)
        return 0;
    else if (std::is_integral_v<T>)
        return 4;
    else if (std::is_same_v<T, long_number>)
        return 16;
    else if (std::is_same_v<T, RationalNumber>)
        return 64;
}

template <RatNumeric T, RatNumeric K>
constexpr unsigned int get_num_type()
{
    constexpr unsigned int res = get_num_from_type<T>() + get_num_from_type<K>();
    return res;
}

template <typename T> requires (std::is_integral_v<T> && std::is_signed_v<T>)
num string_to_num(const std::string& num)
{
    return std::stoll(num);
}

template <typename T> requires (std::is_integral_v<T> && !std::is_signed_v<T>)
nat_num string_to_num(const std::string& num)
{
    return std::stoull(num);
}

template <typename T> requires (std::is_floating_point_v<T>)
double string_to_num(const std::string& num)
{
    return std::stod(num);
}

template <Custom T>
T string_to_num(const std::string& num)
{
    return T(num);
}

template <RatNumeric R, RatNumeric I>
class ComplexNumber;

template <RatNumeric T, RatNumeric K>
using result_type =
typename std::conditional_t<(get_num_type<T, K>() <= 4), double,
        typename std::conditional_t<(get_num_type<T, K>() == 8),
        typename std::conditional_t<(std::is_signed_v<T> || std::is_signed_v<K>), num, nat_num>,
                typename std::conditional_t<(get_num_type<T, K>() > 16 && (get_num_type<T, K>() <= 32)), long_number,
                        RationalNumber>>>;

template <RatNumeric R = double, RatNumeric I = R>
class ComplexNumber {

    using complex = std::pair<R, I>;

    complex number;

public:
    ComplexNumber() : number(0, 0) {};
    ComplexNumber(R first_part, I second_part): number(first_part, second_part) {};
    ComplexNumber(R first_part) : number(first_part, 0) {};

    template<RatNumeric T, RatNumeric K>
    ComplexNumber(ComplexNumber<T, K> num)
    {
        int first_first = get_num_from_type<R>(), first_second = get_num_from_type<I>();
        int second_first = get_num_from_type<T>(), second_second = get_num_from_type<K>();

        bool rtypes_ge = (first_first >= second_first);
        bool itypes_ge = (first_second >= second_second);

        bool rtypes_fi_conv = (first_first + second_first == 4);
        bool itypes_fi_conv = (first_second + second_second == 4);

        bool r_lnum_f_conv = (first_first == 16) && (second_first == 0);
        bool i_lnum_f_conv = (first_second == 16) && (second_second == 0);

        if ( ((rtypes_ge && !r_lnum_f_conv) || rtypes_fi_conv) &&
             ((itypes_ge && !i_lnum_f_conv) || itypes_fi_conv) ) {
            number = complex((R) num.get_real(), (I) num.get_imag());
        }
        else
            throw std::logic_error("ERROR complex numbers");
    }

    explicit ComplexNumber(const char* c_string)
    {
        std::string some_string(c_string);
        auto good_string = string_strip(only_one_space(some_string));

        auto first_pos = good_string.find('('), end_pos = good_string.find(')');


        if (!((first_pos == std::string::npos) && (end_pos == std::string::npos))
            && (!((first_pos == 0) && (end_pos == good_string.length()-1))))
            throw std::logic_error(good_string + " is not a complex number");

        first_pos = (first_pos == std::string::npos) ? 0 : first_pos + 1;
        end_pos = (end_pos == std::string::npos) ? good_string.length() : end_pos-1;


        auto res_string = good_string.substr(first_pos, end_pos);

        auto sl_pos = res_string.find(',');
        if (sl_pos == std::string::npos)
        {
            if (res_string.empty())
            {
                number = complex(0, 0);
                return;
            }
            if (!(is_number(res_string)) &&
            (std::count(res_string.begin(), res_string.end(), '/') != 1) &&
            !((res_string.at(0) == '<') && (res_string.at(res_string.length()-1) == '>')))
                throw not_a_number_error(some_string);

            std::cout << res_string << std::endl;

            R converted = string_to_num<R>(res_string);

            std::cout << "Converted: " << converted << std::endl;
            //if (res_string.
            number = complex(converted, 0);
            return;
        }
        std::string first_part = string_strip(only_one_space(res_string.substr(0, sl_pos))),
        second_part = string_strip(only_one_space(res_string.substr(sl_pos + 1)));
        //std::cout << "F: " << first_part << ", S: " << second_part << std::endl;
        R converted_first = string_to_num<R>(first_part);
        I converted_second = string_to_num<I>(second_part);
        number = complex(converted_first, converted_second);
    }

    template<RatNumeric T, RatNumeric K>
    ComplexNumber& operator= (ComplexNumber<T, K> num)
    {
        number = complex(num.get_num());
        return *this;
    }

    template<RatNumeric T>
    ComplexNumber& operator= (T num)
    {
        number = complex(num, 0);
        return *this;
    }


    complex get_num() {return number;}
    R get_real() {return number.first;}
    I get_imag() {return number.second;}

    template <RatNumeric T, RatNumeric K>
    ComplexNumber<result_type<T, R>, result_type<I, K>> operator+(ComplexNumber<T, K> num)
    {
        auto res_num = ComplexNumber<result_type<T, R>, result_type<I, K>>(get_real() + num.get_real(),
                                                                           get_imag() + num.get_imag());
        return res_num;
    }

    template <RatNumeric T>
    auto operator+(T num)
    {
        auto to_complex = ComplexNumber<T, int>(num);
        return *this + to_complex;
    }

    template <RatNumeric T, RatNumeric K>
    ComplexNumber<result_type<T, R>, result_type<I, K>> operator-(ComplexNumber<T, K>& num)
    {
        auto res_num = ComplexNumber<result_type<T, R>, result_type<I, K>>(get_real() - num.get_real(),
                                                                           get_imag() - num.get_imag());
        return res_num;
    }

    template <RatNumeric T>
    auto operator-(T num)
    {
        auto to_complex = ComplexNumber<T, int>(num);
        return *this - to_complex;
    }

    template <RatNumeric T, RatNumeric K>
    ComplexNumber<result_type<result_type<R, T>, result_type<I, K>>,
            result_type<result_type<R, K>, result_type<I, T>>> operator*(ComplexNumber<T, K>& num)
    {
        using first_mult = result_type<R, T>;
        using second_mult = result_type<I, K>;
        using third_mult = result_type<R, K>;
        using fourth_mult = result_type<I, T>;

        auto res_num = ComplexNumber<result_type<first_mult, second_mult>,
                result_type<third_mult, second_mult>>(
                first_mult(get_real() * num.get_real()) - second_mult(get_imag() * num.get_imag()),
                third_mult(get_real()*num.get_imag()) + fourth_mult(get_imag()*num.get_real()));

        return res_num;
    }

    template <RatNumeric T>
    auto operator*(T num)
    {
        auto to_complex = ComplexNumber<T, int>(num);
        return *this * to_complex;
    }

    template <RatNumeric T, RatNumeric K>
    ComplexNumber<result_type<result_type<R, T>, result_type<I, K>>,
    result_type<result_type<I, T>, result_type<R, K>>> operator/(ComplexNumber<T, K>& num)
    {

        using first_mult = result_type<R, T>;
        using second_mult = result_type<I, K>;
        using third_mult = result_type<R, K>;
        using fourth_mult = result_type<I, T>;

        auto sqr_val = [](I val) {return val * val;};
        I rad_val = sqr_val(num.get_real()) + sqr_val(num.get_imag());

        auto res_num = ComplexNumber<result_type<first_mult, second_mult>,
                result_type<fourth_mult, third_mult>>
                ((first_mult(get_real() * num.get_real()) + second_mult(get_imag() * num.get_imag())) / rad_val,
                 (fourth_mult(get_imag()*num.get_real()) - third_mult(get_real()*num.get_imag()))/rad_val);

        return res_num;
    }

    template <RatNumeric T>
    auto operator/(T num)
    {
        auto to_complex = ComplexNumber<T, int>(num);
        return *this / to_complex;
    }

    ComplexNumber operator-();

    ComplexNumber& operator =(const ComplexNumber& num) = default;

    template <RatNumeric T, RatNumeric K>
    ComplexNumber& operator +=(ComplexNumber<T, K>& num) {return *this = *this + num;}

    template <RatNumeric T, RatNumeric K>
    ComplexNumber& operator -=(ComplexNumber<T, K>& num) {return *this = *this - num;}

    template <RatNumeric T, RatNumeric K>
    ComplexNumber& operator *=(ComplexNumber<T, K>& num) {return *this = *this * num;}

    template <RatNumeric T, RatNumeric K>
    ComplexNumber& operator /=(ComplexNumber<T, K>& num) {return *this = *this / num;}

    ComplexNumber& operator++() {return *this = *this + 1;}
    const ComplexNumber operator++(int)&;
    ComplexNumber& operator--() {return *this = *this - 1;}
    const ComplexNumber operator--(int)&;

    RationalNumber abs() {
        auto pam = RationalNumber(number.first*number.first);
        auto pum = RationalNumber(number.second*number.second);
        return RationalNumber(number.first*number.first) + RationalNumber(number.second*number.second);
    }
    std::string to_string();

    template <RatNumeric T, RatNumeric K>
    bool operator<(ComplexNumber<T, K>& num) {return abs() < num.abs();}

    template <RatNumeric T, RatNumeric K>
    bool operator<=(ComplexNumber<T, K>& num) {return abs() < num.abs() || *this == num;}

    template <RatNumeric T, RatNumeric K>
    bool operator>(ComplexNumber<T, K>& num) {return abs() > num.abs();}

    template <RatNumeric T, RatNumeric K>
    bool operator>=(ComplexNumber<T, K>& num) {return abs() > num.abs() || *this == num;}

    template <RatNumeric T, RatNumeric K>
    bool operator==(ComplexNumber<T, K>& num)
    {
        return RationalNumber(get_real()) == RationalNumber(num.get_real()) &&
        RationalNumber(get_imag()) == RationalNumber(num.get_imag());
    }

    template <RatNumeric T, RatNumeric K>
    bool operator!=(ComplexNumber<T, K>& num) {return !(*this == num);}

    ComplexNumber round() {return ComplexNumber(custom_round<R>(get_real()), custom_round<I>(get_imag()));};
    ComplexNumber ceil() {return ComplexNumber(custom_ceil<R>(get_real()), custom_ceil<I>(get_imag()));};
    ComplexNumber floor() {return ComplexNumber(custom_floor<R>(get_real()), custom_floor<I>(get_imag()));};

};

template<typename T>
struct is_complex : std::false_type {};

template<RatNumeric T, RatNumeric K>
struct is_complex<ComplexNumber<T, K>> : std::true_type {};

template<RatNumeric R, RatNumeric I>
ComplexNumber<R,I> ComplexNumber<R,I>::operator-() {
    return ComplexNumber<R,I>(-get_real(), -get_imag());
}


template<RatNumeric R, RatNumeric I>
const ComplexNumber<R,I> ComplexNumber<R,I>::operator++(int) &{
    RationalNumber old_val(*this);
    *this = old_val + 1;
    return old_val;
}

template<RatNumeric R, RatNumeric I>
const ComplexNumber<R,I> ComplexNumber<R,I>::operator--(int) &{
    RationalNumber old_val(*this);
    *this = old_val - 1;
    return old_val;
}

template<RatNumeric R, RatNumeric I>
std::string ComplexNumber<R,I>::to_string(){
    return "(" + num_to_string<R>(number.first) + ", " + num_to_string<I>(number.second) + ")";
}

template<RatNumeric R, RatNumeric I>
std::ostream& operator<<(std::ostream& os, ComplexNumber<R, I> number) {
    os << number.to_string();
    return os;
}

template <RatNumeric T, RatNumeric K, RatNumeric L>
auto operator+(T num, ComplexNumber<K, L> other_num)
{
    return other_num + num;
}

template <RatNumeric T, RatNumeric K, RatNumeric L>
auto operator-(T num, ComplexNumber<K, L> other_num)
{
    return ComplexNumber<K, L>(other_num) - num;
}

template <RatNumeric T, RatNumeric K, RatNumeric L>
auto operator*(T num, ComplexNumber<K, L> other_num)
{
    return other_num*num;
}

template <RatNumeric T, RatNumeric K, RatNumeric L>
auto operator/(T num, ComplexNumber<K, L> other_num)
{
    return ComplexNumber<K, L>(other_num)/num;
}

template <typename T> requires (is_complex<T>::value)
T string_to_num(const std::string& num)
{
    return T(num.c_str());
}

#endif //SALNIKOV_TASK_COMPLEX_H
