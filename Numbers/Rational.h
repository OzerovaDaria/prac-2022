#include "long_number.h"

#ifndef SALNIKOV_TASK_RATIONAL_H
#define SALNIKOV_TASK_RATIONAL_H

template <class T>
concept numValue = std::is_integral_v<T> || std::is_same_v<T, long_number> ||
std::is_same_v<T, std::string> || std::is_same_v<T, const char*>;

class RationalNumber;

template<typename T>
concept Custom = std::is_same_v<T, long_number> || std::is_same_v<T, RationalNumber>;

class has_frac_part : public std::exception
{
    std::string error_msg;
    RationalNumber& num_to_transform;
public:
    //has_frac_part() = default;
    explicit has_frac_part(RationalNumber &num);

    [[nodiscard]] const char * what () const noexcept override;
    RationalNumber& get_number() {return num_to_transform;}

};

class RationalNumber
{
    RatNum number;

public:

    RationalNumber() : number(0, 1) {};
    RationalNumber(const long_number& int_num) : number(int_num, 1) {};
    RationalNumber(const long_number& numerator, const long_number& denominator);
    RationalNumber(const std::string& numerator, const std::string& denominator);
    explicit RationalNumber(const std::string& some_string);

    template <typename T> requires (std::is_integral_v<T>)
    RationalNumber(T num) : number(num, 1) {};

    template <numValue T, numValue K>
    explicit RationalNumber(T num, K den) : number(num, den) {make_canonical();};

    static std::pair<std::string, std::string> float_to_string(const std::string& floating_string);

    template <typename T> requires (std::is_floating_point_v<T>)
    RationalNumber(T num)
    {
        std::string floating_string = std::to_string(num);
        number = RatNum(float_to_string(floating_string));
        make_canonical();
    }

    RationalNumber make_canonical();

    [[nodiscard]] RatNum get_number() const {return number;}
    [[nodiscard]] long_number get_numerator() const {return number.first;}
    [[nodiscard]] long_number get_denominator() const {return number.second;}
    [[nodiscard]] std::string to_string() const {return "<" + number.first.to_string()+
                (number.second != 1 ? ("/" + number.second.to_string()) : "") + ">";}


    RationalNumber operator+(const RationalNumber& num) const;
    RationalNumber operator-() const;
    RationalNumber operator-(const RationalNumber& num) const;
    RationalNumber operator*(const RationalNumber& num) const;
    RationalNumber operator/(const RationalNumber& num) const;
    RationalNumber round() const;
    RationalNumber floor() const;
    RationalNumber ceil() const;


    RationalNumber& operator++() {return *this = *this + 1;}
    RationalNumber operator++(int)&;
    RationalNumber& operator--() {return *this = *this - 1;}
    RationalNumber operator--(int)&;

    RationalNumber abs() const {return RationalNumber(number.first.abs(), number.second.abs());}

    bool operator<(const RationalNumber& num) const;
    bool operator<=(const RationalNumber& num) const;
    bool operator>(const RationalNumber& num) const;
    bool operator>=(const RationalNumber& num) const;
    bool operator==(const RationalNumber& num) const;
    bool operator!=(const RationalNumber& num) const;

    RationalNumber& operator= (const RationalNumber& num) = default;

    RationalNumber& operator +=(const RationalNumber& num) {return *this = *this + num;}
    RationalNumber& operator -=(const RationalNumber& num) {return *this = *this - num;}
    RationalNumber& operator *=(const RationalNumber& num) {return *this = *this * num;}
    RationalNumber& operator /=(const RationalNumber& num) {return *this = *this / num;}

    template <typename T> requires (std::is_integral_v<T>)
    explicit operator T()
    {
        make_canonical();
        if (number.second != 1)
            throw has_frac_part(*this);
        return (T)number.first;
    }

};

template <typename T, Custom K> requires (std::is_integral_v<T> ||
        ((std::is_same_v<T, long_number> || std::is_floating_point_v<T>) && std::is_same_v<K, RationalNumber>))
K operator+(T first_num, K second_num)
{
    return second_num + first_num;
}

template <typename T, Custom K> requires (std::is_integral_v<T> ||
        ((std::is_same_v<T, long_number> || std::is_floating_point_v<T>) && std::is_same_v<K, RationalNumber>))
K operator-(T first_num, K second_num)
{
    return K(first_num) - second_num;
}

template <typename T, Custom K> requires (std::is_integral_v<T> ||
        ((std::is_same_v<T, long_number> || std::is_floating_point_v<T>) && std::is_same_v<K, RationalNumber>))
K operator*(T first_num, K second_num)
{
    return second_num*first_num;
}

template <typename T, Custom K> requires (std::is_integral_v<T> ||
        ((std::is_same_v<T, long_number> || std::is_floating_point_v<T>) && std::is_same_v<K, RationalNumber>))
K operator/(T first_num, K second_num)
{
    return K(first_num)/second_num;
}

template <typename T, typename K> requires ((std::is_same_v<T, long_number> && std::is_floating_point_v<K>) ||
                                            (std::is_same_v<K, long_number> && std::is_floating_point_v<T>))
RationalNumber operator+(T first_num, K second_num)
{
    return RationalNumber(first_num) + RationalNumber(second_num);
}

template <typename T, typename K> requires ((std::is_same_v<T, long_number> && std::is_floating_point_v<K>) ||
                                            (std::is_same_v<K, long_number> && std::is_floating_point_v<T>))
RationalNumber operator-(T first_num, K second_num)
{
    return RationalNumber(first_num) - RationalNumber(second_num);
}

template <typename T, typename K> requires ((std::is_same_v<T, long_number> && std::is_floating_point_v<K>) ||
                                            (std::is_same_v<K, long_number> && std::is_floating_point_v<T>))
RationalNumber operator*(T first_num, K second_num)
{
    return RationalNumber(first_num) * RationalNumber(second_num);
}

template <typename T, typename K> requires ((std::is_same_v<T, long_number> && std::is_floating_point_v<K>) ||
                                            (std::is_same_v<K, long_number> && std::is_floating_point_v<T>))
RationalNumber operator/(T first_num, K second_num)
{
    return RationalNumber(first_num) / RationalNumber(second_num);
}


std::ostream& operator<<(std::ostream& os, const RationalNumber& number);

#endif //SALNIKOV_TASK_RATIONAL_H
