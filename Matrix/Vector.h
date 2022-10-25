#ifndef SALNIKOV_TASK_VECTOR_H
#define SALNIKOV_TASK_VECTOR_H


#include <map>
#include <fstream>
#include "../Numbers/ComplexNumber.h"
#include "../parsing/parsing.h"

template <typename K>
concept AnyNum = RatNumeric<K> || is_complex<K>::value;


template <RatNumeric T, RatNumeric K>
RationalNumber abs(ComplexNumber<T, K> num)
{
    return num.abs();
}

template <Custom T>
T abs(T num)
{
  return num > 0 ?  num : num.abs();
}

template <typename T> requires (std::is_arithmetic_v<T>)
T abs(T num)
{
    return num > 0 ? num : -num;
}

template <AnyNum T> requires (!std::is_arithmetic_v<T>)
std::string string_transform(T num)
{
    return num.to_string();
}

template <AnyNum T> requires (std::is_arithmetic_v<T>)
std::string string_transform(T num)
{
    return std::to_string(num);
}

template <AnyNum T>
class Vector {

    using sparse_nums = std::map<nat_num, T>;

    nat_num vec_size = 0, last_zero_pose = 0;
    double eps = 0.0;
    sparse_nums nums;

public:

    explicit Vector(nat_num v_siz = 100, double cur_ep = 0.01, sparse_nums cur_nums = sparse_nums()) :
                                                vec_size(v_siz), eps(cur_ep), nums(cur_nums) {};

    sparse_nums get_numbers() {return nums;}
    nat_num get_size() {return vec_size;}
    double get_eps() {return eps;}

    void set_size(nat_num new_size) {vec_size = new_size;}
    void set_eps(double new_eps) {eps = new_eps;}
    void clear_nums() {nums.clear();}

    explicit Vector(T def_val, nat_num v_siz = 100, double cur_ep = 0.01) :
            vec_size(v_siz), eps(cur_ep)
    {
        if (def_val > eps)
            for (int i = 0; i < vec_size; i++)
                nums[i] = def_val;
    };

    Vector(const std::string& filename)
    {
        std::ifstream in(filename);
        std::string buffer;
        std::getline(in, buffer);
        buffer = clean_string(buffer);

        while (buffer.empty())
        {
            std::getline(in, buffer);
            buffer = clean_string(buffer);
        }

        auto vec_pos = buffer.find("vector");
        auto type_name_len = std::string("vector").length();

        if (vec_pos == std::string::npos)
            throw std::logic_error("Bad input");

        buffer = clean_string(buffer.substr(vec_pos+type_name_len));
        vec_size = std::stoull(buffer);

        while (std::getline(in, buffer))
        {
            buffer = clean_string(buffer);
            if (buffer.empty())
                continue;

            auto check = extract_nat_num(buffer);
            auto num_pos = check.first;
            buffer = clean_string(buffer.substr(check.second));
            nat_num start_pos, end_pos;

            if (std::is_same_v<T, RationalNumber>)
            {
                start_pos = buffer.find('<');
                end_pos = buffer.find('>') + 1;
            }
            else if (is_complex<T>::value)
            {
                start_pos = buffer.find('(');
                end_pos = buffer.find(')') + 1;
            }
            else
            {
                if (((std::find(buffer.begin(), buffer.end(), '-') != buffer.begin()) &&
                     std::find(buffer.begin(), buffer.end(), '-') != buffer.end()) ||
                    (std::count(buffer.begin(), buffer.end(), '.') > 1) ||
                    (std::count(buffer.begin(), buffer.end(), '-') > 1))
                    throw std::logic_error("error!");

                start_pos = buffer.find_first_of("0123456789-");
                end_pos = buffer.find_first_not_of("0123456789.-");
            }

            auto need_num = buffer.substr(start_pos, end_pos);
            add_number(num_pos, string_to_num<T>(need_num));
        }
        in.close();
    }

    template <AnyNum K>
    Vector(Vector<K>& other)
    {
        vec_size = other.get_size();
        eps = other.get_eps();
        nums.clear();
        auto other_map = other.get_numbers();
        for (auto it = other_map.begin(); it != other_map.end(); it++)
            add_number(it->first, T(it->second));
    }

    Vector (const Vector& other)
    {
        vec_size = other.get_size();
        eps = other.get_eps();
        nums.clear();
        auto other_map = other.get_numbers();
        for (auto it = other_map.begin(); it != other_map.end(); it++)
            add_number(it->first, it->second);
    }

    Vector(Vector&& other) noexcept
    {
        other.last_zero_remove();
        if (&other != this) {
            vec_size = other.vec_size;
            other.vec_size = 0;

            eps = other.eps;
            other.eps = 0;

            other.last_zero_pose = 0;

            nums = std::move(other.nums);
        }
    }

    template <AnyNum K>
    void add_number(nat_num pos, K val) {
        if (abs(val) >= (is_complex<K>::value ? eps*eps : eps))
            nums[pos] = val;
        else if (nums.find(pos) != nums.end())
            nums.erase(pos);
    }

    void zeros_erase()
    {
        for (auto it = nums.begin(), last = nums.end(); it != last; ) {
            if (it->second == 0) {
                it = nums.erase(it);
            } else {
                it++;
            }
        }
    }

    void zero_remove(nat_num pos)
    {
        if ((nums.find(pos) != nums.end()) && (nums[pos] == T(0)))
            nums.erase(pos);
    }

    void last_zero_remove()
    {
        zero_remove(last_zero_pose);
    }

    T& operator()(nat_num pos)
    {
        //zeros_erase();
        last_zero_remove();
        if (pos > vec_size)
            throw std::logic_error("ERROR!");
        if (nums.find(pos) != nums.end())
            return nums[pos];
        nums[pos] = 0;
        last_zero_pose = pos;
        return nums[pos];
    }

    template <AnyNum K>
    Vector& operator=(Vector<K> other_vec) requires (!std::is_same_v<T, K>)
    {
        eps = other_vec.get_eps();
        vec_size = other_vec.get_size();
        auto copy_map = other_vec.get_numbers();
        nums.clear();
        for (auto it = copy_map.begin(); it != copy_map.end(); it++)
            add_number(it->first, T(it->second));
        return *this;
    }

    Vector& operator=(const Vector& other_vec)
    {
        eps = other_vec.get_eps();
        vec_size = other_vec.get_size();
        auto copy_map = other_vec.get_numbers();
        nums.clear();
        for (auto it = copy_map.begin(); it != copy_map.end(); it++)
            add_number(it->first, it->second);
        return *this;
    }

    Vector& operator=(Vector&& other_vec) noexcept
    {
        other_vec.last_zero_remove();
        if (&other_vec != this) {
            eps = other_vec.get_eps();
            other_vec.eps = 0;

            vec_size = other_vec.get_size();
            other_vec.vec_size = 0;

            other_vec.last_zero_pose = 0;

            nums = std::move(other_vec.nums);
        }
        return *this;
    }


    template <AnyNum K>
    Vector<decltype(T()+K())> operator+(K number)
    {
        Vector<decltype(T()+K())> res_vec(*this);
        res_vec.nums.clear();
        for (auto it = nums.begin(); it != nums.end(); it++)
            res_vec.add_number(it->first, it->second+number);
        return res_vec;
    }

    template <AnyNum K>
    Vector<decltype(T()-K())> operator-(K number)
    {
        Vector<decltype(T()-K())> res_vec(*this);
        res_vec.nums.clear();
        for (auto it = nums.begin(); it != nums.end(); it++)
            res_vec.add_number(it->first, it->second-number);
        return res_vec;
    }

    template <AnyNum K>
    Vector<decltype(T()*K())> operator*(K number)
    {
        Vector<decltype(T()*K())> res_vec(*this);
        res_vec.nums.clear();
        for (auto it = nums.begin(); it != nums.end(); it++)
            res_vec.add_number(it->first, it->second*number);
        return res_vec;
    }

    template <AnyNum K>
    Vector<decltype(T()/K(1))> operator/(K number)
    {
        if (number == 0)
            throw zero_division_error(RatNum(1, number));
        Vector<decltype(T()/K(1))> res_vec(*this);
        res_vec.nums.clear();
        for (auto it = nums.begin(); it != nums.end(); it++)
            res_vec.add_number(it->first, it->second/number);
        return res_vec;
    }

    Vector operator-()
    {
        Vector res_vec(*this);
        res_vec.nums.clear();
        for (auto it = nums.begin(); it != nums.end(); it++)
            res_vec.add_number(it->first, -(it->second));
        return res_vec;
    }

    template<AnyNum K>
    Vector<decltype(T()+K())> operator+(Vector<K> other_vec)
    {
        last_zero_remove();
        Vector<decltype(T()+K())> res_vec(*this);
        auto other_map = other_vec.get_numbers();
        for (auto it = other_map.begin(); it != other_map.end(); it++)
            res_vec.add_number(it->first,
                               decltype(T()+K())(it->second +
                               (nums.find(it->first) == nums.end() ? decltype(T()+K())(0) :
                               nums[it->first]) ) );
        return res_vec;
    }

    template<AnyNum K>
    Vector<decltype(T()-K())> operator-(Vector<K> other_vec)
    {
        last_zero_remove();
        int zero = 0;
        Vector<decltype(T()-K())> res_vec(*this);
        auto other_map = other_vec.get_numbers();
        for (auto it = other_map.begin(); it != other_map.end(); it++)
            res_vec.add_number(it->first,
                               decltype(T()-K())(-it->second +
                               (nums.find(it->first) == nums.end() ? decltype(T()-K())(0) : nums[it->first])) );
        return res_vec;
    }

    std::string to_string()
    {
        last_zero_remove();
        //zeros_erase();
        std::string res_string("vector  " + std::to_string(vec_size) + "\n\n");
        for (auto it = nums.begin(); it != nums.end(); it++)
            res_string += std::to_string((*it).first) + "\t\t" + string_transform((*it).second) + "\n";
        return res_string;
    }

    void to_file(const std::string& filename = "output.txt")
    {
        last_zero_remove();
        //zeros_erase();
        std::ofstream out(filename);
        if (!out) {
            std::cout << "Could not write to file " << filename << std::endl;
            return;
        }
        out << *this << std::endl;
        out.close();
    }

};

template<AnyNum T>
std::ostream& operator<<(std::ostream& os, Vector<T> vec) {
    os << vec.to_string();
    return os;
}

template<typename T, typename K> requires ((std::is_floating_point_v<T> && std::is_same_v<long_number, K>) ||
                                           (std::is_floating_point_v<K> && std::is_same_v<long_number, T>)   )
bool operator>(const T& first_num, K second_num)
{
    return RationalNumber(first_num) > RationalNumber(second_num);
}

template<typename T, typename K> requires ((std::is_floating_point_v<T> && std::is_same_v<long_number, K>) ||
                                           (std::is_floating_point_v<K> && std::is_same_v<long_number, T>)   )
bool operator<(const T& first_num, K second_num)
{
return RationalNumber(first_num) < RationalNumber(second_num);
}

template<typename T, typename K> requires ((std::is_floating_point_v<T> && std::is_same_v<long_number, K>) ||
                                           (std::is_floating_point_v<K> && std::is_same_v<long_number, T>)   )
bool operator>=(const T& first_num, K second_num)
{
return RationalNumber(first_num) >= RationalNumber(second_num);
}

template<typename T, typename K> requires ((std::is_floating_point_v<T> && std::is_same_v<long_number, K>) ||
                                           (std::is_floating_point_v<K> && std::is_same_v<long_number, T>)   )
bool operator==(const T& first_num, K second_num)
{
return RationalNumber(first_num) == RationalNumber(second_num);
}

template<typename T, typename K> requires ((std::is_floating_point_v<T> && std::is_same_v<long_number, K>) ||
                                           (std::is_floating_point_v<K> && std::is_same_v<long_number, T>)   )
bool operator<=(const T& first_num, K second_num)
{
return RationalNumber(first_num) <= RationalNumber(second_num);
}

template<typename T, typename K> requires ((std::is_floating_point_v<T> && std::is_same_v<long_number, K>) ||
                                           (std::is_floating_point_v<K> && std::is_same_v<long_number, T>)   )
bool operator!=(const T& first_num, K second_num)
{
return RationalNumber(first_num) != RationalNumber(second_num);
}


#endif //SALNIKOV_TASK_VECTOR_H
