#include "expression.hpp"
#include <sstream>
#include <cmath>
#include <type_traits>

/*
    Реализация методов класса Expression<T>
*/

template<typename T>
Expression<T>::Expression(const std::string &variable)
    : impl_(std::make_shared<Variable<T> >(variable)) {
}

template<typename T>
Expression<T>::Expression(T value)
    : impl_(std::make_shared<Value<T> >(value)) {
}

template<typename T>
Expression<T>::Expression(const Expression<T> &other)
    : impl_(other.impl_->clone()) {
}

template<typename T>
Expression<T>::Expression(Expression<T> &&other) noexcept
    : impl_(std::move(other.impl_)) {
}

template<typename T>
Expression<T> &Expression<T>::operator=(const Expression<T> &other) {
    if (this != &other) {
        impl_ = other.impl_->clone();
    }
    return *this;
}

template<typename T>
Expression<T> &Expression<T>::operator=(Expression<T> &&other) noexcept {
    if (this != &other) {
        impl_ = std::move(other.impl_);
    }
    return *this;
}

template<typename T>
Expression<T>::Expression(std::shared_ptr<ExpressionImpl<T> > impl)
    : impl_(impl) {
}

template<typename T>
Expression<T> Expression<T>::operator+(const Expression<T> &right) const {
    return Expression<T>(std::make_shared<OperationAdd<T> >(*this, right));
}

template<typename T>
Expression<T> Expression<T>::operator-(const Expression<T> &right) const {
    return Expression<T>(std::make_shared<OperationSub<T> >(*this, right));
}

template<typename T>
Expression<T> Expression<T>::operator*(const Expression<T> &right) const {
    return Expression<T>(std::make_shared<OperationMul<T> >(*this, right));
}

template<typename T>
Expression<T> Expression<T>::operator/(const Expression<T> &right) const {
    return Expression<T>(std::make_shared<OperationDiv<T> >(*this, right));
}

template<typename T>
Expression<T> Expression<T>::operator^(const Expression<T> &right) const {
    return Expression<T>(std::make_shared<OperationPow<T> >(*this, right));
}

template<typename T>
Expression<T> &Expression<T>::operator+=(const Expression<T> &right) {
    *this = *this + right;
    return *this;
}

template<typename T>
Expression<T> &Expression<T>::operator-=(const Expression<T> &right) {
    *this = *this - right;
    return *this;
}

template<typename T>
Expression<T> &Expression<T>::operator*=(const Expression<T> &right) {
    *this = *this * right;
    return *this;
}

template<typename T>
Expression<T> &Expression<T>::operator/=(const Expression<T> &right) {
    *this = *this / right;
    return *this;
}

template<typename T>
Expression<T> &Expression<T>::operator^=(const Expression<T> &right) {
    *this = *this ^ right;
    return *this;
}

template<typename T>
T Expression<T>::eval(const std::map<std::string, T> &context) const {
    return impl_->eval(context);
}

template<typename T>
std::string Expression<T>::to_string() const {
    return impl_->to_string();
}

template<typename T>
Expression<T> Expression<T>::substitute(const std::string &var, const Expression<T> &expr) const {
    return impl_->substitute(var, expr);
}

template<typename T>
Expression<T> Expression<T>::differentiate(const std::string &var) const {
    return impl_->derivative(var);
}

// ===================================================================

/*
    Реализация класса Value<T>
*/

template<typename T>
Value<T>::Value(T value)
    : value_(value) {
}

template<typename T>
T Value<T>::eval(const std::map<std::string, T> &) const {
    return value_;
}

template<typename T>
std::string Value<T>::to_string() const {
    std::ostringstream oss;
    oss << value_;
    return oss.str();
}

template<typename T>
Expression<T> Value<T>::derivative(const std::string &) const {
    return Expression<T>(T(0));
}

template<typename T>
Expression<T> Value<T>::substitute(const std::string &, const Expression<T> &) const {
    return Expression<T>(value_);
}

template<typename T>
std::shared_ptr<ExpressionImpl<T> > Value<T>::clone() const {
    return std::make_shared<Value<T> >(value_);
}

// ===================================================================

/*
    Реализация класса Variable<T>
*/

template<typename T>
Variable<T>::Variable(const std::string &name)
    : name_(name) {
}

template<typename T>
T Variable<T>::eval(const std::map<std::string, T> &context) const {
    auto it = context.find(name_);
    if (it == context.end()) {
        throw std::runtime_error("Variable \"" + name_ + "\" not found in context");
    }
    return it->second;
}

template<typename T>
std::string Variable<T>::to_string() const {
    return name_;
}

template<typename T>
Expression<T> Variable<T>::derivative(const std::string &var) const {
    return (name_ == var) ? Expression<T>(T(1)) : Expression<T>(T(0));
}

template<typename T>
Expression<T> Variable<T>::substitute(const std::string &var, const Expression<T> &expr) const {
    return (name_ == var) ? expr : Expression<T>(std::make_shared<Variable<T> >(name_));
}

template<typename T>
std::shared_ptr<ExpressionImpl<T> > Variable<T>::clone() const {
    return std::make_shared<Variable<T> >(name_);
}

// ===================================================================

/*
    Реализация класса BinaryOperation<T>
*/

template<typename T>
BinaryOperation<T>::BinaryOperation(const Expression<T> &left, const Expression<T> &right)
    : left_(left), right_(right) {
}


template<typename T>
OperationAdd<T>::OperationAdd(const Expression<T> &left, const Expression<T> &right)
    : BinaryOperation<T>(left, right) {
}

template<typename T>
T OperationAdd<T>::eval(const std::map<std::string, T> &context) const {
    return this->left_.eval(context) + this->right_.eval(context);
}

template<typename T>
std::string OperationAdd<T>::to_string() const {
    return "(" + this->left_.to_string() + " + " + this->right_.to_string() + ")";
}

template<typename T>
Expression<T> OperationAdd<T>::derivative(const std::string &var) const {
    return this->left_.differentiate(var) + this->right_.differentiate(var);
}

template<typename T>
Expression<T> OperationAdd<T>::substitute(const std::string &var, const Expression<T> &expr) const {
    return this->left_.substitute(var, expr) + this->right_.substitute(var, expr);
}

template<typename T>
std::shared_ptr<ExpressionImpl<T> > OperationAdd<T>::clone() const {
    return std::make_shared<OperationAdd<T> >(this->left_, this->right_);
}


template<typename T>
OperationSub<T>::OperationSub(const Expression<T> &left, const Expression<T> &right)
    : BinaryOperation<T>(left, right) {
}

template<typename T>
T OperationSub<T>::eval(const std::map<std::string, T> &context) const {
    return this->left_.eval(context) - this->right_.eval(context);
}

template<typename T>
std::string OperationSub<T>::to_string() const {
    return "(" + this->left_.to_string() + " - " + this->right_.to_string() + ")";
}

template<typename T>
Expression<T> OperationSub<T>::derivative(const std::string &var) const {
    return this->left_.differentiate(var) - this->right_.differentiate(var);
}

template<typename T>
Expression<T> OperationSub<T>::substitute(const std::string &var, const Expression<T> &expr) const {
    return this->left_.substitute(var, expr) - this->right_.substitute(var, expr);
}

template<typename T>
std::shared_ptr<ExpressionImpl<T> > OperationSub<T>::clone() const {
    return std::make_shared<OperationSub<T> >(this->left_, this->right_);
}


template<typename T>
OperationMul<T>::OperationMul(const Expression<T> &left, const Expression<T> &right)
    : BinaryOperation<T>(left, right) {
}

template<typename T>
T OperationMul<T>::eval(const std::map<std::string, T> &context) const {
    return this->left_.eval(context) * this->right_.eval(context);
}

template<typename T>
std::string OperationMul<T>::to_string() const {
    return "(" + this->left_.to_string() + " * " + this->right_.to_string() + ")";
}

template<typename T>
Expression<T> OperationMul<T>::derivative(const std::string &var) const {
    return this->left_.differentiate(var) * this->right_ + this->left_ * this->right_.differentiate(var);
}

template<typename T>
Expression<T> OperationMul<T>::substitute(const std::string &var, const Expression<T> &expr) const {
    return this->left_.substitute(var, expr) * this->right_.substitute(var, expr);
}

template<typename T>
std::shared_ptr<ExpressionImpl<T> > OperationMul<T>::clone() const {
    return std::make_shared<OperationMul<T> >(this->left_, this->right_);
}


template<typename T>
OperationDiv<T>::OperationDiv(const Expression<T> &left, const Expression<T> &right)
    : BinaryOperation<T>(left, right) {
}

template<typename T>
T OperationDiv<T>::eval(const std::map<std::string, T> &context) const {
    T denominator = this->right_.eval(context);
    if (denominator == T(0))
        throw std::runtime_error("Division by zero");
    return this->left_.eval(context) / denominator;
}

template<typename T>
std::string OperationDiv<T>::to_string() const {
    return "(" + this->left_.to_string() + " / " + this->right_.to_string() + ")";
}

template<typename T>
Expression<T> OperationDiv<T>::derivative(const std::string &var) const {
    // Правило частного: (f'g - fg') / g^2
    return (this->left_.differentiate(var) * this->right_ - this->left_ * this->right_.differentiate(var))
           / (this->right_ ^ Expression<T>(T(2)));
}

template<typename T>
Expression<T> OperationDiv<T>::substitute(const std::string &var, const Expression<T> &expr) const {
    return this->left_.substitute(var, expr) / this->right_.substitute(var, expr);
}

template<typename T>
std::shared_ptr<ExpressionImpl<T> > OperationDiv<T>::clone() const {
    return std::make_shared<OperationDiv<T> >(this->left_, this->right_);
}


template<typename T>
OperationPow<T>::OperationPow(const Expression<T> &left, const Expression<T> &right)
    : BinaryOperation<T>(left, right) {
}

template<typename T>
T OperationPow<T>::eval(const std::map<std::string, T> &context) const {
    T base = this->left_.eval(context);
    T exponent = this->right_.eval(context);
    return std::pow(base, exponent);
}

template<typename T>
std::string OperationPow<T>::to_string() const {
    return "(" + this->left_.to_string() + " ^ " + this->right_.to_string() + ")";
}

template<typename T>
Expression<T> OperationPow<T>::derivative(const std::string &var) const {
    // Общая формула дифференцирования: d/dx(f^g) = f^g * (g' * ln(f) + g * f'/f)
    Expression<T> f = this->left_;
    Expression<T> g = this->right_;
    return Expression<T>(this->clone()) * (g.differentiate(var) * ln(f) + g * (f.differentiate(var) / f));
}

template<typename T>
Expression<T> OperationPow<T>::substitute(const std::string &var, const Expression<T> &expr) const {
    return this->left_.substitute(var, expr) ^ this->right_.substitute(var, expr);
}

template<typename T>
std::shared_ptr<ExpressionImpl<T> > OperationPow<T>::clone() const {
    return std::make_shared<OperationPow<T> >(this->left_, this->right_);
}


// Функция sin: sin(f)
template<typename T>
FunctionSin<T>::FunctionSin(const Expression<T> &arg)
    : arg_(arg) {
}

template<typename T>
T FunctionSin<T>::eval(const std::map<std::string, T> &context) const {
    return std::sin(arg_.eval(context));
}

template<typename T>
std::string FunctionSin<T>::to_string() const {
    return "sin(" + arg_.to_string() + ")";
}

template<typename T>
Expression<T> FunctionSin<T>::derivative(const std::string &var) const {
    // Производная sin(f) = cos(f) * f'
    return cos(arg_) * arg_.differentiate(var);
}

template<typename T>
Expression<T> FunctionSin<T>::substitute(const std::string &var, const Expression<T> &expr) const {
    return sin(arg_.substitute(var, expr));
}

template<typename T>
std::shared_ptr<ExpressionImpl<T> > FunctionSin<T>::clone() const {
    return std::make_shared<FunctionSin<T> >(arg_);
}


// Функция cos: cos(f)
template<typename T>
FunctionCos<T>::FunctionCos(const Expression<T> &arg)
    : arg_(arg) {
}

template<typename T>
T FunctionCos<T>::eval(const std::map<std::string, T> &context) const {
    return std::cos(arg_.eval(context));
}

template<typename T>
std::string FunctionCos<T>::to_string() const {
    return "cos(" + arg_.to_string() + ")";
}

template<typename T>
Expression<T> FunctionCos<T>::derivative(const std::string &var) const {
    // Производная cos(f) = -sin(f) * f'
    return Expression<T>(T(-1)) * sin(arg_) * arg_.differentiate(var);
}

template<typename T>
Expression<T> FunctionCos<T>::substitute(const std::string &var, const Expression<T> &expr) const {
    return cos(arg_.substitute(var, expr));
}

template<typename T>
std::shared_ptr<ExpressionImpl<T> > FunctionCos<T>::clone() const {
    return std::make_shared<FunctionCos<T> >(arg_);
}


// Функция ln: ln(f)
template<typename T>
FunctionLn<T>::FunctionLn(const Expression<T> &arg)
    : arg_(arg) {
}

template<typename T>
T FunctionLn<T>::eval(const std::map<std::string, T> &context) const {
    T val = arg_.eval(context);
    if constexpr (std::is_floating_point_v<T>) {
        if (val <= T(0))
            throw std::runtime_error("Logarithm of non-positive value");
    }
    return std::log(val);
}

template<typename T>
std::string FunctionLn<T>::to_string() const {
    return "ln(" + arg_.to_string() + ")";
}

template<typename T>
Expression<T> FunctionLn<T>::derivative(const std::string &var) const {
    // Производная ln(f) = f'/f
    return arg_.differentiate(var) / arg_;
}

template<typename T>
Expression<T> FunctionLn<T>::substitute(const std::string &var, const Expression<T> &expr) const {
    return ln(arg_.substitute(var, expr));
}

template<typename T>
std::shared_ptr<ExpressionImpl<T> > FunctionLn<T>::clone() const {
    return std::make_shared<FunctionLn<T> >(arg_);
}


// Функция exp: exp(f)
template<typename T>
FunctionExp<T>::FunctionExp(const Expression<T> &arg)
    : arg_(arg) {
}

template<typename T>
T FunctionExp<T>::eval(const std::map<std::string, T> &context) const {
    return std::exp(arg_.eval(context));
}

template<typename T>
std::string FunctionExp<T>::to_string() const {
    return "exp(" + arg_.to_string() + ")";
}

template<typename T>
Expression<T> FunctionExp<T>::derivative(const std::string &var) const {
    // Производная exp(f) = exp(f) * f'
    return exp(arg_) * arg_.differentiate(var);
}

template<typename T>
Expression<T> FunctionExp<T>::substitute(const std::string &var, const Expression<T> &expr) const {
    return exp(arg_.substitute(var, expr));
}

template<typename T>
std::shared_ptr<ExpressionImpl<T> > FunctionExp<T>::clone() const {
    return std::make_shared<FunctionExp<T> >(arg_);
}

// ===================================================================
// Функции для создания функциональных выражений.
template<typename T>
Expression<T> sin(const Expression<T> &arg) {
    return Expression<T>(std::make_shared<FunctionSin<T> >(arg));
}

template<typename T>
Expression<T> cos(const Expression<T> &arg) {
    return Expression<T>(std::make_shared<FunctionCos<T> >(arg));
}

template<typename T>
Expression<T> ln(const Expression<T> &arg) {
    return Expression<T>(std::make_shared<FunctionLn<T> >(arg));
}

template<typename T>
Expression<T> exp(const Expression<T> &arg) {
    return Expression<T>(std::make_shared<FunctionExp<T> >(arg));
}


// Литералы для создания выражений с действительными числами.
Expression<long double> operator"" _val(const long double val) {
    return Expression<long double>(val);
}

Expression<long double> operator"" _var(const char *variable) {
    return Expression<long double>(std::string(variable));
}

Expression<long double> operator"" _var(const char *variable, size_t) {
    return Expression<long double>(std::string(variable));
}

// ===================================================================
// Инстанциация шаблонов для long double и std::complex<long double>
template class Expression<long double>;
template class Value<long double>;
template class Variable<long double>;
template class BinaryOperation<long double>;
template class OperationAdd<long double>;
template class OperationSub<long double>;
template class OperationMul<long double>;
template class OperationDiv<long double>;
template class OperationPow<long double>;
template class FunctionSin<long double>;
template class FunctionCos<long double>;
template class FunctionLn<long double>;
template class FunctionExp<long double>;

template Expression<long double> sin<long double>(const Expression<long double> &);
template Expression<long double> cos<long double>(const Expression<long double> &);
template Expression<long double> ln<long double>(const Expression<long double> &);
template Expression<long double> exp<long double>(const Expression<long double> &);

template class Expression<std::complex<long double> >;
template class Value<std::complex<long double> >;
template class Variable<std::complex<long double> >;
template class BinaryOperation<std::complex<long double> >;
template class OperationAdd<std::complex<long double> >;
template class OperationSub<std::complex<long double> >;
template class OperationMul<std::complex<long double> >;
template class OperationDiv<std::complex<long double> >;
template class OperationPow<std::complex<long double> >;
template class FunctionSin<std::complex<long double> >;
template class FunctionCos<std::complex<long double> >;
template class FunctionLn<std::complex<long double> >;
template class FunctionExp<std::complex<long double> >;

template Expression<std::complex<long double> > sin<std::complex<long double> >(
    const Expression<std::complex<long double> > &);

template Expression<std::complex<long double> > cos<std::complex<long double> >(
    const Expression<std::complex<long double> > &);

template Expression<std::complex<long double> > ln<std::complex<long double> >(
    const Expression<std::complex<long double> > &);

template Expression<std::complex<long double> > exp<std::complex<long double> >(
    const Expression<std::complex<long double> > &);
