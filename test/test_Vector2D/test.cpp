#include <gtest/gtest.h>
#include <../../Game/Core/Vector2D.h>

#define SOME_DOUBLE_CONSTANT 472843892.28281832759195820

TEST(Vector2D, BasicMethods) {
    Vector2D vector_int{42, INT32_MAX};
    EXPECT_DOUBLE_EQ(42, vector_int.GetCoordinates().first);
    EXPECT_DOUBLE_EQ(INT32_MAX, vector_int.GetCoordinates().second);

    Vector2D vector_double(13.37, SOME_DOUBLE_CONSTANT);
    EXPECT_DOUBLE_EQ(13.37, vector_double.GetCoordinates().first);
    EXPECT_DOUBLE_EQ(SOME_DOUBLE_CONSTANT, vector_double.GetCoordinates().second);

    Vector2D v0(0, 0);

    EXPECT_TRUE(v0 == Vector2D::Up + Vector2D::Down);
    EXPECT_TRUE(v0 == Vector2D::Left + Vector2D::Right);

    Vector2D v1(22.22, 33.00001);
    Vector2D v2(44.44, 66.00002);
    EXPECT_DOUBLE_EQ(22.22 * 44.44 + 33.00001 * 66.00002, 
                     Vector2D::ScalarMultiply(v1, v2));

    EXPECT_DOUBLE_EQ(22.22 * 44.44 + 33.00001 * 66.00002,
                     v1 * v2);

    v1 *= 2;
    EXPECT_TRUE(v1 == v2);

    v2.Normalize();
    EXPECT_DOUBLE_EQ(1, Vector2D::ScalarMultiply(v2, v2));

    v1.SetCoordinates(99, 10);
    EXPECT_DOUBLE_EQ(99, v1.GetCoordinates().first);
    EXPECT_DOUBLE_EQ(10, v1.GetCoordinates().second);
}