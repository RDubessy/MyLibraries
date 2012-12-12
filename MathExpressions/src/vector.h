/* Copyright (C) 2012 Romain Dubessy */
#ifndef VECTOR_H
#define VECTOR_H
#include <iostream>
#include "myexceptions.h"
#include "matrix.h"
using std::ostream;
using std::cerr;
template <class T> class Matrix;
template <class T> class Bra;
template <class T> class Ket;
/*!\brief This class implements a template vector container.
 *
 * This class is a pure abstract class and Vector objects cannot be instancied.
 * Use Bra and Ket derived classes instead.
 */
template <class T> class Vector {
    public:
        /* Default constructor {{{ */
        /*!\brief Default constructor. */
        Vector(int n=0) {
            _n=n;
            _data=0;
            if(_n!=0) {
                _data=new T[_n];
                for(int i=0;i<_n;i++)
                    _data[i]=0;
            }
        };
        /* }}} */
        /* Destructor {{{ */
        /*!\brief Destructor. */
        ~Vector(void) {
            if(_n!=0)
                delete[] _data;
        };
        /* }}} */
        /* Copy constructor {{{ */
        /*!\brief Copy constructor. */
        Vector(const Vector<T> &other) {
            _n=other._n;
            _data=0;
            if(_n!=0) {
                _data=new T[_n];
                for(int i=0;i<_n;i++)
                    _data[i]=other._data[i];
            } 
        };
        /* }}} */
        /* Size method {{{ */
        /*!\brief Returns the vector size. */
        int size(void) const {
            return _n;
        };
        /* }}} */
        /* Access member method {{{ */
        /*!\brief Access member method. */
        T &operator[](int i) {
            if(i<0 || i>=_n)
                throw outOfBounds;
            return _data[i];
        };
        /*!\brief Access member method. */
        T operator[](int i) const {
            if(i<0 || i>=_n)
                throw outOfBounds;
            return _data[i];
        };
        /* }}} */
        /* Print method {{{ */
        /*!\brief Pure virtual display method. */
        virtual void print(void) const =0;
        /* }}} */
    protected:
        T *_data;   //!<\brief Array containing the vector elements.
        int _n;     //!<\brief Size of the vector.
};
/*!\brief This class implements an "horizontal" template vector container.
 */
template <class T> class Bra : public Vector<T> {
    using Vector<T>::_data;
    using Vector<T>::_n;
    public:
        /* Default constructor {{{ */
        /*!\brief Default constructor. */
        Bra(int n=0) : Vector<T>(n) {};
        /* }}} */
        /* Copy constructor {{{ */
        /*!\brief Copy constructor. */
        Bra(const Vector<T> &other) : Vector<T>(other) {};
        /* }}} */
        /* Print method {{{ */
        /*!\brief Print method. */
        void print(void) const {
            cerr << *this;
            cerr.flush();
        };
        /*!\brief Convert to standard stream operator. */
        friend ostream &operator<<(ostream &os, const Bra<T> &other) {
            for(int i=0;i<other.size();i++)
                os << other[i] << " ";
            return os;
        };
        /* }}} */
        /*!\brief Right hand side multiplication by a matrix. */
        Bra<T> operator*(const Matrix<T> &other) const {
            if(_n!=other.n())
                throw incompatibleSizes;
            Bra<T> tmp(other.m());
            for(int j=0;j<tmp._n;j++)
                for(int i=0;i<_n;i++)
                    tmp._data[j]=_data[i]*other.at(i,j);
            return tmp;
        };
        /*!\brief Scalar product. */
        T operator*(const Ket<T> &other) const {
            if(_n!=other.size())
                throw incompatibleSizes;
            T res=0;
            for(int i=0;i<_n;i++)
                res+=_data[i]*other[i];
            return res;
        };
        /*!\brief Transposition method. */
        Ket<T> transpose(void) const {
            return Ket<T>(*this);
        };
        /* Algebraic operators {{{ */
        /*!\brief Assignement operator. */
        Bra<T> &operator=(const Bra<T> &other) {
            if(this!=&other) {
                if(_n!=other._n) {
                    delete[] _data;
                    _n=other._n;
                }
                if(_n!=0) {
                    _data=new T[_n];
                    for(int i=0;i<_n;i++)
                        _data[i]=other._data[i];
                }
            }
            return *this;
        };
        /*!\brief Addition operator. */
        Bra<T> operator+(const Bra<T> &other) const {
            Bra<T> tmp(*this);
            tmp+=other;
            return tmp;
        };
        /*!\brief Addition operator. */
        Bra<T> &operator+=(const Bra<T> &other) {
            if(_n!=other._n)
                throw outOfBounds;
            for(int i=0;i<_n;i++)
                _data[i]+=other._data[i];
            return *this;
        };
        /*!\brief Substraction operator. */
        Bra<T> operator-(const Bra<T> &other) const {
            Bra<T> tmp(*this);
            tmp-=other;
            return tmp;
        };
        /*!\brief Substraction operator. */
        Bra<T> &operator-=(const Bra<T> &other) {
            if(_n!=other._n)
                throw outOfBounds;
            for(int i=0;i<_n;i++)
                _data[i]-=other._data[i];
            return *this;
        };
        /*!\brief Outer division operator. */
        Bra<T> operator/(const T t) const {
            Bra<T> tmp(*this);
            tmp/=t;
            return tmp;
        };
        /*!\brief Outer division operator. */
        Bra<T> &operator/=(const T t) {
            T tmp=(T)(1.0/t);
            for(int i=0;i<_n;i++)
                _data[i]*=tmp;
            return *this;
        };
        /*!\brief Outer multiplication operator. */
        Bra<T> operator*(const T t) const {
            Bra<T> tmp(*this);
            tmp*=t;
            return tmp;
        };
        /*!\brief Outer multiplication operator. */
        Bra<T> &operator*=(const T t) {
            for(int i=0;i<_n;i++)
                _data[i]*=t;
            return *this;
        };
        /*!\brief Outer multiplication operator. */
        friend Bra<T> operator*(const T t, const Bra<T> &other) {
            Bra<T> tmp(other);
            for(int i=0;i<tmp._n;i++)
                tmp._data[i]*=t;
            return tmp;
        };
        /* }}} */
};
/*!\brief This class implements a "vertical" template vector container.
 */
template <class T> class Ket : public Vector<T> {
    using Vector<T>::_data;
    using Vector<T>::_n;
    public:
        /*!\brief Default constructor. */
        Ket(int n=0) : Vector<T>(n) {};
        /*!\brief Copy constructor. */
        Ket(const Vector<T> &other) : Vector<T>(other) {};
        /*!\brief Print method. */
        void print(void) const {
            cerr << *this;
            cerr.flush();
        };
        /*!\brief Convert to standard stream operator. */
        friend ostream &operator<<(ostream &os, const Ket<T> &other) {
            for(int i=0;i<other.size();i++)
                os << other[i] << " ";
            return os;
        };
        /*!\brief Cross product. */
        Matrix<T> operator*(const Bra<T> &other) {
            Matrix<T> tmp(_n,other.size());
            for(int i=0;i<_n;i++)
                for(int j=0;j<other.size();j++)
                    tmp.at(i,j)=_data[i]*other[j];
            return tmp;
        };
        /*!\brief Transposition method. */
        Bra<T> transpose(void) const {
            return Bra<T>(*this);
        };
        /* Algebraic operators {{{ */
        /*!\brief Assignement operator. */
        Ket<T> &operator=(const Ket<T> &other) {
            if(this!=&other) {
                if(_n!=other._n) {
                    delete[] _data;
                    _n=other._n;
                }
                if(_n!=0) {
                    _data=new T[_n];
                    for(int i=0;i<_n;i++)
                        _data[i]=other._data[i];
                }
            }
            return *this;
        };
        /*!\brief Addition operator. */
        Ket<T> operator+(const Ket<T> &other) const {
            Ket<T> tmp(*this);
            tmp+=other;
            return tmp;
        };
        /*!\brief Addition operator. */
        Ket<T> &operator+=(const Ket<T> &other) {
            if(_n!=other._n)
                throw outOfBounds;
            for(int i=0;i<_n;i++)
                _data[i]+=other._data[i];
            return *this;
        };
        /*!\brief Substraction operator. */
        Ket<T> operator-(const Ket<T> &other) const {
            Ket<T> tmp(*this);
            tmp-=other;
            return tmp;
        };
        /*!\brief Substraction operator. */
        Ket<T> &operator-=(const Ket<T> &other) {
            if(_n!=other._n)
                throw outOfBounds;
            for(int i=0;i<_n;i++)
                _data[i]-=other._data[i];
            return *this;
        };
        /*!\brief Outer division operator. */
        Ket<T> operator/(const T t) const {
            Ket<T> tmp(*this);
            tmp/=t;
            return tmp;
        };
        /*!\brief Outer division operator. */
        Ket<T> &operator/=(const T t) {
            T tmp=(T)(1.0/t);
            for(int i=0;i<_n;i++)
                _data[i]*=tmp;
            return *this;
        };
        /*!\brief Outer multiplication operator. */
        Ket<T> operator*(const T t) const {
            Ket<T> tmp(*this);
            tmp*=t;
            return tmp;
        };
        /*!\brief Outer multiplication operator. */
        Ket<T> &operator*=(const T t) {
            for(int i=0;i<_n;i++)
                _data[i]*=t;
            return *this;
        };
        /*!\brief Outer multiplication operator. */
        friend Ket<T> operator*(const T t, const Ket<T> &other) {
            Ket<T> tmp(other);
            for(int i=0;i<tmp._n;i++)
                tmp._data[i]*=t;
            return tmp;
        };
        /* }}} */
};
#endif //VECTOR_H
/* vector.h */
