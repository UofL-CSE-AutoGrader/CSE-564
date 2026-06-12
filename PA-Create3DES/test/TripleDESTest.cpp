#include "des/TripleDES.hpp"

#include <cstdint>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include <gtest/gtest.h>

#ifndef CREATE3DES_TEST_DATA_DIR
#define CREATE3DES_TEST_DATA_DIR "."
#endif

namespace {
std::string readFile(const std::string& path) {
    std::ifstream input(path, std::ios::binary);
    if (!input) {
        throw std::runtime_error("Unable to open test fixture: " + path);
    }

    return {std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>()};
}

std::string blocksToString(const std::vector<std::uint64_t>& blocks, std::size_t original_size) {
    std::string output;
    output.reserve(original_size);

    for (std::uint64_t block : blocks) {
        for (int shift = 56; shift >= 0 && output.size() < original_size; shift -= 8) {
            output.push_back(static_cast<char>((block >> shift) & 0xffU));
        }
    }

    return output;
}
} // namespace

TEST(DESTest, OneKeyTripleDESEncryptsKnownFipsVector) {
    TripleDES tripledes(0x133457799BBCDFF1ULL);

    EXPECT_EQ(tripledes.encrypt(0x0123456789ABCDEFULL), 0x85E813540F0AB405ULL);
    EXPECT_EQ(tripledes.decrypt(0x85E813540F0AB405ULL), 0x0123456789ABCDEFULL);
}

TEST(TripleDESTest, DefaultConstructedObjectDoesNotHaveKeys) {
    TripleDES tripledes;

    EXPECT_FALSE(tripledes.hasKey());
    EXPECT_THROW(static_cast<void>(tripledes.key()), std::logic_error);
    EXPECT_THROW(static_cast<void>(tripledes.encrypt(0x0123456789ABCDEFULL)), std::logic_error);
    EXPECT_THROW(static_cast<void>(tripledes.decrypt(0x85E813540F0AB405ULL)), std::logic_error);
}

TEST(TripleDESTest, OneKeyConstructorUsesSameKeyForEveryStage) {
    const std::uint64_t key = 0x133457799BBCDFF1ULL;
    const std::uint64_t plaintext = 0x0123456789ABCDEFULL;

    TripleDES tripledes(key);

    EXPECT_TRUE(tripledes.hasKey());
    EXPECT_EQ(tripledes.key(), key);
    EXPECT_EQ(tripledes.key1(), key);
    EXPECT_EQ(tripledes.key2(), key);
    EXPECT_EQ(tripledes.key3(), key);
    EXPECT_EQ(tripledes.encrypt(plaintext), 0x85E813540F0AB405ULL);
    EXPECT_EQ(tripledes.decrypt(0x85E813540F0AB405ULL), plaintext);
}

TEST(TripleDESTest, OneKeySetterUsesSameKeyForEveryStage) {
    const std::uint64_t key = 0x133457799BBCDFF1ULL;
    TripleDES tripledes;

    tripledes.setKey(key);

    EXPECT_TRUE(tripledes.hasKey());
    EXPECT_EQ(tripledes.key1(), key);
    EXPECT_EQ(tripledes.key2(), key);
    EXPECT_EQ(tripledes.key3(), key);
}

TEST(TripleDESTest, ThreeKeyConstructorStoresEveryKey) {
    const std::uint64_t key1 = 0x0123456789ABCDEFULL;
    const std::uint64_t key2 = 0x23456789ABCDEF01ULL;
    const std::uint64_t key3 = 0x456789ABCDEF0123ULL;

    TripleDES tripledes(key1, key2, key3);

    EXPECT_TRUE(tripledes.hasKey());
    EXPECT_EQ(tripledes.key1(), key1);
    EXPECT_EQ(tripledes.key2(), key2);
    EXPECT_EQ(tripledes.key3(), key3);
}

TEST(TripleDESTest, EncryptChainsThreeDESInstancesInEDEOrder) {
    const std::uint64_t key1 = 0x0123456789ABCDEFULL;
    const std::uint64_t key2 = 0x23456789ABCDEF01ULL;
    const std::uint64_t key3 = 0x456789ABCDEF0123ULL;
    const std::uint64_t plaintext = 0x0123456789ABCDEFULL;

    TripleDES tripledes(key1, key2, key3);

    const std::uint64_t expected = 0xF2AFD84EE809E2B5ULL;

    EXPECT_EQ(tripledes.encrypt(plaintext), expected);
    EXPECT_EQ(tripledes.decrypt(expected), plaintext);
}

TEST(TripleDESTest, SnakeCaseAliasesMatchCamelCaseMethods) {
    const std::uint64_t key1 = 0x0123456789ABCDEFULL;
    const std::uint64_t key2 = 0x23456789ABCDEF01ULL;
    const std::uint64_t key3 = 0x456789ABCDEF0123ULL;

    TripleDES tripledes;
    tripledes.set_key(key1, key2, key3);

    EXPECT_EQ(tripledes.key1(), key1);
    EXPECT_EQ(tripledes.key2(), key2);
    EXPECT_EQ(tripledes.key3(), key3);
}

TEST(TripleDESTest, StreamRoundTripPreservesBytesFromTestDirectorySampleFile) {
    const std::string message = readFile(std::string(CREATE3DES_TEST_DATA_DIR) + "/sample.txt");
    ASSERT_EQ(message, "Hello World!\n");

    std::stringstream plaintext_input(message);
    TripleDES tripledes(0x0123456789ABCDEFULL, 0x23456789ABCDEF01ULL, 0x456789ABCDEF0123ULL);

    auto encrypted_blocks = tripledes.encrypt(plaintext_input);

    std::string encrypted_bytes = blocksToString(encrypted_blocks, encrypted_blocks.size() * 8);
    std::stringstream encrypted_input(encrypted_bytes);
    auto decrypted_blocks = tripledes.decrypt(encrypted_input);

    EXPECT_EQ(blocksToString(decrypted_blocks, message.size()), message);
}

TEST(TripleDESTest, EmptyStreamProducesNoBlocks) {
    std::stringstream input;
    TripleDES tripledes(0x133457799BBCDFF1ULL);

    EXPECT_TRUE(tripledes.encrypt(input).empty());
}

TEST(TripleDESTest, ValidParityCanBeCheckedForOneOrThreeKeys) {
    EXPECT_TRUE(TripleDES::hasValidParity(0x133457799BBCDFF1ULL));
    EXPECT_TRUE(TripleDES::hasValidParity(
        0x0123456789ABCDEFULL,
        0x23456789ABCDEF01ULL,
        0x456789ABCDEF0123ULL));
    EXPECT_FALSE(TripleDES::hasValidParity(0x133457799BBCDFF0ULL));
    EXPECT_FALSE(TripleDES::hasValidParity(
        0x0123456789ABCDEFULL,
        0x23456789ABCDEF01ULL,
        0x456789ABCDEF0122ULL));
}

