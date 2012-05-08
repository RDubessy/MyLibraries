/* Copyright (C) 2010 Romain Dubessy */
#ifndef MYEXCEPTIONS_H
#define MYEXCEPTIONS_H
#include <exception>
using std::exception;
/*!\brief Invalid memory access exception. */
class OutOfBounds : public exception {
    /*!\brief Print exception error message method. */
    virtual const char* what() const throw() {
        return "[E] Array out of bounds!";
    };
};
/*!\brief Incompatible sizes (mathematical sense) exception. */
class IncompatibleSizes : public exception {
    /*!\brief Print exception error message method. */
    virtual const char * what() const throw() {
        return "[E] Incompatible array sizes!";
    };
};
/*!\brief Not a square matrix exception. */
class NotSquare : public exception {
    /*!\brief Print exception error message method. */
    virtual const char * what() const throw() {
        return "[E] Matrix not square!";
    };
};
/*!\brief Undefinite variable exception. */
class UndefVar : public exception {
    /*!\brief Print exception error message method. */
    virtual const char * what() const throw() {
        return "[E] Variable not defined!";
    };
};
/*!\brief Incorrect expression exception. */
class IncorExpr : public exception {
    /*!\brief Print exception error message method. */
    virtual const char * what() const throw() {
        return "[E] Incorrect expression!";
    };
};
/*!\brief Unknown function. */
class UnknownFunction : public exception {
    /*!\brief Print exception error message method. */
    virtual const char * what() const throw() {
        return "[E] Unknown function!";
    };
};
extern OutOfBounds outOfBounds;
extern IncompatibleSizes incompatibleSizes;
extern NotSquare notSquare;
extern UndefVar undefVar;
extern IncorExpr incorExpr;
extern UnknownFunction unknownFunction;
#endif //MYEXCEPTIONS_H
/* myexceptions.h */
