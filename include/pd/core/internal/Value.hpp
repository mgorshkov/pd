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

            Value(np::bool_ value)
                : m_value{value} {
            }

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

            Value(const np::string_ &value)
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

            Value &operator=(np::bool_ value) {
                m_value = value;
                return *this;
            }

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

            friend inline Value operator+(const Value &value1, const Value &value2) {
                Value result{};
                if (value1.isInt()) {
                    if (value2.isInt()) {
                        result.m_value = *static_cast<const np::int_ *>(value1) + *static_cast<const np::int_ *>(value2);
                    } else if (value2.isSize()) {
                        result.m_value = *static_cast<const np::int_ *>(value1) + static_cast<np::int_>(*static_cast<const np::Size *>(value2));
                    } else if (value2.isIntC()) {
                        result.m_value = *static_cast<const np::int_ *>(value1) + *static_cast<const np::intc *>(value2);
                    } else if (value2.isFloat()) {
                        result.m_value = *static_cast<const np::int_ *>(value1) + *static_cast<const np::float_ *>(value2);
                    }
                } else if (value1.isIntC()) {
                    if (value2.isInt()) {
                        result.m_value = *static_cast<const np::intc *>(value1) + *static_cast<const np::int_ *>(value2);
                    } else if (value2.isSize()) {
                        result.m_value = *static_cast<const np::intc *>(value1) + static_cast<np::intc>(*static_cast<const np::Size *>(value2));
                    } else if (value2.isIntC()) {
                        result.m_value = *static_cast<const np::intc *>(value1) + *static_cast<const np::intc *>(value2);
                    } else if (value2.isFloat()) {
                        result.m_value = *static_cast<const np::intc *>(value1) + *static_cast<const np::float_ *>(value2);
                    }
                } else if (value1.isSize()) {
                    if (value2.isInt()) {
                        result.m_value = *static_cast<const np::Size *>(value1) + static_cast<np::Size>(*static_cast<const np::int_ *>(value2));
                    } else if (value2.isSize()) {
                        result.m_value = *static_cast<const np::Size *>(value1) + *static_cast<const np::Size *>(value2);
                    } else if (value2.isIntC()) {
                        result.m_value = *static_cast<const np::Size *>(value1) + static_cast<np::Size>(*static_cast<const np::intc *>(value2));
                    } else if (value2.isFloat()) {
                        result.m_value = *static_cast<const np::Size *>(value1) + *static_cast<const np::float_ *>(value2);
                    }
                } else if (value1.isFloat()) {
                    if (value2.isInt()) {
                        result.m_value = *static_cast<const np::float_ *>(value1) + static_cast<np::Size>(*static_cast<const np::int_ *>(value2));
                    } else if (value2.isSize()) {
                        result.m_value = *static_cast<const np::float_ *>(value1) + *static_cast<const np::Size *>(value2);
                    } else if (value2.isIntC()) {
                        result.m_value = *static_cast<const np::float_ *>(value1) + static_cast<np::Size>(*static_cast<const np::intc *>(value2));
                    } else if (value2.isFloat()) {
                        result.m_value = *static_cast<const np::float_ *>(value1) + *static_cast<const np::float_ *>(value2);
                    }
                } else if (value1.isString()) {
                    throw std::runtime_error("Invalid type");
                } else if (value1.isUnicode()) {
                    throw std::runtime_error("Invalid type");
                }
                return result;
            }

            friend inline Value operator-(const Value &value1, const Value &value2) {
                Value result{};
                if (value1.isInt()) {
                    if (value2.isInt()) {
                        result.m_value = *static_cast<const np::int_ *>(value1) - *static_cast<const np::int_ *>(value2);
                    } else if (value2.isSize()) {
                        result.m_value = *static_cast<const np::int_ *>(value1) - static_cast<np::int_>(*static_cast<const np::Size *>(value2));
                    } else if (value2.isIntC()) {
                        result.m_value = *static_cast<const np::int_ *>(value1) - *static_cast<const np::intc *>(value2);
                    } else if (value2.isFloat()) {
                        result.m_value = *static_cast<const np::int_ *>(value1) - *static_cast<const np::float_ *>(value2);
                    }
                } else if (value1.isIntC()) {
                    if (value2.isInt()) {
                        result.m_value = *static_cast<const np::intc *>(value1) - *static_cast<const np::int_ *>(value2);
                    } else if (value2.isSize()) {
                        result.m_value = *static_cast<const np::intc *>(value1) - static_cast<np::intc>(*static_cast<const np::Size *>(value2));
                    } else if (value2.isIntC()) {
                        result.m_value = *static_cast<const np::intc *>(value1) - *static_cast<const np::intc *>(value2);
                    } else if (value2.isFloat()) {
                        result.m_value = *static_cast<const np::intc *>(value1) - *static_cast<const np::float_ *>(value2);
                    }
                } else if (value1.isSize()) {
                    if (value2.isInt()) {
                        result.m_value = *static_cast<const np::Size *>(value1) - static_cast<np::Size>(*static_cast<const np::int_ *>(value2));
                    } else if (value2.isSize()) {
                        result.m_value = *static_cast<const np::Size *>(value1) - *static_cast<const np::Size *>(value2);
                    } else if (value2.isIntC()) {
                        result.m_value = *static_cast<const np::Size *>(value1) - static_cast<np::Size>(*static_cast<const np::intc *>(value2));
                    } else if (value2.isFloat()) {
                        result.m_value = *static_cast<const np::Size *>(value1) - *static_cast<const np::float_ *>(value2);
                    }
                } else if (value1.isFloat()) {
                    if (value2.isInt()) {
                        result.m_value = *static_cast<const np::float_ *>(value1) - static_cast<np::Size>(*static_cast<const np::int_ *>(value2));
                    } else if (value2.isSize()) {
                        result.m_value = *static_cast<const np::float_ *>(value1) - *static_cast<const np::Size *>(value2);
                    } else if (value2.isIntC()) {
                        result.m_value = *static_cast<const np::float_ *>(value1) - static_cast<np::Size>(*static_cast<const np::intc *>(value2));
                    } else if (value2.isFloat()) {
                        result.m_value = *static_cast<const np::float_ *>(value1) - *static_cast<const np::float_ *>(value2);
                    }
                } else if (value1.isString()) {
                    throw std::runtime_error("Invalid type");
                } else if (value1.isUnicode()) {
                    throw std::runtime_error("Invalid type");
                }
                return result;
            }

            friend inline Value operator*(const Value &value1, const Value &value2) {
                Value result{};
                if (value1.isInt()) {
                    if (value2.isInt()) {
                        result.m_value = *static_cast<const np::int_ *>(value1) * *static_cast<const np::int_ *>(value2);
                    } else if (value2.isSize()) {
                        result.m_value = *static_cast<const np::int_ *>(value1) * static_cast<np::int_>(*static_cast<const np::Size *>(value2));
                    } else if (value2.isIntC()) {
                        result.m_value = *static_cast<const np::int_ *>(value1) * *static_cast<const np::intc *>(value2);
                    } else if (value2.isFloat()) {
                        result.m_value = *static_cast<const np::int_ *>(value1) * *static_cast<const np::float_ *>(value2);
                    }
                } else if (value1.isIntC()) {
                    if (value2.isInt()) {
                        result.m_value = *static_cast<const np::intc *>(value1) * *static_cast<const np::int_ *>(value2);
                    } else if (value2.isSize()) {
                        result.m_value = *static_cast<const np::intc *>(value1) * static_cast<np::intc>(*static_cast<const np::Size *>(value2));
                    } else if (value2.isIntC()) {
                        result.m_value = *static_cast<const np::intc *>(value1) * *static_cast<const np::intc *>(value2);
                    } else if (value2.isFloat()) {
                        result.m_value = *static_cast<const np::intc *>(value1) * *static_cast<const np::float_ *>(value2);
                    }
                } else if (value1.isSize()) {
                    if (value2.isInt()) {
                        result.m_value = *static_cast<const np::Size *>(value1) * static_cast<np::Size>(*static_cast<const np::int_ *>(value2));
                    } else if (value2.isSize()) {
                        result.m_value = *static_cast<const np::Size *>(value1) * *static_cast<const np::Size *>(value2);
                    } else if (value2.isIntC()) {
                        result.m_value = *static_cast<const np::Size *>(value1) * static_cast<np::Size>(*static_cast<const np::intc *>(value2));
                    } else if (value2.isFloat()) {
                        result.m_value = *static_cast<const np::Size *>(value1) * *static_cast<const np::float_ *>(value2);
                    }
                } else if (value1.isFloat()) {
                    if (value2.isInt()) {
                        result.m_value = *static_cast<const np::float_ *>(value1) * static_cast<np::Size>(*static_cast<const np::int_ *>(value2));
                    } else if (value2.isSize()) {
                        result.m_value = *static_cast<const np::float_ *>(value1) * *static_cast<const np::Size *>(value2);
                    } else if (value2.isIntC()) {
                        result.m_value = *static_cast<const np::float_ *>(value1) * static_cast<np::Size>(*static_cast<const np::intc *>(value2));
                    } else if (value2.isFloat()) {
                        result.m_value = *static_cast<const np::float_ *>(value1) * *static_cast<const np::float_ *>(value2);
                    }
                } else if (value1.isString()) {
                    throw std::runtime_error("Invalid type");
                } else if (value1.isUnicode()) {
                    throw std::runtime_error("Invalid type");
                }
                return result;
            }

            friend inline Value operator/(const Value &value1, const Value &value2) {
                Value result{};
                if (value1.isInt()) {
                    if (value2.isInt()) {
                        result.m_value = *static_cast<const np::int_ *>(value1) / *static_cast<const np::int_ *>(value2);
                    } else if (value2.isSize()) {
                        result.m_value = *static_cast<const np::int_ *>(value1) / static_cast<np::int_>(*static_cast<const np::Size *>(value2));
                    } else if (value2.isIntC()) {
                        result.m_value = *static_cast<const np::int_ *>(value1) / *static_cast<const np::intc *>(value2);
                    } else if (value2.isFloat()) {
                        result.m_value = *static_cast<const np::int_ *>(value1) / *static_cast<const np::float_ *>(value2);
                    }
                } else if (value1.isIntC()) {
                    if (value2.isInt()) {
                        result.m_value = *static_cast<const np::intc *>(value1) / *static_cast<const np::int_ *>(value2);
                    } else if (value2.isSize()) {
                        result.m_value = *static_cast<const np::intc *>(value1) / static_cast<np::intc>(*static_cast<const np::Size *>(value2));
                    } else if (value2.isIntC()) {
                        result.m_value = *static_cast<const np::intc *>(value1) / *static_cast<const np::intc *>(value2);
                    } else if (value2.isFloat()) {
                        result.m_value = *static_cast<const np::intc *>(value1) / *static_cast<const np::float_ *>(value2);
                    }
                } else if (value1.isSize()) {
                    if (value2.isInt()) {
                        result.m_value = *static_cast<const np::Size *>(value1) / static_cast<np::Size>(*static_cast<const np::int_ *>(value2));
                    } else if (value2.isSize()) {
                        result.m_value = *static_cast<const np::Size *>(value1) / *static_cast<const np::Size *>(value2);
                    } else if (value2.isIntC()) {
                        result.m_value = *static_cast<const np::Size *>(value1) / static_cast<np::Size>(*static_cast<const np::intc *>(value2));
                    } else if (value2.isFloat()) {
                        result.m_value = *static_cast<const np::Size *>(value1) / *static_cast<const np::float_ *>(value2);
                    }
                } else if (value1.isFloat()) {
                    if (value2.isInt()) {
                        result.m_value = *static_cast<const np::float_ *>(value1) / static_cast<np::Size>(*static_cast<const np::int_ *>(value2));
                    } else if (value2.isSize()) {
                        result.m_value = *static_cast<const np::float_ *>(value1) / *static_cast<const np::Size *>(value2);
                    } else if (value2.isIntC()) {
                        result.m_value = *static_cast<const np::float_ *>(value1) / static_cast<np::Size>(*static_cast<const np::intc *>(value2));
                    } else if (value2.isFloat()) {
                        result.m_value = *static_cast<const np::float_ *>(value1) / *static_cast<const np::float_ *>(value2);
                    }
                } else if (value1.isString()) {
                    throw std::runtime_error("Invalid type");
                } else if (value1.isUnicode()) {
                    throw std::runtime_error("Invalid type");
                }
                return result;
            }

            Value &operator+=(const Value &another) {
                if (isInt()) {
                    if (another.isInt()) {
                        m_value = *static_cast<const np::int_ *>(*this) + *static_cast<const np::int_ *>(another);
                    } else if (another.isSize()) {
                        m_value = *static_cast<const np::int_ *>(*this) + static_cast<np::int_>(*static_cast<const np::Size *>(another));
                    } else if (another.isIntC()) {
                        m_value = *static_cast<const np::int_ *>(*this) + *static_cast<const np::intc *>(another);
                    } else if (another.isFloat()) {
                        m_value = *static_cast<const np::int_ *>(*this) + *static_cast<const np::float_ *>(another);
                    }
                } else if (isIntC()) {
                    if (another.isInt()) {
                        m_value = *static_cast<const np::intc *>(*this) + *static_cast<const np::int_ *>(another);
                    } else if (another.isSize()) {
                        m_value = *static_cast<const np::intc *>(*this) + static_cast<np::intc>(*static_cast<const np::Size *>(another));
                    } else if (another.isIntC()) {
                        m_value = *static_cast<const np::intc *>(*this) + *static_cast<const np::intc *>(another);
                    } else if (another.isFloat()) {
                        m_value = *static_cast<const np::intc *>(*this) + *static_cast<const np::float_ *>(another);
                    }
                } else if (isSize()) {
                    if (another.isInt()) {
                        m_value = *static_cast<const np::Size *>(*this) + static_cast<np::Size>(*static_cast<const np::int_ *>(another));
                    } else if (another.isSize()) {
                        m_value = *static_cast<const np::Size *>(*this) + *static_cast<const np::Size *>(another);
                    } else if (another.isIntC()) {
                        m_value = *static_cast<const np::Size *>(*this) + static_cast<np::Size>(*static_cast<const np::intc *>(another));
                    } else if (another.isFloat()) {
                        m_value = *static_cast<const np::Size *>(*this) + *static_cast<const np::float_ *>(another);
                    }
                } else if (isFloat()) {
                    if (another.isInt()) {
                        m_value = *static_cast<const np::float_ *>(*this) + static_cast<np::Size>(*static_cast<const np::int_ *>(another));
                    } else if (another.isSize()) {
                        m_value = *static_cast<const np::float_ *>(*this) + *static_cast<const np::Size *>(another);
                    } else if (another.isIntC()) {
                        m_value = *static_cast<const np::float_ *>(*this) + static_cast<np::Size>(*static_cast<const np::intc *>(another));
                    } else if (another.isFloat()) {
                        m_value = *static_cast<const np::float_ *>(*this) + *static_cast<const np::float_ *>(another);
                    }
                } else if (isString()) {
                    throw std::runtime_error("Invalid type");
                } else if (isUnicode()) {
                    throw std::runtime_error("Invalid type");
                }
                return *this;
            }

            [[nodiscard]] auto spaceshipInt(const Value &another) const {
                if (another.isInt()) {
                    if (*static_cast<const np::int_ *>(*this) == *static_cast<const np::int_ *>(another)) {
                        return std::strong_ordering::equal;
                    } else if (*static_cast<const np::int_ *>(*this) < *static_cast<const np::int_ *>(another)) {
                        return std::strong_ordering::less;
                    } else {
                        return std::strong_ordering::greater;
                    }
                }

                if (another.isSize()) {
                    if (*static_cast<const np::int_ *>(*this) == static_cast<np::int_>(*static_cast<const np::Size *>(another))) {
                        return std::strong_ordering::equal;
                    } else if (*static_cast<const np::Size *>(*this) < *static_cast<const np::Size *>(another)) {
                        return std::strong_ordering::less;
                    } else {
                        return std::strong_ordering::greater;
                    }
                }

                if (another.isIntC()) {
                    if (*static_cast<const np::int_ *>(*this) == static_cast<np::int_>(*static_cast<const np::intc *>(another))) {
                        return std::strong_ordering::equal;
                    } else if (*static_cast<const np::int_ *>(*this) < *static_cast<const np::intc *>(another)) {
                        return std::strong_ordering::less;
                    } else {
                        return std::strong_ordering::greater;
                    }
                }

                if (another.isFloat()) {
                    if (np::internal::element_equal(*static_cast<const np::float_ *>(*this), *static_cast<const np::float_ *>(another))) {
                        return std::strong_ordering::equal;
                    } else if (*static_cast<const np::float_ *>(*this) < *static_cast<const np::float_ *>(another)) {
                        return std::strong_ordering::less;
                    } else {
                        return std::strong_ordering::greater;
                    }
                }

                return std::strong_ordering::less;// int < other non-numerical types
            }

            [[nodiscard]] auto spaceshipIntC(const Value &another) const {
                if (another.isInt()) {
                    if (*static_cast<const np::intc *>(*this) == *static_cast<const np::int_ *>(another)) {
                        return std::strong_ordering::equal;
                    } else if (*static_cast<const np::intc *>(*this) < *static_cast<const np::int_ *>(another)) {
                        return std::strong_ordering::less;
                    } else {
                        return std::strong_ordering::greater;
                    }
                }

                if (another.isSize()) {
                    if (*static_cast<const np::Size *>(*this) == *static_cast<const np::Size *>(another)) {
                        return std::strong_ordering::equal;
                    } else if (*static_cast<const np::Size *>(*this) < *static_cast<const np::Size *>(another)) {
                        return std::strong_ordering::less;
                    } else {
                        return std::strong_ordering::greater;
                    }
                }

                if (another.isIntC()) {
                    if (*static_cast<const np::intc *>(*this) == *static_cast<const np::intc *>(another)) {
                        return std::strong_ordering::equal;
                    } else if (*static_cast<const np::intc *>(*this) < *static_cast<const np::intc *>(another)) {
                        return std::strong_ordering::less;
                    } else {
                        return std::strong_ordering::greater;
                    }
                }

                if (another.isFloat()) {
                    if (np::internal::element_equal(*static_cast<const np::float_ *>(*this), *static_cast<const np::float_ *>(another))) {
                        return std::strong_ordering::equal;
                    } else if (*static_cast<const np::float_ *>(*this) < *static_cast<const np::float_ *>(another)) {
                        return std::strong_ordering::less;
                    } else {
                        return std::strong_ordering::greater;
                    }
                }

                return std::strong_ordering::less;// int < other non-numerical types
            }

            [[nodiscard]] auto spaceshipSize(const Value &another) const {
                if (another.isInt()) {
                    if (*static_cast<const np::Size *>(*this) == static_cast<np::Size>(*static_cast<const np::int_ *>(another))) {
                        return std::strong_ordering::equal;
                    } else if (*static_cast<const np::Size *>(*this) < *static_cast<const np::Size *>(another)) {
                        return std::strong_ordering::less;
                    } else {
                        return std::strong_ordering::greater;
                    }
                }

                if (another.isSize()) {
                    if (*static_cast<const np::Size *>(*this) == *static_cast<const np::Size *>(another)) {
                        return std::strong_ordering::equal;
                    } else if (*static_cast<const np::Size *>(*this) < *static_cast<const np::Size *>(another)) {
                        return std::strong_ordering::less;
                    } else {
                        return std::strong_ordering::greater;
                    }
                }

                if (another.isIntC()) {
                    if (*static_cast<const np::Size *>(*this) == *static_cast<const np::Size *>(another)) {
                        return std::strong_ordering::equal;
                    } else if (*static_cast<const np::Size *>(*this) < *static_cast<const np::Size *>(another)) {
                        return std::strong_ordering::less;
                    } else {
                        return std::strong_ordering::greater;
                    }
                }

                if (another.isFloat()) {
                    if (np::internal::element_equal(*static_cast<const np::float_ *>(*this), *static_cast<const np::float_ *>(another))) {
                        return std::strong_ordering::equal;
                    } else if (*static_cast<const np::float_ *>(*this) < *static_cast<const np::float_ *>(another)) {
                        return std::strong_ordering::less;
                    } else {
                        return std::strong_ordering::greater;
                    }
                }

                return std::strong_ordering::less;// int < other types
            }

            [[nodiscard]] auto spaceshipFloat(const Value &another) const {
                if (another.isFloat()) {
                    if (np::internal::element_equal(*static_cast<const np::float_ *>(*this), *static_cast<const np::float_ *>(another))) {
                        return std::strong_ordering::equal;
                    } else if (*static_cast<const np::float_ *>(*this) < *static_cast<const np::float_ *>(another)) {
                        return std::strong_ordering::less;
                    } else {
                        return std::strong_ordering::greater;
                    }
                }

                if (another.isInt()) {
                    if (np::internal::element_equal(*static_cast<const np::float_ *>(*this), *static_cast<const np::float_ *>(another))) {
                        return std::strong_ordering::equal;
                    } else if (*static_cast<const np::Size *>(*this) < *static_cast<const np::float_ *>(another)) {
                        return std::strong_ordering::less;
                    } else {
                        return std::strong_ordering::greater;
                    }
                }

                if (another.isSize()) {
                    if (np::internal::element_equal(*static_cast<const np::float_ *>(*this), *static_cast<const np::float_ *>(another))) {
                        return std::strong_ordering::equal;
                    } else if (*static_cast<const np::Size *>(*this) < *static_cast<const np::float_ *>(another)) {
                        return std::strong_ordering::less;
                    } else {
                        return std::strong_ordering::greater;
                    }
                }

                if (another.isIntC()) {
                    if (np::internal::element_equal(*static_cast<const np::float_ *>(*this), *static_cast<const np::float_ *>(another))) {
                        return std::strong_ordering::equal;
                    } else if (*static_cast<const np::Size *>(*this) < *static_cast<const np::float_ *>(another)) {
                        return std::strong_ordering::less;
                    } else {
                        return std::strong_ordering::greater;
                    }
                }

                return std::strong_ordering::greater;
            }

            [[nodiscard]] auto spaceshipString(const Value &another) const {
                if (another.isString()) {
                    if (*static_cast<const np::string_ *>(*this) == *static_cast<const np::string_ *>(another)) {
                        return std::strong_ordering::equal;
                    } else if (*static_cast<const np::string_ *>(*this) < *static_cast<const np::string_ *>(another)) {
                        return std::strong_ordering::less;
                    }
                }

                return std::strong_ordering::greater;
            }

            [[nodiscard]] auto spaceshipUnicode(const Value &another) const {
                if (another.isUnicode()) {
                    if (*static_cast<const np::unicode_ *>(*this) == *static_cast<const np::unicode_ *>(another)) {
                        return std::strong_ordering::equal;
                    } else if (*static_cast<const np::unicode_ *>(*this) < *static_cast<const np::unicode_ *>(another)) {
                        return std::strong_ordering::less;
                    }
                }

                return std::strong_ordering::greater;
            }

            auto operator<=>(const Value &another) const {
                if (this == &another)
                    return std::strong_ordering::equal;

                if (isInt()) {
                    return spaceshipInt(another);
                }

                if (isIntC()) {
                    return spaceshipIntC(another);
                }

                if (isSize()) {
                    return spaceshipSize(another);
                }

                if (isFloat()) {
                    return spaceshipFloat(another);
                }

                if (isString()) {
                    return spaceshipString(another);
                }

                if (isUnicode()) {
                    return spaceshipUnicode(another);
                }

                return std::strong_ordering::equal;
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
                    return false;
                } else if (isIntC()) {
                    if (another.isInt()) {
                        return *static_cast<const np::intc *>(*this) == *static_cast<const np::int_ *>(another);
                    } else if (another.isSize()) {
                        return *static_cast<const np::intc *>(*this) == static_cast<np::intc>(*static_cast<const np::Size *>(another));
                    } else if (another.isIntC()) {
                        return *static_cast<const np::intc *>(*this) == *static_cast<const np::intc *>(another);
                    }
                    return false;
                } else if (isSize()) {
                    if (another.isInt()) {
                        return *static_cast<const np::Size *>(*this) == static_cast<np::Size>(*static_cast<const np::int_ *>(another));
                    } else if (another.isSize()) {
                        return *static_cast<const np::Size *>(*this) == *static_cast<const np::Size *>(another);
                    } else if (another.isIntC()) {
                        return *static_cast<const np::Size *>(*this) == static_cast<np::Size>(*static_cast<const np::intc *>(another));
                    }
                    return false;
                } else if (isFloat()) {
                    return another.isFloat() && np::internal::element_equal(*static_cast<const np::float_ *>(*this), *static_cast<const np::float_ *>(another));
                } else if (isString()) {
                    return another.isString() && *static_cast<const np::string_ *>(*this) == *static_cast<const np::string_ *>(another);
                } else if (isUnicode()) {
                    return another.isUnicode() && *static_cast<const np::unicode_ *>(*this) == *static_cast<const np::unicode_ *>(another);
                }
                return true;
            }

            bool operator==(np::intc another) const {
                if (isIntC()) {
                    return *static_cast<const np::intc *>(*this) == another;
                }
                return false;
            }

            bool operator==(np::int_ another) const {
                if (isInt()) {
                    return *static_cast<const np::int_ *>(*this) == another;
                }
                return false;
            }

            bool operator==(np::Size another) const {
                if (isSize()) {
                    return *static_cast<const np::Size *>(*this) == another;
                }
                return false;
            }

            bool operator==(np::float_ another) const {
                if (isFloat()) {
                    return *static_cast<const np::float_ *>(*this) == another;
                }
                return false;
            }

            bool operator==(const np::string_ &another) {
                if (isString()) {
                    return *static_cast<const np::string_ *>(*this) == another;
                }
                return false;
            }

            bool operator==(const np::unicode_ &another) const {
                if (isUnicode()) {
                    return *static_cast<const np::unicode_ *>(*this) == another;
                }
                return false;
            }

            bool operator!=(const Value &another) const {
                return !(*this == another);
            }

            [[nodiscard]] bool isBool() const {
                return std::holds_alternative<np::bool_>(m_value);
            }

            explicit operator const np::bool_ *() const {
                return std::get_if<np::bool_>(&m_value);
            }

            explicit operator np::bool_ *() {
                return std::get_if<np::bool_>(&m_value);
            }

            explicit operator np::bool_() const {
                auto valueBool = std::get_if<np::bool_>(&m_value);
                if (valueBool) {
                    return *valueBool;
                }
                auto valueIntc = std::get_if<np::intc>(&m_value);
                if (valueIntc) {
                    return *valueIntc != 0;
                }
                auto valueInt = std::get_if<np::int_>(&m_value);
                if (valueInt) {
                    return *valueInt != 0;
                }
                auto valueSize = std::get_if<np::Size>(&m_value);
                if (valueSize) {
                    return *valueSize != 0;
                }
                throw std::runtime_error("Invalid value");
            }

            [[nodiscard]] bool isIntC() const {
                return std::holds_alternative<np::intc>(m_value);
            }

            explicit operator const np::intc *() const {
                return std::get_if<np::intc>(&m_value);
            }

            explicit operator np::intc *() {
                return std::get_if<np::intc>(&m_value);
            }

            explicit operator np::intc() const {
                auto value = std::get_if<np::intc>(&m_value);
                if (value) {
                    return *value;
                }
                throw std::runtime_error("Invalid value");
            }

            [[nodiscard]] bool isInt() const {
                return std::holds_alternative<np::int_>(m_value);
            }

            explicit operator const np::int_ *() const {
                return std::get_if<np::int_>(&m_value);
            }

            explicit operator np::int_ *() {
                return std::get_if<np::int_>(&m_value);
            }

            explicit operator np::int_() const {
                auto valueInt = std::get_if<np::int_>(&m_value);
                if (valueInt) {
                    return *valueInt;
                }
                auto valueIntc = std::get_if<np::intc>(&m_value);
                if (valueIntc) {
                    return *valueIntc;
                }
                throw std::runtime_error("Invalid value");
            }

            [[nodiscard]] bool isSize() const {
                return std::holds_alternative<np::Size>(m_value);
            }

            explicit operator const np::Size *() const {
                return std::get_if<np::Size>(&m_value);
            }

            explicit operator np::Size *() {
                return std::get_if<np::Size>(&m_value);
            }

            explicit operator np::Size() const {
                auto valueSize = std::get_if<np::Size>(&m_value);
                if (valueSize) {
                    return *valueSize;
                }
                auto valueInt = std::get_if<np::int_>(&m_value);
                if (valueInt) {
                    return *valueInt;
                }
                auto valueIntc = std::get_if<np::intc>(&m_value);
                if (valueIntc) {
                    return *valueIntc;
                }
                throw std::runtime_error("Invalid value");
            }

            [[nodiscard]] bool isFloat() const {
                return std::holds_alternative<np::float_>(m_value);
            }

            explicit operator const np::float_ *() const {
                return std::get_if<np::float_>(&m_value);
            }

            explicit operator np::float_ *() {
                return std::get_if<np::float_>(&m_value);
            }

            explicit operator np::float_() const {
                auto valueFloat = std::get_if<np::float_>(&m_value);
                if (valueFloat) {
                    return *valueFloat;
                }
                auto valueSize = std::get_if<np::Size>(&m_value);
                if (valueSize) {
                    return *valueSize;
                }
                auto valueInt = std::get_if<np::int_>(&m_value);
                if (valueInt) {
                    return static_cast<np::float_>(*valueInt);
                }
                auto valueIntc = std::get_if<np::intc>(&m_value);
                if (valueIntc) {
                    return *valueIntc;
                }
                throw std::runtime_error("Invalid value");
            }

            [[nodiscard]] bool isString() const {
                return std::holds_alternative<np::string_>(m_value);
            }

            explicit operator const np::string_ *() const {
                return std::get_if<np::string_>(&m_value);
            }

            explicit operator np::string_ *() {
                return std::get_if<np::string_>(&m_value);
            }

            explicit operator np::string_() const {
                auto value = std::get_if<np::string_>(&m_value);
                if (value) {
                    return *value;
                }
                throw std::runtime_error("Invalid value");
            }

            [[nodiscard]] bool isUnicode() const {
                return std::holds_alternative<np::unicode_>(m_value);
            }

            explicit operator const np::unicode_ *() const {
                return std::get_if<np::unicode_>(&m_value);
            }

            explicit operator np::unicode_ *() {
                return std::get_if<np::unicode_>(&m_value);
            }

            explicit operator np::unicode_() const {
                auto value = std::get_if<np::unicode_>(&m_value);
                if (value) {
                    return *value;
                }
                throw std::runtime_error("Invalid value");
            }

            [[nodiscard]] bool empty() const {
                return std::holds_alternative<std::monostate>(m_value);
            }

            friend std::ostream &operator<<(std::ostream &stream, const Value &value) {
                if (const auto *intValue = std::get_if<np::int_>(&value.m_value)) {
                    stream << *intValue;
                } else if (const auto *intcValue = std::get_if<np::intc>(&value.m_value)) {
                    stream << *intcValue;
                } else if (const auto *boolValue = std::get_if<np::bool_>(&value.m_value)) {
                    stream << *boolValue;
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
                } else {
                    stream << "Unknown type";
                }
                return stream;
            }

        private:
            using ValueTypes = std::variant<std::monostate,
                                            np::object,
                                            np::bool_,
                                            np::intc,
                                            np::int_,
                                            np::Size,
                                            np::float_,
                                            np::string_,
                                            np::unicode_>;
            ValueTypes m_value;
        };

        inline Value operator+(const Value &value, np::intc number) {
            Value result{};
            if (value.isInt()) {
                result = Value{*static_cast<const np::int_ *>(value) + number};
            } else if (value.isIntC()) {
                result = Value{*static_cast<const np::intc *>(value) + number};
            } else if (value.isSize()) {
                result = Value{*static_cast<const np::Size *>(value) + static_cast<np::Size>(number)};
            } else if (value.isFloat()) {
                result = Value{*static_cast<const np::float_ *>(value) + static_cast<np::Size>(number)};
            } else if (value.isString()) {
                throw std::runtime_error("Invalid type");
            } else if (value.isUnicode()) {
                throw std::runtime_error("Invalid type");
            }
            return result;
        }

        inline Value operator+(np::intc number, const Value &value) {
            return value + number;
        }

        inline Value operator+(const Value &value, np::int_ number) {
            Value result{};
            if (value.isInt()) {
                result = Value{*static_cast<const np::int_ *>(value) + number};
            } else if (value.isIntC()) {
                result = Value{*static_cast<const np::intc *>(value) + number};
            } else if (value.isSize()) {
                result = Value{*static_cast<const np::Size *>(value) + static_cast<np::Size>(number)};
            } else if (value.isFloat()) {
                result = Value{*static_cast<const np::float_ *>(value) + static_cast<np::Size>(number)};
            } else if (value.isString()) {
                throw std::runtime_error("Invalid type");
            } else if (value.isUnicode()) {
                throw std::runtime_error("Invalid type");
            }
            return result;
        }

        inline Value operator+(np::int_ number, const Value &value) {
            return value + number;
        }

        inline Value operator+(const Value &value, np::Size number) {
            Value result{};
            if (value.isInt()) {
                result = Value{*static_cast<const np::int_ *>(value) + static_cast<np::int_>(number)};
            } else if (value.isIntC()) {
                result = Value{*static_cast<const np::intc *>(value) + static_cast<np::intc>(number)};
            } else if (value.isSize()) {
                result = Value{*static_cast<const np::Size *>(value) + number};
            } else if (value.isFloat()) {
                result = Value{*static_cast<const np::float_ *>(value) + number};
            } else if (value.isString()) {
                throw std::runtime_error("Invalid type");
            } else if (value.isUnicode()) {
                throw std::runtime_error("Invalid type");
            }
            return result;
        }

        inline Value operator+(np::Size number, const Value &value) {
            return value + number;
        }

        inline Value operator+(const Value &value, np::float_ number) {
            Value result{};
            if (value.isInt()) {
                result = Value{*static_cast<const np::int_ *>(value) + number};
            } else if (value.isIntC()) {
                result = Value{*static_cast<const np::intc *>(value) + number};
            } else if (value.isSize()) {
                result = Value{*static_cast<const np::Size *>(value) + number};
            } else if (value.isFloat()) {
                result = Value{*static_cast<const np::float_ *>(value) + number};
            } else if (value.isString()) {
                throw std::runtime_error("Invalid type");
            } else if (value.isUnicode()) {
                throw std::runtime_error("Invalid type");
            }
            return result;
        }

        inline Value operator+(np::float_ number, const Value &value) {
            return value + number;
        }

        inline Value operator-(const Value &value, np::intc number) {
            Value result{};
            if (value.isInt()) {
                result = Value{*static_cast<const np::int_ *>(value) - number};
            } else if (value.isIntC()) {
                result = Value{*static_cast<const np::intc *>(value) - number};
            } else if (value.isSize()) {
                result = Value{*static_cast<const np::Size *>(value) - static_cast<np::Size>(number)};
            } else if (value.isFloat()) {
                result = Value{*static_cast<const np::float_ *>(value) - static_cast<np::Size>(number)};
            } else if (value.isString()) {
                throw std::runtime_error("Invalid type");
            } else if (value.isUnicode()) {
                throw std::runtime_error("Invalid type");
            }
            return result;
        }

        inline Value operator-(np::intc number, const Value &value) {
            return value - number;
        }

        inline Value operator-(const Value &value, np::int_ number) {
            Value result{};
            if (value.isInt()) {
                result = Value{*static_cast<const np::int_ *>(value) - number};
            } else if (value.isIntC()) {
                result = Value{*static_cast<const np::intc *>(value) - number};
            } else if (value.isSize()) {
                result = Value{*static_cast<const np::Size *>(value) - static_cast<np::Size>(number)};
            } else if (value.isFloat()) {
                result = Value{*static_cast<const np::float_ *>(value) - static_cast<np::Size>(number)};
            } else if (value.isString()) {
                throw std::runtime_error("Invalid type");
            } else if (value.isUnicode()) {
                throw std::runtime_error("Invalid type");
            }
            return result;
        }

        inline Value operator-(np::int_ number, const Value &value) {
            return value + number;
        }

        inline Value operator-(const Value &value, np::Size number) {
            Value result{};
            if (value.isInt()) {
                result = Value{*static_cast<const np::int_ *>(value) - static_cast<np::int_>(number)};
            } else if (value.isIntC()) {
                result = Value{*static_cast<const np::intc *>(value) - static_cast<np::intc>(number)};
            } else if (value.isSize()) {
                result = Value{*static_cast<const np::Size *>(value) - number};
            } else if (value.isFloat()) {
                result = Value{*static_cast<const np::float_ *>(value) - number};
            } else if (value.isString()) {
                throw std::runtime_error("Invalid type");
            } else if (value.isUnicode()) {
                throw std::runtime_error("Invalid type");
            }
            return result;
        }

        inline Value operator-(np::Size number, const Value &value) {
            return value - number;
        }

        inline Value operator-(const Value &value, np::float_ number) {
            Value result{};
            if (value.isInt()) {
                result = Value{*static_cast<const np::int_ *>(value) - number};
            } else if (value.isIntC()) {
                result = Value{*static_cast<const np::intc *>(value) - number};
            } else if (value.isSize()) {
                result = Value{*static_cast<const np::Size *>(value) - number};
            } else if (value.isFloat()) {
                result = Value{*static_cast<const np::float_ *>(value) - number};
            } else if (value.isString()) {
                throw std::runtime_error("Invalid type");
            } else if (value.isUnicode()) {
                throw std::runtime_error("Invalid type");
            }
            return result;
        }

        inline Value operator-(np::float_ number, const Value &value) {
            return value - number;
        }

        inline Value operator*(const Value &value, np::intc number) {
            Value result{};
            if (value.isInt()) {
                result = Value{*static_cast<const np::int_ *>(value) * number};
            } else if (value.isIntC()) {
                result = Value{*static_cast<const np::intc *>(value) * number};
            } else if (value.isSize()) {
                result = Value{*static_cast<const np::Size *>(value) * static_cast<np::Size>(number)};
            } else if (value.isFloat()) {
                result = Value{*static_cast<const np::float_ *>(value) * static_cast<np::Size>(number)};
            } else if (value.isString()) {
                throw std::runtime_error("Invalid type");
            } else if (value.isUnicode()) {
                throw std::runtime_error("Invalid type");
            }
            return result;
        }

        inline Value operator*(np::intc number, const Value &value) {
            return value * number;
        }

        inline Value operator*(const Value &value, np::int_ number) {
            Value result{};
            if (value.isInt()) {
                result = Value{*static_cast<const np::int_ *>(value) * number};
            } else if (value.isIntC()) {
                result = Value{*static_cast<const np::intc *>(value) * number};
            } else if (value.isSize()) {
                result = Value{*static_cast<const np::Size *>(value) * static_cast<np::Size>(number)};
            } else if (value.isFloat()) {
                result = Value{*static_cast<const np::float_ *>(value) * static_cast<np::Size>(number)};
            } else if (value.isString()) {
                throw std::runtime_error("Invalid type");
            } else if (value.isUnicode()) {
                throw std::runtime_error("Invalid type");
            }
            return result;
        }

        inline Value operator*(np::int_ number, const Value &value) {
            return value * number;
        }

        inline Value operator*(const Value &value, np::Size number) {
            Value result{};
            if (value.isInt()) {
                result = Value{*static_cast<const np::int_ *>(value) * static_cast<np::int_>(number)};
            } else if (value.isIntC()) {
                result = Value{*static_cast<const np::intc *>(value) * static_cast<np::intc>(number)};
            } else if (value.isSize()) {
                result = Value{*static_cast<const np::Size *>(value) * number};
            } else if (value.isFloat()) {
                result = Value{*static_cast<const np::float_ *>(value) * number};
            } else if (value.isString()) {
                throw std::runtime_error("Invalid type");
            } else if (value.isUnicode()) {
                throw std::runtime_error("Invalid type");
            }
            return result;
        }

        inline Value operator*(np::Size number, const Value &value) {
            return value * number;
        }

        inline Value operator*(const Value &value, np::float_ number) {
            Value result{};
            if (value.isInt()) {
                result = Value{*static_cast<const np::int_ *>(value) * number};
            } else if (value.isIntC()) {
                result = Value{*static_cast<const np::intc *>(value) * number};
            } else if (value.isSize()) {
                result = Value{*static_cast<const np::Size *>(value) * number};
            } else if (value.isFloat()) {
                result = Value{*static_cast<const np::float_ *>(value) * number};
            } else if (value.isString()) {
                throw std::runtime_error("Invalid type");
            } else if (value.isUnicode()) {
                throw std::runtime_error("Invalid type");
            }
            return result;
        }

        inline Value operator*(np::float_ number, const Value &value) {
            return value * number;
        }

        inline Value operator/(const Value &value, np::intc number) {
            Value result{};
            if (value.isInt()) {
                result = Value{*static_cast<const np::int_ *>(value) / number};
            } else if (value.isIntC()) {
                result = Value{*static_cast<const np::intc *>(value) / number};
            } else if (value.isSize()) {
                result = Value{*static_cast<const np::Size *>(value) / static_cast<np::Size>(number)};
            } else if (value.isFloat()) {
                result = Value{*static_cast<const np::float_ *>(value) / static_cast<np::Size>(number)};
            } else if (value.isString()) {
                throw std::runtime_error("Invalid type");
            } else if (value.isUnicode()) {
                throw std::runtime_error("Invalid type");
            }
            return result;
        }

        inline Value operator/(np::intc number, const Value &value) {
            return value / number;
        }

        inline Value operator/(const Value &value, np::int_ number) {
            Value result{};
            if (value.isInt()) {
                result = Value{*static_cast<const np::int_ *>(value) / number};
            } else if (value.isIntC()) {
                result = Value{*static_cast<const np::intc *>(value) / number};
            } else if (value.isSize()) {
                result = Value{*static_cast<const np::Size *>(value) / static_cast<np::Size>(number)};
            } else if (value.isFloat()) {
                result = Value{*static_cast<const np::float_ *>(value) / static_cast<np::Size>(number)};
            } else if (value.isString()) {
                throw std::runtime_error("Invalid type");
            } else if (value.isUnicode()) {
                throw std::runtime_error("Invalid type");
            }
            return result;
        }

        inline Value operator/(np::int_ number, const Value &value) {
            return value / number;
        }

        inline Value operator/(const Value &value, np::Size number) {
            Value result{};
            if (value.isInt()) {
                result = Value{*static_cast<const np::int_ *>(value) / static_cast<np::int_>(number)};
            } else if (value.isIntC()) {
                result = Value{*static_cast<const np::intc *>(value) / static_cast<np::intc>(number)};
            } else if (value.isSize()) {
                result = Value{*static_cast<const np::Size *>(value) / number};
            } else if (value.isFloat()) {
                result = Value{*static_cast<const np::float_ *>(value) / number};
            } else if (value.isString()) {
                throw std::runtime_error("Invalid type");
            } else if (value.isUnicode()) {
                throw std::runtime_error("Invalid type");
            }
            return result;
        }

        inline Value operator/(np::Size number, const Value &value) {
            return value / number;
        }

        inline Value operator/(const Value &value, np::float_ number) {
            Value result{};
            if (value.isInt()) {
                result = Value{*static_cast<const np::int_ *>(value) / number};
            } else if (value.isIntC()) {
                result = Value{*static_cast<const np::intc *>(value) / number};
            } else if (value.isSize()) {
                result = Value{*static_cast<const np::Size *>(value) / number};
            } else if (value.isFloat()) {
                result = Value{*static_cast<const np::float_ *>(value) / number};
            } else if (value.isString()) {
                throw std::runtime_error("Invalid type");
            } else if (value.isUnicode()) {
                throw std::runtime_error("Invalid type");
            }
            return result;
        }

        inline Value operator/(np::float_ number, const Value &value) {
            return value / number;
        }

    }// namespace internal
}// namespace pd

inline np::float_ sqrt(const pd::internal::Value &value) {
    if (value.isFloat()) {
        return std::sqrt(static_cast<np::float_>(value));
    }
    throw std::runtime_error("Invalid type");
}

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