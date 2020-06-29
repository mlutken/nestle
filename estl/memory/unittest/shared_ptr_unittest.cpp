#include <stdexcept>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../shared_ptr.hpp"
#include <memory>

using namespace testing;
using namespace estl;

class SharedPtrUnitTest : public testing::Test
{
};


// ----------------------------------------
// --- Simple helper test element class ---
// ----------------------------------------

class MyClass {
public:
    MyClass () {
        std::cerr << "MyClass() CONSTRUCTOR: " << val_ << " this: " << this << "\n";
    }
    MyClass (const MyClass& other) : val_(other.val_) {
        std::cerr << "MyClass(MyClass& other) CONSTRUCTOR: " << val_ << " this: " << this << "\n";
    }

    MyClass (MyClass&& other) noexcept : val_(other.val_) {
        std::cerr << "~MyClass() DESTRUCTOR: " << val_ << " this: " << this <<  "\n";
    }

    MyClass& operator= (const MyClass& other) {
        val_ = other.val_;
        return *this;
    }

    MyClass& operator= (MyClass&& other) noexcept {
        val_ = other.val_;
        return *this;
    }

    MyClass (int val) : val_(val) {
    }

    ~MyClass() {
    }


    int val() const
    {
        return val_;
    }
    void val(int val)
    {
        val_ = val;
    }
private:
    int val_ = -99;
};


// -------------------
// -- Constructors ---
// -------------------
//template <class T>
//using shared_ptr_t = std::shared_ptr<T>;
//template <class T>
//using weak_ptr_t = std::weak_ptr<T>;

template <class T>
using shared_ptr_t = estl::shared_ptr<T>;
template <class T>
using weak_ptr_t = estl::weak_ptr<T>;

TEST_F(SharedPtrUnitTest, constructor)
{
    shared_ptr_t<MyClass> mc1;
    EXPECT_EQ(mc1.use_count(), 0);
}


TEST_F(SharedPtrUnitTest, basic_test)
{
    shared_ptr_t<MyClass> mc1;
    EXPECT_EQ(mc1.use_count(), 0);
    {
        shared_ptr_t<MyClass> mc2 = shared_ptr_t<MyClass>(new MyClass());
        mc2->val(12);
        mc1 = mc2;
        EXPECT_EQ(mc1.use_count(), 2);
        EXPECT_EQ(mc2.use_count(), 2);
    }
    EXPECT_EQ(mc1.use_count(), 1);
}

TEST_F(SharedPtrUnitTest, use_count)
{
    shared_ptr_t<MyClass> mc1;
    EXPECT_EQ(mc1.use_count(), 0);
    mc1 = shared_ptr_t<MyClass>(new MyClass());
    EXPECT_EQ(mc1.use_count(), 1);
    auto mc2 = mc1;
    EXPECT_EQ(mc1.use_count(), 2);
    mc2 = shared_ptr_t<MyClass>(new MyClass());
    EXPECT_EQ(mc1.use_count(), 1);
    EXPECT_EQ(mc2.use_count(), 1);
    mc1.reset();
    EXPECT_EQ(mc1.use_count(), 0);
    EXPECT_EQ(mc2.use_count(), 1);
}

TEST_F(SharedPtrUnitTest, reset)
{
    shared_ptr_t<MyClass> mc1;
    EXPECT_EQ(mc1.use_count(), 0);
    mc1 = shared_ptr_t<MyClass>(new MyClass());
    EXPECT_EQ(mc1.use_count(), 1);
    auto mc2 = mc1;
    EXPECT_EQ(mc1.use_count(), 2);
    mc1.reset(new MyClass());
    EXPECT_EQ(mc1.use_count(), 1);
    EXPECT_EQ(mc2.use_count(), 1);
}

TEST_F(SharedPtrUnitTest, DISABLED_weak_ptr_test)
{
    weak_ptr_t<MyClass> w_ptr;
    {
        shared_ptr_t<MyClass> s_ptr = shared_ptr_t<MyClass>(new MyClass());
        w_ptr = s_ptr;
        EXPECT_EQ(w_ptr.use_count(), 1);
//        std::cout << "w_ptr.use_count() inside scope: " << w_ptr.use_count() << '\n';
    }
    EXPECT_EQ(w_ptr.use_count(), 0);
}


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
