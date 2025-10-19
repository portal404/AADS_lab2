#pragma once
#include "VectorClass.h"
#include <math.h>
#include <fstream>


template <class T>
class TTriangleMatrix
{
protected:
  TVector<T> data;
  int size;
  bool pos; // false - нижняя, true - верхняя
public:
  TTriangleMatrix();
  TTriangleMatrix(int size_, bool pos_ = true);
  TTriangleMatrix(int size_, bool pos_, const T& p);
  TTriangleMatrix(const TTriangleMatrix& p);
  TTriangleMatrix(TTriangleMatrix&& p);
  ~TTriangleMatrix();

  class TIterator
  {
  protected:
    TTriangleMatrix<T>& p;
    int row;
    int col;
  public:
    TIterator(TTriangleMatrix<T>& m, int row_, int col_);
    T& operator*();
    TIterator& operator++();
    TIterator& operator++(int);
    bool operator==(const TIterator& f) const;
    bool operator!= (const TIterator& f);
  };

  TIterator begin();
  TIterator end();

  int GetSize() const;
  bool GetPos() const;
  const TVector<T>& GetData() const;

  TTriangleMatrix<T> operator+(const TTriangleMatrix<T>& obj);
  TTriangleMatrix<T> operator-(const TTriangleMatrix<T>& obj);
  TTriangleMatrix<T> operator*(const TTriangleMatrix<T>& obj);
  TTriangleMatrix<T> operator*(const T& mul); // Если не работает, убрать &

  TTriangleMatrix<T>& operator=(const TTriangleMatrix<T>& obj);
  TTriangleMatrix<T>& operator=(TTriangleMatrix<T>&& obj);
  bool operator==(const TTriangleMatrix<T>& obj);
  bool operator!=(const TTriangleMatrix<T>& obj);

  T& operator()(int row, int col);
  const T& operator()(int row, int col) const;

  template <class O>
  friend ostream& operator<<(ostream& o, TTriangleMatrix<O>& v);
  template <class I>
  friend istream& operator>>(istream& i, TTriangleMatrix<I>& v);

  /*
  virtual void SaveToFile(const char* path = "./data.txt");
  virtual void ReadFromFile(const char* path = "./data.txt");
   */

  TVector<T>& operator()(int row);
  const TVector<T>& operator()(int row) const;
  bool IsEmpty() const;
  bool IsFull() const;
  void ApplyToAll(const function<void(T&)>& func);
  void ApplyToAll(const function<void(const T&)>& func) const;

  int ValueCount(const T looking_for); // Количество вхождений доп
  TTriangleMatrix AllOccurrences(const T value); // Все вхождения значения доп

  T FirstNorm() const;
  T SecondNorm() const;
  T HolderNorm(T p) const;
  T InfinityNorm() const;
};

template<class T>
inline TTriangleMatrix<T>::TTriangleMatrix() : size(0), pos(true)
{}

template<class T>
inline TTriangleMatrix<T>::TTriangleMatrix(int size_, bool pos_)
     : size(size_), pos(pos_)
{
  if (size_ < 0) throw "Size cannot be <0";

  int ans_size = size * (size + 1) / 2;
  data.SetLen(ans_size);
}

template<class T>
inline TTriangleMatrix<T>::TTriangleMatrix(int size_, bool pos_, const T& p)
     : size(size_), pos(pos_)
{
  if (size_ < 0) throw "Size cannot be <0";
  int ans_size = size * (size + 1) / 2;
  data.SetLen(ans_size);
  for (int i = 0; i < data.GetLen(); i++) data[i] = p;
}

template<class T>
inline TTriangleMatrix<T>::TTriangleMatrix(const TTriangleMatrix& p)
     : data(p.data), size(p.size), pos(p.pos)
{}

template<class T>
inline TTriangleMatrix<T>::TTriangleMatrix(TTriangleMatrix&& p)
     : data(std::move(p.data)), size(p.size), pos(p.pos)
{
  p.size = 0; // Нет смысла трогать pos
}

template<class T>
inline TTriangleMatrix<T>::~TTriangleMatrix()
{
  size = 0; // Зануляем size
}

template<class T>
inline int TTriangleMatrix<T>::GetSize() const
{
  return size;
}

template<class T>
inline const TVector<T>& TTriangleMatrix<T>::GetData() const
{
  return data;
}

template<class T>
inline bool TTriangleMatrix<T>::GetPos() const
{
  return pos;
}

//-----------
template<class T>
inline TTriangleMatrix<T>::TIterator::TIterator(TTriangleMatrix<T>& m, int row_, int col_)
     : p(m), row(row_), col(col_){}


template<class T>
inline T& TTriangleMatrix<T>::TIterator::operator*()
{
  return p(row,col);
}

template<class T>
inline typename TTriangleMatrix<T>::TIterator& TTriangleMatrix<T>::TIterator::operator++()
{
  ++col;
  if (col >= p.size)
  {
    col = 0;
    ++row;
  }
  return *this;
}

template<class T>
inline bool TTriangleMatrix<T>::TIterator::operator==(const TIterator& f) const
{
  return p == f.p && row == f.row && col == f.col;
}

template<class T>
inline bool TTriangleMatrix<T>::TIterator::operator!=(const TIterator& f)
{
  return !(*this == f);
}

template<class T>
inline typename TTriangleMatrix<T>::TIterator TTriangleMatrix<T>::begin()
{
  return TIterator(*this, 0, 0);
}

template<class T>
inline typename TTriangleMatrix<T>::TIterator TTriangleMatrix<T>::end()
{
  return TIterator(*this, size, 0);
}
// --------------
template<class T>
inline TTriangleMatrix<T> TTriangleMatrix<T>::operator+(const TTriangleMatrix<T>& obj)
{
  if (size != obj.size || pos != obj.pos)
    throw "Cannot +!";

  TTriangleMatrix<T> res(size, pos);
  for (int i = 0; i < data.GetLen(); i++) res.data[i] = data[i] + obj.data[i];
  return res;
}

template<class T>
inline TTriangleMatrix<T> TTriangleMatrix<T>::operator-(const TTriangleMatrix<T>& obj)
{
  if (size != obj.size || pos != obj.pos)
    throw "Cannot -!";

  TTriangleMatrix<T> res(size, pos);
  for (int i = 0; i < data.GetLen(); i++) res.data[i] = data[i] - obj.data[i];
  return res;
}

template<class T>
inline TTriangleMatrix<T> TTriangleMatrix<T>::operator*(const TTriangleMatrix<T>& obj)
{
  // Нет точного понятия, что такое "умножение треугольных матриц", поэтому пропущу данный оператор
  return 1;
}

template<class T>
inline TTriangleMatrix<T> TTriangleMatrix<T>::operator*(const T& mul)
{
  TTriangleMatrix<T> res(size, pos);
  for (int i = 0; i < data.GetLen(); i++) res.data[i] = data[i] * mul;
  return res;
}

template<class T>
inline TTriangleMatrix<T>& TTriangleMatrix<T>::operator=(const TTriangleMatrix<T>& obj)
{
  if (this == &obj) return *this;
  size = obj.size;
  data = obj.data;
  pos = obj.pos;
  return *this;
}

template<class T>
inline TTriangleMatrix<T>& TTriangleMatrix<T>::operator=(TTriangleMatrix<T>&& obj)
{
  if (this == &obj) return *this;
  size = obj.size;
  data = move(obj.data);
  pos = obj.pos;
  obj.size = 0;
  return *this;
}

template<class T>
inline bool TTriangleMatrix<T>::operator==(const TTriangleMatrix<T>& obj)
{
  if (size != obj.size || pos != obj.pos) return false;
  else return data == obj.data;
}

template<class T>
inline bool TTriangleMatrix<T>::operator!=(const TTriangleMatrix<T>& obj)
{
  return !(*this == obj);
}


template<class T>
inline T& TTriangleMatrix<T>::operator()(int row, int col)
{
  if (row < 0 || row >= size || col < 0 || col >= size)
    throw "Index OutOfRange";

  if (pos && col < row) throw "Error 1";
  if (!pos && col > row) throw "Error 2";
  int index = pos ? // блин как же оно написано?????
              (row * size - row * (row - 1) / 2 + (col - row)) :
              (row * (row + 1) / 2 + col);

  return data[index];
}

template<class T>
inline const T& TTriangleMatrix<T>::operator()(int row, int col) const
{
  if (row < 0 || row >= size || col < 0 || col >= size)
    throw "Index OutOfRange";

  if (pos && col < row) throw "Error 1";
  if (!pos && col > row) throw "Error 2";
  int index = pos ? // блин как же оно написано?????
              (row * size - row * (row - 1) / 2 + (col - row)) :
              (row * (row + 1) / 2 + col);

  return data[index];
}

template <class O>
inline ostream& operator<<(ostream& o, TTriangleMatrix<O>& p)
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

template <class I>
inline istream& operator>>(istream& is, TTriangleMatrix<I>& p)
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

template<class T>
inline bool TTriangleMatrix<T>::IsEmpty() const
{
  return size == 0 || data.GetLen() == 0;
}

template<class T>
inline bool TTriangleMatrix<T>::IsFull() const
{
  return false;
}

template<class T>
inline void TTriangleMatrix<T>::ApplyToAll(const function<void(T&)>& func)
{
  data.ApplyToAll(func);
}

template<class T>
inline void TTriangleMatrix<T>::ApplyToAll(const function<void(const T&)>& func) const
{
  data.ApplyToAll(func);
}

/* Убрал чтобы не захламлять. Оно работает, главное верить х2

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

template<class T>
T TTriangleMatrix<T>::FirstNorm() const
{
  T maxSum = 0;
  for (int j = 0; j < size; ++j)
  {
    T sum = 0;
    for (int i = 0; i < size; ++i)
    {
      if ((pos && i <= j) || (!pos && i >= j))
      {
        sum += abs((*this)(i, j));
      }
    }
    if (sum > maxSum) maxSum = sum;
  }
  return maxSum;
}

template<class T>
T TTriangleMatrix<T>::SecondNorm() const
{
  T sum = 0;
  for (int i = 0; i < size; ++i)
  {
    for (int j = 0; j < size; ++j)
    {
      if ((pos && i <= j) || (!pos && i >= j))
      {
        T val = (*this)(i, j);
        sum += val * val;
      }
    }
  }
  return sqrt(sum);
}

template<class T>
T TTriangleMatrix<T>::HolderNorm(T p) const
{
  T sum = 0;
  for (int i = 0; i < size; ++i)
  {
    for (int j = 0; j < size; ++j)
    {
      if ((pos && i <= j) || (!pos && i >= j))
      {
        sum += std::pow(std::abs((*this)(i, j)), p);
      }
    }
  }
  return pow(sum, T(1) / p);
}

template<class T>
T TTriangleMatrix<T>::InfinityNorm() const
{
  T maxSum = 0;
  for (int i = 0; i < size; ++i)
  {
    T sum = 0;
    for (int j = 0; j < size; ++j)
    {
      if ((pos && i <= j) || (!pos && i >= j))
        sum += abs((*this)(i, j));
    }
    if (sum > maxSum) maxSum = sum;
  }
  return maxSum;
}