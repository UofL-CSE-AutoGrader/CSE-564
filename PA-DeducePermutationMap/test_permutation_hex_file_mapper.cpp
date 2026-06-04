#include <PermutationHexFileMapper.hpp>

#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

namespace {

fs::path writeTempFile(const std::string& name, const std::string& contents) {
  fs::path p = fs::temp_directory_path() / name;
  std::ofstream out(p);
  if (!out) {
    throw std::runtime_error("failed to create temp file");
  }
  out << contents;
  return p;
}

static std::uint64_t parseHex64(const std::string& s) {
  std::string t = s;
  if (t.rfind("0x", 0) == 0 || t.rfind("0X", 0) == 0) {
    t.erase(0, 2);
  }
  std::size_t pos = 0;
  unsigned long long v = 0;
  try {
    v = std::stoull(t, &pos, 16);
  } catch (...) {
    throw std::invalid_argument("invalid hex token");
  }
  if (pos != t.size()) {
    throw std::invalid_argument("invalid hex token");
  }
  return static_cast<std::uint64_t>(v);
}

static std::string toHex16Upper(std::uint64_t v) {
  std::ostringstream oss;
  oss << std::uppercase << std::hex << std::setw(16) << std::setfill('0') << v;
  return oss.str();
}

static std::vector<int> readPermutationFile(const fs::path& p) {
  std::ifstream in(p);
  if (!in) {
    throw std::runtime_error("failed to open permutation file");
  }
  std::vector<int> perm;
  int x = 0;
  while (in >> x) {
    perm.push_back(x);
  }
  return perm;
}

static fs::path sourceFile(const std::string& rel) {
  return fs::path(PA_DEDUCEPERMUTATIONMAP_SOURCE_DIR) / rel;
}

std::string readAll(const fs::path& p) {
  std::ifstream in(p);
  std::string s((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
  return s;
}

PermutationHexFileMapper::Permutation identityPermutation() {
  PermutationHexFileMapper::Permutation p;
  p.reserve(64);
  for (int i = 1; i <= 64; ++i) {
    p.push_back(i);
  }
  return p;
}

}

TEST(PermutationHexFileMapper, IdentityPermutationMapsValuesAndPreservesCommentBlankLines) {
  const std::string input =
      "0x0123456789abcdef\n"
      "FEDCBA9876543210\n";

  auto inPath = writeTempFile("perm_map_in.txt", input);
  auto outPath = fs::temp_directory_path() / "perm_map_out.txt";

  PermutationHexFileMapper mapper(identityPermutation());
  mapper.mapFile(inPath, outPath);

  const std::string expected =
      "0123456789ABCDEF\n"
      "FEDCBA9876543210\n";

  EXPECT_EQ(readAll(outPath), expected);
}

TEST(PermutationHexFileMapper, ThrowsOnBlankLine) {
  auto inPath = writeTempFile("perm_map_blank.txt", "0123456789abcdef\n\n");
  auto outPath = fs::temp_directory_path() / "perm_map_blank_out.txt";

  PermutationHexFileMapper mapper(identityPermutation());

  EXPECT_THROW(mapper.mapFile(inPath, outPath), std::invalid_argument);
}

TEST(PermutationHexFileMapper, ThrowsOnInvalidHexToken) {
  auto inPath = writeTempFile("perm_map_bad.txt", "ZZ\n");
  auto outPath = fs::temp_directory_path() / "perm_map_bad_out.txt";

  PermutationHexFileMapper mapper(identityPermutation());

  EXPECT_THROW(mapper.mapFile(inPath, outPath), std::invalid_argument);
}

TEST(PermutationHexFileMapper, OrderedPairsUnknown5000MatchComputedMap) {
  const auto permPath = sourceFile("examples/maps/unknown.txt");
  const auto pairsPath = sourceFile("tests/ordered_pairs_unknown_5000.txt");

  PermutationHexFileMapper mapper(readPermutationFile(permPath));

  std::ifstream in(pairsPath);
  ASSERT_TRUE(in) << "failed to open ordered pairs file";

  std::string a;
  std::string b;
  int count = 0;
  while (in >> a >> b) {
    const auto inV = parseHex64(a);
    const auto expected = b;
    const auto got = toHex16Upper(mapper.mapValue(inV));
    ASSERT_EQ(got, expected);
    ++count;
  }

  ASSERT_EQ(count, 5000);
}








