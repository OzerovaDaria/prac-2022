#include <iostream>
#include <fstream>
#include <atomic>
#include "./tests/test_rational_number.cpp"
#include "Numbers/Rational.h"
#include "Numbers/ComplexNumber.h"
#include "Matrix/Vector.h"
#include "Matrix/Matrix.h"

int main() {

    auto dzrlva = RationalNumber();
    auto second_num = RationalNumber(-500);
    auto cool_num = RationalNumber("3", "5");
    auto simplify_num = RationalNumber(10, -5);
    auto super_simplify = RationalNumber(-25, 500);
    long_number smthcheck("-123535352626200");
    std::cout << smthcheck.to_string() << std::endl;
    int simple_num = -14;
    unsigned long other_num = 11;
    auto test_comp = ComplexNumber<double, RationalNumber>(3.4, simplify_num);
    auto other_test = ComplexNumber<long_number, unsigned long>(smthcheck, other_num);
    auto check_type = ComplexNumber<RationalNumber, RationalNumber>(1.2, simplify_num);

    Vector<ComplexNumber<RationalNumber, RationalNumber>> scary;

    //Vector<int> check_int("int_vec.txt");

    //std::cout << check_int << std::endl;

//    Matrix<int> smth(7);
//    std::cout << smth.to_string() << std::endl;
//
//    smth(5, 5) = 10;
//    smth(5, 7) = 123;
//
//    std::cout << smth.to_string() << std::endl;
//
//    std::cout << (smth + smth).to_string() << std::endl;
//
//    std::cout << (smth * smth).to_string() << std::endl;

//    Matrix<num> first(cord_pair(3,3)), second(cord_pair(3,3));
//
//    first(1,1) = 5;
//    first(1,2) = 6;
//    first(1,3) = 10;
//
//    first(2, 1) = 7;
//    first(2, 2) = -3;
//    first(2, 3) = 55;
//
//    first(3, 1) = -2;
//    first(3, 2) = 1;
//    first(3, 3) = 23;
//
//    std::cout << first.to_string() << std::endl;
//
//    second(1,1) = 3;
//    second(1,2) = 66;
//    second(1,3) = -1;
//
//    second(2, 1) = 32;
//    second(2, 2) = -1;
//    second(2, 3) = 11;
//
//    second(3, 1) = 1;
//    second(3, 2) = 0;
//    second(3, 3) = 24;
//
//    std::cout << second << std::endl;

    Matrix<num> checking("dzrlva.txt");

    std::cout << checking << std::endl;

    auto proxy_check = checking[Matrix_row_coord(1)];

    proxy_check(1) = 11;

    std::cout << checking << std::endl;

    Vector<int> pef = proxy_check;

    std::cout << pef << std::endl;

//    Matrix_coords left_corner(1, 2, 2, -1);
//
//    std::cout << checking[left_corner] << std::endl;

    //(~(first*second)).to_file("dzrlva.txt");


   // return 0;


    //return 0;

    //std::cout << check_type << std::endl;

//    ComplexNumber<int, int> check_amb("124");
//    ComplexNumber<double, double> check_fl("12.4");
//    ComplexNumber<int, int> check_si("-13");
//    ComplexNumber<long_number, long_number> check_ln("325235235");
//      ComplexNumber<double, RationalNumber> check_rat("( 1.124,   <-13/14>   )");
//      std::cout << check_rat << std::endl;
//    std::cout << check_amb << std::endl;
//    std::cout << check_fl << std::endl;
//    std::cout << check_si << std::endl;
//    std::cout << check_ln << std::endl;
//    std::cout << check_rat << std::endl;
//
//
//    scary.add_number(5, ComplexNumber<int, double>(10, 3.2));
//    scary.add_number(10, ComplexNumber<int, double>(123, 6.123));
//
//    RationalNumber checking("123");
//    RationalNumber checking_two("<123>");
//    RationalNumber checking_three("123/12");
//    RationalNumber checking_four("<123/12>");
//
//    std::cout << checking << ", " << checking_two << ", " << checking_three << ", " << checking_four << std::endl;

//    ComplexNumber<int, int> check_amb("1");
//
//    auto check = ComplexNumber<double, double>(0.001, 0.001);
//
//    scary.add_number(7, test_comp);
//    scary.add_number(3, other_test);
//
//    Vector<int> other_scary;
//
//    other_scary.add_number(5, 53);
//    other_scary.add_number(3, -34);
//
//    other_scary.to_file("int_vec.txt");
//
//    exit(0);

    std::cout << RationalNumber(0) - RationalNumber(53) << std::endl;

//    auto res_vec = (scary - other_scary + cool_num)/10;
//
//    std::cout << scary.to_string() << std::endl;
//    std::cout << other_scary.to_string() << std::endl;
//     std::cout << (res_vec).to_string() << std::endl;
//    std::cout << res_vec(3) << std::endl;
//
//    Vector<long_number> long_vector;
//    long_vector.add_number(4, 100);
//    long_vector.add_number(10, -49);
//    long_vector.add_number(34, 1);
//
//    Vector<double> sinister;
//    sinister.add_number(2, 0.23);
//    sinister.add_number(4, -13.123);
//
//    auto other_vec(long_vector);
//    //std::cout << other_vec.to_string() << std::endl;
//
//    Vector<long_number> other_long_vec(long_vector);
//
//    //std::cout << other_long_vec.to_string() << std::endl;
//
//    //other_vec = other_vec = other_vec;
//
//    //std::cout << other_vec.to_string() << std::endl;
//
//    ComplexNumber<double, double> other_check(check);
//    //std::cout << check << std::endl;
//
//    check_type = check_type = check_type;
//    //std::cout << check_type.to_string() << std::endl;
//    check_type = 5;
//    //std::cout << check_type.to_string() << std::endl;
//
//    //std::cout << other_vec.to_string() << std::endl;
//    auto move_check = std::move(other_vec);
//    //std::cout << move_check.to_string() << std::endl;
//
//    move_check = std::move(std::forward<decltype(move_check)>(move_check));
//
//    //std::cout << move_check.to_string() << std::endl;
//
//    Vector<long_number> move_vec;
//    move_vec.add_number(32, 124);
//    move_vec.add_number(3, -5);
//
//    move_vec(3) = -125;
//    move_vec(25) = 42;
//    move_vec(30), move_vec(10), move_vec(11);
//
//    std::cout << move_vec.to_string() << std::endl << std::endl << std::endl;

    //std::cout << move_vec.to_string() << std::endl;

    //move_check = std::move(std::forward<decltype(move_check)>(move_vec));

    //std::cout << move_check.to_string() << std::endl;
    //std::cout << move_vec << std::endl;

//    std::string filename("output.txt");
//    std::ofstream out(filename);
//    if (!out)
//    {
//        std::cout << "WHAT" << std::endl;
//        return 10;
//    }
//    //out.open(filename, std::ofstream::out | std::ofstream::trunc);
//    out << move_check << std::endl;

   // out.close();

    //scary.to_file("output.txt");
//    move_check.to_file("double_check.txt");
//
//    Vector<RationalNumber> file_check("rat.txt");
//    std::cout << std::endl << std::endl;
//    std::cout << file_check << std::endl;

//    std::ifstream in(filename);
//    std::string check_str;
//    in >> check_str;
//    std::cout << "String is: \n" << check_str << std::endl;
//    in.close();

    //std::cout << (scary + check).to_string() << std::endl;

    //std::cout << (long_vector).to_string() << std::endl;
    //std::cout << (long_vector+5).to_string() << std::endl;

   // auto smth = scary + other_scary;


    //auto other_smth = test_comp + 5;
    //std::cout << other_smth << std::endl;


    /*test_comp = test_comp = test_comp = test_comp;

    std::cout << test_comp << std::endl;

    std::cout << test_comp.abs() << std::endl;

    std::cout << test_comp / other_test << std::endl;*/

    //std::is_same_v<AnyNum<decltype(test_comp)>, int>;
    //static_assert(bool(is_specialization<decltype(test_comp), RationalNumber>), "check that");
    //Vector<ComplexNumber<int, RationalNumber>> prim;

    return 0;
}
