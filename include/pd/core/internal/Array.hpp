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

#include <variant>

#include <np/Array.hpp>

namespace pd {
    namespace internal {
        class Array {
        public:
            Array() = default;

            Array(const Array &) = default;
            Array(Array &&) = default;

            Array &operator=(const Array &) = default;
            Array &operator=(Array &&) = default;

            explicit Array(const np::Array<np::int_> &array)
                : m_array{array} {
            }

            explicit Array(np::Array<np::int_> &&array)
                : m_array{std::move(array)} {
            }

            explicit Array(const np::Array<np::float_> &array)
                : m_array{array} {
            }

            explicit Array(np::Array<np::float_> &&array)
                : m_array{std::move(array)} {
            }

            explicit Array(const np::Array<np::string_> &array)
                : m_array{array} {
            }

            explicit Array(np::Array<np::string_> &array)
                : m_array{std::move(array)} {
            }

            explicit Array(const np::Array<np::unicode_> &array)
                : m_array{array} {
            }

            explicit Array(np::Array<np::unicode_> &&array)
                : m_array{std::move(array)} {
            }

            Array &operator=(const np::Array<np::int_> &array) {
                m_array = array;
                return *this;
            }

            Array &operator=(np::Array<np::int_> &&array) {
                m_array = std::move(array);
                return *this;
            }

            Array &operator=(const np::Array<np::float_> &array) {
                m_array = array;
                return *this;
            }

            Array &operator=(np::Array<np::float_> &&array) {
                m_array = std::move(array);
                return *this;
            }

            Array &operator=(const np::Array<np::string_> &array) {
                m_array = array;
                return *this;
            }

            Array &operator=(np::Array<np::string_> &&array) {
                m_array = std::move(array);
                return *this;
            }

            Array &operator=(const np::Array<np::unicode_> &array) {
                m_array = array;
                return *this;
            }

            Array &operator=(np::Array<np::unicode_> &&array) {
                m_array = std::move(array);
                return *this;
            }

            [[nodiscard]] bool isIntArray() const {
                return std::holds_alternative<np::Array<np::int_>>(m_array);
            }

            operator const np::Array<np::int_> *() const {
                return std::get_if<np::Array<np::int_>>(&m_array);
            }

            operator np::Array<np::int_> *() {
                return std::get_if<np::Array<np::int_>>(&m_array);
            }

            [[nodiscard]] bool isFloatArray() const {
                return std::holds_alternative<np::Array<np::float_>>(m_array);
            }

            operator const np::Array<np::float_> *() const {
                return std::get_if<np::Array<np::float_>>(&m_array);
            }

            operator np::Array<np::float_> *() {
                return std::get_if<np::Array<np::float_>>(&m_array);
            }

            [[nodiscard]] bool isStringArray() const {
                return std::holds_alternative<np::Array<np::string_>>(m_array);
            }

            operator const np::Array<np::string_> *() const {
                return std::get_if<np::Array<np::string_>>(&m_array);
            }

            operator np::Array<np::string_> *() {
                return std::get_if<np::Array<np::string_>>(&m_array);
            }

            [[nodiscard]] bool isUnicodeArray() const {
                return std::holds_alternative<np::Array<np::unicode_>>(m_array);
            }

            operator const np::Array<np::unicode_> *() const {
                return std::get_if<np::Array<np::unicode_>>(&m_array);
            }

            operator np::Array<np::unicode_> *() {
                return std::get_if<np::Array<np::unicode_>>(&m_array);
            }

            bool operator==(const Array &another) const {
                if (this == &another)
                    return true;

                if (const auto *arrayIntPtr = std::get_if<np::Array<np::int_>>(&m_array)) {
                    if (!another.isIntArray()) {
                        return false;
                    }
                    auto array = std::get<np::Array<np::int_>>(another.m_array);
                    return np::array_equal(array, *arrayIntPtr);
                } else if (const auto *arrayFloatPtr = std::get_if<np::Array<np::float_>>(&m_array)) {
                    if (!another.isFloatArray()) {
                        return false;
                    }
                    auto array = std::get<np::Array<np::float_>>(another.m_array);
                    return np::array_equal(array, *arrayFloatPtr);
                } else if (const auto *arrayStringPtr = std::get_if<np::Array<np::string_>>(&m_array)) {
                    if (!another.isStringArray()) {
                        return false;
                    }
                    auto array = std::get<np::Array<np::string_>>(another.m_array);
                    return np::array_equal(array, *arrayStringPtr);
                } else if (const auto *arrayUnicodePtr = std::get_if<np::Array<np::unicode_>>(&m_array)) {
                    if (!another.isUnicodeArray()) {
                        return false;
                    }
                    auto array = std::get<np::Array<np::unicode_>>(another.m_array);
                    return np::array_equal(array, *arrayUnicodePtr);
                } else {
                    throw std::runtime_error("Unsupported type");
                }
            }

            np::Shape shape() const {
                np::Shape shape;
                if (const auto *arrayIntPtr = std::get_if<np::Array<np::int_>>(&m_array)) {
                    shape = arrayIntPtr->shape();
                } else if (const auto *arrayFloatPtr = std::get_if<np::Array<np::float_>>(&m_array)) {
                    shape = arrayFloatPtr->shape();
                } else if (const auto *arrayStringPtr = std::get_if<np::Array<np::string_>>(&m_array)) {
                    shape = arrayStringPtr->shape();
                } else if (const auto *arrayUnicodePtr = std::get_if<np::Array<np::unicode_>>(&m_array)) {
                    shape = arrayUnicodePtr->shape();
                } else {
                    throw std::runtime_error("Unsupported type");
                }
                return shape;
            }

            np::Size size() const {
                return shape().calcSizeByShape();
            }

        private:
            using ArrayTypes = std::variant<std::monostate,
                                            np::Array<np::object>,
                                            np::Array<np::int_>,
                                            np::Array<np::float_>,
                                            np::Array<np::string_>,
                                            np::Array<np::unicode_>>;

            ArrayTypes m_array;
        };

    }// namespace internal
}// namespace pd