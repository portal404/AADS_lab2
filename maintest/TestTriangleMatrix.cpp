#include <gtest.h>
#include "TriangleMatrix.h"

TEST(TTriangleMatrix, DefaultConstructor)
{
  TTriangleMatrix<int> matrix;
  EXPECT_EQ(matrix.GetSize(), 0);
  EXPECT_TRUE(matrix.GetPos());
  EXPECT_TRUE(matrix.IsEmpty());
}

TEST(TTriangleMatrix, ParameterizedConstructor)
{
  TTriangleMatrix<int> matrix(4, false);
  EXPECT_EQ(matrix.GetSize(), 4);
  EXPECT_FALSE(matrix.GetPos());
  EXPECT_FALSE(matrix.IsEmpty());
}

TEST(TTriangleMatrix, ParameterizedConstructorWithValue)
{
  TTriangleMatrix<int> matrix(2, true, 5);
  EXPECT_EQ(matrix.GetSize(), 2);
  EXPECT_TRUE(matrix.GetPos());
  auto data = matrix.GetData();
  EXPECT_EQ(data.GetLen(), 3); // 2*(2+1)/2 = 3
}

TEST(TTriangleMatrix, CopyConstructor)
{
  TTriangleMatrix<int> original(3, true, 7);
  TTriangleMatrix<int> copy(original);

  EXPECT_EQ(copy.GetSize(), 3);
  EXPECT_TRUE(copy.GetPos());
  EXPECT_EQ(original.GetData(), copy.GetData());
}

TEST(TTriangleMatrix, MoveConstructor)
{
  TTriangleMatrix<int> original(3, false, 5);
  TTriangleMatrix<int> moved(std::move(original));

  EXPECT_EQ(moved.GetSize(), 3);
  EXPECT_FALSE(moved.GetPos());
  EXPECT_EQ(original.GetSize(), 0);
}

TEST(TTriangleMatrix, CopyAssignment)
{
  TTriangleMatrix<int> original(2, true, 10);
  TTriangleMatrix<int> copy;
  copy = original;

  EXPECT_EQ(copy.GetSize(), 2);
  EXPECT_TRUE(copy.GetPos());
  EXPECT_TRUE(original == copy);
}

TEST(TTriangleMatrix, MoveAssignment)
{
  TTriangleMatrix<int> original(2, false, 8);
  TTriangleMatrix<int> moved;
  moved = std::move(original);

  EXPECT_EQ(moved.GetSize(), 2);
  EXPECT_FALSE(moved.GetPos());
  EXPECT_EQ(original.GetSize(), 0);
}

TEST(TTriangleMatrix, EqualityOperator)
{
  TTriangleMatrix<int> matrix1(2, true, 5);
  TTriangleMatrix<int> matrix2(2, true, 5);
  TTriangleMatrix<int> matrix3(2, false, 5);
  TTriangleMatrix<int> matrix4(3, true, 5);

  EXPECT_TRUE(matrix1 == matrix2);
  EXPECT_FALSE(matrix1 == matrix3);
  EXPECT_FALSE(matrix1 == matrix4);
}

TEST(TTriangleMatrix, InequalityOperator)
{
  TTriangleMatrix<int> matrix1(2, true, 5);
  TTriangleMatrix<int> matrix2(2, false, 5);

  EXPECT_TRUE(matrix1 != matrix2);
  EXPECT_FALSE(matrix1 != matrix1);
}

TEST(TTriangleMatrix, AdditionOperator)
{
  TTriangleMatrix<int> matrix1(2, true, 3);
  TTriangleMatrix<int> matrix2(2, true, 4);
  TTriangleMatrix<int> result = matrix1 + matrix2;

  auto data = result.GetData();
  for (int i = 0; i < data.GetLen(); i++)
  {
    EXPECT_EQ(data[i], 7);
  }
}

TEST(TTriangleMatrix, SubtractionOperator)
{
  TTriangleMatrix<int> matrix1(2, true, 10);
  TTriangleMatrix<int> matrix2(2, true, 3);
  TTriangleMatrix<int> result = matrix1 - matrix2;

  auto data = result.GetData();
  for (int i = 0; i < data.GetLen(); i++)
  {
    EXPECT_EQ(data[i], 7);
  }
}

TEST(TTriangleMatrix, ScalarMultiplication)
{
  TTriangleMatrix<int> matrix(2, true, 5);
  TTriangleMatrix<int> result = matrix * 3;

  auto data = result.GetData();
  for (int i = 0; i < data.GetLen(); i++)
  {
    EXPECT_EQ(data[i], 15);
  }
}

TEST(TTriangleMatrix, InvalidSizeThrows)
{
  EXPECT_THROW(TTriangleMatrix<int>(-1, true), const char*);
}

TEST(TTriangleMatrix, IncompatibleAdditionThrows)
{
  TTriangleMatrix<int> upper(2, true, 1);
  TTriangleMatrix<int> lower(2, false, 1);
  TTriangleMatrix<int> differentSize(3, true, 1);

  EXPECT_THROW(upper + lower, const char*);
  EXPECT_THROW(upper + differentSize, const char*);
}

TEST(TTriangleMatrix, IncompatibleSubtractionThrows)
{
  TTriangleMatrix<int> upper(2, true, 1);
  TTriangleMatrix<int> lower(2, false, 1);

  EXPECT_THROW(upper - lower, const char*);
}

TEST(TTriangleMatrix, IsEmpty)
{
  TTriangleMatrix<int> empty;
  TTriangleMatrix<int> nonEmpty(2, true);

  EXPECT_TRUE(empty.IsEmpty());
  EXPECT_FALSE(nonEmpty.IsEmpty());
}

TEST(TTriangleMatrix, ApplyToAll)
{
  TTriangleMatrix<int> matrix(2, true, 2);

  matrix.ApplyToAll([](int &x)
                    { x *= 2; });

  auto data = matrix.GetData();
  for (int i = 0; i < data.GetLen(); i++)
  {
    EXPECT_EQ(data[i], 4);
  }
}
