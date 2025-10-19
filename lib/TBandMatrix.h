#pragma once
#include "VectorClass.h"
#include <math.h>
#include <fstream>

template <class T>
class TBandMatrix
{
protected:
  TVector<T> data;
  int size;
  int tape_width;

public:
  TBandMatrix();
  TBandMatrix(int size_, int tape_width_);
  TBandMatrix(int size_, int tape_width_, const T& value);
  TBandMatrix(const TBandMatrix& obj);
  TBandMatrix(TBandMatrix&& obj);
  ~TBandMatrix();

  class TIterator
  {
  protected:
    TBandMatrix<T>& p;
    int row;
    int col;
  public:
    TIterator(TBandMatrix<T>& m, int row_, int col_);
    T& operator*();
    TIterator& operator++();
    TIterator& operator++(int);
    bool operator==(const TIterator& f) const;
    bool operator!= (const TIterator& f);
  };

  TIterator begin();
  TIterator end();
  int GetSize() const;
  int GetTapeWidth() const;
  const TVector<T>& GetTape() const;

  TBandMatrix operator+(const TBandMatrix<T>& obj);
  TBandMatrix operator-(const TBandMatrix<T>& obj);
  TBandMatrix operator*(const TBandMatrix<T>& obj);
  TBandMatrix operator*(const T& scalar);
  TBandMatrix& operator=(const TBandMatrix<T>& obj);
  TBandMatrix& operator=(TBandMatrix<T>&& obj);
  bool operator==(const TBandMatrix<T>& obj);
  bool operator!=(const TBandMatrix<T>& obj);

  T& operator()(int row, int col);
  const T& operator()(int row, int col) const;

  template <class I>
  friend std::ostream& operator<<(std::ostream& os, TBandMatrix<T>& mat);
  template <class U>
  friend std::istream& operator>>(std::istream& is, TBandMatrix<T>& mat);

  bool IsEmpty() const;
  bool IsFull() const;
  void ApplyToAll(const function<void(T&)>& func);
  void ApplyToAll(const function<void(const T&)>& func) const;


  int AllOccurrences(const T& value) const;
  T FirstNorm() const;
  T SecondNorm() const;
  T HelderNorm(T p) const;
  T InfinityNorm() const;

  int GetTapeIndex(int i, int j) const;
  bool IsInBand(int i, int j) const;
};
