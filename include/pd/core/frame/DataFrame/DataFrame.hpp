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

#pragma once

#include <string>
#include <unordered_map>

#include <pd/core/frame/DataFrame/DataFrameParameters.hpp>
#include <pd/core/internal/Index.hpp>
#include <pd/core/internal/Indexing.hpp>
#include <pd/core/series/Series/Series.hpp>

namespace pd {
    class DataFrame {
    public:
        DataFrame();

        DataFrame(const np::Array<np::int_> &data,
                  const DataFrameParameters &parameters = DataFrameParameters{});

        DataFrame(const np::Array<np::float_> &data,
                  const DataFrameParameters &parameters = DataFrameParameters{});

        DataFrame(const np::Array<np::string_> &data,
                  const DataFrameParameters &parameters = DataFrameParameters{});

        DataFrame(const np::Array<np::unicode_> &data,
                  const DataFrameParameters &parameters = DataFrameParameters{});

        DataFrame(const DataFrame &) = default;
        DataFrame(DataFrame &&) = default;

        [[nodiscard]] np::Shape shape() const;
        [[nodiscard]] np::Size size() const;
        [[nodiscard]] internal::Index index() const;

        void add(const DataFrame &df);
        void add(const Series &series);

        const Series &operator[](const internal::Value &columnName) const;
        Series &operator[](const internal::Value &columnName);

        [[nodiscard]] bool hasColumn(const internal::Value &column) const;
        [[nodiscard]] internal::Index columns() const;

        internal::Value &at(np::Size row, const internal::Value &columnName);
        [[nodiscard]] const internal::Value &at(np::Size row, const internal::Value &columnName) const;

        [[nodiscard]] Series iloc(np::Size i) const;
        internal::Value &iloc(np::Size i, np::Size j);
        [[nodiscard]] const internal::Value &iloc(np::Size i, np::Size j) const;
        [[nodiscard]] DataFrame iloc(const std::string &i) const;
        [[nodiscard]] DataFrame iloc(const std::string &i, const std::string &j) const;

    private:
        template<typename DType>
        void init(const np::Array<DType> &data, const DataFrameParameters &dataFrameParameters = DataFrameParameters{}) {
            m_index = internal::Index{dataFrameParameters.index, data.shape()[0]};
            m_columns = internal::Index{dataFrameParameters.columns, data.shape()[1]};
            m_shape = data.shape();

            for (np::Size i = 0; i < data.shape()[1]; ++i) {
                auto column = m_columns[i];
                np::Array<DType> array{np::Shape{data.shape()[0]}};
                for (np::Size j = 0; j < data.shape()[0]; ++j) {
                    array.set(j, data.get(j * data.shape()[1] + i));
                }
                m_columnData[column] = Series{array, column};
            }
        }

        [[nodiscard]] DataFrame slicing1(const std::string &i) const;
        [[nodiscard]] DataFrame slicing2(const std::string &i, const std::string &j) const;
        [[nodiscard]] DataFrame callable1(const std::string &i) const;
        [[nodiscard]] DataFrame callable2(const std::string &i, const std::string &j) const;

        std::unordered_map<internal::Value, Series, std::hash<pd::internal::Value>> m_columnData;
        internal::Index m_index;
        internal::Index m_columns;
        np::Shape m_shape;
    };
}// namespace pd