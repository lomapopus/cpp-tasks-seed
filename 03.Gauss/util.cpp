#include <iomanip>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "util.h"

GaussMatrix load_csv_to_matrix(const char *filename)
{
    std::vector<std::vector<double>> rows{};
    std::size_t width = 0;

    std::ifstream in(filename);
    if (!in)
    {
        throw std::runtime_error(std::string("Cannot open CSV file: ") + filename);
    }

    std::string line;
    bool have_data = false;
    while (std::getline(in, line))
    {
        if (line.empty())
        {
            continue;
        }

        std::vector<double> values{};
        std::stringstream ss(line);
        std::string cell;
        bool numeric_row = true;
        while (std::getline(ss, cell, ','))
        {
            try
            {
                values.push_back(std::stod(cell));
            }
            catch (const std::exception &)
            {
                numeric_row = false;
                break;
            }
        }

        if (!numeric_row)
        {
            if (have_data)
            {
                throw std::runtime_error("CSV contains a non-numeric data row");
            }
            continue;
        }

        if (values.empty())
        {
            continue;
        }

        if (width == 0)
        {
            width = values.size();
        }
        else if (values.size() != width)
        {
            throw std::runtime_error("CSV rows have different lengths");
        }

        rows.push_back(std::move(values));
        have_data = true;
    }

    if (rows.empty())
    {
        throw std::runtime_error("CSV contains no numeric data");
    }

    const Eigen::Index row_count = static_cast<Eigen::Index>(rows.size());
    const Eigen::Index col_count = static_cast<Eigen::Index>(width);
    GaussMatrix matrix(row_count, col_count);
    for (Eigen::Index i = 0; i < row_count; ++i)
    {
        for (Eigen::Index j = 0; j < col_count; ++j)
        {
            matrix(i, j) = rows[static_cast<std::size_t>(i)][static_cast<std::size_t>(j)];
        }
    }

    return matrix;
}

void print_matrix_as_csv(std::ostream& out, const GaussMatrix &matrix, int prec)
{
    for (Eigen::Index j = 0; j + 1 < matrix.cols(); ++j)
    {
        out << "A,";
    }
    out << "B\n";

    out << std::fixed << std::setprecision(prec);

    for (Eigen::Index i = 0; i < matrix.rows(); ++i)
    {
        for (Eigen::Index j = 0; j < matrix.cols(); ++j)
        {
            out << matrix(i, j);
            if (j < matrix.cols() - 1)
            {
                out << ',';
            }
        }
        out << '\n';
    }
}
