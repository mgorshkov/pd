/*
Pandas library methods on top of NP library

Copyright (c) 2023 Mikhail Gorshkov (mikhail.gorshkov@gmail.com)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <pd/core/frame/DataFrame/DataFrame.hpp>
#include <pd/core/frame/DataFrame/DataFrameStreamIo.hpp>

namespace pd {
    static const constexpr np::Size kMaxFullRows = 60;
    static const constexpr np::Size kFragmentRows = 5;

    static std::ostream &outputValueAtRow(std::ostream &stream, const DataFrame &dataFrame, np::Size row) {
        stream << dataFrame.index()[row] << "\t";

        auto columns = dataFrame.columns().getIndex();
        for (np::Size i = 0; i < columns.size(); ++i) {
            const internal::Value &value = dataFrame.at(row, columns[i]);
            if (i > 0) {
                stream << '\t';
            }
            stream << value;
        }
        stream << std::endl;
        return stream;
    }

    std::ostream &operator<<(std::ostream &stream, const DataFrame &dataFrame) {
        /* Pregnancies  Glucose     BloodPressure
        0           6       148                72
        1           1       85                 66
        2           8       183                64
        3           1       89                 66
        4           0       137                40
        ...
        763         10      101                76
        764         2       122                70
        765         5       121                72
        766         1       126                60
        767         1       93                 70
        768 rows x 9 columns
         */
        np::Size rows = dataFrame.index().size();
        stream << '\t';
        auto columns = dataFrame.columns().getIndex();
        for (np::Size i = 0; i < columns.size(); ++i) {
            if (i > 0) {
                stream << '\t';
            }
            stream << columns[i];
        }
        stream << std::endl;
        if (rows > kMaxFullRows) {
            for (np::Size row = 0; row < kFragmentRows; ++row) {
                outputValueAtRow(stream, dataFrame, row);
            }
            stream << "...";
            stream << std::endl;
            for (np::Size row = rows - kFragmentRows; row < rows; ++row) {
                outputValueAtRow(stream, dataFrame, row);
            }
        } else {
            for (np::Size row = 0; row < rows; ++row) {
                outputValueAtRow(stream, dataFrame, row);
            }
        }
        stream << dataFrame.index().size() << " rows x " << dataFrame.columns().size() << " columns" << std::endl;
        return stream;
    }

}// namespace pd
