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

TEST_F(DataFrameTest, initWithDynamicArrayTest) {
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

TEST_F(DataFrameTest, initWithStaticArrayTest) {
    np::float_ array[5][2] = {{6.0, 148.0}, {1.0, 85.0}, {8.0, 183.0}, {1.0, 89.0}, {0.0, 137.0}};
    DataFrame df{np::Array<np::float_, 10>{array}};

    np::Shape shape{5, 2};
    EXPECT_EQ(df.shape(), shape);
    EXPECT_EQ(df.size(), 10);
    std::vector<internal::Value> columns{0, 1};
    EXPECT_EQ(df.columns().getIndex(), columns);
    std::vector<internal::Value> rows{0, 1, 2, 3, 4};
    EXPECT_EQ(df.index().getIndex(), rows);
}

TEST_F(DataFrameTest, initEmptyTest) {
    np::Shape shape{5, 2};
    np::Array<np::Size> array{shape};
    pd::DataFrame df{array};
    for (np::Size row = 0; row < shape[0]; ++row) {
        for (np::Size column = 0; column < shape[1]; ++column) {
            df.set(row, column, row * column);
        }
    }
    EXPECT_EQ(df.shape(), shape);
    EXPECT_EQ(df.size(), 10);
    std::vector<internal::Value> columns{0, 1};
    EXPECT_EQ(df.columns().getIndex(), columns);
    std::vector<internal::Value> rows{0, 1, 2, 3, 4};
    EXPECT_EQ(df.index().getIndex(), rows);

    for (np::Size row = 0; row < shape[0]; ++row) {
        for (np::Size column = 0; column < shape[1]; ++column) {
            auto value = df[internal::Value{column}][row];
            EXPECT_EQ(row * column, static_cast<np::Size>(value));
        }
    }
}

TEST_F(DataFrameTest, outputTest) {
    np::float_ array[5][2] = {{6.0, 148.0}, {1.0, 85.0}, {8.0, 183.0}, {1.0, 89.0}, {0.0, 137.0}};
    DataFrame df{np::Array<np::float_>{array}};

    std::stringstream str;
    str << df;

    static const char *kResult = "\t0\t1\n0\t6\t148\n1\t1\t85\n2\t8\t183\n3\t1\t89\n4\t0\t137\n5 rows x 2 columns\n";
    EXPECT_EQ(kResult, str.str());
}

TEST_F(DataFrameTest, ilocRowEmptyDfTest) {
    DataFrame df{};
    EXPECT_THROW(df.iloc(0), std::runtime_error);
}

TEST_F(DataFrameTest, ilocRowTest) {
    np::float_ array[5][2] = {{6.0, 148.0}, {1.0, 85.0}, {8.0, 183.0}, {1.0, 89.0}, {0.0, 137.0}};
    DataFrame df{np::Array<np::float_>{array}};
    auto iLocSeriesResult = df.iloc(0);
    auto iLocSeriesSample = Series{np::Array<np::float_>{6.0, 148.0}, 0};
    EXPECT_EQ(iLocSeriesResult, iLocSeriesSample);
}

TEST_F(DataFrameTest, indexingTest) {
    np::float_ array[5][2] = {{6.0, 148.0}, {1.0, 85.0}, {8.0, 183.0}, {1.0, 89.0}, {0.0, 137.0}};
    DataFrame df{np::Array<np::float_>{array}};
    auto iLocSeriesResult = df[internal::Value{1}];
    auto iLocSeriesSample = Series{np::Array<np::float_>{148.0, 85.0, 183.0, 89.0, 137.0}, 1};
    EXPECT_EQ(iLocSeriesResult, iLocSeriesSample);
}

TEST_F(DataFrameTest, atTest) {
    np::float_ array[5][2] = {{6.0, 148.0}, {1.0, 85.0}, {8.0, 183.0}, {1.0, 89.0}, {0.0, 137.0}};
    DataFrame df{np::Array<np::float_>{array}};
    auto result = df.at(2, internal::Value{1});
    EXPECT_EQ(result, internal::Value{183.0});
}

TEST_F(DataFrameTest, setTest) {
    np::float_ array[5][2] = {{6.0, 148.0}, {1.0, 85.0}, {8.0, 183.0}, {1.0, 89.0}, {0.0, 137.0}};
    DataFrame df{np::Array<np::float_>{array}};
    df.set(2, internal::Value{1}, 184.0);
    np::float_ array_sample[5][2] = {{6.0, 148.0}, {1.0, 85.0}, {8.0, 184.0}, {1.0, 89.0}, {0.0, 137.0}};
    pd::DataFrame df_sample{np::Array<np::float_>{array_sample}};
    compare(df, df_sample);
}

TEST_F(DataFrameTest, indexingNamedTest) {
    DataFrame df{};
    np::float_ array0[5] = {6.0, 1.0, 8.0, 1.0, 0.0};
    df.append(Series{np::Array<np::float_>{array0}, internal::Value{"column0"}});
    np::float_ array1[5] = {148.0, 85.0, 183.0, 89.0, 137.0};
    df.append(Series{np::Array<np::float_>{array1}, internal::Value{"column1"}});
    {
        auto iLocSeriesResult = df[internal::Value{1}];
        auto iLocSeriesSample = Series{np::Array<np::float_>{148.0, 85.0, 183.0, 89.0, 137.0}, "column1"};
        EXPECT_EQ(iLocSeriesResult, iLocSeriesSample);
    }
    {
        auto iLocSeriesResult = df[internal::Value{"column1"}];
        auto iLocSeriesSample = Series{np::Array<np::float_>{148.0, 85.0, 183.0, 89.0, 137.0}, "column1"};
        EXPECT_EQ(iLocSeriesResult, iLocSeriesSample);
    }
}

TEST_F(DataFrameTest, atNamedTest) {
    DataFrame df{};
    np::float_ array0[5] = {6.0, 1.0, 8.0, 1.0, 0.0};
    df.append(Series{np::Array<np::float_>{array0}, internal::Value{"column0"}});
    np::float_ array1[5] = {148.0, 85.0, 183.0, 89.0, 137.0};
    df.append(Series{np::Array<np::float_>{array1}, internal::Value{"column1"}});
    {
        auto result = df.at(2, internal::Value{1});
        EXPECT_EQ(result, internal::Value{183.0});
    }
    {
        auto result = df.at(2, internal::Value{"column1"});
        EXPECT_EQ(result, internal::Value{183.0});
    }
}

TEST_F(DataFrameTest, setNamedTest) {
    {
        DataFrame df{};
        np::float_ array0[5] = {6.0, 1.0, 8.0, 1.0, 0.0};
        df.append(Series{np::Array<np::float_>{array0}, internal::Value{"column0"}});
        np::float_ array1[5] = {148.0, 85.0, 183.0, 89.0, 137.0};
        df.append(Series{np::Array<np::float_>{array1}, internal::Value{"column1"}});
        df.set(2, internal::Value{1}, 184.0);
        pd::DataFrame df_sample{};
        np::float_ array0_sample[5] = {6.0, 1.0, 8.0, 1.0, 0.0};
        df_sample.append(Series{np::Array<np::float_>{array0_sample}, internal::Value{"column0"}});
        np::float_ array1_sample[5] = {148.0, 85.0, 184.0, 89.0, 137.0};
        df_sample.append(Series{np::Array<np::float_>{array1_sample}, internal::Value{"column1"}});
        compare(df, df_sample);
    }
    {
        DataFrame df{};
        np::float_ array0[5] = {6.0, 1.0, 8.0, 1.0, 0.0};
        df.append(Series{np::Array<np::float_>{array0}, internal::Value{"column0"}});
        np::float_ array1[5] = {148.0, 85.0, 183.0, 89.0, 137.0};
        df.append(Series{np::Array<np::float_>{array1}, internal::Value{"column1"}});
        df.set(2, internal::Value{"column1"}, 184.0);
        pd::DataFrame df_sample{};
        np::float_ array0_sample[5] = {6.0, 1.0, 8.0, 1.0, 0.0};
        df_sample.append(Series{np::Array<np::float_>{array0_sample}, internal::Value{"column0"}});
        np::float_ array1_sample[5] = {148.0, 85.0, 184.0, 89.0, 137.0};
        df_sample.append(Series{np::Array<np::float_>{array1_sample}, internal::Value{"column1"}});
        compare(df, df_sample);
    }
}

TEST_F(DataFrameTest, ilocRowColumnEmptyDfTest) {
    DataFrame df{};
    EXPECT_THROW(df.iloc(0, 0), std::runtime_error);
}

TEST_F(DataFrameTest, ilocRowColumnTest) {
    np::float_ array[5][2] = {{6.0, 148.0}, {1.0, 85.0}, {8.0, 183.0}, {1.0, 89.0}, {0.0, 137.0}};
    DataFrame df{np::Array<np::float_>{array}};
    auto result = df.iloc(0, 1);
    EXPECT_EQ(result, internal::Value{148.0});
}

TEST_F(DataFrameTest, ilocRowsSlicingEmptyDfTest) {
    DataFrame df{};
    EXPECT_THROW(df.iloc("0:1"), std::runtime_error);
}

TEST_F(DataFrameTest, ilocRowsSlicingTest) {
    np::float_ array[5][2] = {{6.0, 148.0}, {1.0, 85.0}, {8.0, 183.0}, {1.0, 89.0}, {0.0, 137.0}};
    DataFrame df{np::Array<np::float_>{array}};
    auto result = df.iloc("1:3");
    np::float_ array_sample[2][2] = {{1.0, 85.0}, {8.0, 183.0}};
    pd::DataFrame df_sample{np::Array<np::float_>{array_sample}};
    compare(result, df_sample);
}

TEST_F(DataFrameTest, ilocRowsSlicingFromBeginningTest) {
    np::float_ array[5][2] = {{6.0, 148.0}, {1.0, 85.0}, {8.0, 183.0}, {1.0, 89.0}, {0.0, 137.0}};
    DataFrame df{np::Array<np::float_>{array}};
    auto result = df.iloc(":2");
    np::float_ array_sample[2][2] = {{6.0, 148.0}, {1.0, 85.0}};
    pd::DataFrame df_sample{np::Array<np::float_>{array_sample}};
    compare(result, df_sample);
}

TEST_F(DataFrameTest, ilocRowsSlicingTillEndTest) {
    np::float_ array[5][2] = {{6.0, 148.0}, {1.0, 85.0}, {8.0, 183.0}, {1.0, 89.0}, {0.0, 137.0}};
    DataFrame df{np::Array<np::float_>{array}};
    auto result = df.iloc("2:");
    np::float_ array_sample[3][2] = {{8.0, 183.0}, {1.0, 89.0}, {0.0, 137.0}};
    pd::DataFrame df_sample{np::Array<np::float_>{array_sample}};
    compare(result, df_sample);
}

TEST_F(DataFrameTest, ilocRowsColumnsSlicingEmptyDfTest) {
    DataFrame df{};
    EXPECT_THROW(df.iloc("0:1", "0:2"), std::runtime_error);
}

TEST_F(DataFrameTest, ilocRowsColumnsSlicingTest) {
    np::float_ array[5][2] = {{6.0, 148.0}, {1.0, 85.0}, {8.0, 183.0}, {1.0, 89.0}, {0.0, 137.0}};
    DataFrame df{np::Array<np::float_>{array}};
    auto result = df.iloc("0:2", "0:1");
    np::float_ array_sample[2] = {6.0, 1.0};
    pd::DataFrame df_sample{np::Array<np::float_>{array_sample}};
    compare(result, df_sample);
}

TEST_F(DataFrameTest, ilocRowsColumnsSlicingNamedTest) {
    DataFrame df{};
    np::float_ array0[5] = {6.0, 1.0, 8.0, 1.0, 0.0};
    df.append(Series{np::Array<np::float_>{array0}, internal::Value{"column0"}});
    np::float_ array1[5] = {148.0, 85.0, 183.0, 89.0, 137.0};
    df.append(Series{np::Array<np::float_>{array1}, internal::Value{"column1"}});

    auto result = df.iloc("0:2", "0:1");

    pd::DataFrame df_sample{};
    np::float_ array0_sample[2] = {6.0, 1.0};
    df_sample.append(Series{np::Array<np::float_>{array0_sample}, internal::Value{"column0"}});
    compare(result, df_sample);
}

TEST_F(DataFrameTest, ilocAllRowsColumnsSlicingTest) {
    np::float_ array[5][2] = {{6.0, 148.0}, {1.0, 85.0}, {8.0, 183.0}, {1.0, 89.0}, {0.0, 137.0}};
    DataFrame df{np::Array<np::float_>{array}};
    auto result = df.iloc(":", "0:1");
    np::float_ array_sample[5] = {6.0, 1.0, 8.0, 1.0, 0.0};
    pd::DataFrame df_sample{np::Array<np::float_>{array_sample}};
    compare(result, df_sample);
}

TEST_F(DataFrameTest, ilocAllRowsColumnsSlicingNamedTest) {
    DataFrame df{};
    np::float_ array0[5] = {6.0, 1.0, 8.0, 1.0, 0.0};
    df.append(Series{np::Array<np::float_>{array0}, internal::Value{"column0"}});
    np::float_ array1[5] = {148.0, 85.0, 183.0, 89.0, 137.0};
    df.append(Series{np::Array<np::float_>{array1}, internal::Value{"column1"}});

    auto result = df.iloc(":", "0:1");

    pd::DataFrame df_sample{};
    np::float_ array0_sample[5] = {6.0, 1.0, 8.0, 1.0, 0.0};
    df_sample.append(Series{np::Array<np::float_>{array0_sample}, internal::Value{"column0"}});
    compare(result, df_sample);
}

TEST_F(DataFrameTest, ilocAllRowsColumnsIndexingTest) {
    np::float_ array[5][2] = {{6.0, 148.0}, {1.0, 85.0}, {8.0, 183.0}, {1.0, 89.0}, {0.0, 137.0}};
    DataFrame df{np::Array<np::float_>{array}};
    auto result = df.iloc(":", "1");
    np::float_ array_sample[5] = {148.0, 85.0, 183.0, 89.0, 137.0};
    DataFrame df_sample{};
    df_sample.append(Series{np::Array<np::float_>{array_sample}, 1});
    compare(result, df_sample);
}

TEST_F(DataFrameTest, ilocAllRowsColumnsIndexingNamedTest) {
    DataFrame df{};
    np::float_ array0[5] = {6.0, 1.0, 8.0, 1.0, 0.0};
    df.append(Series{np::Array<np::float_>{array0}, internal::Value{"column0"}});
    np::float_ array1[5] = {148.0, 85.0, 183.0, 89.0, 137.0};
    df.append(Series{np::Array<np::float_>{array1}, internal::Value{"column1"}});
    auto result = df.iloc(":", "1");
    np::float_ array_sample[5] = {148.0, 85.0, 183.0, 89.0, 137.0};
    DataFrame df_sample{};
    df_sample.append(Series{np::Array<np::float_>{array_sample}, internal::Value{"column1"}});
    compare(result, df_sample);
}

TEST_F(DataFrameTest, addVectorTest) {
    np::float_ array[4][3] = {{1.0, -1.0, 2.0}, {2.0, 0.0, 0.0}, {0.0, 1.0, -1.0}, {1.0, 1.0, -1.0}};
    DataFrame df{np::Array<np::float_>{array}};
    auto result = df.addVector<np::float_>(np::Array<np::float_>{1.0, 0.0, 3.5});
    np::float_ array_sample[4][3] = {{2.0, -1.0, 5.5}, {3.0, 0.0, 3.5}, {1.0, 1.0, 2.5}, {2.0, 1.0, 2.5}};
    pd::DataFrame df_sample{np::Array<np::float_>{array_sample}};
    compare(result, df_sample);
}

TEST_F(DataFrameTest, addTest) {
    np::float_ array[4][3] = {{1.0, -1.0, 2.0}, {2.0, 0.0, 0.0}, {0.0, 1.0, -1.0}, {1.0, 1.0, -1.0}};
    DataFrame df{np::Array<np::float_>{array}};
    auto result = df.add(DataFrame{np::Array<np::float_>{1.0, 0.0, 3.5}});
    np::float_ array_sample[4][3] = {{2.0, -1.0, 5.5}, {3.0, 0.0, 3.5}, {1.0, 1.0, 2.5}, {2.0, 1.0, 2.5}};
    pd::DataFrame df_sample{np::Array<np::float_>{array_sample}};
    compare(result, df_sample);
}

TEST_F(DataFrameTest, subtractVectorTest) {
    np::float_ array[4][3] = {{1.0, -1.0, 2.0}, {2.0, 0.0, 0.0}, {0.0, 1.0, -1.0}, {1.0, 1.0, -1.0}};
    DataFrame df{np::Array<np::float_>{array}};
    auto result = df.subtractVector<np::float_>(np::Array<np::float_>{1.0, 0.0, 3.5});
    np::float_ array_sample[4][3] = {{0.0, -1.0, -1.5}, {1.0, 0.0, -3.5}, {-1.0, 1.0, -4.5}, {0.0, 1.0, -4.5}};
    pd::DataFrame df_sample{np::Array<np::float_>{array_sample}};
    compare(result, df_sample);
}

TEST_F(DataFrameTest, subtractTest) {
    np::float_ array[4][3] = {{1.0, -1.0, 2.0}, {2.0, 0.0, 0.0}, {0.0, 1.0, -1.0}, {1.0, 1.0, -1.0}};
    DataFrame df{np::Array<np::float_>{array}};
    auto result = df.subtract(DataFrame{np::Array<np::float_>{1.0, 0.0, 3.5}});
    np::float_ array_sample[4][3] = {{0.0, -1.0, -1.5}, {1.0, 0.0, -3.5}, {-1.0, 1.0, -4.5}, {0.0, 1.0, -4.5}};
    pd::DataFrame df_sample{np::Array<np::float_>{array_sample}};
    compare(result, df_sample);
}

TEST_F(DataFrameTest, multiplyVectorTest) {
    np::float_ array[4][3] = {{1.0, -1.0, 2.0}, {2.0, 0.0, 0.0}, {0.0, 1.0, -1.0}, {1.0, 1.0, -1.0}};
    DataFrame df{np::Array<np::float_>{array}};
    auto result = df.multiplyVector<np::float_>(np::Array<np::float_>{1.0, 0.0, 3.5});
    np::float_ array_sample[4][3] = {{1.0, 0.0, 7.0}, {2.0, 0.0, 0.0}, {0.0, 0.0, -3.5}, {1.0, 0.0, -3.5}};
    pd::DataFrame df_sample{np::Array<np::float_>{array_sample}};
    compare(result, df_sample);
}

TEST_F(DataFrameTest, multiplyTest) {
    np::float_ array[4][3] = {{1.0, -1.0, 2.0}, {2.0, 0.0, 0.0}, {0.0, 1.0, -1.0}, {1.0, 1.0, -1.0}};
    DataFrame df{np::Array<np::float_>{array}};
    auto result = df.multiply(DataFrame{np::Array<np::float_>{1.0, 0.0, 3.5}});
    np::float_ array_sample[4][3] = {{1.0, 0.0, 7.0}, {2.0, 0.0, 0.0}, {0.0, 0.0, -3.5}, {1.0, 0.0, -3.5}};
    pd::DataFrame df_sample{np::Array<np::float_>{array_sample}};
    compare(result, df_sample);
}

TEST_F(DataFrameTest, divideVectorTest) {
    np::float_ array[4][3] = {{1.0, -1.0, 2.0}, {2.0, 0.0, 0.0}, {0.0, 1.0, -1.0}, {1.0, 1.0, -1.0}};
    DataFrame df{np::Array<np::float_>{array}};
    auto result = df.divideVector<np::float_>(np::Array<np::float_>{1.0, -1.0, 2.0});
    np::float_ array_sample[4][3] = {{1.0, 1.0, 1.0}, {2.0, 0.0, 0.0}, {0.0, -1, -0.5}, {1.0, -1, -0.5}};
    pd::DataFrame df_sample{np::Array<np::float_>{array_sample}};
    compare(result, df_sample);
}

TEST_F(DataFrameTest, divideTest) {
    np::float_ array[4][3] = {{1.0, -1.0, 2.0}, {2.0, 0.0, 0.0}, {0.0, 1.0, -1.0}, {1.0, 1.0, -1.0}};
    DataFrame df{np::Array<np::float_>{array}};
    auto result = df.divide(DataFrame{np::Array<np::float_>{1.0, -1.0, 2.0}});
    np::float_ array_sample[4][3] = {{1.0, 1.0, 1.0}, {2.0, 0.0, 0.0}, {0.0, -1, -0.5}, {1.0, -1, -0.5}};
    pd::DataFrame df_sample{np::Array<np::float_>{array_sample}};
    compare(result, df_sample);
}

TEST_F(DataFrameTest, dotTest) {
    np::float_ array1[1][3] = {{1.0, -1.0, 2.0}};
    DataFrame df1{np::Array<np::float_>{array1}};
    np::float_ array2[1][3] = {{1.0, 0.0, 3.5}};
    DataFrame df2{np::Array<np::float_>{array2}};
    auto result = df1.dot(df2);
    EXPECT_DOUBLE_EQ(static_cast<np::float_>(result), 8.0);
}
