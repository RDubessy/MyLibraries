#ifndef EXPRESSION_H
#define EXPRESSION_H
#include <iostream>
#include <string>
#include <map>
#include <cmath>
#include <stdlib.h>
#include "myexceptions.h"
#include "matrix.h"
using std::map;
using std::string;
using std::cerr;
using std::endl;
using std::ostream;
class Expression;
typedef map<string,Expression *> VarDef;
int find(const string &s, const char c);
Expression *parseString(const string &s);
/* Expression {{{ */
/*!\brief Pure virtual class that represents any kind of mathematical 
 * expression.
 */
class Expression {
    public:
        /*!\brief Default constructor. */
        Expression(void) {};
        /*!\brief Print method. */
        virtual void print(void) =0;
        /*!\brief Set data value method. */
        virtual void set(void *) =0;
        /*!\brief Simplify expression method. */
        virtual Expression *simplify(VarDef &) =0;
        /*!\brief Evaluate expression method. */
        virtual void *evaluate(VarDef &) =0;
        friend ostream &operator<<(ostream &os, Expression *exp);
};
/* }}} */
/* Constant {{{ */
/*!\brief Represents a constant value expression.
 */
class Constant : public Expression {
    public:
        /*!\brief Default constructor. */
        Constant(const string &s="") : Expression() { _c=atof(s.c_str()); };
        /*!\brief Copy constructor. */
        Constant(double d) : Expression() { _c=d; };
        ~Constant(void) {};
        void print(void) { cerr << _c; };
        void set(void *d) { _c=*((double*)d); };
        Expression *simplify(VarDef &) { return this; };
        void *evaluate(VarDef &) { return new double(_c); };
        Constant &operator=(const Constant &other);
        double value(void) const { return _c; };
    private:
        double _c; //!<\brief Constant value, stored as a double.
};
/* }}} */
/* BConstant {{{ */
/*!\brief Represents a constant vector expression.
 */
class BConstant : public Expression {
    public:
        /*!\brief Default constructor. */
        BConstant(const string &s="");
        /*!\brief Copy constructor. */
        BConstant(const Bra<double> &other) : Expression() { _b=other; };
        ~BConstant(void) {};
        void print(void) { cerr << _b; };
        void set(void *other) { _b=*((Bra<double>*)other); };
        Expression *simplify(VarDef &) { return this; };
        void *evaluate(VarDef &) { return new Bra<double>(_b); };
    private:
        Bra<double> _b; //!<\brief Bra constant value, stored as a vector.
};
/* }}} */
/* KConstant {{{ */
/*!\brief Represents a constant vector expression.
 */
class KConstant : public Expression {
    public:
        /*!\brief Default constructor. */
        KConstant(const string &s="");
        /*!\brief Copy constructor. */
        KConstant(const Ket<double> &other) : Expression() { _k=other; };
        ~KConstant(void) {};
        void print(void) { cerr << _k; };
        void set(void *other) { _k=*((Ket<double>*)other); };
        Expression *simplify(VarDef &) { return this; };
        void *evaluate(VarDef &) { return new Ket<double>(_k); };
    private:
        Ket<double> _k; //!<\brief Ket constant value, stored as a vector.
};
/* }}} */
/* MConstant {{{ */
/*!\brief Represents a constant matrix expression.
 */
class MConstant : public Expression {
    public:
        /*!\brief Default constructor. */
        MConstant(const string &s="");
        /*!\brief Copy constructor. */
        MConstant(const Matrix<double> &other) : Expression() { _m=other; };
        ~MConstant(void) {};
        void print(void) { cerr << _m; };
        void set(void *other) { _m=*((Matrix<double>*)other); };
        Expression *simplify(VarDef &) { return this; };
        void *evaluate(VarDef &) { return new Matrix<double>(_m); };
    private:
        Matrix<double> _m; //!<\brief Matrix constant value, stored as a matrix.
};
/* }}} */
/* Variable {{{ */
/*!\brief Represents a variable.
 */
class Variable : public Expression {
    public:
        /*!\brief Default constructor. */
        Variable(const string &s="") : Expression() { _var=s; };
        ~Variable(void) {};
        void print(void) { cerr << _var; };
        void set(void *) {};
        Expression *simplify(VarDef &);
        void *evaluate(VarDef &);
        string name(void) const { return _var; };
    private:
        string _var;    //!<\brief String storing the variable name.
};
/* }}} */
/* BinaryOp {{{ */
/*!\brief Represents a binary operation between two expressions.
 */
class BinaryOp : public Expression {
    public:
        /*!\brief Default constructor. */
        BinaryOp(const char c, const string &sl="", const string &sr="");
        /*!\brief Almost a copy constructor. */
        BinaryOp(const char, Expression *, Expression *);
        ~BinaryOp(void) {};
        void print(void);
        void set(void *) {};
        Expression *simplify(VarDef &);
        void *evaluate(VarDef &);
        Expression *left(void) { return _left; };
        Expression *right(void) { return _right; };
        char op(void) const { return _c; };
    private:
        char _c;            //!<\brief Binary operator stored as a char.
        Expression *_left;  //!<\brief Left hand side of the operator.
        Expression *_right; //!<\brief Right hand side of the operator.
};
/* }}} */
/* SingleValFunction {{{ */
/*!\brief Represents a single value fonction, whith a scalar argument.
 */
class SingleValFunction : public Expression {
    public:
        /*!\brief Default constructor. */
        SingleValFunction(const string &fun, const string &s);
        /*!\brief Almost a copy constructor. */
        SingleValFunction(const int, Expression *);
        ~SingleValFunction(void) {};
        void print(void);
        void set(void *) {};
        Expression *simplify(VarDef &);
        void *evaluate(VarDef &);
        int i() { return _fun; };
        Expression *arg() { return _arg; };
    protected:
        int _fun;           //!<\brief Function unique identifier.
        Expression *_arg;   //!<\brief Argument, stored as an expression.
};
/* }}} */
#endif //EXPRESSION_H
/* expression.h */
