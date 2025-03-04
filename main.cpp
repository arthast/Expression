#include "expression.hpp"
#include <stdexcept>
#include <utility>
#include <math.h>

// Реализация класса Expression
template <typename T>
Expression<T>::Expression(std::shared_ptr<ExpressionImpl<T>> impl) : impl_ (impl) {}

template <typename T>
Expression<T>::Expression(std::string variable) : impl_(std::make_shared<Variable<T>>(variable)) {}

template <typename T>
std::string Expression<T>::toString() const {
    return impl_->toString();
}

template <typename T>
T Expression<T>::evaluate(const std::map<std::string, T>& variables) const {
    return impl_->evaluate(variables);
}

// Expression Expression::derivative(const std::string& var) const {
//     return Expression(impl_->derivative(var));
// }

template <typename T>
Expression<T> Expression<T>::operator+(const Expression& rhs) const {
    return Expression(std::make_shared<OperationAdd>(*this, rhs));
}

template <typename T>
Expression<T>& Expression<T>::operator+=(const Expression& rhs) {
    *this = *this + rhs;
    return *this;
}

template <typename T>
Expression<T> Expression<T>::operator*(const Expression& rhs) const {
    return Expression(std::make_shared<OperationMul>(*this, rhs));
}

template <typename T>
Expression<T>& Expression<T>::operator*=(const Expression& rhs) {
    *this = *this * rhs;
    return *this;
}

template <typename T>
Expression<T> operator""_val(T val) {
    return Expression(std::make_shared<Number<T>>(static_cast<T>(val)));
}

template <typename T>
Expression<T> operator""_var(const char* name, size_t) {
    return Expression<T>(std::make_shared<Variable<T>>(std::string(name)));
}

// Реализация класса Number
template <typename T>
Number<T>::Number(T val) : value(val) {}

template <typename T>
std::string Number<T>::toString() const {
    return std::to_string(value);
}

template <typename T>
T Number<T>::evaluate(const std::map<std::string, T>&) const {
    return value;
}

// std::unique_ptr<ExpressionImpl> Number::derivative(const std::string&) const {
//     return std::make_unique<Number>(0);
// }

// Реализация класса Variable
template <typename T>
Variable<T>::Variable(const std::string& varName) : name(varName) {}

template <typename T>
std::string Variable<T>::toString() const {
    return name;
}

template <typename T>
T Variable<T>::evaluate(const std::map<std::string, T>& variables) const {
    auto iter = variables.find(name);
    if (iter == variables.end()) {
        throw std::runtime_error("Variable '" + name + "' not found in context");
    }
    return iter->second;
}

// std::unique_ptr<ExpressionImpl> Variable::derivative(const std::string& var) const {
//     return std::make_unique<Number>(name == var ? 1 : 0);
// }

// Реализация класса OperationAdd
template <typename T>
OperationAdd<T>::OperationAdd(const Expression<T> &l, const Expression<T> &r) : left (l), right (r) {}

template <typename T>
std::string OperationAdd<T>::toString() const {
    return "(" + left.toString() + " + " + right.toString() + ")";
}

template <typename T>
T OperationAdd<T>::evaluate(const std::map<std::string, T>& variables) const {
    return left.evaluate(variables) + right.evaluate(variables);
}

// std::unique_ptr<ExpressionImpl> OperationAdd::derivative(const std::string& var) const {
//     return std::make_unique<OperationAdd>(left.derivative(var), right.derivative(var));
// }

// Реализация класса OperationMul
template <typename T>
OperationMul<T>::OperationMul(const Expression<T> &l, const Expression<T> &r) : left (l), right (r) {}

template <typename T>
std::string OperationMul<T>::toString() const {
    return "(" + left.toString() + " * " + right.toString() + ")";
}

template <typename T>
T OperationMul<T>::evaluate(const std::map<std::string, T>& variables) const {
    return left.evaluate(variables) * right.evaluate(variables);
}

//std::unique_ptr<ExpressionImpl> OperationMul::derivative(const std::string& var) const {
//    return std::make_unique<OperationAdd>(
//        std::make_unique<OperationMul>(left.derivative(var), right),
//        std::make_unique<OperationMul>(left, right.derivative(var))
//    );
//}

// ========================== //
// Реализация класса Sin      //
// ========================== //

template <typename T>
Sin<T>::Sin(const Expression<T> &argument) : arg (argument) {}

template <typename T>
std::string Sin<T>::toString() const {
    return "sin(" + arg.toString() + ")";
}

template <typename T>
T Sin<T>::evaluate(const std::map<std::string, T>& variables) const {
    return std::sin(arg.evaluate(variables));
}

// std::unique_ptr<ExpressionImpl> Sin::derivative(const std::string& var) const {
//     return std::make_unique<OperationMul>(cos(arg), arg.derivative(var));
// }

// Функция для создания sin-выражения
template <typename T>
Expression<T> sin(Expression<T> arg) {
    return Expression(std::make_shared<Sin>(arg));
}

// ========================== //
// Реализация класса Cos      //
// ========================== //

template <typename T>
Cos<T>::Cos(const Expression<T> &argument) : arg(argument) {}

template <typename T>
std::string Cos<T>::toString() const {
    return "cos(" + arg.toString() + ")";
}

template <typename T>
T Cos<T>::evaluate(const std::map<std::string, T>& variables) const {
    return std::cos(arg.evaluate(variables));
}

// template <typename T>
// std::unique_ptr<ExpressionImpl<T>> Cos<T>::derivative(const std::string& var) const {
//     return std::make_unique<OperationMul>(Expression(-1.0_val) * sin(arg), arg.derivative(var));
// }

// Функция для создания cos-выражения
template <typename T>
Expression<T> cos(Expression<T> arg) {
    return Expression(std::make_shared<Cos>(arg));
}

// ========================== //
// Реализация класса Ln       //
// ========================== //

template <typename T>
Ln<T>::Ln(const Expression<T> &argument) : arg(argument) {}

template <typename T>
std::string Ln<T>::toString() const {
    return "ln(" + arg.toString() + ")";
}

template <typename T>
T Ln<T>::evaluate(const std::map<std::string, T>& variables) const {
    T val = arg.evaluate(variables);
    if (val <= 0) {
        throw std::runtime_error("Logarithm of non-positive number is undefined");
    }
    return std::log(val);
}

// template <typename T>
// std::unique_ptr<ExpressionImpl> Ln::derivative(const std::string& var) const {
//     return std::make_unique<OperationMul>(arg.derivative(var), Expression(1.0_val) / arg);
// }

// Функция для создания ln-выражения
template <typename T>
Expression<T> ln(Expression<T> arg) {
    return Expression(std::make_shared<Ln>(arg));
}

// ========================== //
// Реализация класса Exp      //
// ========================== //

template <typename T>
Exp<T>::Exp(const Expression<T> &argument) : arg(argument) {}

template <typename T>
std::string Exp<T>::toString() const {
    return "exp(" + arg.toString() + ")";
}

template <typename T>
T Exp<T>::evaluate(const std::map<std::string, T>& variables) const {
    return std::exp(arg.evaluate(variables));
}

// template <typename T>
// std::unique_ptr<ExpressionImpl<T>> Exp::derivative(const std::string& var) const {
//     return std::make_unique<OperationMul>(exp(arg), arg.derivative(var));
// }

// Функция для создания exp-выражения
template <typename T>
Expression<T> exp(Expression<T> arg) {
    return Expression(std::make_shared<Exp>(arg));
}

template class Expression<long double>;
template class Expression<std::complex<long double>>;


int main() {
    Expression<double> num1 = 5.0_val;
    Expression<double> num2 = 3.0_val;

    // Создание переменной x
    Expression<double> x = "x"_var;

    // Выражение: (5 + x) * 3
    Expression<double> expr = (num1 + x) * num2;

    // Вывод выражения
    std::cout << "Выражение: " << expr.toString() << std::endl;

    // Создание контекста переменных
    std::map<std::string, double> variables = {{"x", 2.0}};

    // Вычисление выражения
    std::cout << "Результат вычисления (x=2): " << expr.evaluate(variables) << std::endl;


    return 0;
}