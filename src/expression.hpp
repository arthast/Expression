#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include <string>
#include <map>
#include <memory>
#include <complex>


template<typename T>
class Expression;

// Абстрактный базовый класс для реализации выражения.
template<typename T>
class ExpressionImpl {
public:
    ExpressionImpl() = default;

    virtual ~ExpressionImpl() = default;

    // Вычисление значения выражения с заданным контекстом переменных.
    virtual T eval(const std::map<std::string, T> &context) const = 0;

    // Преобразование выражения в строку.
    virtual std::string to_string() const = 0;

    // Символьное дифференцирование по заданной переменной.
    virtual Expression<T> derivative(const std::string &var) const = 0;

    // Подстановка в выражение: замена переменной на другое выражение.
    virtual Expression<T> substitute(const std::string &var, const Expression<T> &expr) const = 0;

    // Клонирование (для реализации операций копирования).
    virtual std::shared_ptr<ExpressionImpl<T> > clone() const = 0;
};

// Класс для выражения.
template<typename T>
class Expression {
public:
    explicit Expression(const std::string &variable);

    explicit Expression(T value);

    Expression(const Expression &other);

    Expression(Expression &&other) noexcept;

    Expression &operator=(const Expression &other);

    Expression &operator=(Expression &&other) noexcept;

    ~Expression() = default;

    Expression operator+(const Expression &right) const;

    Expression operator-(const Expression &right) const;

    Expression operator*(const Expression &right) const;

    Expression operator/(const Expression &right) const;

    Expression operator^(const Expression &right) const;

    Expression &operator+=(const Expression &right);

    Expression &operator-=(const Expression &right);

    Expression &operator*=(const Expression &right);

    Expression &operator/=(const Expression &right);

    Expression &operator^=(const Expression &right);


    T eval(const std::map<std::string, T> &context) const;

    std::string to_string() const;

    Expression substitute(const std::string &var, const Expression &expr) const;

    Expression differentiate(const std::string &var) const;

    explicit Expression(std::shared_ptr<ExpressionImpl<T> > impl);

    std::shared_ptr<ExpressionImpl<T> > getImpl() const { return impl_; }

private:
    std::shared_ptr<ExpressionImpl<T> > impl_;
};


// Литералы для создания выражений с действительными числами.
Expression<long double> operator"" _val(long double val);

Expression<long double> operator"" _var(const char *str);

Expression<long double> operator"" _var(const char *str, size_t);

/*
    Классы, представляющие конкретные виды выражений:
*/

// Класс, представляющий константу в выражении.
template<typename T>
class Value : public ExpressionImpl<T> {
public:
    explicit Value(T value);

    T eval(const std::map<std::string, T> &context) const override;

    std::string to_string() const override;

    // Производная от константы равна 0.
    Expression<T> derivative(const std::string &var) const override;

    // Подстановка не влияет на константу.
    Expression<T> substitute(const std::string &var, const Expression<T> &expr) const override;

    std::shared_ptr<ExpressionImpl<T> > clone() const override;

private:
    T value_;
};

// Класс, представляющий переменную.
template<typename T>
class Variable : public ExpressionImpl<T> {
public:
    explicit Variable(const std::string &name);

    T eval(const std::map<std::string, T> &context) const override;

    std::string to_string() const override;

    // Производная переменной: 1, если имя совпадает, иначе 0.
    Expression<T> derivative(const std::string &var) const override;

    // Подстановка: если имена совпадают, то возвращается подставляемое выражение.
    Expression<T> substitute(const std::string &var, const Expression<T> &expr) const override;

    std::shared_ptr<ExpressionImpl<T> > clone() const override;

private:
    std::string name_;
};

// Базовый класс для бинарных операций.
template<typename T>
class BinaryOperation : public ExpressionImpl<T> {
public:
    BinaryOperation(const Expression<T> &left, const Expression<T> &right);

protected:
    Expression<T> left_;
    Expression<T> right_;
};

// Операция сложения.
template<typename T>
class OperationAdd : public BinaryOperation<T> {
public:
    OperationAdd(const Expression<T> &left, const Expression<T> &right);

    T eval(const std::map<std::string, T> &context) const override;

    std::string to_string() const override;

    Expression<T> derivative(const std::string &var) const override;

    Expression<T> substitute(const std::string &var, const Expression<T> &expr) const override;

    std::shared_ptr<ExpressionImpl<T> > clone() const override;
};

// Операция вычитания.
template<typename T>
class OperationSub : public BinaryOperation<T> {
public:
    OperationSub(const Expression<T> &left, const Expression<T> &right);

    T eval(const std::map<std::string, T> &context) const override;

    std::string to_string() const override;

    Expression<T> derivative(const std::string &var) const override;

    Expression<T> substitute(const std::string &var, const Expression<T> &expr) const override;

    std::shared_ptr<ExpressionImpl<T> > clone() const override;
};

// Операция умножения.
template<typename T>
class OperationMul : public BinaryOperation<T> {
public:
    OperationMul(const Expression<T> &left, const Expression<T> &right);

    T eval(const std::map<std::string, T> &context) const override;

    std::string to_string() const override;

    Expression<T> derivative(const std::string &var) const override;

    Expression<T> substitute(const std::string &var, const Expression<T> &expr) const override;

    std::shared_ptr<ExpressionImpl<T> > clone() const override;
};

// Операция деления.
template<typename T>
class OperationDiv : public BinaryOperation<T> {
public:
    OperationDiv(const Expression<T> &left, const Expression<T> &right);

    T eval(const std::map<std::string, T> &context) const override;

    std::string to_string() const override;

    Expression<T> derivative(const std::string &var) const override;

    Expression<T> substitute(const std::string &var, const Expression<T> &expr) const override;

    std::shared_ptr<ExpressionImpl<T> > clone() const override;
};

// Операция возведения в степень.
template<typename T>
class OperationPow : public BinaryOperation<T> {
public:
    OperationPow(const Expression<T> &left, const Expression<T> &right);

    T eval(const std::map<std::string, T> &context) const override;

    std::string to_string() const override;

    Expression<T> derivative(const std::string &var) const override;

    Expression<T> substitute(const std::string &var, const Expression<T> &expr) const override;

    std::shared_ptr<ExpressionImpl<T> > clone() const override;
};

// Функция sin.
template<typename T>
class FunctionSin : public ExpressionImpl<T> {
public:
    explicit FunctionSin(const Expression<T> &arg);

    T eval(const std::map<std::string, T> &context) const override;

    std::string to_string() const override;

    Expression<T> derivative(const std::string &var) const override;

    Expression<T> substitute(const std::string &var, const Expression<T> &expr) const override;

    std::shared_ptr<ExpressionImpl<T> > clone() const override;

private:
    Expression<T> arg_;
};

// Функция cos.
template<typename T>
class FunctionCos : public ExpressionImpl<T> {
public:
    explicit FunctionCos(const Expression<T> &arg);

    T eval(const std::map<std::string, T> &context) const override;

    std::string to_string() const override;

    Expression<T> derivative(const std::string &var) const override;

    Expression<T> substitute(const std::string &var, const Expression<T> &expr) const override;

    std::shared_ptr<ExpressionImpl<T> > clone() const override;

private:
    Expression<T> arg_;
};

// Функция ln.
template<typename T>
class FunctionLn : public ExpressionImpl<T> {
public:
    explicit FunctionLn(const Expression<T> &arg);

    T eval(const std::map<std::string, T> &context) const override;

    std::string to_string() const override;

    Expression<T> derivative(const std::string &var) const override;

    Expression<T> substitute(const std::string &var, const Expression<T> &expr) const override;

    std::shared_ptr<ExpressionImpl<T> > clone() const override;

private:
    Expression<T> arg_;
};

// Функция exp.
template<typename T>
class FunctionExp : public ExpressionImpl<T> {
public:
    explicit FunctionExp(const Expression<T> &arg);

    T eval(const std::map<std::string, T> &context) const override;

    std::string to_string() const override;

    Expression<T> derivative(const std::string &var) const override;

    Expression<T> substitute(const std::string &var, const Expression<T> &expr) const override;

    std::shared_ptr<ExpressionImpl<T> > clone() const override;

private:
    Expression<T> arg_;
};

// Функции для создания функциональных выражений.
template<typename T>
Expression<T> sin(const Expression<T> &expr);

template<typename T>
Expression<T> cos(const Expression<T> &expr);

template<typename T>
Expression<T> ln(const Expression<T> &expr);

template<typename T>
Expression<T> exp(const Expression<T> &expr);

#endif // EXPRESSION_HPP
