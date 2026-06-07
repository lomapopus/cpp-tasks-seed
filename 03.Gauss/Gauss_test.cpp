#include <gtest/gtest.h>

#include <random>
#include <stdexcept>

#include "Gauss_solve.h"

namespace
{

void expect_vector_near(const GaussVector &actual, const GaussVector &expected, double eps = 1e-9)
{
    ASSERT_EQ(actual.size(), expected.size());
    for (Eigen::Index i = 0; i < actual.size(); ++i)
    {
        EXPECT_NEAR(actual(i), expected(i), eps);
    }
}

} // namespace

TEST(GaussSolve, SmallSolve)
{
    GaussMatrix ab(3, 4);
    ab << 2.0, 1.0, -1.0, 8.0,
    -3.0, -1.0, 2.0, -11.0,
    -2.0, 1.0, 2.0, -3.0;

    const GaussVector expected((GaussVector(3) << 2.0, 3.0, -1.0).finished());
    const GaussVector actual = Gauss_solve(ab);

    expect_vector_near(actual, expected);
}

TEST(GaussSolve, ReproducibleRandomSolve)
{
    constexpr Eigen::Index n = 32;
    std::mt19937_64 rng(0xC0FFEEULL);
    std::uniform_real_distribution<double> dist(-5.0, 5.0);

    GaussMatrix a(n, n);
    GaussVector x_true(n);

    for (Eigen::Index i = 0; i < n; ++i)
    {
        x_true(i) = dist(rng);
        for (Eigen::Index j = 0; j < n; ++j)
        {
            a(i, j) = dist(rng);
        }
        a(i, i) += static_cast<double>(n);
    }

    GaussMatrix ab(n, n + 1);
    for (Eigen::Index i = 0; i < n; ++i)
    {
        for (Eigen::Index j = 0; j < n; ++j)
        {
            ab(i, j) = a(i, j);
        }
        double rhs = 0.0;
        for (Eigen::Index j = 0; j < n; ++j)
        {
            rhs += a(i, j) * x_true(j);
        }
        ab(i, n) = rhs;
    }

    const GaussVector actual = Gauss_solve(ab);
    expect_vector_near(actual, x_true, 1e-8);
}

TEST(GaussSolve, SingularMatrixThrows)
{
    GaussMatrix ab(2, 3);
    ab << 1.0, 2.0, 3.0,
    2.0, 4.0, 6.0;

    EXPECT_THROW(Gauss_solve(ab), std::runtime_error);
}
