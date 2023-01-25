#include <vector>

#include <../../Game/Core/EventHandler.h>
#include <gtest/gtest.h>

EventHandler* eh;

namespace {

    /* Pointer & Reference - Check predicates*/

    bool FooPointer(int* a, int* b) {
        return *a == *b;
    }

    bool FooReference(int& a, int& b) {
        return a == b;
    }

    void SetBoolTrue(bool& b, int& cnt) {
        ++cnt;
        b = true;
    }

    /* Simple functions and predicates */

    void Bar() {
    }

    bool AlwaysTrue() {
        return true;
    }

    bool ReturnBool(bool b) {
        return b;
    }

    bool ReturnBoolRef(bool& b) {
        return b;
    }

    int CastCheck() {
        return 42;
    }

    /* StressForce helper functions */

    bool GoodIndex(const std::vector<bool>& v, size_t index) {
        if (v[index]) {
            return true;
        } else {
            return false;
        }
    }

    void UpdateIndex(std::vector<bool>& v, size_t index) {
        v[index] = true;
    }

    /* StrangeStress helper function */

    bool FakeIf(std::vector<int>* v, size_t k) {
        if (k == v->size()) {
            return true;
        } else {
            return false;
        }
    }

    /* Mock classes for testing */

    struct SomeStruct {
        void SomeMethod(int) {
        }

        void SomeConstMethod(int) const {
        }

        static void SomeStaticMethod() {
        }
    };

    class Functional {
    public:
        int GetStatus() {
            return func_status_;
        }

        void ChangeStatus(int new_status) {
            func_status_ = new_status;
        }
    private:
        int func_status_ = 0;
    };

    /* BasicCorrectness helper functions */

    bool status = false;

    void ChangeStatus() {
        if (status) {
            status = false;
        } else {
            status = true;
        }
    }

    bool CheckStatus() {
        return status;
    }
}

class EventHandlerTest : public ::testing::Test {
public:
    static void SetUpTestSuite() {
        eh = new EventHandler(EventHandler::GetInstance());
    }

    void TearDown() override {  /* Clear queue */
        eh->Clear();
    }

    static void TearDownTestSuite() {
        delete eh;
    }

protected:
    size_t kIterations = 1e5;
};

TEST_F(EventHandlerTest, BasicNoThrow) {
    eh->Clear();
    eh->TryExecuteAll();
    eh->Clear();

    ASSERT_NO_THROW(eh->CreateDisposableEvent(&AlwaysTrue, std::tuple(), &Bar, {}));
    ASSERT_NO_THROW(eh->CreateDisposableEvent(&CastCheck, {}, &Bar, std::tuple()));
    ASSERT_NO_THROW(eh->CreateReusableEvent(&ReturnBool, std::make_tuple(true), &Bar, {}));

    EXPECT_TRUE(!eh->Empty()) << "Fake Emplace";
    eh->Clear();
    EXPECT_TRUE(eh->Empty());

    SomeStruct s;
    int a = 5, b = 6, c = 7;
    ASSERT_NO_THROW(eh->CreateDisposableEvent(&FooPointer, std::make_tuple(&a, &b),  &s,&SomeStruct::SomeMethod, std::tuple(c)));
    ASSERT_NO_THROW(eh->CreateReusableEvent(&FooReference, std::make_tuple(std::ref(a), std::ref(b)), &s, &SomeStruct::SomeConstMethod, std::tuple(7)));
    ASSERT_NO_THROW(eh->CreateDisposableEvent(&AlwaysTrue, {}, &SomeStruct::SomeStaticMethod, {}));
    ASSERT_NO_THROW(eh->TryExecuteAll());

    /* TearDown auto clear */
}

TEST_F(EventHandlerTest, BasicCorrectness) {
    Event* d_ptr = eh->CreateDisposableEvent(&CheckStatus, {}, &ChangeStatus, {});
    EXPECT_FALSE(eh->Empty());
    eh->TryExecuteAll();
    EXPECT_FALSE(eh->Empty());
    d_ptr->ForceExecute();
    EXPECT_TRUE(status);
    eh->TryExecuteAll();
    EXPECT_TRUE(eh->Empty());
    EXPECT_TRUE(status) << "TryExecute should not execute Disposable and ForceExecuted Events";

    Event* r_ptr = eh->CreateReusableEvent(&CheckStatus, {}, &ChangeStatus, {});
    EXPECT_FALSE(eh->Empty());
    eh->TryExecuteAll();
    EXPECT_FALSE(eh->Empty());
    r_ptr->ForceExecute();
    EXPECT_TRUE(status);
    eh->TryExecuteAll();
    EXPECT_FALSE(eh->Empty()) << "Reusable Event is never deleted";
    EXPECT_FALSE(status) << "TryExecute must execute Disposable and ForceExecuted Events";

    /* TearDown auto clear */
}

TEST_F(EventHandlerTest, PointerAndReferencePredicate) {
    Functional f;

    int a = 5, b = 6, c = 7;
    eh->CreateDisposableEvent(&FooPointer, std::tuple(&a, &b), &f, &Functional::ChangeStatus, std::tuple(c));
    eh->TryExecuteAll();
    EXPECT_NE(f.GetStatus(), c);
    EXPECT_FALSE(eh->Empty());
    b = 5;
    eh->TryExecuteAll();
    EXPECT_EQ(f.GetStatus(), c);
    EXPECT_TRUE(eh->Empty());

    f.ChangeStatus(0);
    eh->CreateDisposableEvent(&FooReference, std::tuple(std::ref(a), std::ref(c)), &f, &Functional::ChangeStatus, std::tuple(b));
    eh->TryExecuteAll();
    EXPECT_EQ(f.GetStatus(), 0);
    EXPECT_FALSE(eh->Empty());
    c = 5;
    eh->TryExecuteAll();
    EXPECT_EQ(f.GetStatus(), b);
    EXPECT_TRUE(eh->Empty());

    bool boolean = false;
    int cnt = 0;
    auto ptr = eh->CreateReusableEvent(&ReturnBoolRef, std::make_tuple(std::ref(boolean)), &SetBoolTrue, std::make_tuple(std::ref(boolean), std::ref(cnt)));
    eh->TryExecuteAll();
    EXPECT_FALSE(eh->Empty());
    ptr->ForceExecute();
    EXPECT_TRUE(boolean);
    eh->TryExecuteAll();
    auto ptr2 = eh->CreateReusableEvent(&CastCheck, {}, &SetBoolTrue, std::make_tuple(std::ref(boolean), std::ref(cnt)));
    eh->TryExecuteAll();
    eh->TryExecuteAll();
    eh->CreateDisposableEvent(&AlwaysTrue, {}, &SomeStruct::SomeStaticMethod, {});
    eh->TryExecuteAll();
    ptr2->ForceExecute();
    EXPECT_EQ(cnt, 9);

    /* TearDown auto clear*/
}

TEST_F(EventHandlerTest, StressForce) {
    std::vector<bool> checkers(kIterations, false);
    std::vector<Event*> copies;
    copies.reserve(kIterations);

    for (size_t i = 0; i < kIterations - 1; ++i) {
        auto ptr = eh->CreateDisposableEvent(&GoodIndex, std::make_tuple(std::cref(checkers), i), &UpdateIndex, std::make_tuple(std::ref(checkers), i + 1));
        copies.push_back(ptr);
    }
    checkers[0] = true;
    for (size_t i = 0; i < kIterations - 1; ++i) {
        copies[i]->ForceExecute();
    }
    for (const auto& ch: checkers) {
        ASSERT_TRUE(ch);
    }
}

TEST_F(EventHandlerTest, StrangeStress) {
    std::vector<int> v(1001, 42);
    bool b = false;
    int cnt = 0;

    for (size_t i = 1; i <= 1001; ++i) {
        eh->CreateDisposableEvent(&FakeIf, std::make_tuple(&v, i), &SetBoolTrue, std::make_tuple(std::ref(b), std::ref(cnt)));
    }
    size_t iterations = 0;
    while (!eh->Empty()) {
        eh->TryExecuteAll();
        ++iterations;
        if (b) {
            v.pop_back();
            b = false;
        } else {
            EXPECT_TRUE(false) << "TryExecuteAll works bad";
        }
    }
    ASSERT_EQ(iterations, 1001);
    ASSERT_EQ(cnt, 1001);
}

