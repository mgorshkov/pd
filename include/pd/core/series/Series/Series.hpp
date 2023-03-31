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

        Series &operator=(const Series &) = default;
        Series &operator=(Series &&) = default;

        Series(const internal::Array &data,
               const std::vector<internal::Value> &index = std::vector<internal::Value>{},
               const internal::Value &name = internal::Value{});

        Series(internal::Array &&data,
               const std::vector<internal::Value> &index = std::vector<internal::Value>{},
               const internal::Value &name = internal::Value{});

        Series(const np::Array<np::int_> &data,
               const std::vector<internal::Value> &index = std::vector<internal::Value>{},
               const internal::Value &name = internal::Value{});

        Series(np::Array<np::int_> &&data,
               const std::vector<internal::Value> &index = std::vector<internal::Value>{},
               const internal::Value &name = internal::Value{});

        Series(const np::Array<np::float_> &data,
               const std::vector<internal::Value> &index = std::vector<internal::Value>{},
               const internal::Value &name = internal::Value{});

        Series(np::Array<np::float_> &&data,
               const std::vector<internal::Value> &index = std::vector<internal::Value>{},
               const internal::Value &name = internal::Value{});

        Series(const np::Array<np::string_> &data,
               const std::vector<internal::Value> &index = std::vector<internal::Value>{},
               const internal::Value &name = internal::Value{});

        Series(np::Array<np::string_> &&data,
               const std::vector<internal::Value> &index = std::vector<internal::Value>{},
               const internal::Value &name = internal::Value{});

        Series(const np::Array<np::unicode_> &data,
               const std::vector<internal::Value> &index = std::vector<internal::Value>{},
               const internal::Value &name = internal::Value{});

        Series(np::Array<np::unicode_> &&data,
               const std::vector<internal::Value> &index = std::vector<internal::Value>{},
               const internal::Value &name = internal::Value{});

        Series(const internal::Array &data, const std::vector<internal::Value> &index);

        Series(const np::Array<np::int_> &data, const std::vector<internal::Value> &index);
        Series(np::Array<np::int_> &&data, const std::vector<internal::Value> &index);

        Series(const np::Array<np::float_> &data, const std::vector<internal::Value> &index);
        Series(np::Array<np::float_> &&data, const std::vector<internal::Value> &index);

        Series(const np::Array<np::string_> &data, const std::vector<internal::Value> &index);
        Series(np::Array<np::string_> &&data, const std::vector<internal::Value> &index);

        Series(const np::Array<np::unicode_> &data, const std::vector<internal::Value> &index);
        Series(np::Array<np::unicode_> &&data, const std::vector<internal::Value> &index);

        Series(const internal::Array &data, const internal::Value &name);
        Series(internal::Array &&data, const internal::Value &name);

        Series(const np::Array<np::int_> &data, const internal::Value &name);
        Series(np::Array<np::int_> &&data, const internal::Value &name);

        Series(const np::Array<np::float_> &data, const internal::Value &name);
        Series(np::Array<np::float_> &&data, const internal::Value &name);

        Series(const np::Array<np::string_> &data, const internal::Value &name);
        Series(np::Array<np::string_> &&data, const internal::Value &name);

        Series(const np::Array<np::unicode_> &data, const internal::Value &name);
        Series(np::Array<np::unicode_> &&data, const internal::Value &name);

        template<typename DType, typename Parent, typename ParentStorage>
        Series(const np::ndarray::internal::IndexParent<DType, Parent, ParentStorage> &data, const internal::Value &columnName)
            : m_name{columnName}, m_shape{data.shape()}, m_size{data.shape().calcSizeByShape()} {
            np::Array<DType> array{m_shape};
            for (np::Size i = 0; i < data.size(); ++i) {
                array.set(i, data.get(i));
            }
            m_data = std::move(array);
        }

        template<typename DType, typename Parent, typename ParentStorage>
        Series(const np::ndarray::internal::ConstIndexParent<DType, Parent, ParentStorage> &data, const internal::Value &columnName)
            : m_name{columnName}, m_shape{data.shape()}, m_size{data.shape().calcSizeByShape()} {
            np::Array<DType> array{m_shape};
            for (np::Size i = 0; i < data.size(); ++i) {
                array.set(i, data.get(i));
            }
            m_data = std::move(array);
        }

        bool operator==(const Series &other) const;

        [[nodiscard]] internal::Value name() const;
        [[nodiscard]] internal::Index index() const;
        [[nodiscard]] np::Size size() const;
        [[nodiscard]] np::Shape shape() const;
        [[nodiscard]] std::string dtype() const;

        [[nodiscard]] const internal::Array &values() const;
        internal::Array &values();

        void set(np::Size row, const internal::Value &value);
        [[nodiscard]] const internal::Value &at(np::Size row) const;
        [[nodiscard]] internal::Value &at(np::Size row);

        internal::Value &iloc(np::Size i);
        const internal::Value &iloc(np::Size i) const;
        Series iloc(const std::string &cond) const;
        Series iloc(const std::vector<bool> &indexes) const;

        np::float_ mean(bool skipna = true) const;
        Series replace(internal::Value to_replace, internal::Value value) const;

        void info() const;

    private:
        Series slicing1(const std::string &cond) const;
        Series callable1(const std::string &cond) const;

        internal::Array m_data;
        internal::Index m_index;
        internal::Value m_name;
        np::Shape m_shape;
        np::Size m_size{0};

        mutable internal::Value m_value;
    };
}// namespace pd