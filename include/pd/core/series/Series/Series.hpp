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

#include <pd/core/internal/Array.hpp>
#include <pd/core/internal/Index.hpp>
#include <pd/core/internal/Value.hpp>

namespace pd {
    class Series {
    public:
        Series() = default;

        Series(const Series &) = default;
        Series(Series &&) = default;

        explicit Series(const internal::Array &data,
                        const std::vector<internal::Value> &index = std::vector<internal::Value>{},
                        const internal::Value &name = internal::Value{});

        explicit Series(internal::Array &&data,
                        const std::vector<internal::Value> &index = std::vector<internal::Value>{},
                        const internal::Value &name = internal::Value{});

        template<typename DType, typename Derived, typename Storage>
        explicit Series(const np::ndarray::internal::NDArrayBase<DType, Derived, Storage> &data,
                        const std::vector<internal::Value> &index = std::vector<internal::Value>{},
                        const internal::Value &name = internal::Value{})
            : Series{internal::Array{data.copy()}, index, name} {
        }

        template<typename DType, typename Derived, typename Storage>
        explicit Series(np::ndarray::internal::NDArrayBase<DType, Derived, Storage> &&data,
                        const std::vector<internal::Value> &index = std::vector<internal::Value>{},
                        const internal::Value &name = internal::Value{})
            : Series{internal::Array{data.copy()}, index, name} {
        }

        Series(const internal::Array &data, const std::vector<internal::Value> &index)
            : Series{data, index, internal::Value{}} {
        }

        template<typename DType, typename Derived, typename Storage>
        Series(const np::ndarray::internal::NDArrayBase<DType, Derived, Storage> &data, const std::vector<internal::Value> &index)
            : Series{data, index, internal::Value{}} {
        }

        template<typename DType, typename Derived, typename Storage>
        Series(np::ndarray::internal::NDArrayBase<DType, Derived, Storage> &&data, const std::vector<internal::Value> &index)
            : Series{data, index, internal::Value{}} {
        }

        Series(const internal::Array &data, const internal::Value &name)
            : Series{data, std::vector<internal::Value>{}, name} {
        }

        Series(internal::Array &&data, const internal::Value &name)
            : Series{data, std::vector<internal::Value>{}, name} {
        }

        template<typename DType, typename Derived, typename Storage>
        Series(const np::ndarray::internal::NDArrayBase<DType, Derived, Storage> &data, const internal::Value &name)
            : Series{data, std::vector<internal::Value>{}, name} {
        }

        template<typename DType, typename Derived, typename Storage>
        Series(np::ndarray::internal::NDArrayBase<DType, Derived, Storage> &&data, const internal::Value &name)
            : Series{data, std::vector<internal::Value>{}, name} {
        }

        Series(const np::Shape &shape, const internal::Value &columnName)
            : m_name{columnName}, m_shape{shape}, m_size{shape.calcSizeByShape()} {
        }

        Series &operator=(const Series &) = default;
        Series &operator=(Series &&) = default;

        bool operator==(const Series &other) const;

        [[nodiscard]] internal::Value name() const;
        [[nodiscard]] internal::Index index() const;

        [[nodiscard]] bool empty() const;
        [[nodiscard]] np::Size ndim() const;
        [[nodiscard]] np::Size size() const;
        [[nodiscard]] np::Shape shape() const;
        [[nodiscard]] std::string dtype() const;

        [[nodiscard]] const internal::Array &values() const;
        internal::Array &values();

        void set(np::Size row, const internal::Value &value);
        [[nodiscard]] internal::Value at(np::Size row) const;

        [[nodiscard]] internal::Value operator[](np::Size row) const;

        [[nodiscard]] internal::Value iloc(np::Size row) const;
        [[nodiscard]] Series iloc(const std::string &cond) const;
        [[nodiscard]] Series iloc(const std::vector<bool> &indexes) const;

        [[nodiscard]] np::float_ mean(bool skipna = true) const;
        [[nodiscard]] np::float_ std_(bool skipna = true) const;
        [[nodiscard]] np::float_ var(bool skipna = true) const;

        [[nodiscard]] Series replace(internal::Value to_replace, internal::Value value) const;

        [[nodiscard]] internal::Value dot(const Series &another) const;

        void info() const;

        friend inline Series operator+(const Series &series1, const Series &series2) {
            if (series1.m_shape != series2.m_shape) {
                throw std::runtime_error("Different shapes");
            }
            if (series1.m_name != series2.m_name) {
                throw std::runtime_error("Different names");
            }
            Series result{series1.m_shape, series1.m_name};
            for (np::Size i = 0; i < series1.size(); ++i) {
                result.set(i, series1.at(i) + series2.at(i));
            }
            return result;
        }

        friend inline Series operator-(const Series &series1, const Series &series2) {
            if (series1.m_shape != series2.m_shape) {
                throw std::runtime_error("Different shapes");
            }
            if (series1.m_name != series2.m_name) {
                throw std::runtime_error("Different names");
            }
            Series result{series1.m_shape, series1.m_name};
            for (np::Size i = 0; i < series1.size(); ++i) {
                result.set(i, series1.at(i) - series2.at(i));
            }
            return result;
        }

        friend inline Series operator*(const Series &series1, const Series &series2) {
            if (series1.m_shape != series2.m_shape) {
                throw std::runtime_error("Different shapes");
            }
            if (series1.m_name != series2.m_name) {
                throw std::runtime_error("Different names");
            }
            Series result{series1.m_shape, series1.m_name};
            for (np::Size i = 0; i < series1.size(); ++i) {
                result.set(i, series1.at(i) * series2.at(i));
            }
            return result;
        }

        friend inline Series operator/(const Series &series1, const Series &series2) {
            if (series1.m_shape != series2.m_shape) {
                throw std::runtime_error("Different shapes");
            }
            if (series1.m_name != series2.m_name) {
                throw std::runtime_error("Different names");
            }
            Series result{series1.m_shape, series1.m_name};
            for (np::Size i = 0; i < series1.size(); ++i) {
                result.set(i, series1.at(i) / series2.at(i));
            }
            return result;
        }

    private:
        Series slicing1(const std::string &cond) const;
        Series callable1(const std::string &cond) const;

        internal::Array m_data;
        internal::Index m_index;
        internal::Value m_name;
        np::Shape m_shape;
        np::Size m_size{0};
    };
}// namespace pd