#include "Integer.hpp"

#include <gtest/gtest.h>
#include <sstream>
#include <stdexcept>
#include <string>
#include <cstdlib>
#include <type_traits>

namespace {

const std::string A = "123456789012345678901234567890123456789012345678901234567890";
const std::string B = "98765432109876543210987654321098765432109876543210987654321";
const std::string H = "10000000000000000000000000000000000000000000000000000000000000000000000000123";

void ExpectString(const Integer& value, const std::string& expected) {
    EXPECT_EQ(value.toString(), expected);
}

std::string StreamToString(const Integer& value) {
    std::ostringstream out;
    out << value;
    return out.str();
}

}  // namespace

TEST(IntegerConstructionAndOutput, SupportsLargePositiveNegativeAndStreamValues) {
    ExpectString(Integer(), "0");
    ExpectString(Integer(1234567890), "1234567890");
    ExpectString(Integer(-42), "-42");
    ExpectString(Integer(A), A);
    ExpectString(Integer("-" + A), "-" + A);
    ExpectString(Integer("-0"), "0");
    ExpectString(Integer("-0000"), "0");
    EXPECT_EQ(StreamToString(Integer(A)), A);
    EXPECT_THROW(Integer("12345678901234567890x"), std::invalid_argument);
}

TEST(IntegerUtilities, NormalizesAbsAndIndividualBits) {
    Integer noisy(A);
    noisy.limbs.push_back(0);
    noisy.limbs.push_back(0);
    noisy.normalize();
    ExpectString(noisy, A);

    ExpectString(Integer("-" + A).abs(), A);

    Integer bits;
    bits.setBit(200);
    bits.setBit(65);
    bits.setBit(32);
    bits.setBit(5);
    ExpectString(bits, "1606938044258990275541962092341162602522239887270944549371936");
    EXPECT_TRUE(bits.getBit(200));
    EXPECT_TRUE(bits.getBit(65));
    EXPECT_TRUE(bits.getBit(32));
    EXPECT_TRUE(bits.getBit(5));
    EXPECT_FALSE(bits.getBit(199));
    EXPECT_FALSE(bits.getBit(0));
    EXPECT_THROW(bits.setBit(-1), std::invalid_argument);
    EXPECT_THROW(bits.getBit(-1), std::invalid_argument);
}

TEST(IntegerComparisonOperators, CompareVeryLargeAndNegativeValues) {
    const Integer a(A);
    const Integer b(B);
    const Integer negativeA("-" + A);

    EXPECT_TRUE(a == Integer(A));
    EXPECT_FALSE(a == b);
    EXPECT_TRUE(a != b);
    EXPECT_TRUE(b < a);
    EXPECT_TRUE(b <= a);
    EXPECT_TRUE(a <= Integer(A));
    EXPECT_TRUE(a > b);
    EXPECT_TRUE(a >= b);
    EXPECT_TRUE(a >= Integer(A));
    EXPECT_TRUE(negativeA < Integer(0));
    EXPECT_TRUE(negativeA < b);
    EXPECT_TRUE(Integer(0) > negativeA);
}

TEST(IntegerArithmeticOperators, AddSubtractMultiplyDivideAndModuloLargeValues) {
    const Integer a(A);
    const Integer b(B);

    ExpectString(a + b, "222222221122222222112222222211222222221122222222112222222211");
    ExpectString(a - b, "24691356902469135690246913569024691356902469135690246913569");
    ExpectString(b - a, "-24691356902469135690246913569024691356902469135690246913569");
    ExpectString(a * b, "12193263113702179522618503273386678859451150739156363359236736777929561194939744871208653362292333223746380111126352690");
    ExpectString(a / b, "1");
    ExpectString(a % b, "24691356902469135690246913569024691356902469135690246913569");
    EXPECT_THROW(a / Integer(0), std::invalid_argument);
    EXPECT_THROW(a % Integer(0), std::invalid_argument);
}

TEST(IntegerUnaryIncrementDecrementOperators, PreservePrefixAndPostfixSemanticsForLargeValues) {
    Integer value(A);

    ExpectString(-value, "-" + A);
    ExpectString(-(-value), A);

    ExpectString(++value, "123456789012345678901234567890123456789012345678901234567891");
    ExpectString(value++, "123456789012345678901234567890123456789012345678901234567891");
    ExpectString(value, "123456789012345678901234567890123456789012345678901234567892");
    ExpectString(--value, "123456789012345678901234567890123456789012345678901234567891");
    ExpectString(value--, "123456789012345678901234567890123456789012345678901234567891");
    ExpectString(value, A);
}

TEST(IntegerArithmeticAssignmentOperators, MatchBinaryOperatorsForLargeValues) {
    const Integer a(A);
    const Integer b(B);

    Integer value = a;
    value += b;
    EXPECT_EQ(value, a + b);
    ExpectString(value, "222222221122222222112222222211222222221122222222112222222211");

    value = a;
    value -= b;
    EXPECT_EQ(value, a - b);
    ExpectString(value, "24691356902469135690246913569024691356902469135690246913569");

    value = b;
    value -= a;
    EXPECT_EQ(value, b - a);
    ExpectString(value, "-24691356902469135690246913569024691356902469135690246913569");

    value = a;
    value *= b;
    EXPECT_EQ(value, a * b);
    ExpectString(value, "12193263113702179522618503273386678859451150739156363359236736777929561194939744871208653362292333223746380111126352690");

    value = a;
    value /= b;
    EXPECT_EQ(value, a / b);
    ExpectString(value, "1");

    value = a;
    value %= b;
    EXPECT_EQ(value, a % b);
    ExpectString(value, "24691356902469135690246913569024691356902469135690246913569");
}

TEST(IntegerBitwiseOperators, HandleLargeSparseValues) {
    const Integer x("1606938044258990275541962092342430253122431223184289538506785");
    const Integer y("1606938044258991702789654798301043660808172443277929218048034");

    ExpectString(x & y, "1606938044258990275541962092341162602522202993782792835301408");
    ExpectString(x | y, "1606938044258991702789654798302311311408400672679425921253411");
    ExpectString(x ^ y, "1427247692705961148708886197678896633085952003");
    ExpectString(~Integer(0), "4294967295");
    ExpectString(~Integer("4294967295"), "0");
    ExpectString(~(Integer(1) << 64), "79228162495817593519834398719");
    EXPECT_THROW(~Integer(-1), std::invalid_argument);
}

TEST(IntegerShiftOperators, ShiftLargeValuesAcrossMultipleLimbs) {
    const Integer x("1606938044258990275541962092342430253122431223184289538506785");

    ExpectString(x << 37, "220855883097298041197912187593039039050299007602745617564210375971307520");
    ExpectString(x >> 64, "87112285931760246646623899502601381609472");
    ExpectString(x << -64, "87112285931760246646623899502601381609472");
    ExpectString(x >> -37, "220855883097298041197912187593039039050299007602745617564210375971307520");
    ExpectString(x >> 500, "0");
}

TEST(IntegerBitwiseAssignmentOperators, ExposeCompoundBitwiseApiAndTestImplementedShifts) {
    const Integer x("1606938044258990275541962092342430253122431223184289538506785");

    static_assert(std::is_same_v<decltype(&Integer::operator&=), Integer& (Integer::*)(const Integer&)>);
    static_assert(std::is_same_v<decltype(&Integer::operator|=), Integer& (Integer::*)(const Integer&)>);
    static_assert(std::is_same_v<decltype(&Integer::operator^=), Integer& (Integer::*)(const Integer&)>);

    Integer value = x;
    value <<= 37;
    EXPECT_EQ(value, x << 37);
    ExpectString(value, "220855883097298041197912187593039039050299007602745617564210375971307520");

    value = x;
    value >>= 64;
    EXPECT_EQ(value, x >> 64);
    ExpectString(value, "87112285931760246646623899502601381609472");
}

TEST(IntegerModularAndPowerOperations, ComputeLargeGcdLcmPowerPowerModAndInverse) {
    const Integer m("100000000000000000000000000000000000000000000000000000000000001");
    const Integer n("99999999999999999999999999999999999999999999999999999999999989");
    const Integer base("1234567890123456789012345678901234567890");
    const Integer modulus("100000000000000000000000000000000000000000000000000000000000003");

    ExpectString(Integer::gcd(m, n), "1");
    ExpectString(Integer::lcm(m, n), "9999999999999999999999999999999999999999999999999999999999998999999999999999999999999999999999999999999999999999999999999989");
    ExpectString(base.power(Integer(3)), "1881676372353657772546716040595286755373973700255343476997709998147026668834432100633207693797722198701224860897069000");
    ExpectString(base.power_mod(Integer(12345), modulus), "92370531067142945847329876535944131109180954044957190505703732");
    ExpectString(mod_inverse(Integer(17), Integer(3120)), "2753");
    ExpectString(mod_inverse(Integer("1234567890123456789012345678901234567891"), modulus), "18194460358765768350626355899742037809135811819920680658111833");

    EXPECT_THROW(Integer::gcd(Integer(-1), Integer(2)), std::invalid_argument);
    EXPECT_THROW(base.power(Integer(-1)), std::invalid_argument);
    EXPECT_THROW(base.power_mod(Integer(2), Integer(0)), std::invalid_argument);
    EXPECT_THROW(mod_inverse(Integer(0), Integer(17)), std::invalid_argument);
    EXPECT_THROW(mod_inverse(Integer(6), Integer(12)), std::invalid_argument);
}

TEST(IntegerMixedArithmeticOperators, SupportPrimitiveOnEitherSideOfLargeInteger) {
    const Integer h(H);

    ExpectString(h + 97, "10000000000000000000000000000000000000000000000000000000000000000000000000220");
    ExpectString(97 + h, "10000000000000000000000000000000000000000000000000000000000000000000000000220");
    ExpectString(h - 97, "10000000000000000000000000000000000000000000000000000000000000000000000000026");
    ExpectString(97 - h, "-10000000000000000000000000000000000000000000000000000000000000000000000000026");
    ExpectString(h * 97, "970000000000000000000000000000000000000000000000000000000000000000000000011931");
    ExpectString(97 * h, "970000000000000000000000000000000000000000000000000000000000000000000000011931");
    ExpectString(h / 97, "103092783505154639175257731958762886597938144329896907216494845360824742269");
    ExpectString(97 / h, "0");
    ExpectString(h % 97, "30");
    ExpectString(97 % h, "97");
}

TEST(IntegerMixedBitwiseOperators, SupportPrimitiveOnEitherSideOfLargeInteger) {
    const Integer high("1361129467683753853853498429727072845866");

    ExpectString(high & 15, "10");
    ExpectString(15 & high, "10");
    ExpectString(high | 15, "1361129467683753853853498429727072845871");
    ExpectString(15 | high, "1361129467683753853853498429727072845871");
    ExpectString(high ^ 15, "1361129467683753853853498429727072845861");
    ExpectString(15 ^ high, "1361129467683753853853498429727072845861");
}

TEST(IntegerMixedComparisonOperators, SupportPrimitiveOnEitherSideOfLargeInteger) {
    const Integer h(H);

    EXPECT_TRUE(h == Integer(H));
    EXPECT_TRUE(Integer(97) == 97);
    EXPECT_TRUE(97 == Integer(97));
    EXPECT_TRUE(h != 97);
    EXPECT_TRUE(97 != h);
    EXPECT_TRUE(97 < h);
    EXPECT_TRUE(97 <= h);
    EXPECT_TRUE(h > 97);
    EXPECT_TRUE(h >= 97);
    EXPECT_TRUE(Integer(97) <= 97);
    EXPECT_TRUE(97 >= Integer(97));
}

TEST(IntegerMixedModularHelpers, SupportPrimitiveTemplateHelpers) {
    ExpectString(power(Integer("12345678901234567890"), 5), "286797186173370403767041767776920429666954333495933335798264659838306817363852838672048294900000");
    ExpectString(power_mod(9000000000000000000LL, 12345, 9223372036854775783LL), "3049003758778368259");
    ExpectString(power_mod(Integer("123456789012345678901234567890"), 12345, 987654321), "721644273");
    ExpectString(mod_inverse(17, 3120), "2753");
}

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
    EXPECT_GT(e, f);
    EXPECT_GT(e, f);
    EXPECT_LT(f, e);
    EXPECT_LE(f, e);
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


