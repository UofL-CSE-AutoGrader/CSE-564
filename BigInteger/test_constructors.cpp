#include "Integer.hpp"
#include <string>
#include<cstdlib>

#include <gtest/gtest.h>


TEST(ConstructorTests, FromVariousTypes) {
    EXPECT_EQ(Integer(0).toString(), "0");
    EXPECT_EQ(Integer("-0").toString(), "0");
    EXPECT_EQ(Integer(42).toString(), "42");
    EXPECT_EQ(Integer(-123456789).toString(), "-123456789");
    EXPECT_EQ(Integer("987654321").toString(), "987654321");
    EXPECT_EQ(Integer("000").toString(), "0");
}

TEST(BigConstructorTests, FromLargeString) {
    std::string largeNumber;
    // pick random number from 1 to 100 
    int random = rand()%100 + 100;
    for (int i = 1; i < random; ++i) {
        int j = i % 67;
        // add integer as a string
        largeNumber += std::to_string(j); 
    }
    Integer bigInt(largeNumber);
    EXPECT_EQ(bigInt.toString(), largeNumber);
    Integer bigInt2("00000" + largeNumber);
    EXPECT_EQ(bigInt2.toString(), largeNumber);

}