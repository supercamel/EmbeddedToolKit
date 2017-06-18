/*
    Embedded Tool Kit
    Copyright (C) 2015 Samuel Cowen

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.
*/


#ifndef IMUMATH_MATRIX_HPP
#define IMUMATH_MATRIX_HPP

#include "types.h"
#include "vector.h"
#include "staticstring.h"


namespace etk
{


template <uint8 MAX_X, uint8 MAX_Y> class Matrix
{
public:
    Matrix()
    {
        for (uint32 x = 0; x < MAX_X; x++ )
        {
            for(uint32 y = 0; y < MAX_Y; y++)
            {
                _cell[x][y] = 0.0f;
            }
        }
    }

    Matrix(const Matrix &v)
    {
        for (uint32 x = 0; x < MAX_X; x++ )
        {
            for(uint32 y = 0; y < MAX_Y; y++)
            {
                _cell[x][y] = v._cell[x][y];
            }
        }
    }

    Matrix(const Vector<MAX_X*MAX_Y>& v)
    {
        uint32 c = 0;
        for (uint32 x = 0; x < MAX_X; x++ )
        {
            for(uint32 y = 0; y < MAX_Y; y++)
            {
                _cell[x][y] = v[c];
                c++;
            }
        }
    }

    template<typename... Args> Matrix(real_t a, Args... args)
    {
        set_flag = 0;
        real_t* pcell = &_cell[0][0];
        *(pcell+set_flag++) = a;
        set(args...);
    }


    void operator = (Matrix m)
    {
        for(uint32 x = 0; x < MAX_X; x++)
        {
            for(uint32 y = 0; y < MAX_Y; y++)
            {
                cell(x, y) = m.cell(x, y);
            }
        }
    }

    Vector<MAX_Y> row_to_vector(uint32 row)
    {
        Vector<MAX_Y> ret;
        for(uint32 i = 0; i < MAX_Y; i++)
        {
            ret[i] = cell(i, row);
        }
        return ret;
    }

    Vector<MAX_X> col_to_vector(uint32 col)
    {
        Vector<MAX_X> ret;
        for(uint32 i = 0; i < MAX_X; i++)
        {
            ret[i] = cell(col, i);
        }
        return ret;
    }

    void vector_to_row(Vector<MAX_Y> v, uint32 row)
    {
        for(uint32 i = 0; i < MAX_X; i++)
        {
            cell(i, row) = v(i);
        }
    }

    void vector_to_col(Vector<MAX_X> v, uint32 col)
    {
        for(uint32 i = 0; i < MAX_Y; i++)
        {
            cell(col, i) = v(i);
        }
    }

    template <uint32 nn> Vector<nn> sub_vector(uint32 n)
    {
        real_t* pcell = &_cell[0][0];
        pcell += n;
        Vector<nn> ret;
        for(uint32 i = 0; i < nn; i++)
            ret[i] = *pcell++;
        return ret;
    }

    real_t& operator ()(uint32 x, uint32 y)
    {
        return _cell[x][y];
    }

    uint32 set(uint32 v, real_t value)
    {
        real_t* pcell = _cell;
        *(pcell+v) = value;
        return v;
    }

    void set(real_t a)
    {
        real_t* pcell = &_cell[0][0];
        *(pcell+set_flag) = a;
        set_flag = 0;
    }

    template<typename... Args> void set(real_t a, Args... args)
    {
        real_t* pcell = &_cell[0][0];
        *(pcell+set_flag++) = a;
        set(args...);
    }

    void set_diagonal(real_t a)
    {
        (*this)(set_flag, set_y_flag) = a;
        set_flag = set_y_flag = 0;
    }

    template<typename... Args> void set_diagonal(real_t a, Args... args)
    {
        (*this)(set_flag++, set_y_flag++) = a;
        set_diagonal(args...);
    }

    template<uint32 N> void set_diagonal(Vector<N> v)
    {
        uint32 n = min(MAX_X, MAX_Y);
        for(uint32 i = 0; i < n; i++)
            _cell[i][i] = v[i];
    }

    template<uint32 N> Vector<N> get_diagonal_vector()
    {
        Vector<min(MAX_X, MAX_Y)> v;
        uint32 n = min(MAX_X, MAX_Y);
        for(uint32 i = 0; i < n; i++)
            v[i] = _cell[i][i];
        return v;
    }


    real_t& cell(uint32 x, uint32 y)
    {
        return _cell[x][y];
    }
    
    real_t cell(uint32 x, uint32 y) const
    {
    	return _cell[x][y];
    }

    Matrix operator + (Matrix m)
    {
        Matrix ret;
        for(uint32 x = 0; x < MAX_X; x++)
        {
            for(uint32 y = 0; y < MAX_Y; y++)
            {
                ret._cell[x][y] = _cell[x][y] + m._cell[x][y];
            }
        }
        return ret;
    }

    Matrix operator - (Matrix m)
    {
        Matrix ret;
        for(uint32 x = 0; x < MAX_X; x++)
        {
            for(uint32 y = 0; y < MAX_Y; y++)
            {
                ret._cell[x][y] = _cell[x][y] - m._cell[x][y];
            }
        }
        return ret;
    }

    Matrix operator * (real_t scalar)
    {
        Matrix ret;
        for(uint32 x = 0; x < MAX_X; x++)
        {
            for(uint32 y = 0; y < MAX_Y; y++)
            {
                ret._cell[x][y] = _cell[x][y] * scalar;
            }
        }
        return ret;
    }

    template <uint32 N> Matrix operator * (Matrix<N, MAX_X> m)
    {
        Matrix<N,MAX_X> ret;
        for(uint32 x = 0; x < MAX_X; x++)
        {
            for(uint32 y = 0; y < N; y++)
            {
                Vector<N> row = row_to_vector(x);
                Vector<N> col = m.col_to_vector(y);
                ret.cell(x, y) = row.dot(col);
            }
        }
        return ret;
    }

    Matrix<MAX_Y,MAX_X> transpose()
    {
        Matrix<MAX_Y,MAX_X> ret;
        for(uint32 x = 0; x < MAX_X; x++)
        {
            for(uint32 y = 0; y < MAX_Y; y++)
            {
                ret.cell(y, x) = cell(x, y);
            }
        }
        return ret;
    }

    Matrix<MAX_Y-1,MAX_X-1> minor_matrix(uint32 row, uint32 col) const
    {
        uint32 colCount = 0, rowCount = 0;
        Matrix<MAX_Y-1,MAX_X-1> ret;
        for(uint32 i = 0; i < MAX_X; i++ )
        {
            if( i != row )
            {
                for(uint32 j = 0; j < MAX_Y; j++ )
                {
                    if( j != col )
                    {
                        ret(rowCount, colCount) = cell(i, j);
                        colCount++;
                    }
                }
                rowCount++;
            }
        }
        return ret;
    }

    real_t determinant() const
    {

            real_t det = 0.0;
            for(uint32 i = 0; i < MAX_X; i++ )
            {
                auto minor = minor_matrix(0, i);
                det += (i%2==1?-1.0:1.0) * cell(0, i) * minor.determinant();
        }
        return det;

        return 0;
    }

    Matrix invert() const
    {
        Matrix ret;
        real_t det = determinant();

        for(uint32 x = 0; x < MAX_X; x++)
        {
            for(uint32 y = 0; y < MAX_Y; y++)
            {
                Matrix<MAX_Y-1,MAX_X-1> minor = minor_matrix(y, x);
                ret(x, y) = det*minor.determinant();
                if( (x+y)%2 == 1)
                    ret(x, y) = -ret(x, y);
            }
        }
        return ret;
    }

    void load_identity()
    {
        for(uint32 x = 0; x < MAX_X; x++)
        {
            for(uint32 y = 0; y < MAX_Y; y++)
            {
                cell(x, y) = 0;
            }
        }

        for(uint32 i = 0; i < min(MAX_X, MAX_Y); i++)
            cell(i, i) = 1;
    }

    Matrix lower_triangle() const
    {
        Matrix m = *this;
        for(uint32 x = 0; x < MAX_X; x++)
        {
            for(uint32 y = x+1; y < MAX_Y; y++)
                m.cell(x, y) = 0;
        }
        return m;
    }

    Matrix upper_triangle() const
    {
        Matrix m = *this;
        for(uint32 x = 0; x < MAX_X; x++)
        {
            for(uint32 y = MAX_X-x; y < MAX_Y; y++)
                m.cell(x, MAX_Y-y-1) = 0;
        }
        return m;
    }

    Matrix llt()
    {
        //LLT cholesky decomposition
        Matrix A = *this;
        Matrix L = lower_triangle();
        for (int i = 0; i < MAX_X; i++)
        {
            for (int j = 0; j < (i+1); j++)
            {
                real_t s = 0;
                for (int k = 0; k < j; k++)
                    s += L(i,k) * L(j,k);

                real_t v = 0;
                if(i == j)
                    v = sqrtf(A(i,i)-s);
                else
                    v = (1.0 / L(j,j) * (A(i,j) - s));
                if(isnan(v))
                    return L;
                L(i,j) = v;
            }
        }

        return L;
    }

    StaticString<MAX_X*MAX_Y*12> to_string()
    {
        StaticString<MAX_X*MAX_Y*12> ss;
        for(int i = 0; i < MAX_Y; i++)
        {
            for(int j = 0; j < MAX_X; j++)
            {
                ss += _cell[j][i];
                ss += "\t";
            }

            ss += "\n";
        }

        return ss;
    }

private:
    real_t _cell[MAX_X][MAX_Y];
    uint32 set_flag = 0;
    uint32 set_y_flag = 0;
};

template<>
inline real_t Matrix<1, 1>::determinant() const
{
    return cell(0, 0);
}


typedef Matrix<3, 3> Matrix3x3;
typedef Matrix<4, 4> Matrix4x4;

}

#endif
