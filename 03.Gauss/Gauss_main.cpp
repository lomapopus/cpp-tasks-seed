#include <iostream>
#include <stdexcept>

#include "Gauss_solve.h"
#include "util.h"

int main(int argc, const char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <input.csv>\n";
        return 1;
    }

    try
    {
        GaussMatrix ab = load_csv_to_matrix(argv[1]);
        GaussVector x = Gauss_solve(ab);

        GaussMatrix result(x.size(), 1);
        for (Eigen::Index i = 0; i < x.size(); ++i)
        {
            result(i, 0) = x(i);
        }
        print_matrix_as_csv(std::cout, result);
        return 0;
    }
    catch (const std::exception &ex)
    {
        std::cerr << ex.what() << '\n';
        return 1;
    }
}
