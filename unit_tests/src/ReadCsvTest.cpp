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

#include <pd/read_csv.hpp>

#include <PdTest.hpp>

using namespace pd;

class ReadCsvTest : public PdTest {
protected:
    static void checkDataFrame(const DataFrame &df, pd::internal::Value columns[]) {
        np::Shape shape{768, 9};
        EXPECT_EQ(df.shape(), shape);

        np::Shape seriesShape{768};
        EXPECT_EQ(df[columns[0]].dtype(), "int64");
        EXPECT_EQ(df[columns[0]].shape(), seriesShape);
        EXPECT_EQ(df[columns[0]].size(), 768);
        EXPECT_EQ(df[columns[0]].name(), columns[0]);

        EXPECT_EQ(df[columns[1]].dtype(), "int64");
        EXPECT_EQ(df[columns[1]].shape(), seriesShape);
        EXPECT_EQ(df[columns[1]].size(), 768);
        EXPECT_EQ(df[columns[1]].name(), columns[1]);

        EXPECT_EQ(df[columns[2]].dtype(), "int64");
        EXPECT_EQ(df[columns[2]].shape(), seriesShape);
        EXPECT_EQ(df[columns[2]].size(), 768);
        EXPECT_EQ(df[columns[2]].name(), columns[2]);

        EXPECT_EQ(df[columns[3]].dtype(), "int64");
        EXPECT_EQ(df[columns[3]].shape(), seriesShape);
        EXPECT_EQ(df[columns[3]].size(), 768);
        EXPECT_EQ(df[columns[3]].name(), columns[3]);

        EXPECT_EQ(df[columns[4]].dtype(), "int64");
        EXPECT_EQ(df[columns[4]].shape(), seriesShape);
        EXPECT_EQ(df[columns[4]].size(), 768);
        EXPECT_EQ(df[columns[4]].name(), columns[4]);

        EXPECT_EQ(df[columns[5]].dtype(), "float64");
        EXPECT_EQ(df[columns[5]].shape(), seriesShape);
        EXPECT_EQ(df[columns[5]].size(), 768);
        EXPECT_EQ(df[columns[5]].name(), columns[5]);

        EXPECT_EQ(df[columns[6]].dtype(), "float64");
        EXPECT_EQ(df[columns[6]].shape(), seriesShape);
        EXPECT_EQ(df[columns[6]].size(), 768);
        EXPECT_EQ(df[columns[6]].name(), columns[6]);

        EXPECT_EQ(df[columns[7]].dtype(), "int64");
        EXPECT_EQ(df[columns[7]].shape(), seriesShape);
        EXPECT_EQ(df[columns[7]].size(), 768);
        EXPECT_EQ(df[columns[7]].name(), columns[7]);

        EXPECT_EQ(df[columns[8]].dtype(), "int64");
        EXPECT_EQ(df[columns[8]].shape(), seriesShape);
        EXPECT_EQ(df[columns[8]].size(), 768);
        EXPECT_EQ(df[columns[8]].name(), columns[8]);
    }
};

TEST_F(ReadCsvTest, readFromLocalFile) {
    auto df = read_csv(getTestFile("diabetes.csv").string());
    internal::Value columnNames[] = {"Pregnancies", "Glucose", "BloodPressure", "SkinThickness", "Insulin", "BMI", "DiabetesPedigreeFunction", "Age", "Outcome"};
    checkDataFrame(df, columnNames);
}

TEST_F(ReadCsvTest, readFromLocalFileNoHeader) {
    auto df = read_csv(getTestFile("diabetes_noheader.csv").string());
    internal::Value columnNames[] = {0, 1, 2, 3, 4, 5, 6, 7, 8};
    checkDataFrame(df, columnNames);
}

TEST_F(ReadCsvTest, readFromRemoteFile) {
    auto df = read_csv("https://raw.githubusercontent.com/adityakumar529/Coursera_Capstone/master/diabetes.csv");
    internal::Value columnNames[] = {"Pregnancies", "Glucose", "BloodPressure", "SkinThickness", "Insulin", "BMI", "DiabetesPedigreeFunction", "Age", "Outcome"};
    checkDataFrame(df, columnNames);
}
