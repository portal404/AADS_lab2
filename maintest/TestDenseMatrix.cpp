#include <gtest.h>
#include <fstream>
#include "MatrixClass.h"
#include <sstream>

TEST(TDenseNormalMatrix, DefaultConstructor)
{
  TDenseNormalMatrix<int> m;
  EXPECT_EQ(m.GetRows(), 0);
  EXPECT_EQ(m.GetColumns(), 0);
  EXPECT_TRUE(m.IsEmpty());
}

TEST(TDenseNormalMatrix, ParameterizedConstructor)
{
  TDenseNormalMatrix<int> m(3, 4);
  EXPECT_EQ(m.GetRows(), 3);
  EXPECT_EQ(m.GetColumns(), 4);
  EXPECT_FALSE(m.IsEmpty());
}

TEST(TDenseNormalMatrix, CopyConstructor)
{
  TDenseNormalMatrix<int> original(2, 2);
  original[0][0] = 1;
  original[0][1] = 2;
  original[1][0] = 3;
  original[1][1] = 4;

  TDenseNormalMatrix<int> copy(original);
  EXPECT_EQ(copy.GetRows(), 2);
  EXPECT_EQ(copy.GetColumns(), 2);
  EXPECT_EQ(copy[0][0], 1);
  EXPECT_EQ(copy[1][1], 4);
}

TEST(TDenseNormalMatrix, MoveConstructor)
{
  TDenseNormalMatrix<int> original(2, 2);
  original[0][0] = 1;
  original[0][1] = 2;
  original[1][0] = 3;
  original[1][1] = 4;

  TDenseNormalMatrix<int> moved(std::move(original));
  EXPECT_EQ(moved.GetRows(), 2);
  EXPECT_EQ(moved.GetColumns(), 2);
  EXPECT_EQ(moved[0][0], 1);
  EXPECT_EQ(moved[1][1], 4);
  EXPECT_TRUE(original.IsEmpty());
}

TEST(TDenseNormalMatrix, AdditionOperator)
{
  TDenseNormalMatrix<int> m1(2, 2);
  m1[0][0] = 1;
  m1[0][1] = 2;
  m1[1][0] = 3;
  m1[1][1] = 4;

  TDenseNormalMatrix<int> m2(2, 2);
  m2[0][0] = 5;
  m2[0][1] = 6;
  m2[1][0] = 7;
  m2[1][1] = 8;

  auto result = m1 + m2;
  EXPECT_EQ(result[0][0], 6);
  EXPECT_EQ(result[0][1], 8);
  EXPECT_EQ(result[1][0], 10);
  EXPECT_EQ(result[1][1], 12);
}

TEST(TDenseNormalMatrix, SubtractionOperator)
{
  TDenseNormalMatrix<int> m1(2, 2);
  m1[0][0] = 1;
  m1[0][1] = 2;
  m1[1][0] = 3;
  m1[1][1] = 4;

  TDenseNormalMatrix<int> m2(2, 2);
  m2[0][0] = 5;
  m2[0][1] = 6;
  m2[1][0] = 7;
  m2[1][1] = 8;

  auto result = m1 - m2;
  EXPECT_EQ(result[0][0], -4);
  EXPECT_EQ(result[0][1], -4);
  EXPECT_EQ(result[1][0], -4);
  EXPECT_EQ(result[1][1], -4);
}

TEST(TDenseNormalMatrix, MatrixMultiplication)
{
  TDenseNormalMatrix<int> m1(2, 2);
  m1[0][0] = 1;
  m1[0][1] = 2;
  m1[1][0] = 3;
  m1[1][1] = 4;

  TDenseNormalMatrix<int> m2(2, 2);
  m2[0][0] = 5;
  m2[0][1] = 6;
  m2[1][0] = 7;
  m2[1][1] = 8;

  auto result = m1 * m2;
  EXPECT_EQ(result[0][0], 19);
  EXPECT_EQ(result[0][1], 22);
  EXPECT_EQ(result[1][0], 43);
  EXPECT_EQ(result[1][1], 50);
}

TEST(TDenseNormalMatrix, ScalarMultiplication)
{
  TDenseNormalMatrix<int> m1(2, 2);
  m1[0][0] = 1;
  m1[0][1] = 2;
  m1[1][0] = 3;
  m1[1][1] = 4;

  auto result = m1 * 2;
  EXPECT_EQ(result[0][0], 2);
  EXPECT_EQ(result[0][1], 4);
  EXPECT_EQ(result[1][0], 6);
  EXPECT_EQ(result[1][1], 8);
}

TEST(TDenseNormalMatrix, AssignmentOperator)
{
  TDenseNormalMatrix<int> original(2, 2);
  original[0][0] = 1;
  original[0][1] = 2;
  original[1][0] = 3;
  original[1][1] = 4;

  TDenseNormalMatrix<int> copy(2, 2);
  copy = original;
  EXPECT_EQ(copy[0][0], 1);
  EXPECT_EQ(copy[1][1], 4);
}

TEST(TDenseNormalMatrix, MoveAssignmentOperator)
{
  TDenseNormalMatrix<int> original(2, 2);
  original[0][0] = 1;
  original[0][1] = 2;
  original[1][0] = 3;
  original[1][1] = 4;

  TDenseNormalMatrix<int> moved;
  moved = std::move(original);
  EXPECT_EQ(moved[0][0], 1);
  EXPECT_EQ(moved[1][1], 4);
  EXPECT_TRUE(original.IsEmpty());
}

TEST(TDenseNormalMatrix, EqualityOperator)
{
  TDenseNormalMatrix<int> m1(2, 2);
  m1[0][0] = 1;
  m1[0][1] = 2;
  m1[1][0] = 3;
  m1[1][1] = 4;

  TDenseNormalMatrix<int> m2(2, 2);
  m2[0][0] = 1;
  m2[0][1] = 2;
  m2[1][0] = 3;
  m2[1][1] = 4;

  TDenseNormalMatrix<int> m3(2, 2);
  m3[0][0] = 5;
  m3[0][1] = 6;
  m3[1][0] = 7;
  m3[1][1] = 8;

  EXPECT_TRUE(m1 == m2);
  EXPECT_FALSE(m1 == m3);
}

TEST(TDenseNormalMatrix, SubscriptOperator)
{
  TDenseNormalMatrix<int> m(2, 2);
  m[0][0] = 1;
  m[0][1] = 2;
  m[1][0] = 3;
  m[1][1] = 4;

  EXPECT_EQ(m[0][0], 1);
  EXPECT_EQ(m[1][1], 4);

  m[0][0] = 10;
  EXPECT_EQ(m[0][0], 10);
}

TEST(TDenseNormalMatrix, ConstSubscriptOperator)
{
  TDenseNormalMatrix<int> m(2, 2);
  m[0][0] = 1;
  m[0][1] = 2;
  m[1][0] = 3;
  m[1][1] = 4;

  const auto &const_ref = m;
  EXPECT_EQ(const_ref[0][0], 1);
  EXPECT_EQ(const_ref[1][1], 4);
}

TEST(TDenseNormalMatrix, GetMatrix)
{
  TDenseNormalMatrix<int> m(2, 2);
  m[0][0] = 1;
  m[0][1] = 2;
  m[1][0] = 3;
  m[1][1] = 4;

  auto &matrix_ref = m.GetMatrix();
  matrix_ref[0][0] = 10;
  EXPECT_EQ(m[0][0], 10);

  const auto &const_matrix_ref = m.GetMatrix();
  EXPECT_EQ(const_matrix_ref[1][1], 4);
}

TEST(TDenseNormalMatrix, IsEmpty)
{
  TDenseNormalMatrix<int> empty;
  EXPECT_TRUE(empty.IsEmpty());

  TDenseNormalMatrix<int> non_empty(2, 2);
  EXPECT_FALSE(non_empty.IsEmpty());
}

TEST(TDenseNormalMatrix, ValueCount)
{
  TDenseNormalMatrix<int> m(2, 2);
  m[0][0] = 1;
  m[0][1] = 2;
  m[1][0] = 1;
  m[1][1] = 3;

  EXPECT_EQ(m.ValueCount(1), 2);
  EXPECT_EQ(m.ValueCount(2), 1);
  EXPECT_EQ(m.ValueCount(5), 0);
}

TEST(TDenseNormalMatrix, AllOccurrences)
{
  TDenseNormalMatrix<int> m(2, 2);
  m[0][0] = 1;
  m[0][1] = 2;
  m[1][0] = 1;
  m[1][1] = 3;

  auto occurrences = m.AllOccurrences(1);
  EXPECT_FALSE(occurrences.IsEmpty());
}


TEST(TDenseNormalMatrix, StreamOutput)
{
  TDenseNormalMatrix<int> m(2, 2);
  m[0][0] = 1;
  m[0][1] = 2;
  m[1][0] = 3;
  m[1][1] = 4;

  std::stringstream ss;
  ss << m;

  EXPECT_FALSE(ss.str().empty());
}

TEST(TDenseNormalMatrix, EmptyMatrixOperations)
{
  TDenseNormalMatrix<int> empty1;
  TDenseNormalMatrix<int> empty2;

  auto result_add = empty1 + empty2;
  EXPECT_TRUE(result_add.IsEmpty());

  auto result_sub = empty1 - empty2;
  EXPECT_TRUE(result_sub.IsEmpty());

  auto result_mul = empty1 * empty2;
  EXPECT_TRUE(result_mul.IsEmpty());
}
