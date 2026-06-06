#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <iomanip>

#include "collvalue.h"
#include "sorting.h"

using CollInt = CollectingValue<int>;

// Random data
std::vector<CollInt> generate_data(size_t n)
{
    std::vector<CollInt> data(n);
    std::iota(data.begin(), data.end(), CollInt(0));
    return data;
}

void shuffle_data(std::vector<CollInt>& data)
{
    std::shuffle(data.begin(), data.end(), std::mt19937{std::random_device{}()});
}

bool check_sorted(const std::vector<CollInt>& data, const char* algo, size_t n)
{
    if (std::is_sorted(data.begin(), data.end()))
    {
        return true;
    }

    std::cerr << algo << " failed to sort array with N = " << n << "\n";
    return false;
}

int main()
{
    std::vector<size_t> sizes = {0, 1, 2, 10, 100, 500, 1000, 2000};

    std::cout << "N\tAlgo\t\tComps\t\tSwaps\t\tMoves\n";
    std::cout << "----------------------------------------------------------------\n";

    for (size_t n : sizes)
    {
        auto number_data = generate_data(n);

        // --- Std Sort ---
        shuffle_data(number_data);
        CollInt::reset_stats();
        std::sort(number_data.begin(), number_data.end());
        const auto std_comps = CollInt::comps;
        const auto std_swaps = CollInt::swaps;
        const auto std_moves = CollInt::moves;
        if (!check_sorted(number_data, "std::sort", n))
        {
            return 1;
        }
        std::cout << n << "\tstd::sort\t" << std_comps << "\t\t" << std_swaps << "\t\t" << std_moves << "\n";

        // --- Bubble Sort ---
        shuffle_data(number_data);
        CollInt::reset_stats();
        bubble_sort(number_data.begin(), number_data.end());
        const auto bubble_comps = CollInt::comps;
        const auto bubble_swaps = CollInt::swaps;
        const auto bubble_moves = CollInt::moves;
        if (!check_sorted(number_data, "Bubble", n))
        {
            return 1;
        }
        std::cout << n << "\tBubble\t\t" << bubble_comps << "\t\t" << bubble_swaps << "\t\t" << bubble_moves << "\n";

        // --- Quick Sort ---
        shuffle_data(number_data);
        CollInt::reset_stats();
        quick_sort(number_data.begin(), number_data.end());
        const auto quick_comps = CollInt::comps;
        const auto quick_swaps = CollInt::swaps;
        const auto quick_moves = CollInt::moves;
        if (!check_sorted(number_data, "Quick", n))
        {
            return 1;
        }
        std::cout << n << "\tQuick\t\t" << quick_comps << "\t\t" << quick_swaps << "\t\t" << quick_moves << "\n";

        std::cout << "--------------------------------------------------------------\n";
    }

    return 0;
}
