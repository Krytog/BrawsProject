#include <random>
#include <gtest/gtest.h>

#include <../../Game/Core/BasicSequncer.h>

class BaseClass {
public:
    BaseClass() = default;

    virtual ~BaseClass() = default;
};

BasicSequencer<BaseClass>* sequencer;

using VectorArgs = std::vector<std::pair<std::string_view, BaseClass*>>;

#define MAGIC_NUMBER 42
#define ERROR std::runtime_error /* For ASSERT test-pack */

namespace /* Mock Functions and Objects */ {
    class DerivedClass : public BaseClass {
    public:
        DerivedClass() = default;
    };

    class DerivedCustomDestructor : public BaseClass {
    public:
        DerivedCustomDestructor(int* callback): callback_(callback) {};

        ~DerivedCustomDestructor() {
            *callback_ = MAGIC_NUMBER;
        }
    private:
        int* callback_;
    };
}

#define ANIMATED "animated"
#define STATIC "static"
#define ANY "any"

class BasicSequencerTest : public ::testing::Test {
public:
    static void SetUpTestSuite() {
        sequencer = new BasicSequencer<BaseClass>();
    }

    void TearDown() override {
        sequencer->Clear();
    }

    static void TearDownTestSuite() {
        delete sequencer;
    }

    static void CreateDefaultSeq(const std::vector<std::string_view> tags) {
        for (const auto& tag: tags) {
            sequencer->AddByTag(tag, new BaseClass());
        }
    }

protected:
    BaseClass* visible_object_ = new BaseClass();
    DerivedClass* derived_obj_ = new DerivedClass();
};

TEST_F(BasicSequencerTest, BasicMethods) {
    VectorArgs empty_vec;
    ASSERT_NO_THROW(BasicSequencer<BaseClass>());
    ASSERT_NO_THROW(BasicSequencer<BaseClass>(empty_vec));

    VectorArgs one_object_vec = {{STATIC, new BaseClass()}};
    ASSERT_NO_THROW(BasicSequencer<BaseClass>(one_object_vec, true));

    VectorArgs decoy_vec_cycled = {{ANIMATED, new BaseClass()}, {STATIC, new DerivedClass()}};
    ASSERT_NO_THROW(BasicSequencer<BaseClass>(decoy_vec_cycled, true)); /* destructor of sequencer call destructor of unique_ptr that free memory */

    VectorArgs decoy_vec_not_cycled = {{ANIMATED, new BaseClass()}, {STATIC, new DerivedClass()}};
    ASSERT_NO_THROW(BasicSequencer<BaseClass>(decoy_vec_not_cycled, false));

    { /* Check destructor was called */
        int mutable_number = 0;
        BaseClass* derived_ptr = new DerivedCustomDestructor(&mutable_number);
        VectorArgs decoy_vec = {{STATIC, derived_ptr}};
        ASSERT_NO_THROW(BasicSequencer<BaseClass>(decoy_vec, false));
        EXPECT_EQ(mutable_number, MAGIC_NUMBER);
    }

    { /* Checks Clear calls destructors */
        int mutable_number = 0;
        ASSERT_NO_THROW(sequencer->AddByTag(STATIC, new DerivedCustomDestructor(&mutable_number)));
        ASSERT_NO_THROW(sequencer->Clear());
        EXPECT_EQ(mutable_number, MAGIC_NUMBER);
    }

    /* Clearing sequencer */
}

TEST_F(BasicSequencerTest, AddMethods) {
    ASSERT_NO_THROW(sequencer->AddByTag(STATIC, new BaseClass()));
    ASSERT_THROW(sequencer->AddByTag(STATIC, new BaseClass()), ERROR);
    ASSERT_NO_THROW(sequencer->RemoveByTag(STATIC));
    ASSERT_THROW(sequencer->GetObjectByTag(STATIC), std::runtime_error); /* sequencer is empty now */

    ASSERT_NO_THROW(CreateDefaultSeq({STATIC, ANIMATED}));
    ASSERT_NO_THROW(sequencer->AddEdgeFromTo(STATIC, ANIMATED));
    ASSERT_THROW(sequencer->AddEdgeFromTo(STATIC, ANIMATED), std::runtime_error);
    ASSERT_NO_THROW(sequencer->Clear());

    ASSERT_NO_THROW(CreateDefaultSeq({STATIC, ANIMATED}));
    ASSERT_THROW(sequencer->AddAfterByTag(STATIC, ANIMATED, new BaseClass()), ERROR);
    ASSERT_NO_THROW(sequencer->RemoveByTag(ANIMATED));
    ASSERT_NO_THROW(sequencer->AddAfterByTag(STATIC, ANIMATED, new BaseClass()));
    ASSERT_THROW(sequencer->AddEdgeFromTo(STATIC, ANIMATED), ERROR);
    ASSERT_NO_THROW(sequencer->AddEdgeFromTo(ANIMATED, STATIC));
}

TEST_F(BasicSequencerTest, RemoveMethods) {
    ASSERT_THROW(sequencer->RemoveByTag(STATIC), ERROR);
    ASSERT_NO_THROW(CreateDefaultSeq({STATIC, ANIMATED, ANY}));
    ASSERT_NO_THROW(sequencer->RemoveByTag(ANY));
    ASSERT_THROW(sequencer->AddEdgeFromTo(STATIC, ANY), ERROR);
    ASSERT_THROW(sequencer->RemoveEdgeFromTo(STATIC, ANIMATED), ERROR);
    ASSERT_NO_THROW(sequencer->AddEdgeFromTo(STATIC, ANIMATED));
    ASSERT_NO_THROW(sequencer->RemoveEdgeFromTo(STATIC, ANIMATED));
    ASSERT_NO_THROW(sequencer->Clear());

    { /* Removing from sequencer calls destructor */
        int mutable_number = 0;
        BaseClass* derived_ptr = new DerivedCustomDestructor(&mutable_number);
        sequencer->AddByTag(STATIC, derived_ptr);
        sequencer->RemoveByTag(STATIC);
        EXPECT_EQ(mutable_number, MAGIC_NUMBER);
    }
}

TEST_F(BasicSequencerTest, ChangeStartEndPoints) {
    ASSERT_THROW(sequencer->SetStartTag(STATIC), ERROR);
    ASSERT_NO_THROW(sequencer->AddByTag(STATIC, new BaseClass()));
    ASSERT_THROW(sequencer->SetEndTag(ANIMATED), ERROR);
    ASSERT_NO_THROW(sequencer->SetEndTag(STATIC));
    ASSERT_NO_THROW(sequencer->AddByTag(ANIMATED, new DerivedClass()));
    ASSERT_NO_THROW(sequencer->SetEndTag(ANIMATED));
}