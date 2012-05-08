/* Copyright (C) 2010 Romain Dubessy */
#ifndef COMPLEX_H
#define COMPLEX_H
#include <iostream>
#include <cmath>
using std::ostream;
/*!\brief This class implements a template complex container.
 * 
 * An object of this class represents a complex number whose real and
 * imaginary parts are stored in a specified type (ie Complex<double> stores
 * real and imaginary parts in two doubles).
 */
template <class T> class Complex {
    public:
        /* Default constructor {{{ */
        /*!\brief Default constructor.
         *
         * Construct a complex number from two real numbers.
         * If nothing is supplied 0 is instancied.
         * \param re Real part.
         * \param im Imaginary part.
         */
        Complex(const T re=0, const T im=0) {
            _re=re;
            _im=im;
        };
        /* }}} */
        /* Copy constructor {{{ */
        /*!\brief Copy constructor.
         *
         * \param other Object to copy.
         */
        Complex(const Complex<T> &other) {
            _re=other._re;
            _im=other._im;
        };
        /* }}} */
        /* Norm Squared {{{ */
        /*!\brief Computes the L2 norm squared.
         *
         * This method returns the L2 norm squared of a complex number.
         * \return L2 norm squared.
         */
        T abs2(void) const {
            return _re*_re+_im*_im;
        };
        /* }}} */
        /* Real part {{{ */
        /*!\brief Returns the real part.
         *
         * This method returns the complex number real part.
         * \return Real part.
         */
        T re(void) const {
            return _re;
        };
        /* }}} */
        /* Imaginary part {{{ */
        /*!\brief Returns the imaginary part.
         * 
         * This method returns the complex number imaginary part.
         * \return Imaginary part.
         */
        T im(void) const {
            return _im;
        };
        /* }}} */
        /* Modulus {{{ */
        /*!\brief Computes the L2 norm.
         *
         * This method returns the L2 norm of a complex number.
         * \return L2 norm.
         */
        T mod(void) const {
            return sqrt(abs2());
        };
        /* }}} */
        /* Argument {{{ */
        /*!\brief Computes the argument.
         *
         * This method returns the complex number argument.
         * \return Argument.
         */
        T arg(void) const {
            if(_re==0) {
                if(_im>0)
                    return asin(1);              //Pi/2
                return -asin(1);                 //-Pi/2
            }
            return atan(_im/_re);
        };
        /* }}} */
        /* Conjugate {{{ */
        /*!\brief Computes the complex conjugate.
         *
         * This method returns the complex conjugate.
         * \return Complex conjugate.
         */
        Complex<T> conjugate(void) const {
            Complex<T> tmp;
            tmp._re=_re;
            tmp._im=-_im;
            return tmp;
        };
        /* }}} */
        /* Iostream method {{{ */
        /*!\brief Converts a complex number to an output stream.
         *
         * This overloaded operator translates a complex number to an output 
         * stream of type std::ostream.
         * Both parameters are passed by reference, avoiding to make temporary
         * copies.
         * \param os Output stream to store the translation.
         * \param other Complex object to translate.
         * \return The updated output stream reference.
         */
        friend ostream &operator<<(ostream &os, const Complex<T> &other) {
            os << other._re << "+I*" << other._im;
            return os;
        };
        /* }}} */
        /* Algebraic operators {{{ */
        /* Assignement {{{ */
        /*!\brief Assignement operator.
         *
         * \param other .
         * \return .
         */
        Complex<T> &operator=(const Complex<T> &other) {
            if(this!=&other) {
                _re=other._re;
                _im=other._im;
            }
            return *this;
        };
        /* }}} */
        /* Addition {{{ */
        /*!\brief Complex addition.
         *
         * \param other .
         * \return .
         */
        Complex<T> operator+(const Complex<T> &other) const {
            Complex<T> tmp(*this);
            tmp+=other;
            return tmp;
        };
        /*!\brief Complex addition.
         *
         * \param other .
         * \return .
         */
        Complex<T> &operator+=(const Complex<T> &other) {
            _re+=other._re;
            _im+=other._im;
            return *this;
        };
        /* }}} */
        /* Substraction {{{ */
        /*!\brief Complex substraction.
         *
         * \param other .
         * \return .
         */
        Complex<T> operator-(const Complex<T> &other) const {
            Complex<T> tmp(*this);
            tmp-=other;
            return tmp;
        };
        /*!\brief Complex substraction.
         *
         * \param other .
         * \return .
         */
        Complex<T> &operator-=(const Complex<T> &other) {
            _re-=other._re;
            _im-=other._im;
            return *this;
        };
        /* }}} */
        /* Multiplication {{{ */
        /*!\brief Complex multiplication.
         *
         * \param other .
         * \return .
         */
        Complex<T> operator*(const Complex<T> &other) const {
            Complex<T> tmp(*this);
            tmp*=other;
            return tmp;
        };
        /*!\brief Complex multiplication.
         *
         * \param other .
         * \return .
         */
        Complex<T> &operator*=(const Complex<T> &other) {
            T tmp=_re;
            _re=_re*other._re-_im*other._im;
            _im=tmp*other._im+_im*other._re;
            return *this;
        };
        /*!\brief Complex outer product.
         *
         * \param t .
         * \return .
         */
        Complex<T> operator*(const T t) const {
            Complex<T> tmp(*this);
            tmp*=t;
            return tmp;
        };
        /*!\brief Complex outer product.
         *
         * \param t .
         * \return .
         */
        Complex<T> &operator*=(const T t) {
            _re*=t;
            _im*=t;
            return *this;
        };
        /*!\brief Complex outer product.
         *
         * \param t .
         * \param other .
         * \return .
         */
        friend Complex<T> operator*(const T t, const Complex &other) {
            Complex<T> tmp(other);
            tmp*=t;
            return tmp;
        };
        /* }}} */
        /* Division {{{ */
        /*!\brief Complex division.
         *
         * \param other .
         * \return .
         */
        Complex<T> operator/(const Complex &other) const {
            Complex<T> tmp(*this);
            tmp/=other;
            return tmp;
        };
        /*!\brief Complex division.
         *
         * \param other .
         * \return .
         */
        Complex<T> &operator/=(const Complex &other) {
            T tmp=other.abs2();
            T re=_re;
            _re=(_re*other._re+_im*other._im)/tmp;
            _im=(_im*other._re-_re*other._im)/tmp;
            return *this;
        };
        /*!\brief Complex outer division.
         *
         * \param t .
         * \return .
         */
        Complex<T> operator/(const T t) const {
            Complex<T> tmp(*this);
            tmp/=t;
            return tmp;
        };
        /*!\brief Complex outer division.
         *
         * \param t .
         * \return .
         */
        Complex<T> operator/=(const T t) const {
            _re/=t;
            _im/=t;
        };
        /* }}} */
        /* }}} */
        /* Comparison operators {{{ */
        /*!\brief Comparison operator. */
        bool operator==(const Complex<T> &other) const {
            return _re==other._re && _im==other._im;
        };
        /*!\brief Comparison operator. */
        bool operator!=(const Complex<T> &other) const {
            return _re!=other._re || _im!=other._im;
        };
        /* }}} */
    private:
        T _re;     /*!<\brief Real part. */
        T _im;     /*!<\brief Imaginary part. */
};
#endif //COMPLEX_H
/* complex.h */
