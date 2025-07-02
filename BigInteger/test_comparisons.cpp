#include "Integer.hpp"
#include <cstdlib>
#include <gtest/gtest.h>


TEST(ComparisonTests, Relational) {
    Integer a(1000), b(2000), c(1000);
    EXPECT_LT(a, b);
    EXPECT_LE(a, b);
    EXPECT_GT(b, a);
    EXPECT_GE(b, a);
    EXPECT_NE(a, b);
    EXPECT_EQ(a, c);

    Integer d("98798292837429834928283835357650098098098");
    Integer dd = d;
    int n = 5;
    for (int i = 0; i< n; i++) {
        dd *= d;
    }
    Integer ddd = dd * d;
    EXPECT_EQ((ddd/d).toString(),dd.toString());
    
    Integer e("98798292837429834928283835357650098098099");
    EXPECT_EQ(d/2, (e-1)/2);
    EXPECT_LT(d, e);
    EXPECT_LE(d, e);
    EXPECT_GT(e, d);
    EXPECT_GE(e, d);
    EXPECT_NE(d, e);
    EXPECT_NE(e, d);
    EXPECT_EQ(e, ++d);
    EXPECT_EQ(--e, --d);
    EXPECT_EQ(e++,e-- - 1);
    EXPECT_EQ(e, e);
    EXPECT_EQ(d, d);
    EXPECT_EQ(d, d + Integer(0));
    EXPECT_EQ(e, e + Integer(0));
    EXPECT_EQ(d, d - Integer(0));
    EXPECT_EQ(e, e - Integer(0));
    EXPECT_EQ(d, d * Integer(1));
    EXPECT_EQ(e, e * Integer(1));
    EXPECT_EQ(d, d / Integer(1));
    EXPECT_EQ(e, e / Integer(1));
    EXPECT_EQ(d, d + d % Integer(1));
    EXPECT_EQ(e, -(e % Integer(1) - e));
}

TEST(ComparisonTests, EdgeCases) {
    Integer a(0), b(0);
    EXPECT_EQ(a, b);
    EXPECT_LE(a, b);
    EXPECT_GE(a, b);
    EXPECT_FALSE(a < b);
    EXPECT_FALSE(a > b);
    
    Integer c(-1), d(1);
    EXPECT_LT(c, d);
    EXPECT_LE(c, d);
    EXPECT_GT(d, c);
    EXPECT_GE(d, c);
    
    Integer e("123456789012345678901234567890");
    Integer f("123456789012345678901234567891");
    EXPECT_LT(e, f);
    EXPECT_LE(e, f);
    EXPECT_GT(f, e);
    EXPECT_GE(f, e);
}

TEST(ComparisonTests, RandomValues) {
    for (int i = 0; i < 100; ++i) {
        int64_t a_val = rand() % 1000000;
        int64_t b_val = rand() % 1000000;
        Integer a(a_val), b(b_val);
        
        if (a_val < b_val) {
            EXPECT_LT(a, b);
            EXPECT_LE(a, b);
            EXPECT_GT(b, a);
            EXPECT_GE(b, a);
        } else if (a_val > b_val) {
            EXPECT_GT(a, b);
            EXPECT_GE(a, b);
            EXPECT_LT(b, a);
            EXPECT_LE(b, a);
        } else {
            EXPECT_EQ(a, b);
            EXPECT_LE(a, b);
            EXPECT_GE(a, b);
        }
    }
}




TEST(ComparisonTests, MixedNegativeValues) {
    Integer a(-1000);
    int b = -2000;
    EXPECT_TRUE(b < a);
    EXPECT_TRUE(b <= a);
    EXPECT_TRUE(a > b);
    EXPECT_TRUE(a >= b);
    EXPECT_TRUE(a != b);
    EXPECT_TRUE(Integer(-1000) == a);
    EXPECT_TRUE(-1000 == a);
    EXPECT_TRUE(Integer(-1000) == Integer(-1000));
    EXPECT_TRUE(-1000 == Integer(-1000));
    EXPECT_TRUE(Integer(-1000) != Integer(1000));
    EXPECT_TRUE(-1000 != Integer(1000));
    EXPECT_FALSE(a < b);
    EXPECT_FALSE(a <= b);
    EXPECT_FALSE(a < a);
    EXPECT_TRUE(a <= a);    
    EXPECT_FALSE(a > 3);
    EXPECT_FALSE(a >= 3);
    EXPECT_TRUE(a < 3);
    EXPECT_TRUE(a <= 3);
    EXPECT_TRUE(a > -2000);
    EXPECT_TRUE(a >= -2000);
    Integer c(-1000);
    int d = -1000;
    EXPECT_TRUE(c == d);
    Integer e(500);
    int f = 1000;
    EXPECT_TRUE(-e > -f);
    EXPECT_TRUE(-e >= -f);
    EXPECT_TRUE(f > e);
    EXPECT_TRUE(f >= e);
    EXPECT_TRUE(Integer(-55) == -55);
    EXPECT_TRUE(55 > Integer(-55));
    EXPECT_TRUE(Integer(-55) < 55);
    EXPECT_TRUE(Integer(-55) <= 55);
    EXPECT_TRUE(Integer(-55) >= -55);
    EXPECT_TRUE(Integer(-55) != 55);
    EXPECT_TRUE(Integer(-55) != Integer(55));
    EXPECT_FALSE(Integer(55) < Integer(-55));
    EXPECT_FALSE(Integer(55) <= Integer(-55));
    EXPECT_FALSE(55 < Integer(-55));
    EXPECT_FALSE(55 <= Integer(-55));
    EXPECT_FALSE(Integer(-55) > 55);
    EXPECT_FALSE(Integer(-55) >= 55);
    EXPECT_FALSE(Integer(-55) > Integer(55));
}

TEST(ComparisonTests, EdgeCasesNegative) {
    Integer a(-1), b(-1);
    EXPECT_EQ(a, b);
    EXPECT_LE(a, b);
    EXPECT_GE(a, b);
    EXPECT_FALSE(a < b);
    EXPECT_FALSE(a > b);
    
    Integer c(0), d(-1);
    EXPECT_LT(d, c);
    EXPECT_LE(d, c);
    EXPECT_GT(c, d);
    EXPECT_GE(c, d);
    
    Integer e("-123456789012345678901234567890");
    Integer f("-123456789012345678901234567891");
    EXPECT_LT(e, f);
    EXPECT_LE(e, f);
    EXPECT_GT(f, e);
    EXPECT_GE(f, e);
}

TEST(ComparisonTests, RandomNegativeValues) {
    for (int i = 0; i < 1000; ++i) {
        int64_t a_val = - (rand() % 1000000);
        int64_t b_val = - (rand() % 1000000);
        Integer a(a_val), b(b_val);
        
        if (a_val < b_val) {
            EXPECT_TRUE(a < b);
            EXPECT_TRUE(a <= b);
            EXPECT_TRUE(b > a);
            EXPECT_TRUE(b >= a);
            EXPECT_TRUE(a != b);
            EXPECT_FALSE(a == b);
            EXPECT_FALSE(a >= b);
            EXPECT_FALSE(a > b);

            EXPECT_TRUE(a_val < b);
            EXPECT_TRUE(a_val <= b);
            EXPECT_TRUE(b > a_val);
            EXPECT_TRUE(b >= a_val);
            EXPECT_TRUE(a != b_val);
            EXPECT_FALSE(a == b_val);
            EXPECT_TRUE(a <= b_val);
            EXPECT_TRUE(a < b_val);

        } else if (a_val > b_val) {
            EXPECT_TRUE(a > b);
            EXPECT_TRUE(a >= b);
            EXPECT_TRUE(b < a);
            EXPECT_TRUE(b <= a);
            EXPECT_TRUE(a != b);
            EXPECT_FALSE(a == b);
            EXPECT_FALSE(a <= b);
            EXPECT_FALSE(a < b);

            EXPECT_TRUE(a_val > b);
            EXPECT_TRUE(a_val >= b);
            EXPECT_TRUE(b < a_val);
            EXPECT_TRUE(b <= a_val);
            EXPECT_TRUE(a != b_val);
            EXPECT_FALSE(a == b_val);
            EXPECT_FALSE(a <= b_val);
            EXPECT_FALSE(a < b_val);

        } else {
            EXPECT_TRUE(a == b);
            EXPECT_TRUE(a <= b);
            EXPECT_TRUE(a >= b);
            EXPECT_FALSE(a < b);
            EXPECT_FALSE(a > b);
            EXPECT_TRUE(a_val == b);
            EXPECT_TRUE(a_val <= b);
            EXPECT_TRUE(a_val >= b);
            EXPECT_FALSE(a_val < b);
            EXPECT_FALSE(a_val > b);
        }
    }
}

TEST(ComparisonTests, MixedNegativeTypes) {
    Integer a(-1000);
    int b = -2000;
    EXPECT_TRUE(b < a);
    EXPECT_TRUE(b <= a);
    EXPECT_TRUE(a > b);
    EXPECT_TRUE(a >= b);
    EXPECT_TRUE(a != b);
    EXPECT_TRUE(Integer(-1000) == a);
    EXPECT_TRUE(-1000 == a);
    EXPECT_TRUE(Integer(-1000) == Integer(-1000));
    EXPECT_TRUE(-1000 == Integer(-1000));

    EXPECT_TRUE(Integer(-1000) != Integer(1000));
    EXPECT_TRUE(-1000 != Integer(1000));
    EXPECT_FALSE(a < b);
    EXPECT_FALSE(a <= b);
    EXPECT_FALSE(a < a);
    EXPECT_TRUE(a <= a);
    EXPECT_FALSE(a > 3);
    EXPECT_FALSE(a >= 3);
    EXPECT_TRUE(a < 3);
    EXPECT_TRUE(a <= 3);
    EXPECT_TRUE(a > -2000);
    EXPECT_TRUE(a >= -2000);
    
    Integer c(-1000);
    int d = -1000;
    EXPECT_EQ(c, d);
    
    Integer e(500);
    int f = 1000;
    EXPECT_TRUE(-e > -f);
    EXPECT_TRUE(-e >= -f);
    EXPECT_TRUE(f > e);
    EXPECT_TRUE(f >= e);
    EXPECT_TRUE(Integer(-55) == -55);
    EXPECT_TRUE(55 > Integer(-55));
    EXPECT_TRUE(Integer(-55) < 55);
    EXPECT_TRUE(Integer(-55) <= 55);
    EXPECT_TRUE(Integer(-55) >= -55);
    EXPECT_TRUE(Integer(-55) != 55);
    EXPECT_TRUE(Integer(-55) != Integer(55));
}




TEST(ComparisonTests, LessThanTest) {
    int a = 334324;
    Integer aa = a;
    Integer aaa = aa;
    EXPECT_FALSE(a < aa);
    EXPECT_FALSE(aa < a);
    EXPECT_FALSE(aa < aa);
    EXPECT_TRUE(a <= aa);
    EXPECT_TRUE(aa <= a);
    EXPECT_TRUE(aa <= aa);
    for (int i = 0; i< 20; i++) {
        aaa *= aa;
        EXPECT_TRUE(a < aaa);
        EXPECT_FALSE(aaa < a);
        EXPECT_TRUE(aa < aaa);
        EXPECT_FALSE(aaa < aa);
        EXPECT_TRUE(a <= aaa);
        EXPECT_FALSE(aaa <= a);
        EXPECT_TRUE(aa <= aaa);
        EXPECT_FALSE(aaa <= aa);
    }
}

TEST(ComparisonTests, GreaterThanTest) {
    int a = 334324;
    Integer aa = a;
    Integer aaa = aa;
    EXPECT_FALSE(a > aa);
    EXPECT_FALSE(aa > a);
    EXPECT_FALSE(aa > aa);
    EXPECT_TRUE(a >= aa);
    EXPECT_TRUE(aa >= a);
    EXPECT_TRUE(aa >= aa);
    for (int i = 0; i< 20; i++) {
        aaa *= aa;
        EXPECT_FALSE(a > aaa);
        EXPECT_TRUE(aaa > a);
        EXPECT_FALSE(aa > aaa);
        EXPECT_TRUE(aaa > aa);
        EXPECT_FALSE(a >= aaa);
        EXPECT_TRUE(aaa >= a);
        EXPECT_FALSE(aa >= aaa);
        EXPECT_TRUE(aaa >= aa);
    }
}

TEST(ComparisonTests, EqualityTest) {
    int a = 334324;

    Integer aa = a;
    Integer aaa = aa;
    EXPECT_TRUE(a == aa);
    EXPECT_TRUE(aa == a);
    EXPECT_TRUE(aa == aa);
    EXPECT_FALSE(a != aa);
    EXPECT_FALSE(aa != a);
    for (int i = 0; i< 20; i++) {
        aaa *= aa;
        EXPECT_FALSE(a == aaa);
        EXPECT_FALSE(aa == aaa);
        EXPECT_FALSE(aaa == a);
        EXPECT_FALSE(aaa == aa);
        EXPECT_TRUE(a != aaa);
        EXPECT_TRUE(aa != aaa);
        EXPECT_TRUE(aaa != a);
        EXPECT_TRUE(aaa != aa);
    }
}
