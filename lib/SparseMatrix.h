#pragma once
#include "VectorClass.h"
#include <math.h>
#include <fstream>

#define _CRT_SECURE_NO_WARNINGS

using namespace std;


template <class T>
class TSparseMatrix
{
protected:
  TVector<TVector<T>> m;
  int row;
  int col;
public:
  TSparseMatrix();
  TSparseMatrix(int row_, int col_);
  TSparseMatrix(int row_, int col_, const T& p);
  TSparseMatrix(const TSparseMatrix& obj);
  TSparseMatrix(TSparseMatrix&& obj);
  ~TSparseMatrix();

  int GetRows() const;
  int GetColumns() const;
  TVector<TVector<T>>& GetMatrix();
  const TVector<TVector<T>>& GetMatrix() const;

  // Класс итератора

  class TIterator
  {
  protected:
    TSparseMatrix<T>& p;
    int row;
    int col;
  public:
    TIterator(TSparseMatrix<T>& m, int row_, int col_);
    T& operator*();
    TIterator& operator++();
    TIterator& operator++(int);
    bool operator==(const TIterator& f) const;
    bool operator!= (const TIterator& f);
  };


  TIterator begin();
  TIterator end();

  TSparseMatrix<T> operator+(const TSparseMatrix<T>& obj);
  TSparseMatrix<T> operator-(const TSparseMatrix<T>& obj);
  TSparseMatrix<T> operator*(const TSparseMatrix<T>& obj);
  TSparseMatrix<T> operator*(const T mul);

  TSparseMatrix<T>& operator=(const TSparseMatrix<T>& obj);
  TSparseMatrix<T>& operator=(TSparseMatrix<T>&& obj);
  bool operator==(const TSparseMatrix<T>& obj);
  bool operator!=(const TSparseMatrix<T>& obj);

  template <class O>
  friend ostream& operator<<(ostream& o, TSparseMatrix<O>& v);
  template <class I>
  friend istream& operator>>(istream& i, TSparseMatrix<I>& v);

  /*
  virtual void SaveToFile(const char* path = "./data.txt");
  virtual void ReadFromFile(const char* path = "./data.txt");
   */

  TVector<T>& operator[](int row);
  const TVector<T>& operator[](int row) const;
  bool IsEmpty() const;
  bool IsFull() const;
  void ApplyToAll(const function<void(T&)>& func);
  void ApplyToAll(const function<void(const T&)>& func) const;

  // Допы:
  int ValueCount(const T looking_for);
  TSparseMatrix AllOccurrences(const T value);

  T FirstNorm() const;
  T SecondNorm() const;
  T HolderNorm(T p) const;
  T InfinityNorm() const;
};




/// Сам код
template<class T>
inline TSparseMatrix<T>::TSparseMatrix(): row(0), col(0)
{
}

template<class T>
inline TSparseMatrix<T>::TSparseMatrix(int row_, int col_)
     : row(row_), col(col_)
{
  if (row_ < 0 || col_ <0) throw "IndexOutOfRange";
  m.SetLen(row_);
  for (int i = 0; i < row_; ++i) m[i].SetLen(col);
}

template<class T>
inline TSparseMatrix<T>::TSparseMatrix(int rows_, int cols_, const T& value)
     : row(rows_), col(cols_)
{
  if (rows_ < 0 || cols_ < 0) throw "Le error";
  m.SetLen(row);
  for (int i = 0; i < row; i++)
  {
    m[i].SetLen(col);
    for (int j = 0; j < col; j++) m[i][j] = value;
  }
}

template<class T>
inline TSparseMatrix<T>::TSparseMatrix(const TSparseMatrix<T> &obj)
     : row(obj.row), col(obj.col)
{
  m.SetLen(row);
  for (int i = 0; i < row; i++) m[i] = obj.m[i];
}

template<class T>
inline TSparseMatrix<T>::TSparseMatrix(TSparseMatrix<T> &&obj)
     : m(move(obj.m)), row(obj.row), col(obj.col) // move чтобы не копировать все
{
  obj.row = 0;
  obj.col = 0;
}

template<class T>
inline TSparseMatrix<T>::~TSparseMatrix()
{
  row = 0;
  col = 0;
}

template<class T>
inline int TSparseMatrix<T>::GetRows() const
{
  return row;
}

template<class T>
inline int TSparseMatrix<T>::GetColumns() const
{
  return col;
}

template<class T>
inline TVector<TVector<T>>& TSparseMatrix<T>::GetMatrix()
{
  return m;
}

template<class T>
inline const TVector<TVector<T>>& TSparseMatrix<T>::GetMatrix() const
{
  return m;
}

template<class T>
inline TSparseMatrix<T> TSparseMatrix<T>::operator+(const TSparseMatrix& obj)
{
  int row_ = GetRows();
  int col_ = GetColumns();
  if (row_ != obj.GetRows() || col_ != obj.GetColumns()) throw "Can't plus";
  TSparseMatrix<T> res(row_, col_);
  for (int i = 0; i < row_; ++i)
    for (int j = 0; j < col_; ++j) res.m[i][j] = (*this)[i][j] + obj[i][j]; // Если падает + проблема в этой строке
  return res;
}

template<class T>
inline TSparseMatrix<T> TSparseMatrix<T>::operator-(const TSparseMatrix& obj)
{
  int row_ = GetRows();
  int col_ = GetColumns();
  if (row_ != obj.GetRows() || col_ != obj.GetColumns()) throw "Can't minus";
  TSparseMatrix<T> res(row_, col_);
  for (int i = 0; i < row_; ++i)
    for (int j = 0; j < col_; ++j) res.m[i][j] = (*this)[i][j] - obj[i][j]; // оно работает o_O
  return res;
}

// Умножение на число
template<class T>
inline TSparseMatrix<T> TSparseMatrix<T>::operator*(const T mul)
{
  int row_ = GetRows();
  int col_ = GetColumns();
  TSparseMatrix<T> res(row_, col_);
  if (mul == 0) return res;
  for (int i = 0; i < row_; ++i)
    for (int j = 0; j < col_; ++j) res.m[i][j] = (*this)[i][j] * mul;
  return res;
}

// Умножение матриц
template<class T>
inline TSparseMatrix<T> TSparseMatrix<T>::operator*(const TSparseMatrix& obj)
{
  int row_ = GetRows();
  int col_ = GetColumns();
  if (row_ != obj.GetColumns()) throw "Cannot multiply";
  TSparseMatrix<T> res(row_, col_);
  for (int i = 0; i < row_; ++i)
    for (int j = 0; j < obj.GetColumns(); ++j)
    {
      res.m[i][j] = 0;
      for (int k = 0; k < col; ++k) res.m[i][j] += (*this)[i][k] * obj[k][j];
    }
  return res;
}

// Присваивает только матрицы одинакогого размера
template<class T>
inline TSparseMatrix<T>& TSparseMatrix<T>::operator=(const TSparseMatrix<T>& obj)
{
  int row = GetRows();
  int col = GetColumns();
  if (row != obj.GetRows() || col != obj.GetColumns()) throw "Cannot equal those";
  for (int i = 0; i < row; ++i)
    for (int j = 0; j < col; ++j) (*this)[i][j] = obj[i][j];
  return *this;
}

// Посмотреть где отличия (upd 18.10.25 я не посмотрел)
template<class T>
inline TSparseMatrix<T>& TSparseMatrix<T>::operator=(TSparseMatrix<T>&& obj)
{
  if (this != &obj) {
    m = std::move(obj.m);
    row = obj.row;
    col = obj.col;
    obj.row = 0;
    obj.col = 0;
  }
  return *this;
}
template<class T>
inline bool TSparseMatrix<T>::operator==(const TSparseMatrix<T>& obj)
{
  int row = GetRows();
  int col = GetColumns();
  if (row != obj.GetRows() || col != obj.GetColumns()) throw "Cannot equal";
  bool flag = true;
  for (int i = 0; i < row; ++i)
    for (int j = 0; j < col; ++j) if ((*this)[i][j] != obj[i][j]) flag = false;
  return flag;
}

template<class T>
inline bool TSparseMatrix<T>::operator!=(const TSparseMatrix<T>& obj)
{
  return !(*this == obj);
}

// Output (not sure if it works)
template <class O>
inline ostream& operator<<(ostream& o, TSparseMatrix<O>& p)
{
  int row_ = p.GetRows();
  int col_ = p.GetColumns();
  for (int i = 0; i < row_; ++i)
  {
    for (int j = 0; j < col_; ++j)
    {
      o << p[i][j] << " ";
    }
    o << endl;
  }
  return o;
}

// Input
template <class I>
inline istream& operator>>(istream& is, TSparseMatrix<I>& p)
{
  int row = p.GetRows();
  int col = p.GetColumns();
  for (int i = 0; i < row; i++)
  {
    for (int j = 0; j < col; j++)
    {
      cout <<"a[" << i << "][" << j << "]:  ";
      is >> p[i][j];
      cout << endl;
    }
  }
  return is;
}

// ----------------
template<class T>
inline TSparseMatrix<T>::TIterator::TIterator(TSparseMatrix<T>& m, int row_, int col_)
     : p(m), row(row_), col(col_){}

template<class T>
inline T& TSparseMatrix<T>::TIterator::operator*()
{
  return p[row][col];
}

template<class T>
inline typename TSparseMatrix<T>::TIterator& TSparseMatrix<T>::TIterator::operator++()
{
  ++col;
  if (col >= p.GetColumns())
  {
    col = 0;
    ++row;
  }
  return *this;
}

template<class T>
inline bool TSparseMatrix<T>::TIterator::operator==(const TIterator& f) const
{
  return p == f.p && row == f.row && col == f.col;
}

template<class T>
inline bool TSparseMatrix<T>::TIterator::operator!=(const TIterator& f)
{
  return !(*this == f);
}

template<class T>
inline typename TSparseMatrix<T>::TIterator TSparseMatrix<T>::begin()
{
  return Iterator(this, 0, 0);
}

template<class T>
inline typename TSparseMatrix<T>::TIterator TSparseMatrix<T>::end()
{
  return Iterator(this, row, 0);
}

// ----------------------


/* Убрал чтобы не захламлять. Оно работает, главное верить

template<class T>
inline void TSparseMatrix<T>::SaveToFile(const char* path)
{
  ofstream FileLoc(path);
  if (!FileLoc.is_open()) throw "File dosen't work";
  if (FileLoc.is_open())
  {
    FileLoc << GetRows() << "\n";
    FileLoc << GetColumns() << "\n";
    for (int i = 0; i < GetRows(); ++i)
    {
      for (int j = 0; j < GetColumns(); ++j)
        FileLoc << (*this)[i][j] << "\n";
    }
  }
  FileLoc.close();
}

template<class T>
inline void TSparseMatrix<T>::ReadFromFile(const char* path)
{
  ifstream FileLoc(path);
  if (!FileLoc.is_open()) throw "File dosen't work";
  if (FileLoc.is_open())
  {
    int row_ = 0, col_ = 0;
    FileLoc >> row_;
    FileLoc >> col_;
    if (row_ != GetRows() || col_ != GetColumns()) throw "Cannot read, the size is different";
    for (int i = 0; i < row_; ++i)
      for (int j = 0; j < col_; ++j)
        FileLoc >> (*this)[i][j];
  }
  FileLoc.close();
}
*/

template<class T>
inline TVector<T>& TSparseMatrix<T>::operator[](int row_)
{
  if (row_ < 0 || row_ >= row)
    throw std::out_of_range("Row index out of range");
  return m[row_];
}

template<class T>
inline const TVector<T>& TSparseMatrix<T>::operator[](int row_) const
{
  if (row_ < 0 || row_ >= row) throw "Index out of range!";
  return m[row_];
}

template<class T>
inline bool TSparseMatrix<T>::IsEmpty() const
{
  return row == 0 || col == 0;
}

template<class T>
inline bool TSparseMatrix<T>::IsFull() const
{
  return false; // 0 clues how this method even supposed to work, math gives no clear definition of a "full" matrix
}

template<class T>
inline void TSparseMatrix<T>::ApplyToAll(const function<void(T&)>& func)
{
  m.ApplyToAll(func);
}

template<class T>
inline void TSparseMatrix<T>::ApplyToAll(const function<void(const T&)>& func) const
{
  m.ApplyToAll(func);
}


// Допы:

template<class T>
T TSparseMatrix<T>::FirstNorm() const
{
  T maxSum = 0;
  for (int j = 0; j < col; ++j)
  {
    T sum = 0;
    for (int i = 0; i < row; ++i)
      sum += std::abs(m[i][j]);
    if (sum > maxSum) maxSum = sum;
  }
  return maxSum;
}

template<class T>
T TSparseMatrix<T>::SecondNorm() const
{
  T sum = 0;
  for (int i = 0; i < row; ++i)
    for (int j = 0; j < col; ++j)
      sum += m[i][j] * m[i][j];
  return sqrt(sum);
}

template<class T>
T TSparseMatrix<T>::HolderNorm(T p) const
{
  T sum = 0;
  for (int i = 0; i < row; ++i)
    for (int j = 0; j < col; ++j)
      sum += pow(std::abs(m[i][j]), p);
  return pow(sum, T(1) / p);
}

template<class T>
T TSparseMatrix<T>::InfinityNorm() const
{
  T maxSum = 0;
  for (int i = 0; i < row; ++i)
  {
    T sum = 0;
    for (int j = 0; j < col; ++j)
      sum += std::abs(m[i][j]);
    if (sum > maxSum) maxSum = sum;
  }
  return maxSum;
}

template <class T>
inline int TSparseMatrix<T>::ValueCount(const T looking_for)
{
  int count = 0;
  auto& matrix = GetMatrix();
  int rows = GetRows();
  int cols = GetColumns();

  for (int i = 0; i < rows; ++i)
    for (int j = 0; j < cols; ++j)
      if (matrix[i][j] == looking_for) count++;
  return count;
}

template <class T>
inline TSparseMatrix<T> TSparseMatrix<T>::AllOccurrences(const T value)
{
  int rows = GetRows();
  int cols = GetColumns();

  int count = 0;
  for (int i = 0; i < rows; ++i)
    for (int j = 0; j < cols; ++j)
      if ((*this)[i][j] == value) count++;

  TSparseMatrix<T> ans(count, 2);

  int n = 0;
  for (int i = 0; i < rows; ++i)
  {
    for (int j = 0; j < cols; ++j)
    {
      if ((*this)[i][j] == value)
      {
        if (n < count)
        {
          ans[n][0] = i;
          ans[n][1] = j;
          n++;
        }
      }
    }
  }
  return ans;
}
