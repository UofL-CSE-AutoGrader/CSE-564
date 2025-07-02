#include "Integer.hpp"
#include <string>
#include <cstdlib>
#include <stdexcept>

#include <gtest/gtest.h>

TEST(ArithmeticTests, BasicOps) {
    Integer a(1234), b(5678);
    EXPECT_EQ((a + b).toString(), "6912");
    EXPECT_EQ((b - a).toString(), "4444");
    EXPECT_EQ((a * b).toString(), std::to_string(1234LL * 5678));
    EXPECT_EQ((b / a).toString(), std::to_string(5678 / 1234));
    EXPECT_EQ((b % a).toString(), std::to_string(5678 % 1234));
}

TEST(ArithmeticTests, IncrementDecrement) {
    Integer a(1000);
    EXPECT_EQ((++a).toString(), "1001");
    EXPECT_EQ((a++).toString(), "1001");
    EXPECT_EQ(a.toString(), "1002");
    EXPECT_EQ((--a).toString(), "1001");
    EXPECT_EQ((a--).toString(), "1001");
    EXPECT_EQ(a.toString(), "1000");
}

TEST(ArithmeticTests, Negation) {
    Integer a(1000);
    EXPECT_EQ((-a).toString(), "-1000");
    // EXPECT_EQ((-(-a)).toString(), "1000");
    Integer b(-2000);
    EXPECT_EQ((-b).toString(), "2000");
}

TEST(ArithmeticTests, LargeNumbers) {
    Integer a("1234567890123456789012345678901234567890");
    Integer b("9876543210987654321098765432109876543210");
    EXPECT_EQ((a + b).toString(), "11111111101111111110111111111011111111100");
    EXPECT_EQ((b - a).toString(), "8641975320864197532086419753208641975320");
    EXPECT_EQ((a - b).toString(), "-8641975320864197532086419753208641975320");
    EXPECT_EQ((a * b).toString(), "12193263113702179522618503273386678859448712086533622923332237463801111263526900");
    EXPECT_EQ((a / b).toString(), "0");
    EXPECT_EQ((b / a).toString(), "8");
    EXPECT_EQ((a % b).toString(), a.toString());
    EXPECT_EQ((b % a).toString(), "90000000009000000000900000000090");
}

TEST(ArithmeticTests, DivisionByZero) {
    Integer a(1000);
    Integer b(0);
    EXPECT_THROW(a / b, std::invalid_argument);
}

TEST(ArithmeticTests, ModuloByZero) {
    Integer a(1000);
    Integer b(0);
    EXPECT_THROW(a % b, std::invalid_argument);
}

TEST(ArithmeticTests, DivisionAndModulo) {
    Integer a(1000), b(3);
    EXPECT_EQ((a / b).toString(), "333");
    EXPECT_EQ((a % b).toString(), "1");
    
    Integer c(1001);
    EXPECT_EQ((c / b).toString(), "333");
    EXPECT_EQ((c % b).toString(), "2");
}

TEST(ArithmeticTests, MixedOperations) {
    Integer a(1000), b(2000);
    Integer c = a + b;
    EXPECT_EQ(c.toString(), "3000");
    
    c -= a;
    EXPECT_EQ(c.toString(), "2000");
    
    c *= 2;
    EXPECT_EQ(c.toString(), "4000");
    
    c /= 4;
    EXPECT_EQ(c.toString(), "1000");
    
    c %= 3;
    EXPECT_EQ(c.toString(), "1");
}

TEST(ArithmeticTests, MixedOpsWithIntegers) {
    Integer a(1000);
    EXPECT_EQ((a + 500).toString(), "1500");
    EXPECT_EQ((a - 500).toString(), "500");
    EXPECT_EQ((a * 2).toString(), "2000");
    EXPECT_EQ((a / 2).toString(), "500");
    EXPECT_EQ((a % 3).toString(), "1");

    Integer b(2000);
    EXPECT_EQ((b + 1000).toString(), "3000");
    EXPECT_EQ((1000 + b).toString(), "3000");
    EXPECT_EQ((b - 1000).toString(), "1000");
    EXPECT_EQ((1000 - b).toString(), "-1000");
    
    EXPECT_EQ((b * 3).toString(), "6000");
    EXPECT_EQ((3 * b).toString(), "6000");
    EXPECT_EQ((b / 4).toString(), "500");
    EXPECT_EQ((4/ b).toString(), "0");
    EXPECT_EQ((50005/ Integer(5)).toString(), "10001");
    EXPECT_EQ((b % 5).toString(), "0");
    EXPECT_EQ((5 % b).toString(), "5");
    EXPECT_EQ((-(-b)).toString(), "2000");

    b += 5;
    EXPECT_EQ(b.toString(), "2005");
    b -= 10;
    EXPECT_EQ(b.toString(), "1995");
    b *= 2;
    EXPECT_EQ(b.toString(), "3990");
    b /= 3;
    EXPECT_EQ(b.toString(), "1330");
    b %= 100;
    EXPECT_EQ(b.toString(), "30");
    b += b;
    EXPECT_EQ(b.toString(), "60");
    b -= 2*b;
    EXPECT_EQ(b.toString(), "-60");
    b *= -1;
    EXPECT_EQ(b.toString(), "60");
    b = 3*b - 2*b + 1;
    EXPECT_EQ(b.toString(), "61");
}

TEST(ArithmeticTests, EdgeCases) {
    Integer c(4), b(1), d(6), e(2), f(4);
    Integer a = (c * d) /((e*f*d)/2) - 1; // Should be 0
    EXPECT_EQ(a.toString(), "0");
    EXPECT_EQ((a + b).toString(), "1");
    EXPECT_EQ((b - a).toString(), "1");
    EXPECT_EQ((a * b).toString(), "0");
    EXPECT_EQ((a / b).toString(), "0");
    EXPECT_EQ((a % b).toString(), "0");
    EXPECT_THROW(b / a, std::invalid_argument); // Division by zero
    EXPECT_THROW(b % a, std::invalid_argument); // Modulo by zero
}

TEST(ArithmeticTests, LoopOperations) {
    Integer a(0);
    Integer b("454334");
    Integer bb = b;
    int random = rand() % 50 + 10;
    for (int i = 0; i <= random; i++) {
        a += Integer(i);
        bb *= b;
    }
    EXPECT_EQ(a.toString(), std::to_string((random * (random + 1)) / 2)); 
    EXPECT_EQ((bb*(1 + bb)).toString(), ((bb +1)* bb).toString());
    EXPECT_EQ((bb + bb).toString(), (bb * 2).toString());
    EXPECT_EQ((bb - bb).toString(), "0");
    EXPECT_EQ((bb / bb).toString(), "1");
    EXPECT_EQ((bb % bb).toString(), "0");
    EXPECT_EQ((bb * 0).toString(), "0");
    EXPECT_EQ((bb + Integer(0)).toString(), bb.toString());
    EXPECT_EQ((bb - Integer(0)).toString(), bb.toString());
    EXPECT_EQ((bb * Integer(1)).toString(), bb.toString());
    EXPECT_EQ((bb / Integer(1)).toString(), bb.toString());
    EXPECT_EQ((bb % Integer(1)).toString(), "0");
    EXPECT_EQ((bb + Integer(random)).toString(), (bb + random).toString());
    EXPECT_EQ((bb - Integer(random)).toString(), (bb - random).toString());
}