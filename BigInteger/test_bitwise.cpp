#include "Integer.hpp"
#include <gtest/gtest.h>
#include <cstdlib>




TEST(BitwiseTests, Operations) {
    Integer a(0b101010), b(0b1100);
    
    EXPECT_EQ((a & b).toString(), "8");
    EXPECT_EQ((a & 0b1100).toString(), "8");
    EXPECT_EQ((0b101010 & b).toString(), "8");
    EXPECT_EQ((a & b) == Integer(8), true);
    EXPECT_EQ((a | b).toString(), "46");
    EXPECT_EQ((a | 0b1100).toString(), "46");
    EXPECT_EQ((0b101010 | b).toString(), "46");
    EXPECT_EQ((a ^ b).toString(), "38");
    EXPECT_EQ((a ^ 0b1100).toString(), "38");
    EXPECT_EQ((0b101010 ^ b).toString(), "38");

    Integer d("1234098712347612376");
    Integer e("8384745747363263655555555");
    EXPECT_EQ((d & e).toString(), "1153029499447742656");
    EXPECT_EQ((d | e).toString(), "8384745828432476555425275");
    EXPECT_EQ((d ^ e).toString(), "8384744675402977107682619");


    Integer c(1);
    int shift = rand() % 20 + 1; // Random shift between 1 and 20
    EXPECT_EQ((c << shift).toString(), std::to_string(1ULL << shift));
    c <<= shift;
    EXPECT_EQ(c.toString(), std::to_string(1ULL << shift));
    EXPECT_EQ((c >> shift).toString(), "1");
    c >>= shift;
    EXPECT_EQ(c.toString(), "1");

}