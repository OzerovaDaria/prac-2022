#ifndef SALNIKOV_TASK_MATRIX_H
#define SALNIKOV_TASK_MATRIX_H

#include <unordered_map>
#include <memory>
#include "Vector.h"

using cord_pair = std::pair<num, num>;

bool operator>(cord_pair first, cord_pair second);
bool operator>=(cord_pair first, cord_pair second);
bool operator<(cord_pair first, cord_pair second);
bool operator<=(cord_pair first, cord_pair second);
bool operator==(cord_pair first, cord_pair second);
bool operator!=(cord_pair first, cord_pair second);

cord_pair operator~(cord_pair changed);
cord_pair operator-(cord_pair first, cord_pair second);
cord_pair operator+(cord_pair pair_num, nat_num num);


inline void hash_combine(size_t & seed, num v) {
    auto hasher = std::hash<num>();
    seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

template <>
struct std::hash<cord_pair>
{
    inline size_t operator()(const cord_pair & v) const
    {
        size_t seed = 0;
        hash_combine(seed, v.first);
        hash_combine(seed, v.second);
        return seed;
    }
};
//template<typename T, typename U>
//std::size_t operator()(const std::pair<T,U> &rhs) const   {
//    size_t retval = hash_value<T>(rhs.first);
//    hash_combine(retval, rhs.second);
//    return retval;
//}

struct Matrix_row_coord
{
    nat_num val;

    Matrix_row_coord(nat_num row_val) : val(row_val) {};
};

struct Matrix_column_coord
{
    nat_num val;

    Matrix_column_coord(nat_num col_val) : val(col_val) {};
};

class Matrix_coords
{
    cord_pair left_corner, right_corner;

public:

    Matrix_coords() : left_corner(0, 0), right_corner(0, 0) {};
    Matrix_coords(num r_1, num c_1, num r_2 = -2, num c_2 = -2);

    Matrix_coords(cord_pair first, cord_pair second = cord_pair(-2, -2));

    cord_pair get_left() {return left_corner;}
    cord_pair get_right() {return right_corner;}

};

template<AnyNum T>
class Matrix_proxy;

template <AnyNum T>
class Matrix {

    using sparse_mat = std::unordered_map<cord_pair, T>;

    static const num NUM_BEGIN = 1;
    cord_pair size = cord_pair(0, 0), last_zero_pose = cord_pair(0, 0);
    sparse_mat nums;
    double eps = 0.0;

    std::vector<Matrix_proxy<T>*> references;


public:

    [[nodiscard]] cord_pair get_size() const {return size;}
    [[nodiscard]] double get_eps() const {return eps;}
    sparse_mat get_numbers() const {return nums;}

    void set_numbers(sparse_mat new_nums) {nums = std::move(new_nums);}

    Matrix(nat_num mat_size = 100, T value = T(1))
    {
        size = cord_pair(mat_size, mat_size);
        for (nat_num ind = NUM_BEGIN; ind < NUM_BEGIN + mat_size; ind++)
            add_number(cord_pair(ind, ind), value);
    }

    Matrix(cord_pair mat_size, T value = T(0))
    {
        size = mat_size;
        for (nat_num ind = NUM_BEGIN; ind < NUM_BEGIN + mat_size.first; ind++)
            for (nat_num s_ind = NUM_BEGIN; s_ind < NUM_BEGIN + mat_size.second; s_ind++) {
                add_number(cord_pair(ind, s_ind), value);
            }
    }

    template <AnyNum K>
    Matrix(Matrix<K>& other)
    {
        size = other.get_size();
        eps = other.get_eps();
        nums.clear();
        auto other_map = other.get_numbers();
        for (auto it = other_map.begin(); it != other_map.end(); it++)
            add_number(it->first, T(it->second));
    }

    Matrix (const Matrix& other)
    {
        size = other.get_size();
        eps = other.get_eps();
        nums.clear();
        auto other_map = other.get_numbers();
        for (auto it = other_map.begin(); it != other_map.end(); it++)
            add_number(it->first, it->second);
    }


    Matrix(Matrix&& other) noexcept
    {
        other.last_zero_remove();
        if (&other != this) {
            size = other.size;
            other.size = cord_pair(0, 0);

            eps = other.eps;
            other.eps = 0;

            other.last_zero_pose = cord_pair(0, 0);

            nums = std::move(other.nums);
        }
    }

    Matrix (const Vector<T>& other)
    {
        auto vec_size = other.get_size();
        size = cord_pair(1, vec_size);
        eps = other.get_eps();
        nums.clear();
        auto other_map = other.get_numbers();
        for (auto it = other_map.begin(); it != other_map.end(); it++)
            add_number(cord_pair(1, it->first), it->second);
    }

    Matrix(const std::string& filename)
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

        auto vec_pos = buffer.find("matrix");
        auto type_name_len = std::string("matrix").length();

        if (vec_pos == std::string::npos)
            throw std::logic_error("Bad input");

        buffer = clean_string(buffer.substr(vec_pos+type_name_len));

        auto first_pair = extract_nat_num(buffer);
        auto first_num = first_pair.first;
        auto second_num = std::stoull(clean_string(buffer.substr(first_pair.second)));

        size = cord_pair(first_num, second_num);

        while (std::getline(in, buffer))
        {
            buffer = clean_string(buffer);
            if (buffer.empty())
                continue;

            auto check = extract_nat_num(buffer);
            auto first_pos = check.first;
            buffer = clean_string(buffer.substr(check.second));

            auto check_two = extract_nat_num(buffer);
            auto second_pos = check_two.first;
            buffer = clean_string(buffer.substr(check_two.second));

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
                    throw std::logic_error("ERROR!!!");
                start_pos = buffer.find_first_of("0123456789-");
                end_pos = buffer.find_first_not_of("0123456789.-");
            }
            auto need_num = buffer.substr(start_pos, end_pos);
            add_number(cord_pair(first_pos, second_pos), string_to_num<T>(need_num));
        }
        in.close();
    }

    template <AnyNum K>
    Matrix& operator=(Matrix<K> other) requires (!std::is_same_v<T, K>)
    {
        eps = other.get_eps();
        size = other.get_size();
        auto copy_map = other.get_numbers();
        nums.clear();
        for (auto it = copy_map.begin(); it != copy_map.end(); it++)
            add_number(it->first, T(it->second));
        return *this;
    }

    Matrix& operator=(const Matrix& other)
    {
        eps = other.get_eps();
        size = other.get_size();
        auto copy_map = other.get_numbers();
        nums.clear();
        for (auto it = copy_map.begin(); it != copy_map.end(); it++)
            add_number(it->first, it->second);
        return *this;
    }

    Matrix& operator=(Matrix&& other) noexcept
    {
        other.last_zero_remove();
        if (&other != this) {
            eps = other.get_eps();
            other.eps = 0;

            size = other.get_size();
            other.size = cord_pair(0, 0);

            other.last_zero_pose = cord_pair(0, 0);

            nums = std::move(other.nums);
        }
        return *this;
    }

    std::string to_string()
    {
        last_zero_remove();
        //zeros_erase();
        std::string res_string("matrix \t\t" + std::to_string(size.first) +
                               "\t" + std::to_string(size.second) + "\n\n");
        for (auto it = nums.begin(); it != nums.end(); it++)
            res_string += std::to_string((it->first).first) + "\t" + std::to_string((it->first).second) +
                          "\t\t" + string_transform((*it).second) + "\n";
        return res_string;
    }


    template <AnyNum K>
    void add_number(cord_pair pos, K val) {
        if (abs(val) >= (is_complex<K>::value ? eps*eps : eps))
            nums[pos] = val;
        else if (nums.find(pos) != nums.end())
            nums.erase(pos);
    }

    void zero_remove(cord_pair pos)
    {
        if ((nums.find(pos) != nums.end()) && (nums[pos] == T(0)))
            nums.erase(pos);
    }

    void last_zero_remove()
    {
        zero_remove(last_zero_pose);
    }

    T& operator()(cord_pair pos)
    {
        //zeros_erase();
        last_zero_remove();
        if (!(pos <= size))
            throw std::logic_error("Request error!");
        if (nums.find(pos) != nums.end())
            return nums[pos];
        nums[pos] = 0;
        last_zero_pose = pos;
        return nums[pos];
    }

    T& operator()(num line_pos, num row_pos)
    {
        return (*this)(cord_pair(line_pos, row_pos));
    }

    Matrix operator~()
    {
        Matrix tr_mat;
        tr_mat.eps = get_eps();
        std::cout << get_size().first << std::endl;
        tr_mat.size = ~get_size();
        tr_mat.nums.clear();
        for (auto it = nums.begin(); it != nums.end(); it++)
            tr_mat.add_number(~(it->first), it->second);
        return tr_mat;
    }

    Matrix operator-()
    {
        Matrix res_mat(*this);
        res_mat.nums.clear();
        for (auto it = nums.begin(); it != nums.end(); it++)
            res_mat.add_number(it->first, -(it->second));
        return res_mat;
    }


    template<AnyNum K>
    Matrix<decltype(T()+K())> operator+(Matrix<K> other_mat)
    {
        using res_type = decltype(T()+K());
        last_zero_remove();
        Matrix<res_type> res_mat(*this);
        auto other_map = other_mat.get_numbers();
        for (auto it = other_map.begin(); it != other_map.end(); it++)
            res_mat.add_number(it->first,
                               res_type(it->second +
                                        (nums.find(it->first) == nums.end() ? res_type(0) :
                                         nums[it->first]) ) );
        return res_mat;
    }

    template<AnyNum K>
    Matrix<decltype(T()-K())> operator-(Matrix<K> other_mat)
    {
        using res_type = decltype(T()-K());
        last_zero_remove();
        Matrix<res_type> res_mat(*this);
        auto other_map = other_mat.get_numbers();
        for (auto it = other_map.begin(); it != other_map.end(); it++)
            res_mat.add_number(it->first,
                               res_type(it->second +
                                        (nums.find(it->first) == nums.end() ? res_type(0) :
                                         nums[it->first]) ) );
        return res_mat;
    }

    template<AnyNum K>
    Matrix<decltype(T()*K())> operator*(Matrix<K> other_mat)
    {
        using res_type = decltype(T()*K());
        last_zero_remove();
        auto first_size = get_size(), second_size = other_mat.get_size();
        if (first_size.second != second_size.first)
            throw std::logic_error("Wrong sizes");

        Matrix<res_type> res_mat(cord_pair(first_size.first, second_size.second),
                                 res_type(0));
        auto other_nums = other_mat.get_numbers();
        std::unordered_map<cord_pair, res_type> res_map;
        for (auto it = nums.begin(); it != nums.end(); it++)
            for (auto s_it = other_nums.begin(); s_it != other_nums.end(); s_it++)
            {
                if ((it->first).second == (s_it->first).first) {

                    auto res_pos = cord_pair((it->first).first, (s_it->first).second);

                    res_map[res_pos] = res_type(((res_map.find(res_pos) == res_map.end()) ?
                                                 res_type(0) : res_map[res_pos]) + (it->second)*(s_it->second));
                }
            }
        res_mat.set_numbers(res_map);
        return res_mat;
    }

    template<AnyNum K>
    Matrix<decltype(T()*K())> operator*(Vector<K> vec)
    {
        last_zero_remove();
        Matrix<K> matrix_vec(vec);
        auto vec_size = vec.get_size();
        if (size.first == vec_size)
            return matrix_vec*(*this);
        else if (size.second == vec_size)
            return (*this)*(~matrix_vec);
        else
            throw std::logic_error("Not correct vector size!");
    }

    Matrix operator[](Matrix_coords boundaries)
    {
        Matrix new_mat;
        new_mat.eps = eps;
        auto cur_numbers = new_mat.get_numbers();
        cur_numbers.clear();

        auto left_boundary = boundaries.get_left();
        if (left_boundary.first == -1)
            left_boundary.first = NUM_BEGIN;
        if (left_boundary.second == -1)
            left_boundary.second = NUM_BEGIN;

        auto right_boundary = boundaries.get_right();
        if (right_boundary.first == -1)
            right_boundary.first = size.first;
        if (right_boundary.second == -1)
            right_boundary.second = size.second;

        auto new_size = right_boundary - left_boundary;
        new_size = new_size + NUM_BEGIN;

        new_mat.size = new_size;

        for (auto it = nums.begin(); it != nums.end(); it++)
        {
            auto pos = it->first;
            if ((pos >= left_boundary) && (pos <= right_boundary))
                cur_numbers[pos-left_boundary+NUM_BEGIN] = it->second;
        }
        new_mat.set_numbers(cur_numbers);
        return new_mat;
    }

    Matrix_proxy<T> operator[](Matrix_row_coord num) {return Matrix_proxy<T>(this, num);};
    Matrix_proxy<T> operator[](Matrix_column_coord num) {return Matrix_proxy<T>(this, num);};

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

    void add_reference(Matrix_proxy<T>* new_ref) {references.push_back(new_ref);}

    ~Matrix() {
        for (auto it = references.begin(); it != references.end(); it++)
            (*it)->object_deleted();
        references.clear();
    }
};

template<AnyNum T>
std::ostream& operator<<(std::ostream& os, Matrix<T> mat) {
    os << mat.to_string();
    return os;
}

template<AnyNum T, AnyNum K>
Matrix<decltype(T()*K())> operator*(Vector<T> vec, Matrix<K> mat)
{
    return mat*vec;
}


template<AnyNum T>
class Matrix_proxy
{

    friend class Matrix<T>;

    Matrix<T>* mat_ptr = nullptr;
    nat_num row_num = 0;
    nat_num column_num = 0;

    Matrix_proxy(Matrix<T>* mat, Matrix_row_coord num) : mat_ptr(mat), row_num(num.val) {};
    Matrix_proxy(Matrix<T>* mat, Matrix_column_coord num) : mat_ptr(mat), column_num(num.val) {};
    void object_deleted() {mat_ptr = nullptr;}

public:

    T& operator()(nat_num pos)
    {
        if (mat_ptr == nullptr)
            throw std::logic_error("Not existing reference");
        if (row_num != 0)
            return (*mat_ptr)(row_num, pos);
        else if (column_num != 0)
            return (*mat_ptr)(pos, column_num);
        throw std::logic_error("Invalid proxy");
    }


    template <AnyNum K>
    operator Vector<K>()
    {
        if (mat_ptr == nullptr)
            throw std::logic_error("Not existing reference");

        bool need_rows = (row_num != 0), need_columns = (column_num != 0);

        Vector<K> new_vec;
        new_vec.set_eps(mat_ptr->get_eps());

        auto mat_size = mat_ptr->get_size();
        auto vec_size = (need_rows) ? mat_size.second : ((need_columns) ? mat_size.first : 0);
        new_vec.set_size(vec_size);

        auto new_vec_nums = new_vec.get_numbers();
        auto mat_nums = mat_ptr -> get_numbers();

        for (auto it = mat_nums.begin(); it != mat_nums.end(); it++)
        {
            auto pos = it->first;
            auto cur_pos = (need_rows) ? pos.second : ((need_columns) ? pos.first : 0);
            if ((need_rows && (pos.first == row_num)) || (need_columns && (pos.second == column_num)))
                new_vec.add_number(cur_pos, K(it->second));
        }
        return new_vec;
    }
};


#endif //SALNIKOV_TASK_MATRIX_H