#ifndef SORTING_H
#define SORTING_H

// Header-only

#include <iterator>
#include <type_traits>
#include <utility>

// Для swap
#include "collvalue.h"

// ----------------------------- Пузырёк (например) --------------------------------------
template <typename Iterator>
void bubble_sort(Iterator begin, Iterator end)
{
    if (begin == end)
    {
        return;
    }

    for (Iterator last = end; last != begin;)
    {
        bool swapped = false;
        Iterator current = begin;
        Iterator next = current;
        ++next;

        while (next != last)
        {
            if (*next < *current)
            {
                using std::swap;
                swap(*current, *next);
                swapped = true;
            }

            ++current;
            ++next;
        }

        if (!swapped)
        {
            return;
        }

        --last;
    }
}

// --------------------------- QuickSort (например) ---------------------------------------
template <typename Iterator>
void quick_sort(Iterator begin, Iterator end)
{
    using Category = typename std::iterator_traits<Iterator>::iterator_category;
    static_assert(std::is_base_of_v<std::random_access_iterator_tag, Category>,
                  "quick_sort requires random access iterators");

    if (end - begin < 2)
    {
        return;
    }

    Iterator left = begin;
    Iterator right = end - 1;
    auto pivot = *(begin + (end - begin) / 2);

    while (left <= right)
    {
        while (*left < pivot)
        {
            ++left;
        }

        while (pivot < *right)
        {
            --right;
        }

        if (left <= right)
        {
            using std::swap;
            swap(*left, *right);
            ++left;

            if (right == begin)
            {
                break;
            }

            --right;
        }
    }

    if (begin < right + 1)
    {
        quick_sort(begin, right + 1);
    }

    if (left < end)
    {
        quick_sort(left, end);
    }
}

#endif // SORTING_H
