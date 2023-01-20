#include <random>
#include <gtest/gtest.h>
#include <../../Game/Core/Colliders.h>

namespace {
    void CheckCollisionProperties(const Collider *collider1, const Collider *collider2) {
        bool first_with_second = collider1->CheckCollision(collider2);
        bool second_with_first = collider2->CheckCollision(collider1);
        EXPECT_EQ(first_with_second, second_with_first) << "CheckCollision is not symmetric";

        auto pos1 = collider1->GetIntersectionPosition(collider2);
        auto pos2 = collider2->GetIntersectionPosition(collider1);
        if (pos1 == std::nullopt || pos2 == std::nullopt) {
            EXPECT_EQ(pos1, std::nullopt) << "GetIntersectionPosition is not symmetric";
            EXPECT_EQ(pos2, std::nullopt) << "GetIntersectionPosition is not symmetric";
            EXPECT_FALSE(first_with_second) << "CheckCollision works wrong";
            EXPECT_FALSE(second_with_first) << "CheckCollision works wrong";
        } else {
            EXPECT_TRUE(collider1->IsInside(pos1.value()) && collider1->IsInside(pos2.value()) &&
                        collider2->IsInside(pos1.value()) && collider2->IsInside(pos2.value()))
                                << "Intersection position is not inside the collider";
            EXPECT_TRUE(first_with_second) << "CheckCollision works wrong";
            EXPECT_TRUE(second_with_first) << "CheckCollision works wrong";
        }
    }
}

class StressTest : public ::testing::Test {
public:
    /* Use of C-tor instead of SetUp() for getting rid of copying */
    StressTest() : dist_(-10, 10), rd_(), /* Initializer list is ordered */ e2_(rd_()) {}

    double RandDouble() {
        return dist_(e2_);
    }

    void SetUp() override {
        fails = 0;
    }

    void TearDown() override {
        if (fails) {
            std::cerr << "\nFixture::TearDown detected " << fails << " failed checks\n\n";
        }
    }

    size_t fails;
    static const size_t kIterations = 1e5;

private:
    std::uniform_real_distribution<> dist_;
    std::random_device rd_;
    std::mt19937 e2_;
};

TEST(Colliders, ConstructorsAndBasicMethods) {
    Position pos(1, 1);
    CircleCollider circle_collider(Position(2, 4), 42, true);
    RectangleCollider rectangle_collider(pos, 4.5, 5.555);

    EXPECT_TRUE(circle_collider.IsTrigger());
    EXPECT_FALSE(rectangle_collider.IsTrigger());

    ASSERT_NO_THROW(circle_collider.UpdatePosition(Position(2, 4)));
    ASSERT_NO_THROW(rectangle_collider.Translate(Vector2D(4.5, 3.2)));

    ASSERT_NO_THROW(circle_collider.CheckTrigger(&rectangle_collider));
    ASSERT_NO_THROW(rectangle_collider.CheckCollision(&circle_collider));
    ASSERT_NO_THROW(circle_collider.CheckCollision(&circle_collider));  // collision with itself

    ASSERT_NO_THROW(circle_collider.GetIntersectionPosition(&rectangle_collider));
    ASSERT_NO_THROW(rectangle_collider.GetIntersectionPosition(&rectangle_collider));  // intersection with itself
}


TEST(Intersections, CircleColliders) {
    CircleCollider cc1(Position(100, 100), 5);
    CircleCollider cc2(Position(5, 5), 2.5, true);
    ASSERT_TRUE(cc2.IsTrigger());
    CheckCollisionProperties(&cc1, &cc2);

    cc1.UpdatePosition(Position(11, 7));
    CheckCollisionProperties(&cc1, &cc2);

    cc1.UpdatePosition(Position(6, 7));
    CheckCollisionProperties(&cc1, &cc2);

    cc1.UpdatePosition(Position(12.5, 5));
    CheckCollisionProperties(&cc1, &cc2);  // Intersection by a single point (7.5, 5)
    ASSERT_TRUE(cc1.GetIntersectionPosition(&cc2) != std::nullopt);
    EXPECT_DOUBLE_EQ(7.5, cc1.GetIntersectionPosition(&cc2).value().GetCoordinates().first);
    EXPECT_DOUBLE_EQ(5, cc1.GetIntersectionPosition(&cc2).value().GetCoordinates().second);

    cc1.UpdatePosition(Position(7.5, 5));
    CheckCollisionProperties(&cc1, &cc2);
    ASSERT_TRUE(cc1.CheckTrigger(&cc2));

    cc1.UpdatePosition(Position(-2, 5));
    CheckCollisionProperties(&cc1, &cc2);
    ASSERT_TRUE(cc1.CheckTrigger(&cc2));
    ASSERT_FALSE(cc1.CheckCollision(&cc2));

    cc1.UpdatePosition(Position(-2.5555, 5));
    CheckCollisionProperties(&cc1, &cc2);
    ASSERT_FALSE(cc1.CheckTrigger(&cc2));
}

TEST(Intersections, RectangleColliders) {
    RectangleCollider rc1(Position(4, 4), 4, 4);
    RectangleCollider rc2(Position(3.5, 3.5), 1, 1);
    CheckCollisionProperties(&rc1, &rc2);

    rc1.UpdatePosition(Position(10, 10));
    CheckCollisionProperties(&rc1, &rc2);

    rc1.UpdatePosition(Position(2.5, 2.5));
    CheckCollisionProperties(&rc1, &rc2);

    rc1.UpdatePosition(Position(1.499999, 1.4999999));
    CheckCollisionProperties(&rc1, &rc2);
    ASSERT_FALSE(rc1.CheckCollision(&rc2));
    ASSERT_FALSE(rc1.CheckTrigger(&rc2));

    rc1.UpdatePosition(Position(1.5, 1.5));
    CheckCollisionProperties(&rc1, &rc2);  // Intersection by a single point (2, 2)
    ASSERT_TRUE(rc1.GetIntersectionPosition(&rc2) != std::nullopt);
    EXPECT_DOUBLE_EQ(2, rc1.GetIntersectionPosition(&rc2).value().GetCoordinates().first);
    EXPECT_DOUBLE_EQ(2, rc1.GetIntersectionPosition(&rc2).value().GetCoordinates().second);

    rc2.UpdatePosition(Position(4, 3));
    CheckCollisionProperties(&rc1, &rc2);  // Intersection by outer line x = 2
    ASSERT_TRUE(rc1.GetIntersectionPosition(&rc2) != std::nullopt);
    EXPECT_DOUBLE_EQ(2, rc1.GetIntersectionPosition(&rc2).value().GetCoordinates().first);

    rc1.UpdatePosition(Position(3.5, 5.5));
    CheckCollisionProperties(&rc1, &rc2);  // Intersection by inner line y = 5
    ASSERT_TRUE(rc1.GetIntersectionPosition(&rc2) != std::nullopt);
    EXPECT_DOUBLE_EQ(5, rc1.GetIntersectionPosition(&rc2).value().GetCoordinates().second);
}

TEST(Intersections, RectangleAndCircleColliders) {
    CircleCollider big_cc(Position(5, 2), 5);
    CircleCollider small_cc(Position(6, 5), 1.5, true);
    RectangleCollider rc(Position(5, 5), 8, 4);
    CheckCollisionProperties(&big_cc, &rc);
    CheckCollisionProperties(&small_cc, &rc);

    /* Big Circle Tests */
    big_cc.UpdatePosition(Position(3, 6));
    CheckCollisionProperties(&big_cc, &rc);
    ASSERT_TRUE(rc.CheckCollision(&big_cc));

    big_cc.UpdatePosition(Position(4, 6));
    CheckCollisionProperties(&big_cc, &rc);
    ASSERT_TRUE(rc.CheckCollision(&big_cc));

    big_cc.UpdatePosition(Position(10, 7));
    CheckCollisionProperties(&big_cc, &rc);
    ASSERT_TRUE(rc.CheckCollision(&big_cc));

    big_cc.UpdatePosition(Position(-2, -1));
    CheckCollisionProperties(&big_cc, &rc);  // Intersection by single point (3, 1)
    ASSERT_TRUE(big_cc.GetIntersectionPosition(&rc) != std::nullopt);
    EXPECT_DOUBLE_EQ(3, big_cc.GetIntersectionPosition(&rc).value().GetCoordinates().first);
    EXPECT_DOUBLE_EQ(1, big_cc.GetIntersectionPosition(&rc).value().GetCoordinates().second);

    big_cc.UpdatePosition(Position(-5, -6));
    CheckCollisionProperties(&big_cc, &rc);
    ASSERT_FALSE(rc.CheckCollision(&big_cc));

    big_cc.UpdatePosition(Position(5, 5));
    CheckCollisionProperties(&big_cc, &rc);  // Full cover
    ASSERT_TRUE(rc.CheckCollision(&big_cc));

    /* Small Circle Tests */
    small_cc.UpdatePosition(Position(9, 5));
    CheckCollisionProperties(&small_cc, &rc);
    ASSERT_TRUE(rc.CheckTrigger(&small_cc));

    small_cc.UpdatePosition(Position(7, 8));
    CheckCollisionProperties(&small_cc, &rc);
    ASSERT_TRUE(rc.CheckTrigger(&small_cc));

    small_cc.UpdatePosition(Position(3, 8.5));
    CheckCollisionProperties(&small_cc, &rc);  // Intersection by single point (3, 7)
    ASSERT_TRUE(small_cc.GetIntersectionPosition(&rc) != std::nullopt);
    EXPECT_DOUBLE_EQ(3, small_cc.GetIntersectionPosition(&rc).value().GetCoordinates().first);
    EXPECT_DOUBLE_EQ(7, small_cc.GetIntersectionPosition(&rc).value().GetCoordinates().second);

    small_cc.UpdatePosition(Position(2, 6));
    CheckCollisionProperties(&small_cc, &rc);
    ASSERT_TRUE(rc.CheckTrigger(&small_cc));

    small_cc.UpdatePosition(Position(7, 1));
    CheckCollisionProperties(&small_cc, &rc);
    ASSERT_FALSE(rc.CheckTrigger(&small_cc));

    small_cc.UpdatePosition(Position(10, 2));
    CheckCollisionProperties(&small_cc, &rc);
    ASSERT_TRUE(rc.CheckTrigger(&small_cc));

    small_cc.UpdatePosition(Position(4, 4.788));
    CheckCollisionProperties(&small_cc, &rc);  // Full cover
    ASSERT_TRUE(rc.CheckTrigger(&small_cc));

}

TEST_F(StressTest, RectangleColliders) {
    RectangleCollider rc1(Position(), 13.92842930132, 16.9381940277);
    RectangleCollider rc2(Position(), 17.4829582551, 12.52917520189);

    for (size_t i = 0; i < kIterations; ++i) {
        rc1.UpdatePosition(Position(RandDouble(), RandDouble()));
        rc2.UpdatePosition(Position(RandDouble(), RandDouble()));
        CheckCollisionProperties(&rc1, &rc2);
        fails += ::testing::Test::HasFailure();
    }
}

TEST_F(StressTest, CircleColliders) {
    CircleCollider cc1(Position(), 8.85105918433);
    CircleCollider cc2(Position(), 7.19391048208);

    for (size_t i = 0; i < kIterations; ++i) {
        cc1.UpdatePosition(Position(RandDouble(), RandDouble()));
        cc2.UpdatePosition(Position(RandDouble(), RandDouble()));
        CheckCollisionProperties(&cc1, &cc2);
        fails += ::testing::Test::HasFailure();
    }
}

TEST_F(StressTest, CircleAndRectangleColliders) {
    CircleCollider cc(Position(), 6.679813579);
    RectangleCollider rc(Position(), 11.4819471291, 12.4904819444);

    for (size_t i = 0; i < kIterations; ++i) {
        cc.UpdatePosition(Position(RandDouble(), RandDouble()));
        rc.UpdatePosition(Position(RandDouble(), RandDouble()));
        CheckCollisionProperties(&cc, &rc);
        fails += ::testing::Test::HasFailure();
    }
}
