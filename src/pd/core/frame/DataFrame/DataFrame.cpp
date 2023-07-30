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
        : m_shape{} {
    }

    bool DataFrame::operator==(const DataFrame &another) const {
        return shape() == another.shape() && m_columnData == another.m_columnData;
    }

    bool DataFrame::empty() const {
        return m_shape.empty();
    }

    np::Size DataFrame::ndim() const {
        return m_shape.size();
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

    void DataFrame::append(const DataFrame &df) {
        if (!empty() && m_shape[0] != df.shape()[0]) {
            throw std::runtime_error("Added dataframe has different number of rows from existing dataframe");
        }

        if (empty()) {
            m_shape.addDim(df.shape()[0]);
        } else if (ndim() == 1) {
            m_shape.addDim(df.shape()[1] + 1);
        } else {
            m_shape[1] += df.shape()[1];
        }

        for (const auto &column: df.columns().getIndex()) {
            m_columnData.emplace(column, df[column]);
        }
        m_columns.addIndex(df.m_columns);
        m_index.addIndex(df.m_index);
    }

    void DataFrame::append(const Series &series) {
        if (!empty() && m_shape[0] != series.shape()[0]) {
            throw std::runtime_error("Added series has different number of rows from existing dataframe");
        }

        if (empty()) {
            m_shape.addDim(series.shape()[0]);
        } else if (ndim() == 1) {
            m_shape.addDim(2);
        } else {
            ++m_shape[1];
        }

        m_columnData.emplace(series.name(), series);
        m_columns.addIndex(series.name());
        if (m_index.empty()) {
            m_index.addIndex(internal::Index{series.shape()[0]});
        }
    }

    const Series &DataFrame::operator[](const internal::Value &column) const {
        return m_columnData.at(m_columns[column]);
    }

    Series &DataFrame::operator[](const internal::Value &column) {
        return m_columnData.at(m_columns[column]);
    }

    bool DataFrame::hasColumn(const internal::Value &column) const {
        return m_columns.has(column);
    }

    internal::Index DataFrame::columns() const {
        return m_columns;
    }

    DataFrame DataFrame::operator[](np::Size row) const {
        DataFrame dataFrame{};
        for (const auto &columnName: m_columns.getIndex()) {
            const auto &value = at(row, columnName);
            if (value.isBool()) {
                np::Array<np::bool_> array{np::Shape{1}};
                array.set(0, *static_cast<const np::bool_ *>(value));
                dataFrame.append(Series{array, columnName});
            } else if (value.isInt()) {
                np::Array<np::int_> array{np::Shape{1}};
                array.set(0, *static_cast<const np::int_ *>(value));
                dataFrame.append(Series{array, columnName});
            } else if (value.isIntC()) {
                np::Array<np::intc> array{np::Shape{1}};
                array.set(0, *static_cast<const np::intc *>(value));
                dataFrame.append(Series{array, columnName});
            } else if (value.isSize()) {
                np::Array<np::Size> array{np::Shape{1}};
                array.set(0, *static_cast<const np::Size *>(value));
                dataFrame.append(Series{array, columnName});
            } else if (value.isFloat()) {
                np::Array<np::float_> array{np::Shape{1}};
                array.set(0, *static_cast<const np::float_ *>(value));
                dataFrame.append(Series{array, columnName});
            } else if (value.isString()) {
                np::Array<np::string_> array{np::Shape{1}};
                array.set(0, *static_cast<const np::string_ *>(value));
                dataFrame.append(Series{array, columnName});
            } else if (value.isUnicode()) {
                np::Array<np::unicode_> array{np::Shape{1}};
                array.set(0, *static_cast<const np::unicode_ *>(value));
                dataFrame.append(Series{array, columnName});
            } else {
                throw std::runtime_error("Unknown type");
            }
        }
        return dataFrame;
    }

    internal::Value DataFrame::at(np::Size row, const internal::Value &column) const {
        return m_columnData.at(m_columns[column]).at(row);
    }

    void DataFrame::set(np::Size row, const internal::Value &column, const internal::Value &value) {
        m_columnData.at(m_columns[column]).set(row, value);
    }

    DataFrame DataFrame::slicing1(const std::string &rows) const {
        auto totalRows = m_shape[0];
        auto colonPos = rows.find(':');
        if (colonPos == std::string::npos) {
            np::Size i = std::stoi(rows);
            if (i >= totalRows) {
                throw std::runtime_error("Index " + std::to_string(i) + " out of bounds");
            }
            return operator[](i);
        }

        auto first = rows.substr(0, colonPos);
        np::Size firstIndex = 0;
        try {
            firstIndex = std::stoul(first);
        } catch (std::invalid_argument const &) {
        } catch (std::out_of_range const &) {
        }
        auto last = rows.substr(colonPos + 1, rows.size() - colonPos - 1);
        np::Size lastIndex = totalRows;
        try {
            lastIndex = std::stoul(last);
        } catch (std::invalid_argument const &) {
        } catch (std::out_of_range const &) {
        }
        np::Size length = lastIndex - firstIndex;
        if (length > totalRows) {
            throw std::runtime_error("Incorrect range");
        }

        DataFrame dataFrame{};
        for (const auto &columnName: m_columns.getIndex()) {
            auto value = at(firstIndex, columnName);
            if (value.isBool()) {
                np::Array<np::bool_> array{np::Shape{lastIndex - firstIndex}};
                for (np::Size i = 0; i < lastIndex - firstIndex; ++i) {
                    auto cell = static_cast<np::bool_>(at(i + firstIndex, columnName));
                    array.set(i, cell);
                }
                dataFrame.append(Series{array, columnName});
            } else if (value.isInt()) {
                np::Array<np::int_> array{np::Shape{lastIndex - firstIndex}};
                for (np::Size i = 0; i < lastIndex - firstIndex; ++i) {
                    auto cell = static_cast<np::int_>(at(i + firstIndex, columnName));
                    array.set(i, cell);
                }
                dataFrame.append(Series{array, columnName});
            } else if (value.isIntC()) {
                np::Array<np::intc> array{np::Shape{lastIndex - firstIndex}};
                for (np::Size i = 0; i < lastIndex - firstIndex; ++i) {
                    auto cell = static_cast<np::intc>(at(i + firstIndex, columnName));
                    array.set(i, cell);
                }
                dataFrame.append(Series{array, columnName});
            } else if (value.isFloat()) {
                np::Array<np::float_> array{np::Shape{lastIndex - firstIndex}};
                for (np::Size i = 0; i < lastIndex - firstIndex; ++i) {
                    auto cell = static_cast<np::float_>(at(i + firstIndex, columnName));
                    array.set(i, cell);
                }
                dataFrame.append(Series{array, columnName});
            } else if (value.isString()) {
                np::Array<np::string_> array{np::Shape{lastIndex - firstIndex}};
                for (np::Size i = 0; i < lastIndex - firstIndex; ++i) {
                    auto cell = static_cast<np::string_>(at(i + firstIndex, columnName));
                    array.set(i, cell);
                }
                dataFrame.append(Series{array, columnName});
            } else if (value.isUnicode()) {
                np::Array<np::unicode_> array{np::Shape{lastIndex - firstIndex}};
                for (np::Size i = 0; i < lastIndex - firstIndex; ++i) {
                    auto cell = static_cast<np::unicode_>(at(i + firstIndex, columnName));
                    array.set(i, cell);
                }
                dataFrame.append(Series{array, columnName});
            } else if (value.isSize()) {
                np::Array<np::Size> array{np::Shape{lastIndex - firstIndex}};
                for (np::Size i = 0; i < lastIndex - firstIndex; ++i) {
                    auto cell = static_cast<np::Size>(at(i + firstIndex, columnName));
                    array.set(i, cell);
                }
                dataFrame.append(Series{array, columnName});
            } else {
                throw std::runtime_error("Unknown type");
            }
        }
        return dataFrame;
    }

    DataFrame DataFrame::slicing2(const std::string &rows, const std::string &columns) const {
        auto totalRows = m_shape[0];
        auto totalColumns = m_shape[1];
        auto colonPos = rows.find(':');
        if (colonPos == std::string::npos) {
            np::Size i = std::stoi(rows);
            if (i >= totalRows) {
                throw std::runtime_error("Index " + std::to_string(i) + " out of bounds");
            }
            DataFrame dataFrame{};
            std::vector<internal::Value> columnIndex;
            colonPos = columns.find(':');
            if (colonPos == std::string::npos) {
                columnIndex.emplace_back(columns);
            } else {
                auto first = columns.substr(0, colonPos);
                np::Size firstIndex = 0;
                try {
                    firstIndex = std::stoul(first);
                } catch (std::invalid_argument const &) {
                } catch (std::out_of_range const &) {
                }
                auto last = columns.substr(colonPos + 1, columns.size() - colonPos - 1);
                np::Size lastIndex = totalColumns;
                try {
                    lastIndex = std::stoul(last);
                } catch (std::invalid_argument const &) {
                } catch (std::out_of_range const &) {
                }
                bool add = false;
                auto index = m_columns.getIndex();
                for (const auto &columnName: index) {
                    if (columnName == firstIndex || columnName == first) {
                        add = true;
                    } else if (columnName == lastIndex || columnName == last) {
                        break;
                    }
                    if (add) {
                        columnIndex.emplace_back(columnName);
                    }
                }
                if (columnIndex.empty()) {
                    for (i = firstIndex; i < lastIndex; ++i) {
                        columnIndex.emplace_back(index[i]);
                    }
                }
            }

            for (const auto &columnName: columnIndex) {
                auto value = at(i, columnName);
                if (value.isBool()) {
                    np::Array<np::bool_> array{np::Shape{1}};
                    array.set(0, *static_cast<const np::int_ *>(value));
                    dataFrame.append(Series{array, columnName});
                } else if (value.isInt()) {
                    np::Array<np::int_> array{np::Shape{1}};
                    array.set(0, *static_cast<const np::int_ *>(value));
                    dataFrame.append(Series{array, columnName});
                } else if (value.isFloat()) {
                    np::Array<np::float_> array{np::Shape{1}};
                    array.set(0, *static_cast<const np::float_ *>(value));
                    dataFrame.append(Series{array, columnName});
                } else if (value.isString()) {
                    np::Array<np::string_> array{np::Shape{1}};
                    array.set(0, *static_cast<const np::string_ *>(value));
                    dataFrame.append(Series{array, columnName});
                } else if (value.isUnicode()) {
                    np::Array<np::unicode_> array{np::Shape{1}};
                    array.set(0, *static_cast<const np::unicode_ *>(value));
                    dataFrame.append(Series{array, columnName});
                } else {
                    throw std::runtime_error("Unknown type");
                }
            }
            return dataFrame;
        }

        auto rowsFirst = rows.substr(0, colonPos);
        np::Size rowsFirstIndex = 0;
        try {
            rowsFirstIndex = std::stoul(rowsFirst);
        } catch (std::invalid_argument const &) {
        } catch (std::out_of_range const &) {
        }
        auto rowsLast = rows.substr(colonPos + 1, rows.size() - colonPos - 1);
        np::Size rowsLastIndex = totalRows;
        try {
            rowsLastIndex = std::stoul(rowsLast);
        } catch (std::invalid_argument const &) {
        } catch (std::out_of_range const &) {
        }
        np::Size length = rowsLastIndex - rowsFirstIndex;
        if (length > totalRows) {
            throw std::runtime_error("Incorrect range");
        }

        std::vector<internal::Value> columnIndex;
        colonPos = columns.find(':');
        if (colonPos == std::string::npos) {
            auto column = std::stol(columns);
            auto index = m_columns.getIndex();
            for (const auto &columnName: index) {
                if (columnName == column) {
                    columnIndex.emplace_back(columnName);
                    break;
                }
            }
            if (columnIndex.empty()) {
                columnIndex.emplace_back(index[column]);
            }
        } else {
            auto columnsFirst = columns.substr(0, colonPos);
            np::Size columnsFirstIndex = 0;
            try {
                columnsFirstIndex = std::stoul(columnsFirst);
            } catch (std::invalid_argument const &) {
            } catch (std::out_of_range const &) {
            }
            auto columnsLast = columns.substr(colonPos + 1, columns.size() - colonPos - 1);
            np::Size columnsLastIndex = totalColumns;
            try {
                columnsLastIndex = std::stoul(columnsLast);
            } catch (std::invalid_argument const &) {
            } catch (std::out_of_range const &) {
            }
            bool add = false;
            auto index = m_columns.getIndex();
            for (const auto &columnName: index) {
                if (columnName == columnsFirstIndex || columnName == columnsFirst) {
                    add = true;
                } else if (columnName == columnsLastIndex || columnName == columnsLast) {
                    break;
                }
                if (add) {
                    columnIndex.emplace_back(columnName);
                }
            }
            if (columnIndex.empty()) {
                for (np::Size i = columnsFirstIndex; i < columnsLastIndex; ++i) {
                    columnIndex.emplace_back(index[i]);
                }
            }
        }

        DataFrame dataFrame{};
        for (const auto &columnName: columnIndex) {
            auto value = at(rowsFirstIndex, columnName);
            if (value.isBool()) {
                np::Array<np::bool_> array{np::Shape{rowsLastIndex - rowsFirstIndex}};
                for (np::Size i = 0; i < rowsLastIndex - rowsFirstIndex; ++i) {
                    auto cell = static_cast<np::bool_>(at(rowsFirstIndex + i, columnName));
                    array.set(i, cell);
                }
                dataFrame.append(Series{array, columnName});
            } else if (value.isInt()) {
                np::Array<np::int_> array{np::Shape{rowsLastIndex - rowsFirstIndex}};
                for (np::Size i = 0; i < rowsLastIndex - rowsFirstIndex; ++i) {
                    auto cell = static_cast<np::int_>(at(rowsFirstIndex + i, columnName));
                    array.set(i, cell);
                }
                dataFrame.append(Series{array, columnName});
            } else if (value.isIntC()) {
                np::Array<np::intc> array{np::Shape{rowsLastIndex - rowsFirstIndex}};
                for (np::Size i = 0; i < rowsLastIndex - rowsFirstIndex; ++i) {
                    auto cell = static_cast<np::intc>(at(rowsFirstIndex + i, columnName));
                    array.set(i, cell);
                }
                dataFrame.append(Series{array, columnName});
            } else if (value.isFloat()) {
                np::Array<np::float_> array{np::Shape{rowsLastIndex - rowsFirstIndex}};
                for (np::Size i = 0; i < rowsLastIndex - rowsFirstIndex; ++i) {
                    auto cell = static_cast<np::float_>(at(rowsFirstIndex + i, columnName));
                    array.set(i, cell);
                }
                dataFrame.append(Series{array, columnName});
            } else if (value.isString()) {
                np::Array<np::string_> array{np::Shape{rowsLastIndex - rowsFirstIndex}};
                for (np::Size i = 0; i < rowsLastIndex - rowsFirstIndex; ++i) {
                    auto cell = static_cast<np::string_>(at(rowsFirstIndex + i, columnName));
                    array.set(i, cell);
                }
                dataFrame.append(Series{array, columnName});
            } else if (value.isUnicode()) {
                np::Array<np::unicode_> array{np::Shape{rowsLastIndex - rowsFirstIndex}};
                for (np::Size i = 0; i < rowsLastIndex - rowsFirstIndex; ++i) {
                    auto cell = static_cast<np::unicode_>(at(rowsFirstIndex + i, columnName));
                    array.set(i, cell);
                }
                dataFrame.append(Series{array, columnName});
            } else if (value.isSize()) {
                np::Array<np::Size> array{np::Shape{rowsLastIndex - rowsFirstIndex}};
                for (np::Size i = 0; i < rowsLastIndex - rowsFirstIndex; ++i) {
                    auto cell = static_cast<np::Size>(at(rowsFirstIndex + i, columnName));
                    array.set(i, cell);
                }
                dataFrame.append(Series{array, columnName});
            } else {
                throw std::runtime_error("Unknown type");
            }
        }
        return dataFrame;
    }

    DataFrame DataFrame::callable1(const std::string &) const {
        return DataFrame{};
    }

    DataFrame DataFrame::callable2(const std::string &, const std::string &) const {
        return DataFrame{};
    }

    Series DataFrame::iloc(np::Size row) const {
        if (empty()) {
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
            if (dtype == "bool") {
                np::Array<np::bool_> array{np::Shape{m_columns.size()}};
                np::Size index = 0;
                for (const auto &column: m_columns.getIndex()) {
                    auto value = static_cast<np::bool_>(at(row, column));
                    array.set(index++, value);
                }
                return Series{array, internal::Value{std::to_string(row)}};
            }
            if (dtype == "float64") {
                np::Array<np::float_> array{np::Shape{m_columns.size()}};
                np::Size index = 0;
                for (const auto &column: m_columns.getIndex()) {
                    auto value = static_cast<np::float_>(at(row, column));
                    array.set(index++, value);
                }
                return Series{array, internal::Value{std::to_string(row)}};
            }
            if (dtype == "int64") {
                np::Array<np::int_> array{np::Shape{m_columns.size()}};
                np::Size index = 0;
                for (const auto &column: m_columns.getIndex()) {
                    auto value = static_cast<np::int_>(at(row, column));
                    array.set(index++, value);
                }
                return Series{array, internal::Value{std::to_string(row)}};
            }
            if (dtype == "int32") {
                np::Array<np::intc> array{np::Shape{m_columns.size()}};
                np::Size index = 0;
                for (const auto &column: m_columns.getIndex()) {
                    auto value = static_cast<np::intc>(at(row, column));
                    array.set(index++, value);
                }
                return Series{array, internal::Value{std::to_string(row)}};
            }
            if (dtype == "uint64") {
                np::Array<np::Size> array{np::Shape{m_columns.size()}};
                np::Size index = 0;
                for (const auto &column: m_columns.getIndex()) {
                    auto value = static_cast<np::Size>(at(row, column));
                    array.set(index++, value);
                }
                return Series{array, internal::Value{std::to_string(row)}};
            }
            if (dtype == "str") {
                np::Array<np::string_> array{np::Shape{m_columns.size()}};
                np::Size index = 0;
                for (const auto &column: m_columns.getIndex()) {
                    auto value = static_cast<np::string_>(at(row, column));
                    array.set(index++, value);
                }
                return Series{array, internal::Value{std::to_string(row)}};
            }
            if (dtype == "unicode") {
                np::Array<np::unicode_> array{np::Shape{m_columns.size()}};
                np::Size index = 0;
                for (const auto &column: m_columns.getIndex()) {
                    auto value = static_cast<np::unicode_>(at(row, column));
                    array.set(index++, value);
                }
                return Series{array, internal::Value{std::to_string(row)}};
            }
            throw std::runtime_error("Invalid value type");
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
                for (const auto &column: m_columns.getIndex()) {
                    const auto &series = m_columnData.at(column);
                    if (series.dtype() == "int32") {
                        dtype = "int32";
                    }
                }
            }
            if (dtype.empty()) {
                for (const auto &column: m_columns.getIndex()) {
                    const auto &series = m_columnData.at(column);
                    if (series.dtype() == "int64") {
                        dtype = "int64";
                    }
                }
            }
            if (dtype.empty()) {
                for (const auto &column: m_columns.getIndex()) {
                    const auto &series = m_columnData.at(column);
                    if (series.dtype() == "uint64") {
                        dtype = "uint64";
                    }
                }
            }
            if (dtype.empty()) {
                for (const auto &column: m_columns.getIndex()) {
                    const auto &series = m_columnData.at(column);
                    if (series.dtype() == "bool") {
                        dtype = "bool";
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
                    auto value = static_cast<np::float_>(at(row, column));
                    array.set(index++, value);
                }
                return Series{array, internal::Value(static_cast<np::int_>(row))};
            }
            if (dtype == "int32") {
                np::Array<np::intc> array{np::Shape{m_columns.size()}};
                np::Size index = 0;
                for (const auto &column: m_columns.getIndex()) {
                    auto value = static_cast<np::intc>(at(row, column));
                    array.set(index++, value);
                }
                return Series{array, internal::Value{static_cast<np::int_>(row)}};
            }
            if (dtype == "int64") {
                np::Array<np::int_> array{np::Shape{m_columns.size()}};
                np::Size index = 0;
                for (const auto &column: m_columns.getIndex()) {
                    auto value = static_cast<np::int_>(at(row, column));
                    array.set(index++, value);
                }
                return Series{array, internal::Value{static_cast<np::int_>(row)}};
            }
            if (dtype == "uint64") {
                np::Array<np::Size> array{np::Shape{m_columns.size()}};
                np::Size index = 0;
                for (const auto &column: m_columns.getIndex()) {
                    auto value = static_cast<np::Size>(at(row, column));
                    array.set(index++, value);
                }
                return Series{array, internal::Value{static_cast<np::int_>(row)}};
            }
            if (dtype == "bool") {
                np::Array<np::bool_> array{np::Shape{m_columns.size()}};
                np::Size index = 0;
                for (const auto &column: m_columns.getIndex()) {
                    auto value = static_cast<np::bool_>(at(row, column));
                    array.set(index++, value);
                }
                return Series{array, internal::Value{static_cast<np::int_>(row)}};
            }
            if (dtype == "str") {
                np::Array<np::string_> array{np::Shape{m_columns.size()}};
                np::Size index = 0;
                for (const auto &column: m_columns.getIndex()) {
                    auto value = static_cast<np::string_>(at(row, column));
                    array.set(index++, value);
                }
                return Series{array, internal::Value{static_cast<np::int_>(row)}};
            }
            if (dtype == "unicode") {
                np::Array<np::unicode_> array{np::Shape{m_columns.size()}};
                np::Size index = 0;
                for (const auto &column: m_columns.getIndex()) {
                    auto value = static_cast<np::unicode_>(at(row, column));
                    array.set(index++, value);
                }
                return Series{array, internal::Value{static_cast<np::int_>(row)}};
            }
            throw std::runtime_error("Invalid value type");
        }
    }

    internal::Value DataFrame::iloc(np::Size row, np::Size column) const {
        if (empty()) {
            throw std::runtime_error("DataFrame is empty, cannot do iloc");
        }
        return at(row, m_columns[column]);
    }

    DataFrame DataFrame::iloc(const std::string &rows) const {
        if (empty()) {
            throw std::runtime_error("DataFrame is empty, cannot do iloc");
        }
        static constexpr std::size_t kIndexingHandlersSize{static_cast<std::size_t>(np::ndarray::internal::IndexingMode::Size)};

        const internal::IndexingHandler<internal::IndexingChecker1, internal::IndexingWorker1<DataFrame>> indexingHandlers[kIndexingHandlersSize] = {
                {internal::IndexingMode::Slicing,
                 internal::isSlicing1,
                 std::bind(&DataFrame::slicing1, this, std::placeholders::_1)},
                {internal::IndexingMode::Callable,
                 internal::isCallable1,
                 std::bind(&DataFrame::callable1, this, std::placeholders::_1)}};

        for (const auto &indexing: indexingHandlers) {
            if (indexing.checker(rows)) {
                return indexing.worker(rows);
            }
        }
        throw std::runtime_error("Invalid operator[] argument");
    }

    DataFrame DataFrame::iloc(const std::string &rows, const std::string &columns) const {
        if (empty()) {
            throw std::runtime_error("DataFrame is empty, cannot do iloc");
        }
        static constexpr std::size_t kIndexingHandlersSize{static_cast<std::size_t>(np::ndarray::internal::IndexingMode::Size)};

        const internal::IndexingHandler<internal::IndexingChecker2, internal::IndexingWorker2<DataFrame>> indexingHandlers[kIndexingHandlersSize] = {
                {internal::IndexingMode::Slicing,
                 internal::isSlicing2,
                 std::bind(&DataFrame::slicing2, this, std::placeholders::_1, std::placeholders::_2)},
                {internal::IndexingMode::Callable,
                 internal::isCallable2,
                 std::bind(&DataFrame::callable2, this, std::placeholders::_1, std::placeholders::_2)}};

        for (const auto &indexing: indexingHandlers) {
            if (indexing.checker(rows, columns)) {
                return indexing.worker(rows, columns);
            }
        }
        throw std::runtime_error("Invalid operator[] argument");
    }

    internal::Value DataFrame::dot(const DataFrame &another) const {
        if (shape().size() != 2 || another.shape().size() != 2 || shape()[0] != 1 || another.shape()[0] != 1 || shape()[1] != another.shape()[1]) {
            throw std::runtime_error("Shapes are different or arguments are not 1D dataframes");
        }
        std::string dtype;
        for (const auto &columnName: m_columns.getIndex()) {
            const auto &series1 = operator[](columnName);
            const auto &series2 = another[columnName];
            if ((!dtype.empty() && dtype != series1.dtype()) || series1.dtype() != series2.dtype()) {
                throw std::runtime_error("Different column types");
            }
            if (dtype.empty()) {
                dtype = series1.dtype();
            }
        }
        np::Array<np::float_> columnArray1{np::Shape{shape()[1]}};
        np::Array<np::float_> columnArray2{np::Shape{shape()[1]}};
        np::Size i = 0;
        for (const auto &columnName: m_columns.getIndex()) {
            {
                auto value = static_cast<np::float_>(at(0, columnName));
                columnArray1.set(i, value);
            }
            {
                auto value = static_cast<np::float_>(another.at(0, columnName));
                columnArray2.set(i, value);
            }
            ++i;
        }
        return Series{columnArray1, 0}.dot(Series{columnArray2, 0});
    }

    DataFrame DataFrame::add(const DataFrame &dataFrame) const {
        if (dataFrame.ndim() != 1) {
            throw std::runtime_error("DataFrame must be 1D");
        }
        auto newShape = shape().broadcast(dataFrame.shape());
        DataFrame result;
        np::Size row = 0;
        for (const auto &columnName: m_columns.getIndex()) {
            auto series = operator[](columnName);
            if (series.dtype() == "int32") {
                np::Array<np::int_> columnArray{np::Shape{shape()[0]}};
                for (np::Size i = 0; i < shape()[0]; ++i) {
                    auto value1 = static_cast<np::int_>(series.iloc(i));
                    auto value2 = static_cast<np::int_>(dataFrame.at(row, internal::Value{0}));
                    columnArray.set(i, value1 + value2);
                }
                result.append(Series{columnArray, columnName});
            } else if (series.dtype() == "float64") {
                np::Array<np::float_> columnArray{np::Shape{shape()[0]}};
                for (np::Size i = 0; i < shape()[0]; ++i) {
                    auto value1 = static_cast<np::float_>(series.iloc(i));
                    auto value2 = static_cast<np::float_>(dataFrame.at(row, internal::Value{0}));
                    columnArray.set(i, value1 + value2);
                }
                result.append(Series{columnArray, columnName});
            } else {
                throw std::runtime_error("Unknown series type");
            }
            ++row;
        }
        return result;
    }

    DataFrame DataFrame::subtract(const DataFrame &dataFrame) const {
        if (dataFrame.ndim() != 1) {
            throw std::runtime_error("DataFrame must be 1D");
        }
        auto newShape = shape().broadcast(dataFrame.shape());
        DataFrame result;
        np::Size row = 0;
        for (const auto &columnName: m_columns.getIndex()) {
            auto series = operator[](columnName);
            if (series.dtype() == "int32") {
                np::Array<np::int_> columnArray{np::Shape{shape()[0]}};
                for (np::Size i = 0; i < shape()[0]; ++i) {
                    auto value1 = static_cast<np::int_>(series.iloc(i));
                    auto value2 = static_cast<np::int_>(dataFrame.at(row, internal::Value{0}));
                    columnArray.set(i, value1 - value2);
                }
                result.append(Series{columnArray, columnName});
            } else if (series.dtype() == "float64") {
                np::Array<np::float_> columnArray{np::Shape{shape()[0]}};
                for (np::Size i = 0; i < shape()[0]; ++i) {
                    auto value1 = static_cast<np::float_>(series.iloc(i));
                    auto value2 = static_cast<np::float_>(dataFrame.at(row, internal::Value{0}));
                    columnArray.set(i, value1 - value2);
                }
                result.append(Series{columnArray, columnName});
            } else {
                throw std::runtime_error("Unknown series type");
            }
            ++row;
        }
        return result;
    }

    DataFrame DataFrame::multiply(const DataFrame &dataFrame) const {
        if (dataFrame.ndim() != 1) {
            throw std::runtime_error("DataFrame must be 1D");
        }
        auto newShape = shape().broadcast(dataFrame.shape());
        DataFrame result;
        np::Size row = 0;
        for (const auto &columnName: m_columns.getIndex()) {
            auto series = operator[](columnName);
            if (series.dtype() == "int32") {
                np::Array<np::int_> columnArray{np::Shape{shape()[0]}};
                for (np::Size i = 0; i < shape()[0]; ++i) {
                    auto value1 = static_cast<np::int_>(series.iloc(i));
                    auto value2 = static_cast<np::int_>(dataFrame.at(row, internal::Value{0}));
                    columnArray.set(i, value1 * value2);
                }
                result.append(Series{columnArray, columnName});
            } else if (series.dtype() == "float64") {
                np::Array<np::float_> columnArray{np::Shape{shape()[0]}};
                for (np::Size i = 0; i < shape()[0]; ++i) {
                    auto value1 = static_cast<np::float_>(series.iloc(i));
                    auto value2 = static_cast<np::float_>(dataFrame.at(row, internal::Value{0}));
                    columnArray.set(i, value1 * value2);
                }
                result.append(Series{columnArray, columnName});
            } else {
                throw std::runtime_error("Unknown series type");
            }
            ++row;
        }
        return result;
    }

    DataFrame DataFrame::divide(const DataFrame &dataFrame) const {
        if (dataFrame.ndim() != 1) {
            throw std::runtime_error("DataFrame must be 1D");
        }
        auto newShape = shape().broadcast(dataFrame.shape());
        DataFrame result;
        np::Size row = 0;
        for (const auto &columnName: m_columns.getIndex()) {
            auto series = operator[](columnName);
            if (series.dtype() == "int32") {
                np::Array<np::int_> columnArray{np::Shape{shape()[0]}};
                for (np::Size i = 0; i < shape()[0]; ++i) {
                    auto value1 = static_cast<np::int_>(series.iloc(i));
                    auto value2 = static_cast<np::int_>(dataFrame.at(row, internal::Value{0}));
                    columnArray.set(i, value1 / value2);
                }
                result.append(Series{columnArray, columnName});
            } else if (series.dtype() == "float64") {
                np::Array<np::float_> columnArray{np::Shape{shape()[0]}};
                for (np::Size i = 0; i < shape()[0]; ++i) {
                    auto value1 = static_cast<np::float_>(series.iloc(i));
                    auto value2 = static_cast<np::float_>(dataFrame.at(row, internal::Value{0}));
                    columnArray.set(i, value1 / value2);
                }
                result.append(Series{columnArray, columnName});
            } else {
                throw std::runtime_error("Unknown series type");
            }
            ++row;
        }
        return result;
    }

}// namespace pd