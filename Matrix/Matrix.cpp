#include "Matrix.h"

bool operator>(cord_pair first, cord_pair second) {return (first.first > second.first) &&
                                                          (first.second > second.second);}

bool operator<(cord_pair first, cord_pair second) {return (first.first < second.first) &&
                                                          (first.second < second.second);}

bool operator>=(cord_pair first, cord_pair second) {return (first.first >= second.first) &&
                                                           (first.second >= second.second);}

bool operator<=(cord_pair first, cord_pair second) {return (first.first <= second.first) &&
                                                           (first.second <= second.second);}

bool operator==(cord_pair first, cord_pair second) {return (first.first == second.first) &&
                                                           (first.second == second.second);}

bool operator!=(cord_pair first, cord_pair second) {return !(first == second);}

cord_pair operator~(cord_pair changed) {return cord_pair(changed.second, changed.first);}

cord_pair operator-(cord_pair first, cord_pair second)
{
    return cord_pair(first.first - second.first, first.second - second.second);
}

cord_pair operator+(cord_pair pair_num, nat_num num)
{
    return cord_pair(pair_num.first + num, pair_num.second + num);
}



Matrix_coords::Matrix_coords(num r_1, num c_1, num r_2, num c_2) {

    r_2 = (r_2 == -2) ? r_1 : r_2;
    c_2 = (c_2 == -2) ? c_1 : c_2;
    if ((r_1 > r_2) && (r_2 != -1))
        std::swap<num>(r_1, r_2);
    if ((c_1 > c_2) && (c_2 != -1))
        std::swap<num>(c_1, c_2);
    if ((c_1 < -1) || (r_1 < -1))
        throw std::logic_error("Wrong coordinates");
    left_corner = cord_pair(r_1, c_1);
    right_corner = cord_pair(r_2, c_2);
}

Matrix_coords::Matrix_coords(cord_pair first, cord_pair second) {

    second = (second == cord_pair(-2, -2)) ? first : second;
    if ((first.first > second.first) && (second.first != -1))
        std::swap<num>(first.first, second.first);
    if ((first.second > second.second) && (second.second != -1))
        std::swap<num>(first.second, second.second);
    if ((first.first < -1) || (first.second < -2))
        throw std::logic_error("Wrong coordinates");
    left_corner = first;
    right_corner = second;
}