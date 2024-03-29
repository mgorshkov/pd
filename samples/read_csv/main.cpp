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

#include <iostream>

#include <pd/core/frame/DataFrame/DataFrame.hpp>
#include <pd/core/frame/DataFrame/DataFrameStreamIo.hpp>
#include <pd/read_csv.hpp>

int main(int, char **) {
    using namespace pd;

    auto df = read_csv("https://raw.githubusercontent.com/adityakumar529/Coursera_Capstone/master/diabetes.csv");
    std::cout << "df.shape=" << df.shape() << std::endl;
    const char *non_zero[] = {"Glucose", "BloodPressure", "SkinThickness", "Insulin", "BMI"};
    for (const auto &column: non_zero) {
        df[column] = df[column].replace(0L, np::NaN);
        auto mean = df[column].mean(true);
        df[column] = df[column].replace(np::NaN, mean);
    }

    auto X = df.iloc(":", "0:8");
    auto y = df.iloc(":", "8");

    std::cout << "X=" << X << std::endl;
    std::cout << "y=" << y << std::endl;

    return 0;
}
