#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include <iostream>
#include <memory>
#include <string>
#include <map>
#include <complex>

// Абстрактный класс, задающий интерфейс выражений
template<typename T>
class ExpressionImpl {
public:
    ExpressionImpl() = default;

    virtual ~ExpressionImpl() = default;

    virtual std::string toString() const = 0;

    virtual T evaluate(const std::map<std::string, T> &variables) const = 0;

    //virtual std::unique_ptr<ExpressionImpl<T>> derivative(const std::string& var) const = 0;
};

// Класс, задающий выражение и методы работы с ним
template<typename T>
class Expression {
public:
    explicit Expression(std::string variable);

    // Copy constructor
    Expression(const Expression &other) {
        impl_ = other.impl_;
    }

    // Copy operator
    Expression &operator=(const Expression &other) {
        impl_ = other.impl_;
        return *this;
    }

    // Move constructor
    Expression(Expression &&other) {
        impl_ = other.impl_;
    }

    // Move operator
    Expression &operator=(Expression &&other) {
        impl_ = other.impl_;
        return *this;
    }

    ~Expression() = default;

    std::string toString() const;

    T evaluate(const std::map<std::string, T> &variables) const;

    //Expression derivative(const std::string& var) const;

    // Арифметические операции
    Expression<T> operator+(const Expression<T> &rhs) const;

    Expression<T> &operator+=(const Expression<T> &rhs);

    Expression<T> operator*(const Expression<T> &rhs) const;

    Expression<T> &operator*=(const Expression<T> &rhs);

    Expression<T> operator-(const Expression<T> &rhs) const;

    Expression<T> operator/(const Expression<T> &rhs) const;

    Expression<T> operator^(const Expression<T> &rhs) const;

private:
    explicit Expression(std::shared_ptr<ExpressionImpl<T> > impl);

    std::shared_ptr<ExpressionImpl<T> > impl_;
};

// Класс для представления чисел
template<typename T>
class Number : public ExpressionImpl<T> {
private:
    T value;

public:
    explicit Number(T val);

    virtual ~Number() override = default;

    std::string toString() const override;

    T evaluate(const std::map<std::string, T> &variables) const override;

    //std::unique_ptr<ExpressionImpl> derivative(const std::string& var) const override;
};

// Класс для представления переменных
template<typename T>
class Variable : public ExpressionImpl<T> {
private:
    std::string name;

public:
    explicit Variable(const std::string &varName);

    virtual ~Variable() override = default;

    std::string toString() const override;

    T evaluate(const std::map<std::string, T> &variables) const override;

    //std::unique_ptr<ExpressionImpl> derivative(const std::string& var) const override;
};

// Операции сложения и умножения
template<typename T>
class OperationAdd : public ExpressionImpl<T> {
private:
    Expression<T> left, right;

public:
    OperationAdd(const Expression<T> &l, const Expression<T> &r);

    virtual ~OperationAdd() override = default;

    std::string toString() const override;

    T evaluate(const std::map<std::string, T> &variables) const override;

    //std::unique_ptr<ExpressionImpl> derivative(const std::string& var) const override;
};

template<typename T>
class OperationMul : public ExpressionImpl<T> {
private:
    Expression<T> left, right;

public:
    OperationMul(const Expression<T> &l, const Expression<T> &r);

    virtual ~OperationMul() override = default;

    std::string toString() const override;

    T evaluate(const std::map<std::string, T> &variables) const override;

    //std::unique_ptr<ExpressionImpl> derivative(const std::string& var) const override;
};

// Класс для вычисления синуса
template<typename T>
class Sin : public ExpressionImpl<T> {
private:
    Expression<T> arg;

public:
    explicit Sin(const Expression<T> &argument);

    std::string toString() const override;

    T evaluate(const std::map<std::string, T> &variables) const override;

    //std::unique_ptr<ExpressionImpl> derivative(const std::string& var) const override;
};

// Класс для вычисления косинуса
template<typename T>
class Cos : public ExpressionImpl<T> {
private:
    Expression<T> arg;

public:
    explicit Cos(const Expression<T> &argument);

    std::string toString() const override;

    T evaluate(const std::map<std::string, T> &variables) const override;

    //std::unique_ptr<ExpressionImpl> derivative(const std::string& var) const override;
};

// Класс для вычисления натурального логарифма
template<typename T>
class Ln : public ExpressionImpl<T> {
private:
    Expression<T> arg;

public:
    explicit Ln(const Expression<T> &argument);

    std::string toString() const override;

    T evaluate(const std::map<std::string, T> &variables) const override;

    //std::unique_ptr<ExpressionImpl> derivative(const std::string& var) const override;
};

// Класс для вычисления экспоненты (e^x)
template<typename T>
class Exp : public ExpressionImpl<T> {
private:
    Expression<T> arg;

public:
    explicit Exp(const Expression<T> &argument);

    std::string toString() const override;

    T evaluate(const std::map<std::string, T> &variables) const override;

    //std::unique_ptr<ExpressionImpl> derivative(const std::string& var) const override;
};

template<typename T>
Expression<T> sin(Expression<T> arg);

template<typename T>
Expression<T> cos(Expression<T> arg);

template<typename T>
Expression<T> ln(Expression<T> arg);

template<typename T>
Expression<T> exp(Expression<T> arg);

// template<typename T>
// Expression<T> make_val(T val);
//
// template<typename T>
// Expression<T> make_var(const std::string &name);
//
// Expression<long double> operator""_val(long double val);
//
// Expression<long double> operator""_var(const char *name, size_t);
//
// // Литералы для комплексных чисел
// Expression<std::complex<long double> > operator""_val(long double val);
//
// Expression<std::complex<long double> > operator""_var(const char *name, size_t);


#endif // EXPRESSION_HPP
