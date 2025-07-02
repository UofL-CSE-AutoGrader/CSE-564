#ifndef INTEGER_HPP
#define INTEGER_HPP

#include <vector>
#include <string>
#include <cstdint>
#include <iostream>
#include <stdexcept>

class Integer {
public:
    using Limb = uint32_t;
    static constexpr uint64_t LIMB_BASE = static_cast<uint64_t>(1) << 32;

    std::vector<Limb> limbs;
    bool isNegative = false;
    Integer abs() const;  // Returns the absolute value of the integer
    static Integer gcd(const Integer& a, const Integer& b);
    static Integer lcm(const Integer& a, const Integer& b);

    Integer();
    Integer(int64_t n);
    Integer(const std::string& str);

    std::string toString() const;
    void normalize();
    void setBit(int i);
    bool getBit(int i) const;


    // Arithmetic
    Integer operator+(const Integer& rhs) const;
    Integer operator-(const Integer& rhs) const;
    Integer operator*(const Integer& rhs) const;
    Integer operator/(const Integer& rhs) const;
    Integer operator%(const Integer& rhs) const;

    Integer& operator++();     
    Integer operator++(int);   
    Integer& operator--();    
    Integer operator--(int);  
    
    Integer operator-() const; 


    // Bitwise
    Integer operator&(const Integer& rhs) const;
    Integer operator|(const Integer& rhs) const;
    Integer operator^(const Integer& rhs) const;
    Integer operator~() const;
    Integer operator<<(int shift) const;
    Integer operator>>(int shift) const;

    // Comparison
    bool operator==(const Integer& rhs) const;
    bool operator!=(const Integer& rhs) const;
    bool operator<(const Integer& rhs) const;
    bool operator<=(const Integer& rhs) const;
    bool operator>(const Integer& rhs) const;
    bool operator>=(const Integer& rhs) const;

    // Assignment
    Integer& operator+=(const Integer& rhs);
    Integer& operator-=(const Integer& rhs);
    Integer& operator*=(const Integer& rhs);
    Integer& operator/=(const Integer& rhs);
    Integer& operator%=(const Integer& rhs);

    Integer& operator&=(const Integer& rhs);
    Integer& operator|=(const Integer& rhs);
    Integer& operator^=(const Integer& rhs);
    Integer& operator<<=(int shift);
    Integer& operator>>=(int shift);
    

    // Modular and exponentiation
    Integer power(const Integer& exponent) const;
    Integer power_mod(const Integer& exponent, const Integer& modulus) const;
    Integer mod_inverse(const Integer& a, const Integer& m);


};

std::ostream& operator<<(std::ostream& os, const Integer& val);


#include "Integer_Mixed_Ops.hpp"

#endif
