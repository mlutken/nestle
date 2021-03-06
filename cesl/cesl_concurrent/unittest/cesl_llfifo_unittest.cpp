#include <stdio.h>
#include <iostream>
#include <string>
#include <gtest/gtest.h>
#include <cesl_concurrent/cesl_llfifo.h>

constexpr int my_fifo_elem_max_size = 1024;
constexpr int my_fifo_elems_max_count = 4;

static char my_fifo_buffer_g[my_fifo_elems_max_count][my_fifo_elem_max_size];
static llfifo_t my_fifo_g;

static int test_llfifo_push(const char* str)
{
    return cesl_llfifo_push(&my_fifo_g, str, strlen(str));
}

// static char* test_llfifo_front_strcpy(char* dst_str)
// {
//     return strncpy(dst_str, llfifo_front(&my_fifo_g), my_fifo_elem_max_size);
// }

TEST(cesl_llfifo_unittest, create)
{
    const auto res = cesl_llfifo_create(&my_fifo_g, my_fifo_elem_max_size, my_fifo_elems_max_count, (char*)my_fifo_buffer_g);
    EXPECT_EQ(1, res);
    EXPECT_EQ(0, cesl_llfifo_size(&my_fifo_g));
    EXPECT_EQ(0, cesl_llfifo_full(&my_fifo_g));
    EXPECT_EQ(1, cesl_llfifo_empty(&my_fifo_g));
}

TEST(cesl_llfifo_unittest, push_single_element)
{
    cesl_llfifo_create(&my_fifo_g, my_fifo_elem_max_size, my_fifo_elems_max_count, (char*)my_fifo_buffer_g);
    auto ok = test_llfifo_push("first");
    EXPECT_EQ(1, ok);
    EXPECT_EQ(1, cesl_llfifo_size(&my_fifo_g));
    EXPECT_EQ(0, cesl_llfifo_full(&my_fifo_g));
    EXPECT_EQ(0, cesl_llfifo_empty(&my_fifo_g));
    EXPECT_EQ(std::string("first"), cesl_llfifo_front(&my_fifo_g));
}

TEST(cesl_llfifo_unittest, push_to_full)
{
    cesl_llfifo_create(&my_fifo_g, my_fifo_elem_max_size, my_fifo_elems_max_count, (char*)my_fifo_buffer_g);
    test_llfifo_push("first");
    test_llfifo_push("second");
    EXPECT_EQ(2, cesl_llfifo_size(&my_fifo_g));
    EXPECT_EQ(0, cesl_llfifo_full(&my_fifo_g));
    EXPECT_EQ(0, cesl_llfifo_empty(&my_fifo_g));
    EXPECT_EQ(std::string("first"), cesl_llfifo_front(&my_fifo_g));

    auto ok = test_llfifo_push("third");
    EXPECT_EQ(1, ok);
    EXPECT_EQ(3, cesl_llfifo_size(&my_fifo_g));
    EXPECT_EQ(1, cesl_llfifo_full(&my_fifo_g));
    EXPECT_EQ(0, cesl_llfifo_empty(&my_fifo_g));
    EXPECT_EQ(std::string("first") ,cesl_llfifo_front(&my_fifo_g));
}

TEST(cesl_llfifo_unittest, push_beyond_full)
{
    cesl_llfifo_create(&my_fifo_g, my_fifo_elem_max_size, my_fifo_elems_max_count, (char*)my_fifo_buffer_g);
    test_llfifo_push("first");
    test_llfifo_push("second");
    test_llfifo_push("third");
    EXPECT_EQ(3, cesl_llfifo_size(&my_fifo_g));
    EXPECT_EQ(1, cesl_llfifo_full(&my_fifo_g));
    EXPECT_EQ(0, cesl_llfifo_empty(&my_fifo_g));
    EXPECT_EQ(std::string("first"), cesl_llfifo_front(&my_fifo_g));

    // Beyond
    auto ok = test_llfifo_push("fourth");
    EXPECT_EQ(0, ok);
    EXPECT_EQ(3, cesl_llfifo_size(&my_fifo_g));
    EXPECT_EQ(1, cesl_llfifo_full(&my_fifo_g));
    EXPECT_EQ(0, cesl_llfifo_empty(&my_fifo_g));
    EXPECT_EQ(std::string("first"), cesl_llfifo_front(&my_fifo_g));
}

TEST(cesl_llfifo_unittest, pop_single_element)
{
    cesl_llfifo_create(&my_fifo_g, my_fifo_elem_max_size, my_fifo_elems_max_count, (char*)my_fifo_buffer_g);
    test_llfifo_push("first");
    EXPECT_EQ(1, cesl_llfifo_size(&my_fifo_g));
    EXPECT_EQ(0, cesl_llfifo_full(&my_fifo_g));
    EXPECT_EQ(0, cesl_llfifo_empty(&my_fifo_g));
    EXPECT_EQ(std::string("first"), cesl_llfifo_front(&my_fifo_g));
    auto ok = cesl_llfifo_pop(&my_fifo_g);
    EXPECT_EQ(1, ok);
    EXPECT_EQ(0, cesl_llfifo_size(&my_fifo_g));
    EXPECT_EQ(0, cesl_llfifo_full(&my_fifo_g));
    EXPECT_EQ(1, cesl_llfifo_empty(&my_fifo_g));
}

TEST(cesl_llfifo_unittest, pop_all_from_full)
{
    cesl_llfifo_create(&my_fifo_g, my_fifo_elem_max_size, my_fifo_elems_max_count, (char*)my_fifo_buffer_g);
    test_llfifo_push("first");
    test_llfifo_push("second");
    test_llfifo_push("third");

    EXPECT_EQ(3, cesl_llfifo_size(&my_fifo_g));
    EXPECT_EQ(1, cesl_llfifo_full(&my_fifo_g));
    EXPECT_EQ(0, cesl_llfifo_empty(&my_fifo_g));
    EXPECT_EQ(std::string("first"), cesl_llfifo_front(&my_fifo_g));

    auto ok = cesl_llfifo_pop(&my_fifo_g);
    EXPECT_EQ(1, ok);
    EXPECT_EQ(2, cesl_llfifo_size(&my_fifo_g));
    EXPECT_EQ(0, cesl_llfifo_full(&my_fifo_g));
    EXPECT_EQ(0, cesl_llfifo_empty(&my_fifo_g));
    EXPECT_EQ(std::string("second"), cesl_llfifo_front(&my_fifo_g));

    ok = cesl_llfifo_pop(&my_fifo_g);
    EXPECT_EQ(ok, 1);
    EXPECT_EQ(1, cesl_llfifo_size(&my_fifo_g));
    EXPECT_EQ(0, cesl_llfifo_full(&my_fifo_g));
    EXPECT_EQ(0, cesl_llfifo_empty(&my_fifo_g));
    EXPECT_EQ(std::string("third"), cesl_llfifo_front(&my_fifo_g));

    ok = cesl_llfifo_pop(&my_fifo_g);
    EXPECT_EQ(1, ok);
    EXPECT_EQ(0, cesl_llfifo_size(&my_fifo_g));
    EXPECT_EQ(0, cesl_llfifo_full(&my_fifo_g));
    EXPECT_EQ(1, cesl_llfifo_empty(&my_fifo_g));
}

TEST(cesl_llfifo_unittest, pop_beyond_empty)
{
    cesl_llfifo_create(&my_fifo_g, my_fifo_elem_max_size, my_fifo_elems_max_count, (char*)my_fifo_buffer_g);
    test_llfifo_push("first");
    test_llfifo_push("second");
    test_llfifo_push("third");

    EXPECT_EQ(3, cesl_llfifo_size(&my_fifo_g));
    EXPECT_EQ(1, cesl_llfifo_full(&my_fifo_g));
    EXPECT_EQ(0, cesl_llfifo_empty(&my_fifo_g));
    EXPECT_EQ(std::string("first"), cesl_llfifo_front(&my_fifo_g));

    cesl_llfifo_pop(&my_fifo_g);
    cesl_llfifo_pop(&my_fifo_g);
    auto ok = cesl_llfifo_pop(&my_fifo_g);
    EXPECT_EQ(1, ok);
    EXPECT_EQ(0, cesl_llfifo_size(&my_fifo_g));
    EXPECT_EQ(0, cesl_llfifo_full(&my_fifo_g));
    EXPECT_EQ(1, cesl_llfifo_empty(&my_fifo_g));

    ok = cesl_llfifo_pop(&my_fifo_g);
    EXPECT_EQ(0, ok);
    EXPECT_EQ(0, cesl_llfifo_size(&my_fifo_g));
    EXPECT_EQ(0, cesl_llfifo_full(&my_fifo_g));
    EXPECT_EQ(1, cesl_llfifo_empty(&my_fifo_g));
}
