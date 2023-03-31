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

#include <sstream>

#include <pd/core/frame/DataFrame/DataFrame.hpp>
#include <pd/core/frame/DataFrame/DataFrameStreamIo.hpp>
#include <pd/core/series/Series/SeriesStreamIo.hpp>
#include <pd/read_csv.hpp>

#include <PdTest.hpp>

using namespace pd;

class DataFrameTest : public PdTest {
protected:
};

TEST_F(DataFrameTest, constructTest) {
    np::float_ array[5][2] = {{6.0, 148.0}, {1.0, 85.0}, {8.0, 183.0}, {1.0, 89.0}, {0.0, 137.0}};
    DataFrame df{np::Array<np::float_>{array}};

    np::Shape shape{5, 2};
    EXPECT_EQ(df.shape(), shape);
    EXPECT_EQ(df.size(), 10);
    std::vector<internal::Value> columns{0, 1};
    EXPECT_EQ(df.columns().getIndex(), columns);
    std::vector<internal::Value> rows{0, 1, 2, 3, 4};
    EXPECT_EQ(df.index().getIndex(), rows);
}

TEST_F(DataFrameTest, outputTest) {
    np::float_ array[5][2] = {{6.0, 148.0}, {1.0, 85.0}, {8.0, 183.0}, {1.0, 89.0}, {0.0, 137.0}};
    DataFrame df{np::Array<np::float_>{array}};

    std::stringstream str;
    str << df;

    static const char *kResult = "\t0\t1\n0\t6\t148\n1\t1\t85\n2\t8\t183\n3\t1\t89\n4\t0\t137\n5 rows x 2 columns\n";
    EXPECT_EQ(kResult, str.str());
}

TEST_F(DataFrameTest, iloc1Test) {
    np::float_ array[5][2] = {{6.0, 148.0}, {1.0, 85.0}, {8.0, 183.0}, {1.0, 89.0}, {0.0, 137.0}};
    DataFrame df{np::Array<np::float_>{array}};
    auto iLocSeriesResult = df.iloc(0);
    auto iLocSeriesSample = Series{np::Array<np::float_>{6.0, 148.0}, 0};
    EXPECT_EQ(iLocSeriesResult, iLocSeriesSample);
}

TEST_F(DataFrameTest, iloc2Test) {
    np::float_ array[5][2] = {{6.0, 148.0}, {1.0, 85.0}, {8.0, 183.0}, {1.0, 89.0}, {0.0, 137.0}};
    DataFrame df{np::Array<np::float_>{array}};
    const np::float_ *result = df.iloc(0, 1);
    EXPECT_EQ(*result, 148);
}
