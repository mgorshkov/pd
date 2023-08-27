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

        template<np::Size SizeT>
        explicit DataFrame(const np::ndarray::array_static::NDArrayStatic<np::bool_, SizeT> &data,
                           const DataFrameParameters &dataFrameParameters = DataFrameParameters{}) {
            init<np::bool_, SizeT>(data, dataFrameParameters);
        }

        explicit DataFrame(const np::ndarray::array_dynamic::NDArrayDynamic<np::bool_> &data,
                           const DataFrameParameters &dataFrameParameters = DataFrameParameters{}) {
            init<np::bool_>(data, dataFrameParameters);
        }

        template<np::Size SizeT>
        explicit DataFrame(const np::ndarray::array_static::NDArrayStatic<np::intc, SizeT> &data,
                           const DataFrameParameters &dataFrameParameters = DataFrameParameters{}) {
            init<np::intc, SizeT>(data, dataFrameParameters);
        }

        explicit DataFrame(const np::ndarray::array_dynamic::NDArrayDynamic<np::intc> &data,
                           const DataFrameParameters &dataFrameParameters = DataFrameParameters{}) {
            init<np::intc>(data, dataFrameParameters);
        }

        template<np::Size SizeT>
        explicit DataFrame(const np::ndarray::array_static::NDArrayStatic<np::int_, SizeT> &data,
                           const DataFrameParameters &dataFrameParameters = DataFrameParameters{}) {
            init<np::int_, SizeT>(data, dataFrameParameters);
        }

        explicit DataFrame(const np::ndarray::array_dynamic::NDArrayDynamic<np::int_> &data,
                           const DataFrameParameters &dataFrameParameters = DataFrameParameters{}) {
            init<np::int_>(data, dataFrameParameters);
        }

        template<np::Size SizeT>
        explicit DataFrame(const np::ndarray::array_static::NDArrayStatic<np::Size, SizeT> &data,
                           const DataFrameParameters &dataFrameParameters = DataFrameParameters{}) {
            init<np::Size, SizeT>(data, dataFrameParameters);
        }

        explicit DataFrame(const np::ndarray::array_dynamic::NDArrayDynamic<np::Size> &data,
                           const DataFrameParameters &dataFrameParameters = DataFrameParameters{}) {
            init<np::Size>(data, dataFrameParameters);
        }

        template<np::Size SizeT>
        explicit DataFrame(const np::ndarray::array_static::NDArrayStatic<np::float_, SizeT> &data,
                           const DataFrameParameters &dataFrameParameters = DataFrameParameters{}) {
            init<np::float_, SizeT>(data, dataFrameParameters);
        }

        explicit DataFrame(const np::ndarray::array_dynamic::NDArrayDynamic<np::float_> &data,
                           const DataFrameParameters &dataFrameParameters = DataFrameParameters{}) {
            init<np::float_>(data, dataFrameParameters);
        }

        template<np::Size SizeT>
        explicit DataFrame(const np::ndarray::array_static::NDArrayStatic<np::string_, SizeT> &data,
                           const DataFrameParameters &dataFrameParameters = DataFrameParameters{}) {
            init<np::string_, SizeT>(data, dataFrameParameters);
        }

        explicit DataFrame(const np::ndarray::array_dynamic::NDArrayDynamic<np::string_> &data,
                           const DataFrameParameters &dataFrameParameters = DataFrameParameters{}) {
            init<np::string_>(data, dataFrameParameters);
        }

        template<np::Size SizeT>
        explicit DataFrame(const np::ndarray::array_static::NDArrayStatic<np::unicode_, SizeT> &data,
                           const DataFrameParameters &dataFrameParameters = DataFrameParameters{}) {
            init<np::unicode_, SizeT>(data, dataFrameParameters);
        }

        explicit DataFrame(const np::ndarray::array_dynamic::NDArrayDynamic<np::unicode_> &data,
                           const DataFrameParameters &dataFrameParameters = DataFrameParameters{}) {
            init<np::unicode_>(data, dataFrameParameters);
        }

        template<np::Size SizeT>
        explicit DataFrame(const np::ndarray::array_static::NDArrayStatic<internal::Value, SizeT> &data,
                           const DataFrameParameters &dataFrameParameters = DataFrameParameters{}) {
            init<internal::Value, SizeT>(data, dataFrameParameters);
        }

        explicit DataFrame(const np::ndarray::array_dynamic::NDArrayDynamic<internal::Value> &data,
                           const DataFrameParameters &dataFrameParameters = DataFrameParameters{}) {
            init<internal::Value>(data, dataFrameParameters);
        }

        template<np::Size NRows, np::Size NColumns>
        explicit DataFrame(internal::Value data[NRows][NColumns],
                           const DataFrameParameters &dataFrameParameters = DataFrameParameters{}) {
            init(data, dataFrameParameters);
        }

        DataFrame(const DataFrame &) = default;
        DataFrame(DataFrame &&) = default;

        DataFrame &operator=(const DataFrame &) = default;
        DataFrame &operator=(DataFrame &&) = default;

        bool operator==(const DataFrame &another) const;

        [[nodiscard]] bool empty() const;
        [[nodiscard]] np::Size ndim() const;

        [[nodiscard]] np::Shape shape() const;
        [[nodiscard]] np::Size size() const;
        [[nodiscard]] internal::Index index() const;

        void append(const DataFrame &df);
        void append(const Series &series);

        const Series &operator[](const internal::Value &column) const;
        Series &operator[](const internal::Value &column);

        [[nodiscard]] bool hasColumn(const internal::Value &column) const;
        [[nodiscard]] internal::Index columns() const;

        void drop(const internal::Value &column);

        DataFrame operator[](np::Size row) const;

        [[nodiscard]] internal::Value at(np::Size row, const internal::Value &column) const;
        void set(np::Size row, const internal::Value &column, const internal::Value &value);

        [[nodiscard]] Series iloc(np::Size row) const;
        [[nodiscard]] internal::Value iloc(np::Size row, np::Size column) const;
        [[nodiscard]] DataFrame iloc(const std::string &rows) const;
        [[nodiscard]] DataFrame iloc(const std::string &rows, const std::string &columns) const;

        [[nodiscard]] internal::Value dot(const DataFrame &another) const;

        template<typename DType>
        DataFrame addVector(const np::Array<DType> &array) const {
            if (array.ndim() != 1) {
                throw std::runtime_error("Array must be 1D");
            }
            auto newShape = shape().broadcast(array.shape());
            DataFrame result;
            np::Size column = 0;
            for (auto columnName: m_columns.getIndex()) {
                np::Array<DType> columnArray{np::Shape{shape()[0]}};
                auto cell = array.get(column++);
                for (np::Size i = 0; i < shape()[0]; ++i) {
                    columnArray.set(i, static_cast<DType>(at(i, columnName)) + cell);
                }
                result.append(Series{columnArray, columnName});
            }
            return result;
        }

        template<typename DType>
        DataFrame operator+(const np::Array<DType> &array) const {
            return addVector(array);
        }

        [[nodiscard]] DataFrame add(const DataFrame &dataFrame) const;

        DataFrame operator+(const DataFrame &dataFrame) const {
            return add(dataFrame);
        }

        template<typename DType>
        DataFrame subtractVector(const np::Array<DType> &array) const {
            if (array.ndim() != 1) {
                throw std::runtime_error("Array must be 1D");
            }
            auto newShape = shape().broadcast(array.shape());
            DataFrame result;
            np::Size column = 0;
            for (auto columnName: m_columns.getIndex()) {
                np::Array<DType> columnArray{np::Shape{shape()[0]}};
                auto cell = array.get(column++);
                for (np::Size i = 0; i < shape()[0]; ++i) {
                    columnArray.set(i, static_cast<DType>(at(i, columnName)) - cell);
                }
                result.append(Series{columnArray, columnName});
            }
            return result;
        }

        template<typename DType>
        DataFrame operator-(const np::Array<DType> &array) const {
            return subtractVector(array);
        }

        [[nodiscard]] DataFrame subtract(const DataFrame &dataFrame) const;

        DataFrame operator-(const DataFrame &dataFrame) const {
            return subtract(dataFrame);
        }

        template<typename DType>
        DataFrame multiplyVector(const np::Array<DType> &array) const {
            if (array.ndim() != 1) {
                throw std::runtime_error("Array must be 1D");
            }
            auto newShape = shape().broadcast(array.shape());
            DataFrame result;
            np::Size column = 0;
            for (auto columnName: m_columns.getIndex()) {
                np::Array<DType> columnArray{np::Shape{shape()[0]}};
                auto cell = array.get(column++);
                for (np::Size i = 0; i < shape()[0]; ++i) {
                    columnArray.set(i, static_cast<DType>(at(i, columnName)) * cell);
                }
                result.append(Series{columnArray, columnName});
            }
            return result;
        }

        template<typename DType>
        DataFrame operator*(const np::Array<DType> &array) const {
            return multiplyVector(array);
        }

        [[nodiscard]] DataFrame multiply(const DataFrame &dataFrame) const;

        DataFrame operator*(const DataFrame &dataFrame) const {
            return multiply(dataFrame);
        }

        template<typename DType>
        DataFrame divideVector(const np::Array<DType> &array) const {
            if (array.ndim() != 1) {
                throw std::runtime_error("Array must be 1D");
            }
            auto newShape = shape().broadcast(array.shape());
            DataFrame result;
            np::Size column = 0;
            for (auto columnName: m_columns.getIndex()) {
                np::Array<DType> columnArray{np::Shape{shape()[0]}};
                auto cell = array.get(column++);
                for (np::Size i = 0; i < shape()[0]; ++i) {
                    columnArray.set(i, static_cast<DType>(at(i, columnName)) / cell);
                }
                result.append(Series{columnArray, columnName});
            }
            return result;
        }

        template<typename DType>
        DataFrame operator/(const np::Array<DType> &array) const {
            return divideVector(array);
        }

        [[nodiscard]] DataFrame divide(const DataFrame &dataFrame) const;

        DataFrame operator/(const DataFrame &dataFrame) const {
            return divide(dataFrame);
        }

    private:
        template<typename DType, np::Size SizeT = np::SIZE_DEFAULT>
        void init(const np::Array<DType, SizeT> &data, const DataFrameParameters &dataFrameParameters = DataFrameParameters{}) {
            m_index = internal::Index{dataFrameParameters.index, data.shape()[0]};
            m_shape = data.shape();

            if (m_shape.size() == 1) {
                m_columns = internal::Index{dataFrameParameters.columns, 1};
                auto column = m_columns[0];
                np::Array<DType> array = data.copy();
                m_columnData[column] = Series{array, column};
            } else {
                m_columns = internal::Index{dataFrameParameters.columns, data.shape()[1]};
                for (np::Size i = 0; i < data.shape()[1]; ++i) {
                    auto column = m_columns[i];
                    np::Array<DType> array{np::Shape{data.shape()[0]}};
                    for (np::Size j = 0; j < data.shape()[0]; ++j) {
                        array.set(j, data.get(j * data.shape()[1] + i));
                    }
                    m_columnData[column] = Series{array, column};
                }
            }
        }

        template<np::Size NRows, np::Size NColumns>
        void init(internal::Value data[NRows][NColumns], const DataFrameParameters &dataFrameParameters) {
            m_index = internal::Index{dataFrameParameters.index, NRows};
            m_columns = internal::Index{dataFrameParameters.columns, NColumns};
            m_shape = np::Shape{NRows, NColumns};

            for (np::Size i = 0; i < NColumns; ++i) {
                auto column = m_columns[i];
                np::Array<internal::Value> array{np::Shape{NRows}};
                for (np::Size j = 0; j < NRows; ++j) {
                    array.set(j, data[i][j]);
                }
                m_columnData[column] = Series{array, column};
            }
        }

        [[nodiscard]] DataFrame slicing1(const std::string &rows) const;
        [[nodiscard]] DataFrame slicing2(const std::string &rows, const std::string &columns) const;
        [[nodiscard]] DataFrame callable1(const std::string &rows) const;
        [[nodiscard]] DataFrame callable2(const std::string &rows, const std::string &columns) const;

        std::unordered_map<internal::Value, Series, std::hash<pd::internal::Value>> m_columnData;
        internal::Index m_index;
        internal::Index m_columns;
        np::Shape m_shape;
    };
}// namespace pd