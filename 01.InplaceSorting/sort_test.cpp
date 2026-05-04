#include <gtest/gtest.h>

#include <algorithm>
#include <vector>

#include "sorting.h"

namespace
{

using SortFunction = void (*)(std::vector<int>::iterator, std::vector<int>::iterator);

void check_sort(SortFunction sort)
{
    std::vector<std::vector<int>> cases = {
        {},
        {1},
        {2, 1},
        {1, 2, 3, 4, 5},
        {5, 4, 3, 2, 1},
        {4, 2, 5, 2, 1, 4, 3},
        {0, -3, 7, -3, 2, 0, 9, 1},
    };

    for (auto data : cases)
    {
        auto expected = data;
        std::sort(expected.begin(), expected.end());

        sort(data.begin(), data.end());

        EXPECT_EQ(expected, data);
        EXPECT_TRUE(std::is_sorted(data.begin(), data.end()));
    }
}

} // namespace

TEST(SortingSuite, BubbleSortWorks)
{
    check_sort(bubble_sort<std::vector<int>::iterator>);
}

TEST(SortingSuite, QuickSortWorks)
{
    check_sort(quick_sort<std::vector<int>::iterator>);
}
