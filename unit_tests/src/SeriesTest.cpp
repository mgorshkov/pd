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

#include <np/Constants.hpp>

#include <pd/core/series/Series/Series.hpp>
#include <pd/core/series/Series/SeriesStreamIo.hpp>

#include <PdTest.hpp>

using namespace pd;

class SeriesTest : public PdTest {
protected:
};

TEST_F(SeriesTest, constructTest) {
    Series glucose{np::Array<np::float_>{148.0, 85.0, 183.0, 89.0, 137.0, 116.0, 78.0}, "Glucose"};

    EXPECT_EQ(glucose.shape(), np::Shape{7});
    EXPECT_EQ(glucose.size(), 7);
    EXPECT_EQ(glucose.dtype(), "float64");
    EXPECT_EQ(static_cast<std::string>(glucose.name()), "Glucose");
}

TEST_F(SeriesTest, outputTest) {
    Series glucose{np::Array<np::float_>{148.0, 85.0, 183.0, 89.0, 137.0, 116.0, 78.0}, "Glucose"};
    std::stringstream str;
    str << glucose;

    static const char *kResult = R"(0 148
1 85
2 183
3 89
4 137
5 116
6 78
Name: Glucose, Length: 7, dtype: float64
)";
    EXPECT_EQ(kResult, str.str());
}

TEST_F(SeriesTest, ilocTest) {
    Series pregnancies{np::Array<np::float_>{6, 1, 8, 1, 0, 5, 3}, "Pregnancies"};
    {
        auto result = pregnancies.iloc(0);
        EXPECT_EQ(result, internal::Value{6.0});
    }
    {
        auto result = pregnancies.iloc(6);
        EXPECT_EQ(result, internal::Value{3.0});
    }

    auto iLocSeries = pregnancies.iloc(":3");
    Series iLocSeriesResult{np::Array<np::float_>{6.0, 1.0, 8.0}, "Pregnancies"};
    EXPECT_EQ(iLocSeries, iLocSeriesResult);
}

TEST_F(SeriesTest, replaceTest) {
    Series pregnancies{np::Array<np::float_>{148.0, 85.0, 183.0, 0.0, 137.0, 116.0, 78.0}, "Pregnancies"};
    auto pregnanciesReplacedResult = pregnancies.replace(0, np::NaN);

    Series pregnanciesReplacedSample{np::Array<np::float_>{148.0, 85.0, 183.0, np::NaN, 137.0, 116.0, 78.0}, "Pregnancies"};
    EXPECT_EQ(pregnanciesReplacedResult, pregnanciesReplacedSample);
}
