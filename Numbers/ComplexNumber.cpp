//
//#include "ComplexNumber.h"
//
//template<RatNumeric R, RatNumeric I>
//ComplexNumber<R,I> ComplexNumber<R,I>::operator+(const ComplexNumber<R, I> &num) const {
//    return ComplexNumber<R,I>(get_real() + num.get_real(), get_imag() + num.get_imag());
//}
//
//template<RatNumeric R, RatNumeric I>
//ComplexNumber<R,I> ComplexNumber<R,I>::operator-(const ComplexNumber<R, I> &num) const {
//    return ComplexNumber<R,I>(get_real() - num.get_real(), get_imag() - num.get_imag());
//}
//
//template<RatNumeric R, RatNumeric I>
//ComplexNumber<R,I> ComplexNumber<R,I>::operator*(const ComplexNumber<R, I> &num) const {
//    return ComplexNumber<R,I>(get_real() * num.get_real() - get_imag() * num.get_imag(),
//                              get_real()*num.get_imag() + get_imag().get_real());
//}
//
//template<RatNumeric R, RatNumeric I>
//ComplexNumber<R,I> ComplexNumber<R,I>::operator/(const ComplexNumber<R, I> &num) const {
//    auto sqr_val = [](I val) {return val * val;};
//    I rad_val = sqr_val(num.get_real()) + sqr_val(num.get_imag());
//    return ComplexNumber<R,I>((get_real() * num.get_real() + get_imag() * num.get_imag()) / rad_val,
//                              (get_imag()*num.get_real() - get_real()*num.get_imag())/rad_val);
//}
//
//template<RatNumeric R, RatNumeric I>
//ComplexNumber<R,I> ComplexNumber<R,I>::operator-() const {
//    return ComplexNumber<R,I>(-get_real(), -get_imag());
//}
//
//
//template<RatNumeric R, RatNumeric I>
//const ComplexNumber<R,I> ComplexNumber<R,I>::operator++(int) &{
//    RationalNumber old_val(*this);
//    *this = old_val + 1;
//    return old_val;
//}
//
//template<RatNumeric R, RatNumeric I>
//const ComplexNumber<R,I> ComplexNumber<R,I>::operator--(int) &{
//    RationalNumber old_val(*this);
//    *this = old_val - 1;
//    return old_val;
//}
//
//template<RatNumeric R, RatNumeric I>
//ComplexNumber<R, I> ComplexNumber<R,I>::round()
//{
//    return ComplexNumber<R, I>(std::is_integral<R>::value ? get_real() : get_real().round(),
//                               std::is_integral<I>::value ? get_real() : get_imag().round());
//}
//
//template<RatNumeric R, RatNumeric I>
//ComplexNumber<R, I> ComplexNumber<R,I>::ceil()
//{
//    return ComplexNumber<R, I>(std::is_integral<R>::value ? get_real() : get_real().ceil(),
//                               std::is_integral<I>::value ? get_real() : get_imag().ceil());
//}
//
//template<RatNumeric R, RatNumeric I>
//ComplexNumber<R, I> ComplexNumber<R,I>::floor()
//{
//    return ComplexNumber<R, I>(std::is_integral<R>::value ? get_real() : get_real().floor(),
//                               std::is_integral<I>::value ? get_real() : get_imag().floor());
//}
//
//template<RatNumeric R, RatNumeric I>
//std::string ComplexNumber<R,I>::to_string(){
//    std::string res_string("(");
//    if (std::is_arithmetic<R>::value)
//        res_string += std::to_string(number.first);
//    else
//        res_string += number.first.to_string();
//    res_string += ", ";
//    if (std::is_arithmetic<I>::value)
//        res_string += std::to_string(number.second);
//    else
//        res_string += number.second.to_string();
//    res_string += ")";
//    return res_string;
//}
//
//template<RatNumeric R, RatNumeric I>
//std::ostream& operator<<(std::ostream& os, const ComplexNumber<R, I>& number) {
//    os << number.to_string();
//    return os;
//    /*os << number.get_real();
//    if (number.get_imag() != 0)
//        os << " + " << number.get_imag() << "i";
//    return os;*/
//}
