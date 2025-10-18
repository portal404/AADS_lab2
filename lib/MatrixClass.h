#pragma once
#include "VectorClass.h"
#include <math.h>
#include <fstream>

#define _CRT_SECURE_NO_WARNINGS


// This is unironicly one of the worst things i've ever done in my entire life
// This code sucks
using namespace std;


template <class T>
class TDenseNormalMatrix
{
protected:
  TVector<TVector<T>> m;
  int row;
  int col;
public:
  TDenseNormalMatrix();
  TDenseNormalMatrix(int row_, int col_);
  TDenseNormalMatrix(int row_, int col_, const T& p);
  TDenseNormalMatrix(const TDenseNormalMatrix& obj);
  TDenseNormalMatrix(TDenseNormalMatrix&& obj);
  ~TDenseNormalMatrix();

  int GetRows() const;
  int GetColumns() const;
  TVector<TVector<T>>& GetMatrix();
  const TVector<TVector<T>>& GetMatrix() const;

  TDenseNormalMatrix<T> operator+(const TDenseNormalMatrix<T>& obj);
  TDenseNormalMatrix<T> operator-(const TDenseNormalMatrix<T>& obj);
  TDenseNormalMatrix<T> operator*(const TDenseNormalMatrix<T>& obj);
  TDenseNormalMatrix<T> operator*(const T mul);

  TDenseNormalMatrix<T>& operator=(const TDenseNormalMatrix<T>& obj);
  TDenseNormalMatrix<T>& operator=(TDenseNormalMatrix<T>&& obj);
  bool operator==(const TDenseNormalMatrix<T>& obj);

  template <class O>
  friend ostream& operator<<(ostream& o, TDenseNormalMatrix<O>& v);
  template <class I>
  friend istream& operator>>(istream& i, TDenseNormalMatrix<I>& v);

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

  int ValueCount(const T looking_for); // Количество вхождений доп номер 2
  TDenseNormalMatrix AllOccurrences(const T value); // Все вхождения значения доп номер 3
};


// Класс итератора

template<class T>
class TIterator
{
protected:
  TDenseNormalMatrix<T>& p;
  int row;
  int col;
public:
  TIterator(TDenseNormalMatrix<T>& m, int row_, int col_);
  T& operator*();
  TIterator& operator++();
  TIterator& operator++(int);
  bool operator != (const TIterator& p);
};


/// Сам код
template<class T>
inline TDenseNormalMatrix<T>::TDenseNormalMatrix(): row(0), col(0)
{
}

template<class T>
inline TDenseNormalMatrix<T>::TDenseNormalMatrix(int row_, int col_)
: row(row_), col(col_)
{
  if (row_ < 0 || col_ <0) throw "IndexOutOfRange";
  m.SetLen(row_);
  for (int i = 0; i < row_; ++i) m[i].SetLen(col);
}

template<class T>
inline TDenseNormalMatrix<T>::TDenseNormalMatrix(int rows_, int cols_, const T& value)
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
inline TDenseNormalMatrix<T>::TDenseNormalMatrix(const TDenseNormalMatrix<T> &obj)
: row(obj.row), col(obj.col)
{
  m.SetLen(row);
  for (int i = 0; i < row; i++) m[i] = obj.m[i];
}

template<class T>
inline TDenseNormalMatrix<T>::TDenseNormalMatrix(TDenseNormalMatrix<T> &&obj)
: m(move(obj.m)), row(obj.row), col(obj.col) // move чтобы не копировать все
{
  obj.row = 0;
  obj.col = 0;
}

template<class T>
inline TDenseNormalMatrix<T>::~TDenseNormalMatrix()
{
  row = 0;
  col = 0;
}

template<class T>
inline int TDenseNormalMatrix<T>::GetRows() const
{
  return row;
}

template<class T>
inline int TDenseNormalMatrix<T>::GetColumns() const
{
  return col;
}

template<class T>
inline TVector<TVector<T>>& TDenseNormalMatrix<T>::GetMatrix()
{
  return m;
}

template<class T>
inline const TVector<TVector<T>>& TDenseNormalMatrix<T>::GetMatrix() const
{
  return m;
}

template<class T>
inline TDenseNormalMatrix<T> TDenseNormalMatrix<T>::operator+(const TDenseNormalMatrix& obj)
{
  int row_ = GetRows();
  int col_ = GetColumns();
  if (row_ != obj.GetRows() || col_ != obj.GetColumns()) throw "Can't plus";
  TDenseNormalMatrix<T> res(row_, col_);
  for (int i = 0; i < row_; ++i)
    for (int j = 0; j < col_; ++j) res.m[i][j] = (*this)[i][j] + obj[i][j]; // Если падает + проблема в этой строке
  return res;
}

template<class T>
inline TDenseNormalMatrix<T> TDenseNormalMatrix<T>::operator-(const TDenseNormalMatrix& obj)
{
  int row_ = GetRows();
  int col_ = GetColumns();
  if (row_ != obj.GetRows() || col_ != obj.GetColumns()) throw "Can't minus";
  TDenseNormalMatrix<T> res(row_, col_);
  for (int i = 0; i < row_; ++i)
    for (int j = 0; j < col_; ++j) res.m[i][j] = (*this)[i][j] - obj[i][j]; // Тут тоже смотреть (*this) если не робит
  return res;
}

// Умножение на число
template<class T>
inline TDenseNormalMatrix<T> TDenseNormalMatrix<T>::operator*(const T mul)
{
  int row_ = GetRows();
  int col_ = GetColumns();
  TDenseNormalMatrix<T> res(row_, col_);
  if (mul == 0) return res;
  for (int i = 0; i < row_; ++i)
    for (int j = 0; j < col_; ++j) res.m[i][j] = (*this)[i][j] * mul;
  return res;
}

// Умножение матриц
template<class T>
inline TDenseNormalMatrix<T> TDenseNormalMatrix<T>::operator*(const TDenseNormalMatrix& obj)
{
  int row_ = GetRows();
  int col_ = GetColumns();
  if (row_ != obj.GetColumns()) throw "Cannot multiply";
  TDenseNormalMatrix<T> res(row_, col_);
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
inline TDenseNormalMatrix<T>& TDenseNormalMatrix<T>::operator=(const TDenseNormalMatrix<T>& obj)
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
inline TDenseNormalMatrix<T>& TDenseNormalMatrix<T>::operator=(TDenseNormalMatrix<T>&& obj)
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
inline bool TDenseNormalMatrix<T>::operator==(const TDenseNormalMatrix<T>& obj)
{
  int row = GetRows();
  int col = GetColumns();
  if (row != obj.GetRows() || col != obj.GetColumns()) throw "Cannot equal";
  bool flag = true;
  for (int i = 0; i < row; ++i)
    for (int j = 0; j < col; ++j) if ((*this)[i][j] != obj[i][j]) flag = false;
  return flag;
}


// Output i really hope it works
template <class O>
inline ostream& operator<<(ostream& o, TDenseNormalMatrix<O>& p)
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
// Input plz work lol
template <class I>
inline istream& operator>>(istream& is, TDenseNormalMatrix<I>& p)
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
/*
template<class T>
inline void TDenseNormalMatrix<T>::SaveToFile(const char* path)
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
inline void TDenseNormalMatrix<T>::ReadFromFile(const char* path)
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

template <class T>
inline int TDenseNormalMatrix<T>::ValueCount(const T looking_for)
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
inline TDenseNormalMatrix<T> TDenseNormalMatrix<T>::AllOccurrences(const T value)
{
  int rows = GetRows();
  int cols = GetColumns();

  int count = 0;
  for (int i = 0; i < rows; ++i)
    for (int j = 0; j < cols; ++j)
      if ((*this)[i][j] == value) count++;

  TDenseNormalMatrix<T> ans(count, 2);

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


template<class T>
inline TVector<T>& TDenseNormalMatrix<T>::operator[](int row_)
{
  if (row_ < 0 || row_ >= row)
    throw std::out_of_range("Row index out of range");
  return m[row_];
}

template<class T>
inline const TVector<T>& TDenseNormalMatrix<T>::operator[](int row_) const
{
  if (row_ < 0 || row_ >= row)
    throw std::out_of_range("Row index out of range");
  return m[row_];
}

template<class T>
inline bool TDenseNormalMatrix<T>::IsEmpty() const
{
  return row == 0 || col == 0;
}

