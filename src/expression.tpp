#include "expression.hpp"

// Реализация шаблонных функций

template <typename T>
Expression<T> make_val(T val) {
    return Expression<T>(std::make_shared<Number<T>>(val));
}

template <typename T>
Expression<T> make_var(const std::string& name) {
    return Expression<T>(std::make_shared<Variable<T>>(name));
}

// Литералы для double
Expression<double> operator""_val(long double val) {
    return make_val(static_cast<double>(val));
}

Expression<double> operator""_var(const char* name, size_t) {
    return make_var<double>(std::string(name));
}

// Литералы для комплексных чисел
Expression<std::complex<double>> operator""_val(unsigned long long int val) {
    return make_val(std::complex<double>(static_cast<double>(val), 0.0));
}

Expression<std::complex<double>> operator""_var(const char* name, size_t) {
    return make_var<std::complex<double>>(std::string(name));
}
