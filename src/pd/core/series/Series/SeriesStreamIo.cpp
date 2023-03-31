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

#include <pd/core/series/Series/Series.hpp>
#include <pd/core/series/Series/SeriesStreamIo.hpp>

namespace pd {
    static const constexpr np::Size kMaxFullRows = 60;
    static const constexpr np::Size kFragmentRows = 5;

    static std::ostream &outputValueAtRow(std::ostream &stream, const Series &series, np::Size row) {
        stream << series.index()[row] << " ";

        const internal::Value &value = series.at(row);
        stream << value;
        stream << std::endl;
        return stream;
    }

    std::ostream &operator<<(std::ostream &stream, const Series &series) {
        /*
        0      148.0
        1       85.0
        2      183.0
        3       89.0
        4      137.0
        ...
        763    101.0
        764    122.0
        765    121.0
        766    126.0
        767     93.0
        Name: Glucose, Length: 768, dtype: float64
         */
        np::Size rows = series.size();
        if (rows > kMaxFullRows) {
            for (np::Size row = 0; row < kFragmentRows; ++row) {
                outputValueAtRow(stream, series, row);
            }
            stream << "...";
            stream << std::endl;
            for (np::Size row = rows - kFragmentRows; row < rows; ++row) {
                outputValueAtRow(stream, series, row);
            }
        } else {
            for (np::Size row = 0; row < rows; ++row) {
                outputValueAtRow(stream, series, row);
            }
        }
        auto name = series.name();
        if (name.empty()) {
            name = internal::Value{"None"};
        }
        stream << "Name: " << name << ", Length: " << series.size() << ", dtype: " << series.dtype() << std::endl;
        return stream;
    }

}// namespace pd
