#include "Integer.hpp"
#include <gtest/gtest.h>
#include <cstdlib>

TEST(ModularTests, InverseMod) {
    int a = 3;
    int m = 11;
    Integer a_int(a);
    Integer m_int(m);
    Integer inv = mod_inverse(a_int, m_int);
    Integer inv2 = mod_inverse(a, m);
    EXPECT_EQ(inv, inv2); // Check if both methods yield the same result
    EXPECT_EQ((a_int * inv) % m_int, Integer(1));
    EXPECT_EQ((a * inv % m), Integer(1)); // Check if the inverse is correct
    EXPECT_EQ((a_int * inv % m), Integer(1)); // Check if the inverse is correct
    EXPECT_EQ((a * inv) % m, Integer(1));

    
    // Edge case: Inverse of 0 should throw
    EXPECT_THROW(mod_inverse(Integer(0), 5), std::invalid_argument);
    EXPECT_THROW(mod_inverse(Integer(5), Integer(0)), std::invalid_argument); // Modulus cannot be zero
    EXPECT_THROW(mod_inverse(Integer(5), Integer(-1)), std::invalid_argument); // Modulus cannot be negative
    EXPECT_THROW(mod_inverse(Integer(5), Integer(10)), std::invalid_argument); // Inverse does not exist for these values
    EXPECT_THROW(mod_inverse(Integer(4), 8), std::invalid_argument); // Inverse does not exist for these values
    EXPECT_THROW(mod_inverse(Integer(4), 0), std::invalid_argument); // Modulus cannot be zero
    EXPECT_THROW(mod_inverse(7, Integer(0)), std::invalid_argument); // Modulus cannot be zero
    EXPECT_THROW(mod_inverse(7, -5), std::invalid_argument); // Modulus cannot be negative
}

TEST(ModularTests, LargeValueInverseMod) {
    Integer val1("4855234577788899234509871023409817234098172350987690227473");
    Integer val2("44854567892341234123409850987908709869876123409812734987654321113893929239484");
    Integer inv = mod_inverse(val1, val2);
    EXPECT_TRUE((val1 * inv) % val2 == Integer(1));  // Check if the inverse is correct
    EXPECT_EQ(inv.toString(), "30658726546415385294719313215095585612681228684500232158304370609132223744965");
}

TEST(ModularTests, PowerTest) {
    int a = 56;
    int exponent = 39998;
    std::string modValue = "4995958382272737434858969690760695493838";
    Integer mod(modValue);
    Integer a_int(a);
    Integer e_int(exponent);
    Integer result = a_int.power(e_int);
    Integer expected = Integer(a).power(Integer(exponent));
    EXPECT_EQ(result, expected); // Check if the power function works correctly
    Integer modvalue1 = a_int.power_mod(e_int, mod);
    Integer modvalue2 = Integer(a).power_mod(Integer(exponent), mod);
    EXPECT_EQ(modvalue1, modvalue2); // Check if the modular power function works correctly
    EXPECT_EQ(modvalue1.toString(), (result % mod).toString());
}