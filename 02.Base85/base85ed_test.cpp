#include <gtest/gtest.h>

#include <vector>
#include <cstdint>
#include <cstring>
#include <string>
#include <utility>

#include "base85ed.h"

using Bytes = std::vector<uint8_t>;

static Bytes cstr2v(const char *s)
{
    return Bytes(s, s + std::strlen(s));
}

static Bytes from_string(const std::string &s)
{
    return Bytes(s.begin(), s.end());
}

static std::string to_string(const Bytes &v)
{
    return std::string(v.begin(), v.end());
}

const std::vector<std::pair<std::string, std::string>> short_cases =
{
    { "",      ""      },
    { "F#",    "1"     },
    { "F){",   "12"    },
    { "F)}j",  "123"   },
    { "F)}kW", "1234"  },
};

const std::vector<std::pair<std::string, Bytes>> binary_cases =
{
    { "00",                   { 0x00 } },
    { "00000",                { 0x00, 0x00, 0x00, 0x00 } },
    { "0RI",                  { 0x00, 0xff } },
    { "{{",                   { 0xff } },
    { "|NZ",                  { 0xff, 0xfe } },
    { "|Ni{",                 { 0xff, 0xfe, 0xfd } },
    { "|Ni~_",                { 0xff, 0xfe, 0xfd, 0xfc } },
    { "|Ni~_`v",              { 0xff, 0xfe, 0xfd, 0xfc, 0xfb } },
    { "009C61O)~M2nh-c3=Iws", from_string(
            std::string("\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09"
                        "\x0a\x0b\x0c\x0d\x0e\x0f", 16)) },
    { "009C61O)~M2nh-c3=Iws5C", from_string(
            std::string("\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09"
                        "\x0a\x0b\x0c\x0d\x0e\x0f\x10", 17)) },
    { "s;a80s;a80s;a80s;a80", Bytes(16, 0xaa) },
};

const std::vector<std::pair<std::string, std::string>> text_cases =
{
    { "F)}kWH2", "12345" },
    { "Xk~0{Zv", "hello" },
};

static Bytes iota_bytes(size_t n)
{
    Bytes v(n);
    for (size_t i = 0; i < n; ++i)
    {
        v[i] = static_cast<uint8_t>(i & 0xFF);
    }
    return v;
}

static Bytes bytes_of_size(size_t n, uint8_t fill = 0)
{
    return Bytes(n, fill);
}

TEST(Base85ShortsEncode, TrivialShortEncodes)
{
    for (const auto &p : short_cases)
    {
        EXPECT_EQ(to_string(base85::encode(cstr2v(p.second.c_str()))), p.first);
    }
}

TEST(Base85ShortsDecode, TrivialShortDecodes)
{
    for (const auto &p : short_cases)
    {
        EXPECT_EQ(base85::decode(from_string(p.first)), cstr2v(p.second.c_str()));
    }
}

TEST(Base85TextEncode, KnownTextEncodes)
{
    for (const auto &p : text_cases)
    {
        EXPECT_EQ(to_string(base85::encode(cstr2v(p.second.c_str()))), p.first);
    }
}

TEST(Base85TextDecode, KnownTextDecodes)
{
    for (const auto &p : text_cases)
    {
        EXPECT_EQ(base85::decode(from_string(p.first)), cstr2v(p.second.c_str()));
    }
}

TEST(Base85BinaryEncode, KnownBinaryEncodes)
{
    for (const auto &p : binary_cases)
    {
        EXPECT_EQ(to_string(base85::encode(p.second)), p.first);
    }
}

TEST(Base85BinaryDecode, KnownBinaryDecodes)
{
    for (const auto &p : binary_cases)
    {
        EXPECT_EQ(base85::decode(from_string(p.first)), p.second);
    }
}

TEST(Base85Roundtrip, EncodeDecodeVariousLengths)
{
    for (size_t len = 0; len <= 64; ++len)
    {
        const auto original = iota_bytes(len);
        EXPECT_EQ(base85::decode(base85::encode(original)), original) << "len=" << len;
    }
}

TEST(Base85Roundtrip, EncodeDecodeConstantFill)
{
    for (size_t len : { 1, 2, 3, 4, 5, 8, 9, 15, 16, 31, 32, 63, 64, 100, 127, 128 })
    {
        for (uint8_t fill : { 0x00, 0x01, 0x7f, 0x80, 0xff })
        {
            const auto original = bytes_of_size(len, fill);
            EXPECT_EQ(base85::decode(base85::encode(original)), original)
                << "len=" << len << " fill=0x" << std::hex << static_cast<int>(fill);
        }
    }
}

TEST(Base85Roundtrip, EncodeDecodeAlternatingPattern)
{
    Bytes original(48);
    for (size_t i = 0; i < original.size(); ++i)
    {
        original[i] = static_cast<uint8_t>((i % 3 == 0) ? 0x00 : (i % 3 == 1) ? 0x55 : 0xAA);
    }
    EXPECT_EQ(base85::decode(base85::encode(original)), original);
}

TEST(Base85EncodedLength, MatchesPythonPaddingRule)
{
    for (size_t len = 0; len <= 32; ++len)
    {
        const auto encoded = base85::encode(iota_bytes(len));
        const size_t expected = len == 0 ? 0 : (len + 3) / 4 * 5 - (4 - len % 4) % 4;
        EXPECT_EQ(encoded.size(), expected) << "len=" << len;
    }
}

TEST(Base85Chained, DoubleEncodeDiffersFromSingle)
{
    const auto once = base85::encode(cstr2v("test"));
    const auto twice = base85::encode(once);
    EXPECT_NE(twice, once);
    EXPECT_EQ(base85::decode(base85::decode(twice)), cstr2v("test"));
}
