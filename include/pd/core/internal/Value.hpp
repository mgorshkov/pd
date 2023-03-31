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

#include <functional>

#include <np/Array.hpp>
#include <np/DType.hpp>

namespace pd {
    namespace internal {
        class Value {
        public:
            Value() = default;

            Value(const Value &) = default;
            Value(Value &&) = default;

            Value(np::intc value)
                : m_value{value} {
            }

            Value(np::int_ value)
                : m_value{value} {
            }

            Value(np::Size value)
                : m_value{value} {
            }

            Value(np::float_ value)
                : m_value{value} {
            }

            Value(const char *value)
                : Value{np::string_{value}} {
            }

            Value(const np::string_ value)
                : m_value{value} {
            }

            Value(const np::unicode_ &value)
                : m_value{value} {
            }

            Value(const wchar_t *value)
                : m_value{np::unicode_{value}} {
            }

            Value &operator=(const Value &) = default;
            Value &operator=(Value &&) = default;

            Value &operator=(np::intc value) {
                m_value = value;
                return *this;
            }

            Value &operator=(np::int_ value) {
                m_value = value;
                return *this;
            }

            Value &operator=(np::Size value) {
                m_value = value;
                return *this;
            }

            Value &operator=(np::float_ value) {
                m_value = value;
                return *this;
            }

            Value &operator=(const np::string_ &value) {
                m_value = value;
                return *this;
            }

            Value &operator=(const np::unicode_ &value) {
                m_value = value;
                return *this;
            }

            bool operator==(const Value &another) const {
                if (this == &another)
                    return true;
                if (isInt()) {
                    if (another.isInt()) {
                        return *static_cast<const np::int_ *>(*this) == *static_cast<const np::int_ *>(another);
                    } else if (another.isSize()) {
                        return *static_cast<const np::int_ *>(*this) == static_cast<np::int_>(*static_cast<const np::Size *>(another));
                    } else if (another.isIntC()) {
                        return *static_cast<const np::int_ *>(*this) == *static_cast<const np::intc *>(another);
                    }
                } else if (isIntC()) {
                    if (another.isInt()) {
                        return *static_cast<const np::intc *>(*this) == *static_cast<const np::int_ *>(another);
                    } else if (another.isSize()) {
                        return *static_cast<const np::intc *>(*this) == static_cast<np::intc>(*static_cast<const np::Size *>(another));
                    } else if (another.isIntC()) {
                        return *static_cast<const np::intc *>(*this) == *static_cast<const np::intc *>(another);
                    }
                } else if (isSize()) {
                    if (another.isInt()) {
                        return *static_cast<const np::Size *>(*this) == static_cast<np::Size>(*static_cast<const np::int_ *>(another));
                    } else if (another.isSize()) {
                        return *static_cast<const np::Size *>(*this) == *static_cast<const np::Size *>(another);
                    } else if (another.isIntC()) {
                        return *static_cast<const np::Size *>(*this) == static_cast<np::Size>(*static_cast<const np::intc *>(another));
                    }
                } else if (isFloat()) {
                    return another.isFloat() && np::internal::element_equal(*static_cast<const np::float_ *>(*this), *static_cast<const np::float_ *>(another));
                } else if (isString()) {
                    return another.isString() && *static_cast<const np::string_ *>(*this) == *static_cast<const np::string_ *>(another);
                } else if (isUnicode()) {
                    return another.isUnicode() && *static_cast<const np::unicode_ *>(*this) == *static_cast<const np::unicode_ *>(another);
                }
                return true;
            }

            [[nodiscard]] bool isIntC() const {
                return std::holds_alternative<np::intc>(m_value);
            }

            operator const np::intc *() const {
                return std::get_if<np::intc>(&m_value);
            }

            operator np::intc *() {
                return std::get_if<np::intc>(&m_value);
            }

            [[nodiscard]] bool isInt() const {
                return std::holds_alternative<np::int_>(m_value);
            }

            operator const np::int_ *() const {
                return std::get_if<np::int_>(&m_value);
            }

            operator np::int_ *() {
                return std::get_if<np::int_>(&m_value);
            }

            [[nodiscard]] bool isSize() const {
                return std::holds_alternative<np::Size>(m_value);
            }

            operator const np::Size *() const {
                return std::get_if<np::Size>(&m_value);
            }

            operator np::Size *() {
                return std::get_if<np::Size>(&m_value);
            }

            [[nodiscard]] bool isFloat() const {
                return std::holds_alternative<np::float_>(m_value);
            }

            operator const np::float_ *() const {
                return std::get_if<np::float_>(&m_value);
            }

            operator np::float_ *() {
                return std::get_if<np::float_>(&m_value);
            }

            [[nodiscard]] bool isString() const {
                return std::holds_alternative<np::string_>(m_value);
            }

            operator const np::string_ *() const {
                return std::get_if<np::string_>(&m_value);
            }

            operator np::string_ *() {
                return std::get_if<np::string_>(&m_value);
            }

            [[nodiscard]] bool isUnicode() const {
                return std::holds_alternative<np::unicode_>(m_value);
            }

            operator const np::unicode_ *() const {
                return std::get_if<np::unicode_>(&m_value);
            }

            operator np::unicode_ *() {
                return std::get_if<np::unicode_>(&m_value);
            }

            bool empty() const {
                return std::holds_alternative<std::monostate>(m_value);
            }

            friend std::ostream &operator<<(std::ostream &stream, const Value &value) {
                if (const auto *intValue = std::get_if<np::int_>(&value.m_value)) {
                    stream << *intValue;
                } else if (const auto *intcValue = std::get_if<np::intc>(&value.m_value)) {
                    stream << *intcValue;
                } else if (const auto *SizeValue = std::get_if<np::Size>(&value.m_value)) {
                    stream << *SizeValue;
                } else if (const auto *floatValue = std::get_if<np::float_>(&value.m_value)) {
                    stream << std::setprecision(8);
                    stream << *floatValue;
                } else if (const auto *stringValue = std::get_if<np::string_>(&value.m_value)) {
                    stream << *stringValue;
                } else if (const auto *unicodeValue = std::get_if<np::unicode_>(&value.m_value)) {
                    std::string str(unicodeValue->length(), 0);
                    std::transform(unicodeValue->begin(), unicodeValue->end(), str.begin(), [](wchar_t c) {
                        return static_cast<char>(c);
                    });
                    stream << str;
                }
                return stream;
            }

        private:
            using ValueTypes = std::variant<std::monostate,
                                            np::object,
                                            np::intc,
                                            np::int_,
                                            np::Size,
                                            np::float_,
                                            np::string_,
                                            np::unicode_>;
            ValueTypes m_value;
        };

    }// namespace internal
}// namespace pd

namespace std {
    template<>
    struct hash<pd::internal::Value> {
        std::size_t operator()(const pd::internal::Value &value) const {
            if (value.isInt()) {
                return hash<np::int_>()(*static_cast<const np::int_ *>(value));
            } else if (value.isIntC()) {
                return hash<np::intc>()(*static_cast<const np::intc *>(value));
            } else if (value.isSize()) {
                return hash<np::Size>()(*static_cast<const np::Size *>(value));
            } else if (value.isFloat()) {
                return hash<np::float_>()(*static_cast<const np::float_ *>(value));
            } else if (value.isString()) {
                return hash<np::string_>()(*static_cast<const np::string_ *>(value));
            } else if (value.isUnicode()) {
                return hash<np::unicode_>()(*static_cast<const np::unicode_ *>(value));
            }
            throw runtime_error("Invalid type");
        }
    };
}// namespace std