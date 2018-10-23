#ifndef MATRIX_H
#define MATRIX_H

template <class T>
class Matrix {
public:
  Matrix();
  Matrix(size_t rows, size_t cols);
  T& operator() (size_t row, size_t col);
  T  operator() (size_t row, size_t col) const;
  // ...
 ~Matrix();                              // Destructor
  Matrix(const Matrix& m);               // Copy constructor
  Matrix& operator= (const Matrix& m);   // Assignment operator
  // ...
private:
  size_t rows_, cols_;
  T* data_;
};

template <class T>
Matrix<T>::Matrix()
  : rows_ (0), cols_ (0)
{
  data_ = NULL;
}

template<class T>
inline
Matrix<T>::Matrix(size_t rows, size_t cols)
  : rows_ (rows), cols_ (cols)
{
  data_ = new T[rows * cols];
}

template<class T>
inline
Matrix<T>::~Matrix()
{
  delete[] data_;
}

template<class T>
inline
T& Matrix<T>::operator() (size_t row, size_t col)
{
  return data_[cols_*row + col];
}

template<class T>
inline
T Matrix<T>::operator() (size_t row, size_t col) const
{
  return data_[cols_*row + col];
}

template<class T>
inline
Matrix<T>& Matrix<T>::operator= (const Matrix<T>& m)
{
    if (this == &m)
        return *this;
    for (int x = 0; x < m.rows_; x++) {
          for (int y = 0; y < m.cols_; y++){
              *this(x,y) = m(x,y);
          }
      }
    return *this;
}

#endif
