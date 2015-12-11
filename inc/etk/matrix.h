
#ifndef IMUMATH_MATRIX_HPP
#define IMUMATH_MATRIX_HPP

#include <stdint.h>
#include "vector.h"

namespace etk
{


template <uint32_t MAX_X, uint32_t MAX_Y> class Matrix
{
public:
    Matrix()
    {
        for (uint32_t x = 0; x < MAX_X; x++ )
        {
            for(uint32_t y = 0; y < MAX_Y; y++)
            {
                _cell[x][y] = 0.0f;
            }
        }
    }

    Matrix(const Matrix &v)
    {
        for (uint32_t x = 0; x < MAX_X; x++ )
        {
            for(uint32_t y = 0; y < MAX_Y; y++)
            {
                _cell[x][y] = v._cell[x][y];
            }
        }
    }

    Matrix(const Vector<MAX_X*MAX_Y>& v)
    {
        uint32_t c = 0;
        for (uint32_t x = 0; x < MAX_X; x++ )
        {
            for(uint32_t y = 0; y < MAX_Y; y++)
            {
                _cell[x][y] = v[c];
                c++;
            }
        }
    }

    template<typename... Args> Matrix(float a, Args... args)
    {
        float* pcell = &_cell[0][0];
        *(pcell+set_flag++) = a;
        set(args...);
    }


    void operator = (Matrix m)
    {
        for(uint32_t x = 0; x < MAX_X; x++)
        {
            for(uint32_t y = 0; y < MAX_Y; y++)
            {
                cell(x, y) = m.cell(x, y);
            }
        }
    }

    Vector<MAX_Y> row_to_vector(uint32_t y)
    {
        Vector<MAX_Y> ret;
        for(uint32_t i = 0; i < MAX_Y; i++)
        {
            ret[i] = _cell[i][y];
        }
        return ret;
    }

    Vector<MAX_X> col_to_vector(uint32_t x)
    {
        Vector<MAX_X> ret;
        for(uint32_t i = 0; i < MAX_X; i++)
        {
            ret[i] = _cell[x][i];
        }
        return ret;
    }

    void vector_to_row(Vector<MAX_Y> v, uint32_t row)
    {
        for(uint32_t i = 0; i < MAX_X; i++)
        {
            cell(i, row) = v(i);
        }
    }

    void vector_to_col(Vector<MAX_X> v, uint32_t col)
    {
        for(uint32_t i = 0; i < MAX_Y; i++)
        {
            cell(col, i) = v(i);
        }
    }

    template <uint32_t nn> Vector<nn> sub_vector(uint32_t n)
    {
        float* pcell = &_cell[0][0];
        pcell += n;
        Vector<nn> ret;
        for(uint32_t i = 0; i < nn; i++)
            ret[i] = *pcell++;
        return ret;
    }

    float& operator ()(uint32_t x, uint32_t y)
    {
        return _cell[x][y];
    }

    uint32_t set(uint32_t v, float value)
    {
        float* pcell = _cell;
        *(pcell+v) = value;
        return v;
    }

    void set(float a)
    {
        float* pcell = &_cell[0][0];
        *(pcell+set_flag) = a;
        set_flag = 0;
    }

    template<typename... Args> void set(float a, Args... args)
    {
        float* pcell = &_cell[0][0];
        *(pcell+set_flag++) = a;
        set(args...);
    }

    void set_diagonal(float a)
    {
        (*this)(set_flag, set_y_flag) = a;
        set_flag = set_y_flag = 0;
    }

    template<typename... Args> void set_diagonal(float a, Args... args)
    {
        (*this)(set_flag++, set_y_flag++) = a;
        set_diagonal(args...);
    }

    template<uint32_t N> void set_diagonal(Vector<N> v)
    {
        uint32_t n = etk::min<uint32_t>(MAX_X, MAX_Y);
        for(uint32_t i = 0; i < n; i++)
            _cell[i][i] = v[i];
    }

    template<uint32_t N> Vector<N> get_diagonal_vector()
    {
        Vector<etk::min<uint32_t>(MAX_X, MAX_Y)> v;
        uint32_t n = etk::min<uint32_t>(MAX_X, MAX_Y);
        for(uint32_t i = 0; i < n; i++)
            v[i] = _cell[i][i];
        return v;
    }


    float& cell(uint32_t x, uint32_t y)
    {
        return _cell[x][y];
    }


    Matrix operator + (Matrix m)
    {
        Matrix ret;
        for(uint32_t x = 0; x < MAX_X; x++)
        {
            for(uint32_t y = 0; y < MAX_Y; y++)
            {
                ret._cell[x][y] = _cell[x][y] + m._cell[x][y];
            }
        }
        return ret;
    }

    Matrix operator - (Matrix m)
    {
        Matrix ret;
        for(uint32_t x = 0; x < MAX_X; x++)
        {
            for(uint32_t y = 0; y < MAX_Y; y++)
            {
                ret._cell[x][y] = _cell[x][y] - m._cell[x][y];
            }
        }
        return ret;
    }

    Matrix operator * (float scalar)
    {
        Matrix ret;
        for(uint32_t x = 0; x < MAX_X; x++)
        {
            for(uint32_t y = 0; y < MAX_Y; y++)
            {
                ret._cell[x][y] = _cell[x][y] * scalar;
            }
        }
        return ret;
    }

    template <uint32_t N> Matrix operator * (Matrix<N, MAX_X> m)
    {
        Matrix<N,MAX_X> ret;
        for(uint32_t x = 0; x < MAX_X; x++)
        {
            for(uint32_t y = 0; y < N; y++)
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
        for(uint32_t x = 0; x < MAX_X; x++)
        {
            for(uint32_t y = 0; y < MAX_Y; y++)
            {
                ret.cell(y, x) = cell(x, y);
            }
        }
        return ret;
    }

    Matrix<MAX_Y-1,MAX_X-1> minor_matrix(uint32_t row, uint32_t col)
    {
        uint32_t colCount = 0, rowCount = 0;
        Matrix<MAX_Y-1,MAX_X-1> ret;
        for(uint32_t i = 0; i < MAX_X; i++ )
        {
            if( i != row )
            {
                for(uint32_t j = 0; j < MAX_Y; j++ )
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

    float determinant()
    {
        if(MAX_X == 1)
            return cell(0, 0);

        float det = 0.0;
        for(uint32_t i = 0; i < MAX_X; i++ )
        {
            Matrix<MAX_Y-1,MAX_X-1> minor = minor_matrix(0, i);
            det += (i%2==1?-1.0:1.0) * cell(0, i) * minor.determinant();
        }
        return det;
    }

    Matrix invert()
    {
        Matrix ret;
        float det = determinant();

        for(uint32_t x = 0; x < MAX_X; x++)
        {
            for(uint32_t y = 0; y < MAX_Y; y++)
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
        for(uint32_t x = 0; x < MAX_X; x++)
        {
            for(uint32_t y = 0; y < MAX_Y; y++)
            {
                cell(x, y) = 0;
            }
        }

        for(uint32_t i = 0; i < etk::min<uint32_t>(MAX_X, MAX_Y); i++)
            cell(i, i) = 1;
    }

    Matrix lower_triangle()
    {
        Matrix m = *this;
        for(uint32_t x = 0; x < MAX_X; x++)
        {
            for(uint32_t y = x+1; y < MAX_Y; y++)
                m.cell(x, y) = 0;
        }
        return m;
    }

    Matrix upper_triangle()
    {
        Matrix m = *this;
        for(uint32_t x = 0; x < MAX_X; x++)
        {
            for(uint32_t y = MAX_X-x; y < MAX_Y; y++)
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
                float s = 0;
                for (int k = 0; k < j; k++)
                    s += L(i,k) * L(j,k);

                float v = 0;
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

private:
    float _cell[MAX_X][MAX_Y];
    uint32_t set_flag = 0;
    uint32_t set_y_flag = 0;
};


}

#endif

