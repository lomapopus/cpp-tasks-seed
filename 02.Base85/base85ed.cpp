#include <vector>
#include <cstdint>
#include <unordered_map>
#include <stdexcept>

#include "base85ed.h"

namespace {

constexpr char B85_ALPHABET[] =
    "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz!#$%&()*+-;<=>?@^_`{|}~";

constexpr char B85_PAD_CHAR = B85_ALPHABET[84];

const std::unordered_map<uint8_t, int> &b85_decode_map()
{
    static const std::unordered_map<uint8_t, int> map = [] {
        std::unordered_map<uint8_t, int> m;
        for (int i = 0; i < 85; ++i)
        {
            m[static_cast<uint8_t>(B85_ALPHABET[i])] = i;
        }
        return m;
    }();
    return map;
}

int b85_value(uint8_t ch)
{
    const auto it = b85_decode_map().find(ch);
    if (it == b85_decode_map().end())
    {
        throw std::runtime_error("invalid base85 character");
    }
    return it->second;
}

} // namespace

std::vector<uint8_t> base85::encode(std::vector<uint8_t> const &bytes)
{
    std::vector<uint8_t> result;
    const size_t len = bytes.size();
    const size_t padding = (4 - len % 4) % 4;
    const size_t num_words = (len + 3) / 4;

    for (size_t w = 0; w < num_words; ++w)
    {
        uint32_t word = 0;
        for (size_t j = 0; j < 4; ++j)
        {
            const size_t idx = w * 4 + j;
            const uint8_t b = (idx < len) ? bytes[idx] : 0;
            word = (word << 8) | b;
        }

        char chars[5];
        uint32_t v = word;
        for (int i = 4; i >= 0; --i)
        {
            chars[i] = B85_ALPHABET[v % 85];
            v /= 85;
        }

        size_t out_len = 5;
        if (w + 1 == num_words && padding != 0)
        {
            out_len = 5 - padding;
        }
        for (size_t i = 0; i < out_len; ++i)
        {
            result.push_back(static_cast<uint8_t>(chars[i]));
        }
    }
    return result;
}

std::vector<uint8_t> base85::decode(std::vector<uint8_t> const &b85str)
{
    const size_t len = b85str.size();
    if (len == 0)
    {
        return {};
    }

    const size_t padding = (5 - len % 5) % 5;
    std::vector<uint8_t> padded = b85str;
    padded.insert(padded.end(), padding, static_cast<uint8_t>(B85_PAD_CHAR));

    std::vector<uint8_t> result;
    result.reserve(padded.size() / 5 * 4);

    for (size_t i = 0; i < padded.size(); i += 5)
    {
        uint64_t acc = 0;
        for (size_t j = 0; j < 5; ++j)
        {
            acc = acc * 85 + static_cast<uint64_t>(b85_value(padded[i + j]));
        }
        for (int k = 3; k >= 0; --k)
        {
            result.push_back(static_cast<uint8_t>((acc >> (8 * k)) & 0xFF));
        }
    }

    if (padding != 0)
    {
        result.resize(result.size() - padding);
    }
    return result;
}
