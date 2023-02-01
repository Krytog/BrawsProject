#include <random>
#include <vector>
#include <../../Game/Core/DelayQueue.h>
#include <gtest/gtest.h>

DelayQueue* dq;

using namespace std::chrono_literals;

namespace /* Mock Functions */ {
void DoNothing() {
}

void DoNothingWithParams(int a, int& b, const int& c) {
    b = a + 28 + c * 2;
}

void ChangePointer(int* ptr) {
    *ptr = 42;
}

void FillVector(std::vector<uint64_t>& vector, uint64_t value) {
    vector.push_back(value);
}

void CheckSort(const std::vector<uint64_t>& vector) {
    if (vector.size() <= 1)
        return;
    for (uint64_t i = 1; i < vector.size(); ++i) {
        EXPECT_LE(vector[i - 1], vector[i]) << "Vector is not sorted";
    }
}

class FuncOwner {
    const int kUninitializedStatus = -1;

public:
    void ChangeStatus(int new_status) {
        status = new_status;
    }

    void CheckValidStatus() const {
        EXPECT_TRUE(status != kUninitializedStatus);
    }

    void JustConstMethod() const {
    }

public:
    int status = kUninitializedStatus;
};

struct SomeStruct {
    void Foo() {
    }

    void ConstFoo(int* a) const {
        *a = 0;
    }

    static void SomeStaticMethod() {
    }
};
}  // namespace

class DelayQueueTest : public ::testing::Test {
public:
    static void SetUpTestSuite() {
        dq = new DelayQueue(DelayQueue::GetInstance());
    }

    void TearDown() override { /* Clear queue */
        dq->TryExecute(std::chrono::steady_clock::time_point::max(), UINT64_MAX);
    }

    static void TearDownTestSuite() {
        delete dq;
    }

protected:
    static DelayQueue::TimePoint Now() {
        return std::chrono::steady_clock::now();
    }

protected:
    size_t kIterations = 1e5;
};

class DelayQueueStressTest : public DelayQueueTest {
    class Randomizer {
    public:
        Randomizer()
            : dist_(std::numeric_limits<uint64_t>::min(), std::numeric_limits<uint64_t>::max()),
              rd_(),
              /* Initializer list is ordered */ e2_(rd_()) {
        }

        uint64_t RandUInt() {
            return dist_(e2_);
        }

    private:
        std::uniform_int_distribution<uint64_t> dist_;
        std::random_device rd_;
        std::mt19937 e2_;
    };

public:
    uint64_t RandUInt() {
        return randomizer.RandUInt();
    }

    void TearDown() override {
        test_time_vector.clear();
        test_ticks_vector.clear();
        big_buf.clear();
    }

    void CheckSuccess(uint64_t push_iterations, bool stressTime, bool stressTicks) {
        DelayQueue::TimePoint time_deadline = std::chrono::steady_clock::time_point::min();
        uint64_t ticks_deadline = 0;
        if (stressTime) {
            time_deadline = std::chrono::time_point_cast<std::chrono::nanoseconds>(
                time_deadline + operator""ns(UINT64_MAX));
        }
        if (stressTicks) {
            ticks_deadline = UINT64_MAX;
        }

        ASSERT_TRUE(test_time_vector.empty());
        ASSERT_TRUE(test_ticks_vector.empty());
        ASSERT_TRUE(big_buf.empty());

        dq->TryExecute(time_deadline, ticks_deadline);

        ASSERT_EQ(test_time_vector.size() + test_ticks_vector.size(), push_iterations);
        ASSERT_TRUE(dq->Empty());

        CheckSort(test_time_vector);
        CheckSort(test_ticks_vector);

        if (stressTime && stressTicks) {
            ASSERT_EQ(big_buf.size(), push_iterations);
        }
    }

protected:
    std::vector<uint64_t> test_ticks_vector;
    std::vector<uint64_t> test_time_vector;
    std::vector<uint64_t> big_buf;
    Randomizer randomizer;
};

TEST_F(DelayQueueTest, BasicMethods) {
    auto now = Now();

    dq->TryExecute(now, 0);
    dq->TryExecute(now + 10ms, 42);

    dq->PushTime(now + 2ms, &DoNothing);
    dq->PushTicks(42, &DoNothing);
    dq->TryExecute(now + 1ms, 1000);
    EXPECT_FALSE(dq->Empty());
    dq->TryExecute(now + 3ms, 0);
    EXPECT_TRUE(dq->Empty());

    /* Check that arguments are copied */

    struct Bar {
        void Foo(int b) {
            ASSERT_EQ(b, 1);
        }

        void Baz(int* a) {
            ASSERT_EQ(*a, 42);
        }
    };
    Bar bar;

    int a = 1;
    const int& b = a;
    dq->PushTime(now + 20ms, &Bar::Foo, &bar, b);
    dq->PushTicks(700, &Bar::Baz, &bar, &a);
    a = 42;
    dq->TryExecute(now + 100ms, 1000);
    EXPECT_TRUE(dq->Empty());
}

TEST_F(DelayQueueTest, PushTime) {
    auto now = Now();
    FuncOwner owner;
    const FuncOwner c_owner;

    int a = 0, b = 42, c = 100;
    dq->PushTime(now + 1000000ms, &DoNothing);
    dq->PushTime(now + 1ms, DoNothingWithParams, a, std::ref(b), c);
    dq->TryExecute(now + 2ms, 88);
    EXPECT_FALSE(dq->Empty());
    ASSERT_EQ(b, 228);
    dq->TryExecute(now + 100000000ms, 0);
    ASSERT_TRUE(dq->Empty());

    ASSERT_NO_THROW(dq->PushTime(now + 40s, &FuncOwner::ChangeStatus, &owner, 99));
    ASSERT_NO_THROW(dq->PushTime(now + 50s, &FuncOwner::CheckValidStatus, &owner));
    ASSERT_NO_THROW(dq->PushTime(now + 60s, &FuncOwner::JustConstMethod, &c_owner));
    ASSERT_NO_THROW(dq->TryExecute(now + 1h, 41));

    dq->PushTime(now + 1ms, &FuncOwner::ChangeStatus, &owner, 1);
    dq->PushTime(now + 100ms, &FuncOwner::ChangeStatus, &owner, 2);
    dq->PushTime(now + 31s, &FuncOwner::ChangeStatus, &owner, 3);
    dq->PushTime(now + 1h, &FuncOwner::ChangeStatus, &owner, 4);
    dq->PushTime(now + 1ns, &FuncOwner::ChangeStatus, &owner, 5);
    dq->PushTime(now + 7min, &FuncOwner::ChangeStatus, &owner, 6);

    dq->TryExecute(now + 7ns, 100);
    EXPECT_EQ(owner.status, 5);
    dq->TryExecute(now + 9s, 200);
    EXPECT_EQ(owner.status, 2);
    dq->TryExecute(now + 48min, 1);
    EXPECT_EQ(owner.status, 6);
    dq->TryExecute(now + 19s, 8888888888);
    EXPECT_EQ(owner.status, 6);

    /* TearDown auto clear */
}

TEST_F(DelayQueueTest, PushTicks) {
    auto now = Now();
    FuncOwner owner;
    const FuncOwner c_owner;
    const FuncOwner* cp_owner = new FuncOwner();
    int a = 0, b = 42, c = 100;

    dq->PushTicks(20, &DoNothing);
    dq->PushTicks(3, DoNothingWithParams, a, std::ref(b), c);
    dq->TryExecute(now, 4);
    EXPECT_FALSE(dq->Empty());
    ASSERT_EQ(b, 228);
    for (uint64_t i = 10; i < 20; ++i) {
        dq->TryExecute(now, i);
        EXPECT_FALSE(dq->Empty());
    }
    dq->TryExecute(now, 20);
    EXPECT_TRUE(dq->Empty());

    ASSERT_NO_THROW(dq->PushTicks(40, &FuncOwner::ChangeStatus, &owner, 99));
    ASSERT_NO_THROW(dq->PushTicks(50, &FuncOwner::CheckValidStatus, &owner));
    ASSERT_NO_THROW(dq->PushTicks(60, &FuncOwner::JustConstMethod, &c_owner));
    ASSERT_NO_THROW(dq->PushTicks(70, &FuncOwner::JustConstMethod, cp_owner));
    ASSERT_NO_THROW(dq->TryExecute(now, 100));
    delete cp_owner;

    dq->PushTicks(4, &FuncOwner::ChangeStatus, &owner, 100);
    dq->PushTicks(50, &FuncOwner::ChangeStatus, &owner, 200);
    dq->PushTicks(27, &FuncOwner::ChangeStatus, &owner, 300);
    dq->PushTicks(13, &FuncOwner::ChangeStatus, &owner, 400);
    dq->PushTicks(38, &FuncOwner::ChangeStatus, &owner, 500);
    dq->PushTicks(9, &FuncOwner::ChangeStatus, &owner, 600);

    dq->TryExecute(now, 11);
    EXPECT_EQ(owner.status, 600);
    dq->TryExecute(now, 26);
    EXPECT_EQ(owner.status, 400);
    dq->TryExecute(now, 35);
    EXPECT_EQ(owner.status, 300);
    dq->TryExecute(now, 47);
    EXPECT_EQ(owner.status, 500);

    /* TearDown auto clear */
}

TEST_F(DelayQueueTest, SomeCombinations) {
    auto now = Now();
    FuncOwner owner;
    SomeStruct s;
    int a = 100;

    dq->PushTime(now + 3ms, &ChangePointer, &a);
    dq->PushTicks(4, DoNothingWithParams, a, std::ref(a), a);
    dq->PushTime(now + 8ms, &FuncOwner::JustConstMethod, &owner);
    dq->PushTicks(3, &SomeStruct::Foo, &s);
    dq->PushTicks(8, &SomeStruct::ConstFoo, &s, &a);

    dq->TryExecute(now + 4ms, 5);
    EXPECT_TRUE(a == 42 || a == 328);
    dq->TryExecute(now, 6);
    EXPECT_TRUE(a == 42 || a == 328);
    dq->TryExecute(now + 8ms, 8);
    EXPECT_TRUE(dq->Empty());
    EXPECT_EQ(a, 0);

    ASSERT_NO_THROW(dq->PushTime(now + 1h, &SomeStruct::SomeStaticMethod));  // static method

    /* TearDown auto clear */
}

TEST_F(DelayQueueStressTest, StressTime) {
    auto start_point = std::chrono::steady_clock::time_point::min(); /*No need to sleep on this thread for
                                                                        hours and prevent overflow*/

    for (size_t i = 0; i < kIterations; ++i) {
        uint64_t val = RandUInt();
        DelayQueue::TimePoint tp =
            std::chrono::time_point_cast<std::chrono::nanoseconds>(start_point + operator""ns(val));
        dq->PushTime(tp, &FillVector, std::ref(test_time_vector), val);
    }
    CheckSuccess(kIterations, true, false);
}

TEST_F(DelayQueueStressTest, StressTicks) {
    for (size_t i = 0; i < kIterations; ++i) {
        uint64_t val = randomizer.RandUInt();
        dq->PushTicks(val, &FillVector, std::ref(test_ticks_vector), val);
    }
    CheckSuccess(kIterations, false, true);
}

TEST_F(DelayQueueStressTest, StressAll) {
    auto start_point =
        std::chrono::steady_clock::time_point::min(); /*No need to sleep on this thread for hours*/

    for (size_t i = 0; i < kIterations; ++i) {
        uint64_t val = RandUInt();
        uint64_t big_buf_val = RandUInt();
        if (i % 2) {
            dq->PushTicks(val, &FillVector, std::ref(test_ticks_vector), val);
            dq->PushTicks(big_buf_val, &FillVector, std::ref(big_buf), big_buf_val);
        } else {
            DelayQueue::TimePoint tp =
                std::chrono::time_point_cast<std::chrono::nanoseconds>(start_point + operator""ns(val));
            DelayQueue::TimePoint big_buf_tp = std::chrono::time_point_cast<std::chrono::nanoseconds>(
                start_point + operator""ns(big_buf_val));
            dq->PushTime(tp, &FillVector, std::ref(test_time_vector), val);
            dq->PushTime(big_buf_tp, &FillVector, std::ref(big_buf), big_buf_val);
        }
    }
    CheckSuccess(kIterations, true, true);
}