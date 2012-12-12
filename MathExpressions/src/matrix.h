/* Copyright (C) 2012 Romain Dubessy */
#ifndef MATRIX_H
#define MATRIX_H
#include <iostream>
#include "myexceptions.h"
#include "vector.h"
#include "complex.h"
using std::endl;
using std::cerr;
using std::ostream;
/* \todo det.
 */
template <class T> class Complex;
/*! \brief This class implements a template matrix container.
 */
template <class T> class Matrix {
    public:
        /* Default constructor {{{ */
        /*!\brief Default constructor. */
        Matrix(int n=0,int m=0) {
            _n=n;
            _m=m;
            _nm=n*m;
            _data=0;
            if(_nm!=0) {
                _data=new T[_nm];
                for(int i=0;i<_nm;i++)
                    _data[i]=0;
            }
        };
        /* }}} */
        /* Destructor {{{ */
        /*!\brief Destructor. */
        ~Matrix(void) {
            _n=_m=0;
            if(_nm!=0)
                delete[] _data;
            _nm=0;
        };
        /* }}} */
        /* Copy constructor {{{ */
        /*!\brief Copy constructor. */
        Matrix(const Matrix<T> &other) {
            _n=other._n;
            _m=other._m;
            _nm=_n*_m;
            _data=0;
            if(_nm!=0) {
                _data=new T[_nm];
                for(int i=0;i<_nm;i++)
                    _data[i]=other._data[i];
            }
        };
        /* }}} */
        /* Print {{{ */
        /*!\brief Print method. */
        void print(void) const {
            cerr << *this;
            cerr.flush();
        };
        /*!\brief Friend standard output operator. */
        friend ostream &operator<<(ostream &os, const Matrix<T> &other) {
            int c=0;
            for(int n=0;n<other._n;n++) {
                for(int m=0;m<other._m;m++) {
                    os << other._data[c] << " ";
                    c++;
                }
                //os << "\n";
            }
            return os;
        };
        /* }}} */
        /* Access member method {{{ */
        /*!\brief Access member method. */
        T &at(int i, int j) {
            if(i<0 || i>=_n || j<0 || j>=_m)
                throw outOfBounds;
            return _data[i*_m+j];
        };
        /*!\brief Access member method. */
        T at(int i, int j) const {
            if(i<0 || i>=_n || j<0 || j>=_m)
                throw outOfBounds;
            return _data[i*_m+j];
        };
        /*!\brief Row access method. */
        Bra<T> row(int i) const {
            if(i<0 || i>=_n)
                throw outOfBounds;
            Bra<T> tmp(_m);
            for(int j=0;j<_m;j++)
                tmp[j]=_data[i*_m+j];
            return tmp;
        };
        /*!\brief Column access method. */
        Ket<T> col(int j) const {
            if(j<0 || j>=_m)
                throw outOfBounds;
            Ket<T> tmp(_n);
            for(int i=0;i<_n;i++)
                tmp[i]=_data[i*_m+j];
            return tmp;
        };
        /* }}} */
        /* Transpose {{{ */
        /*!\brief Returns the transposed matrix. */
        Matrix<T> transpose(void) const {
            Matrix<T> tmp(_m,_n);
            for(int i=0;i<_n;i++)
                for(int j=0;j<_m;j++)
                    tmp._data[j*_n+i]=_data[i*_m+j];
            return tmp;
        };
        /* }}} */
        /* Trace {{{ */
        /*!\brief Returns the matrix trace. */
        T trace(void) const {
            if(_n!=_m)
                throw notSquare;
            T res=0;
            for(int i=0;i<_n;i++)
                res+=_data[i*_m+i];
            return res;
        };
        /* }}} */
        /* Submatrix {{{ */
        /*!\brief Returns a submatrix. */
        Matrix<T> sub(int ii, int jj) const {
            Matrix<T> tmp(_n-1,_m-1);
            for(int i=0;i<ii;i++) {
                for(int j=0;j<jj;j++)
                    tmp._data[i*tmp._m+j]=_data[i*_m+j];
                for(int j=jj+1;j<_m;j++)
                    tmp._data[i*tmp._m+j-1]=_data[i*_m+j];
            }
            for(int i=ii+1;i<_n;i++) {
                for(int j=0;j<jj;j++)
                    tmp._data[(i-1)*tmp._m+j]=_data[i*_m+j];
                for(int j=jj+1;j<_m;j++)
                    tmp._data[(i-1)*tmp._m+j-1]=_data[i*_m+j];
            }
            return tmp;
        };
        /* }}} */
        /* n {{{ */
        /*!\brief Returns the number of rows. */
        int n(void) const { return _n; };
        /* }}} */
        /* m {{{ */
        /*!\brief Return the number of columns. */
        int m(void) const { return _m; };
        /* }}} */
        /* Algebraic operators {{{ */
        /* Addition {{{ */
        /*!\brief Addition operator. */
        Matrix<T> operator+(const Matrix<T> &other) const {
            Matrix<T> tmp(*this);
            tmp+=other;
            return tmp;
        };
        /*!\brief Addition operator. */
        Matrix<T> &operator+=(const Matrix<T> &other) {
            if(_n!=other._n || _m!=other._m)
                throw incompatibleSizes;
            for(int i=0;i<_nm;i++)
                _data[i]+=other._data[i];
            return *this;
        };
        /* }}} */
        /* Substraction {{{ */
        /*!\brief Substraction operator. */
        Matrix<T> operator-(const Matrix<T> &other) const {
            Matrix<T> tmp(*this);
            tmp-=other;
            return tmp;
        };
        /*!\brief Substraction operator. */
        Matrix<T> &operator-=(const Matrix<T> &other) {
            if(_n!=other._n || _m!=other._m)
                throw incompatibleSizes;
            for(int i=0;i<_nm;i++)
                _data[i]-=other._data[i];
            return *this;
        };
        /* }}} */
        /* Inner Product {{{ */
        /*!\brief Inner product operator. */
        Matrix<T> operator*(const Matrix<T> &other) const {
            if(_m!=other._n)
                throw incompatibleSizes;
            Matrix<T> tmp(_n,other._m);
            for(int i=0;i<_n;i++) {
                for(int j=0;j<other._m;j++) {
                    for(int k=0;k<_n;k++)
                        tmp._data[i*other._m+j]
                            +=_data[i*_m+k]*other._data[k*other._m+j];
                }
            }
            return tmp;
        };
        /* }}} */
        /* Assignement {{{ */
        /*!\brief Assignement operator. */
        Matrix<T> &operator=(const Matrix<T> &other) {
            if(&other!=this) {
                if(_n!=other._n || _m!=other._m) {
                    _n=other._n;
                    _m=other._m;
                    _nm=_n*_m;
                    delete[] _data;
                    _data=0;
                }
                if(_nm!=0) {
                    _data=new T[_nm];
                    for(int i=0;i<_nm;i++)
                        _data[i]=other._data[i];
                }
            }
            return *this;
        };
        /* }}} */
        /* Outer Product {{{ */
        /*!\brief Outer product. */
        Matrix<T> operator*(const T t) const {
            Matrix tmp(*this);
            tmp*=t;
            return tmp;
        };
        /*!\brief Outer product. */
        Matrix<T> &operator*=(const T t) {
            if(t!=(T)1) {
                for(int i=0;i<_nm;i++)
                    _data[i]*=t;
            }
            return *this;
        };
        /*!\brief Outer product. */
        friend Matrix<T> operator*(const T t,const Matrix<T> &other) {
            Matrix tmp(other);
            tmp*=t;
            return tmp;
        };
        /* }}} */
        /* Outer Division {{{ */
        /*!\brief Outer division. */
        Matrix<T> operator/(const T t) const {
            Matrix tmp(*this);
            tmp/=t;
            return tmp;
        };
        /*!\brief Outer division. */
        Matrix<T> &operator/=(const T t) {
            if(t!=(T)1) {
                T tmp=((T)1)/t;
                for(int i=0;i<_nm;i++)
                    _data[i]*=tmp;
            }
            return *this;
        };
        /* }}} */
        /*!\brief Ket reduction. */
        Ket<T> operator*(const Ket<T> &k) const {
            if(_m!=k.size())
                throw incompatibleSizes;
            Ket<T> tmp(_n);
            for(int i=0;i<_n;i++)
                for(int j=0;j<_m;j++)
                    tmp[i]+=_data[i*_m+j]*k[j];
            return tmp;
        };
        /* }}} */
        /* Comparison {{{ */
        /*!\brief Comparison operator. */
        bool operator==(const Matrix<T> &other) const {
            if(_n!=other._n || _m!=other._m)
                return false;
            for(int i=0;i<_nm;i++)
                if(_data[i]!=other._data[i])
                    return false;
            return true;
        };
        /*!\brief Comparison operator. */
        bool operator!=(const Matrix<T> &other) const {
            return !((*this)==other);
        };
        /* }}} */
        /* isSquare {{{ */
        /*!\brief Returns true if the matrix is square, false otherwise. */
        bool isSquare(void) const {
            return _n==_m;
        };
        /* }}} */
        /* isSymmetric {{{ */
        /*!\brief Returns true if the matrix is symmetric, false otherwise. */
        bool isSymmetric(void) const {
            if(_n!=_m)
                return false;
            for(int i=0;i<_n;i++)
                for(int j=i+1;j<_n;j++)
                    if(_data[i*_n+j]!=_data[j*_n+i])
                        return false;
            return true;
        };
        /* }}} */
    private:
        T *_data;   //!<\brief Matrix data, stored as a linear array.
        int _n;     //!<\brief Number of matrix rows.
        int _m;     //!<\brief Number of matrix columns.
        int _nm;    //!<\brief Size of the array.
};
/* identity {{{ */
/*! Return an identity matrix*/
template <class T> Matrix<T> identity(int n) {
    Matrix<T> tmp(n,n);
    for(int i=0;i<n;i++) {
        try {
            tmp.at(i,i)=(T)1;
        }
        catch (exception &e) {
            cerr << e.what() << endl;
        }
    }
    return tmp;
};
/* }}} */
#endif //MATRIX_H
/* matrix.h */
