#include "Gauss_solve.h"

#include <cmath>
#include <stdexcept>
#include <utility>

GaussVector Gauss_solve(GaussMatrix &ab)
{
    const Eigen::Index n = ab.rows();
    if (ab.cols() != n + 1)
    {
        throw std::invalid_argument("Gauss_solve expects an augmented matrix with one RHS column");
    }

    constexpr double pivot_eps = 1e-12;

    for (Eigen::Index k = 0; k < n; ++k)
    {
        Eigen::Index pivot_row = k;
        double max_abs = std::abs(ab(k, k));

        for (Eigen::Index i = k + 1; i < n; ++i)
        {
            const double candidate = std::abs(ab(i, k));
            if (candidate > max_abs)
            {
                max_abs = candidate;
                pivot_row = i;
            }
        }

        if (max_abs <= pivot_eps)
        {
            throw std::runtime_error("Singular matrix");
        }

        if (pivot_row != k)
        {
            for (Eigen::Index j = 0; j < ab.cols(); ++j)
            {
                std::swap(ab(k, j), ab(pivot_row, j));
            }
        }

        const double pivot = ab(k, k);
        for (Eigen::Index j = k; j < ab.cols(); ++j)
        {
            ab(k, j) /= pivot;
        }

        for (Eigen::Index i = k + 1; i < n; ++i)
        {
            const double factor = ab(i, k);
            if (std::abs(factor) <= pivot_eps)
            {
                ab(i, k) = 0.0;
                continue;
            }

            for (Eigen::Index j = k; j < ab.cols(); ++j)
            {
                ab(i, j) -= factor * ab(k, j);
            }
            ab(i, k) = 0.0;
        }
    }

    GaussVector x(n);
    for (Eigen::Index i = n; i-- > 0; )
    {
        double rhs = ab(i, n);
        for (Eigen::Index j = i + 1; j < n; ++j)
        {
            rhs -= ab(i, j) * x(j);
        }
        x(i) = rhs;
    }

    return x;
}
