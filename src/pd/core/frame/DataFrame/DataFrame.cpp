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
#include <pd/core/internal/Indexing.hpp>

namespace pd {
    DataFrame::DataFrame()
        : m_shape{0, 0} {
    }

    DataFrame::DataFrame(const np::Array<np::int_> &data,
                         const DataFrameParameters &dataFrameParameters) {
        init<np::int_>(data, dataFrameParameters);
    }

    DataFrame::DataFrame(const np::Array<np::float_> &data,
                         const DataFrameParameters &dataFrameParameters) {
        init<np::float_>(data, dataFrameParameters);
    }

    DataFrame::DataFrame(const np::Array<np::string_> &data,
                         const DataFrameParameters &dataFrameParameters) {
        init<np::string_>(data, dataFrameParameters);
    }

    DataFrame::DataFrame(const np::Array<np::unicode_> &data,
                         const DataFrameParameters &dataFrameParameters) {
        init<np::unicode_>(data, dataFrameParameters);
    }

    np::Shape DataFrame::shape() const {
        return m_shape;
    }

    np::Size DataFrame::size() const {
        return m_index.size() * m_columns.size();
    }

    internal::Index DataFrame::index() const {
        return m_index;
    }

    void DataFrame::add(const DataFrame &df) {
        if ((m_shape[0] != 0 && m_shape[0] != df.shape()[0]) || (m_shape[1] != 0 && m_shape[1] != df.shape()[1])) {
            throw std::runtime_error("Added dataframe has different size from existing dataframe");
        }
        ++m_shape[1];
        m_shape[0] = df.shape()[0];
        for (const auto &column: df.columns().getIndex()) {
            m_columnData.emplace(column, df[column]);
        }
        m_columns.addIndex(df.m_columns);
        m_index.addIndex(df.m_index);
    }

    void DataFrame::add(const Series &series) {
        if (m_shape[0] != 0 && m_shape[0] != series.shape()[0]) {
            throw std::runtime_error("Added series has different size from existing dataframe");
        }
        ++m_shape[1];
        m_shape[0] = series.shape()[0];
        m_columnData.emplace(series.name(), series.values());
        m_columns.addIndex(series.name());
    }

    const Series &DataFrame::operator[](const internal::Value &columnName) const {
        return m_columnData.at(columnName);
    }

    Series &DataFrame::operator[](const internal::Value &columnName) {
        return m_columnData[columnName];
    }

    bool DataFrame::hasColumn(const internal::Value &column) const {
        return m_columns.has(column);
    }

    internal::Index DataFrame::columns() const {
        return m_columns;
    }

    internal::Value &DataFrame::at(np::Size row, const internal::Value &columnName) {
        return m_columnData.at(columnName).at(row);
    }

    const internal::Value &DataFrame::at(np::Size row, const internal::Value &columnName) const {
        return m_columnData.at(columnName).at(row);
    }

    DataFrame DataFrame::slicing1(const std::string &cond) const {
        auto shape{m_shape};
        auto layerSize = shape.empty() ? 0 : shape.calcSizeByShape() / shape[0];
        auto colonPos = cond.find(':');
        if (colonPos == std::string::npos) {
            np::Size i = std::stoi(cond);
            if (i >= layerSize) {
                throw std::runtime_error("Index " + std::to_string(i) + " out of bounds");
            }
            shape.removeFirstDim();
            DataFrame dataFrame{};
            for (const auto &columnName: m_columns.getIndex()) {
                const auto &value = at(i, columnName);
                if (value.isInt()) {
                    np::Array<np::int_> array{np::Shape{1}};
                    array.set(0, *static_cast<const np::int_ *>(value));
                    Series series{array, columnName};
                    dataFrame.add(series);
                } else if (value.isFloat()) {
                    np::Array<np::float_> array{np::Shape{1}};
                    array.set(0, *static_cast<const np::float_ *>(value));
                    Series series{array, columnName};
                    dataFrame.add(series);
                } else if (value.isString()) {
                    np::Array<np::string_> array{np::Shape{1}};
                    array.set(0, *static_cast<const np::string_ *>(value));
                    Series series{array, columnName};
                    dataFrame.add(series);
                } else if (value.isUnicode()) {
                    np::Array<np::unicode_> array{np::Shape{1}};
                    array.set(0, *static_cast<const np::unicode_ *>(value));
                    Series series{array, columnName};
                    dataFrame.add(series);
                } else {
                    throw std::runtime_error("Unknown type");
                }
            }
            return dataFrame;
        }

        auto first = cond.substr(0, colonPos);
        np::Size firstIndex = 0;
        try {
            firstIndex = std::stoul(first);
        } catch (std::invalid_argument const &) {
        } catch (std::out_of_range const &) {
        }
        auto last = cond.substr(colonPos + 1, cond.size() - colonPos - 1);
        np::Size lastIndex = shape.calcSizeByShape();
        try {
            lastIndex = std::stoul(last);
        } catch (std::invalid_argument const &) {
        } catch (std::out_of_range const &) {
        }
        np::Size length = lastIndex - firstIndex;
        if (length > shape.calcSizeByShape()) {
            throw std::runtime_error("Incorrect range");
        }

        DataFrame dataFrame{};
        for (const auto &columnName: m_columns.getIndex()) {
            const auto &value = at(firstIndex, columnName);
            if (value.isInt()) {
                np::Array<np::int_> array{np::Shape{lastIndex - firstIndex}};
                for (np::Size i = firstIndex + 1; i < lastIndex; ++i) {
                    const np::int_ *cell = at(i, columnName);
                    array.set(i, *cell);
                }
                Series series{array, columnName};
                dataFrame.add(series);
            } else if (value.isFloat()) {
                np::Array<np::float_> array{np::Shape{lastIndex - firstIndex}};
                for (np::Size i = firstIndex + 1; i < lastIndex; ++i) {
                    const np::float_ *cell = at(i, columnName);
                    array.set(i, *cell);
                }
                Series series{array, columnName};
                dataFrame.add(series);
            } else if (value.isString()) {
                np::Array<np::string_> array{np::Shape{lastIndex - firstIndex}};
                for (np::Size i = firstIndex + 1; i < lastIndex; ++i) {
                    const np::string_ *cell = at(i, columnName);
                    array.set(i, *cell);
                }
                Series series{array, columnName};
                dataFrame.add(series);
            } else if (value.isUnicode()) {
                np::Array<np::unicode_> array{np::Shape{lastIndex - firstIndex}};
                for (np::Size i = firstIndex + 1; i < lastIndex; ++i) {
                    const np::unicode_ *cell = at(i, columnName);
                    array.set(i, *cell);
                }
                Series series{array, columnName};
                dataFrame.add(series);
            } else {
                throw std::runtime_error("Unknown type");
            }
        }
        return dataFrame;
    }

    DataFrame DataFrame::slicing2(const std::string &, const std::string &) const {
        return DataFrame{};
    }

    DataFrame DataFrame::callable1(const std::string &) const {
        return DataFrame{};
    }

    DataFrame DataFrame::callable2(const std::string &, const std::string &) const {
        return DataFrame{};
    }

    Series DataFrame::iloc(np::Size i) const {
        if (m_shape.calcSizeByShape() == 0) {
            throw std::runtime_error("DataFrame is empty, cannot do iloc");
        }

        std::string dtype;
        {
            const auto &series = m_columnData.at(m_columns[0]);
            dtype = series.dtype();
        }
        bool homogeneous = false;
        for (const auto &column: m_columns.getIndex()) {
            if (m_columnData.at(column).dtype() != dtype) {
                homogeneous = true;
            }
        }
        if (homogeneous) {
            if (dtype == "float64") {
                np::Array<np::float_> array{np::Shape{m_columns.size()}};
                np::Size index = 0;
                for (const auto &column: m_columns.getIndex()) {
                    const np::float_ *value = at(i, column);
                    array.set(index++, *value);
                }
                return Series{array, internal::Value{std::to_string(i)}};
            } else if (dtype == "int64") {
                np::Array<np::int_> array{np::Shape{m_columns.size()}};
                np::Size index = 0;
                for (const auto &column: m_columns.getIndex()) {
                    const np::int_ *value = at(i, column);
                    array.set(index++, *value);
                }
                return Series{array, internal::Value{std::to_string(i)}};
            } else if (dtype == "str") {
                np::Array<np::string_> array{np::Shape{m_columns.size()}};
                np::Size index = 0;
                for (const auto &column: m_columns.getIndex()) {
                    const np::string_ *value = at(i, column);
                    array.set(index++, *value);
                }
                return Series{array, internal::Value{std::to_string(i)}};
            } else if (dtype == "unicode") {
                np::Array<np::unicode_> array{np::Shape{m_columns.size()}};
                np::Size index = 0;
                for (const auto &column: m_columns.getIndex()) {
                    const np::unicode_ *value = at(i, column);
                    array.set(index++, *value);
                }
                return Series{array, internal::Value{std::to_string(i)}};
            } else {
                throw std::runtime_error("Invalid value type");
            }
        } else {
            dtype.clear();
            for (const auto &column: m_columns.getIndex()) {
                const auto &series = m_columnData.at(column);
                if (series.dtype() == "unicode") {
                    dtype = "unicode";
                }
            }
            if (dtype.empty()) {
                for (const auto &column: m_columns.getIndex()) {
                    const auto &series = m_columnData.at(column);
                    if (series.dtype() == "str") {
                        dtype = "str";
                    }
                }
            }
            if (dtype.empty()) {
                for (const auto &column: m_columns.getIndex()) {
                    const auto &series = m_columnData.at(column);
                    if (series.dtype() == "float64") {
                        dtype = "float64";
                    }
                }
            }
            if (dtype.empty()) {
                dtype = "int64";
            }
            if (dtype == "float64") {
                np::Array<np::float_> array{np::Shape{m_columns.size()}};
                np::Size index = 0;
                for (const auto &column: m_columns.getIndex()) {
                    const np::float_ *value = at(i, column);
                    array.set(index++, *value);
                }
                return Series{array, internal::Value(static_cast<np::int_>(i))};
            } else if (dtype == "int64") {
                np::Array<np::int_> array{np::Shape{m_columns.size()}};
                np::Size index = 0;
                for (const auto &column: m_columns.getIndex()) {
                    const np::int_ *value = at(i, column);
                    array.set(index++, *value);
                }
                return Series{array, internal::Value{static_cast<np::int_>(i)}};
            } else if (dtype == "str") {
                np::Array<np::string_> array{np::Shape{m_columns.size()}};
                np::Size index = 0;
                for (const auto &column: m_columns.getIndex()) {
                    const np::string_ *value = at(i, column);
                    array.set(index++, *value);
                }
                return Series{array, internal::Value{static_cast<np::int_>(i)}};
            } else if (dtype == "unicode") {
                np::Array<np::unicode_> array{np::Shape{m_columns.size()}};
                np::Size index = 0;
                for (const auto &column: m_columns.getIndex()) {
                    const np::unicode_ *value = at(i, column);
                    array.set(index++, *value);
                }
                return Series{array, internal::Value{static_cast<np::int_>(i)}};
            } else {
                throw std::runtime_error("Invalid value type");
            }
        }
    }

    internal::Value &DataFrame::iloc(np::Size i, np::Size j) {
        if (m_shape.calcSizeByShape() == 0) {
            throw std::runtime_error("DataFrame is empty, cannot do iloc");
        }
        return at(i, m_columns[j]);
    }

    const internal::Value &DataFrame::iloc(np::Size i, np::Size j) const {
        if (m_shape.calcSizeByShape() == 0) {
            throw std::runtime_error("DataFrame is empty, cannot do iloc");
        }
        return at(i, m_columns[j]);
    }

    DataFrame DataFrame::iloc(const std::string &i) const {
        static constexpr std::size_t kIndexingHandlersSize{static_cast<std::size_t>(np::ndarray::internal::IndexingMode::None)};

        const internal::IndexingHandler<internal::IndexingChecker1, internal::IndexingWorker1<DataFrame>> indexingHandlers[kIndexingHandlersSize] = {
                {internal::IndexingMode::Slicing,
                 internal::isSlicing1,
                 std::bind(&DataFrame::slicing1, this, std::placeholders::_1)},
                {internal::IndexingMode::Callable,
                 internal::isCallable1,
                 std::bind(&DataFrame::callable1, this, std::placeholders::_1)}};

        for (const auto &indexing: indexingHandlers) {
            if (indexing.checker(i)) {
                return indexing.worker(i);
            }
        }
        throw std::runtime_error("Invalid operator[] argument");
    }

    DataFrame DataFrame::iloc(const std::string &i, const std::string &j) const {
        static constexpr std::size_t kIndexingHandlersSize{static_cast<std::size_t>(np::ndarray::internal::IndexingMode::None)};

        const internal::IndexingHandler<internal::IndexingChecker2, internal::IndexingWorker2<DataFrame>> indexingHandlers[kIndexingHandlersSize] = {
                {internal::IndexingMode::Slicing,
                 internal::isSlicing2,
                 std::bind(&DataFrame::slicing2, this, std::placeholders::_1, std::placeholders::_2)},
                {internal::IndexingMode::Callable,
                 internal::isCallable2,
                 std::bind(&DataFrame::callable2, this, std::placeholders::_1, std::placeholders::_2)}};

        for (const auto &indexing: indexingHandlers) {
            if (indexing.checker(i, j)) {
                return indexing.worker(i, j);
            }
        }
        throw std::runtime_error("Invalid operator[] argument");
    }

}// namespace pd