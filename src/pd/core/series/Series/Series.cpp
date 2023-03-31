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

#include <pd/core/internal/Indexing.hpp>
#include <pd/core/series/Series/Series.hpp>

namespace pd {
    Series::Series(const internal::Array &data, const std::vector<internal::Value> &index, const internal::Value &name)
        : m_data{data}, m_index{index, data.size()}, m_name{name} {
        np::Shape shape = m_data.shape();
        if (shape.size() != 1) {
            throw std::runtime_error("Only 1D arrays supported");
        }
        m_shape = shape;
        m_size = shape.calcSizeByShape();
        if (!m_index.empty() && m_index.size() != m_size) {
            throw std::runtime_error("Index has an invalid size");
        }
    }

    Series::Series(internal::Array &&data, const std::vector<internal::Value> &index, const internal::Value &name)
        : m_data{std::move(data)}, m_index{index, data.size()}, m_name{name} {
        np::Shape shape = m_data.shape();
        if (shape.size() != 1) {
            throw std::runtime_error("Only 1D arrays supported");
        }
        m_shape = shape;
        m_size = shape.calcSizeByShape();
        if (!m_index.empty() && m_index.size() != m_size) {
            throw std::runtime_error("Index has an invalid size");
        }
    }

    Series::Series(const np::Array<np::int_> &data, const std::vector<internal::Value> &index, const internal::Value &name)
        : Series{internal::Array{data}, index, name} {
    }

    Series::Series(np::Array<np::int_> &&data, const std::vector<internal::Value> &index, const internal::Value &name)
        : Series{internal::Array{data}, index, name} {
    }

    Series::Series(const np::Array<np::float_> &data, const std::vector<internal::Value> &index, const internal::Value &name)
        : Series{internal::Array{data}, index, name} {
    }

    Series::Series(np::Array<np::float_> &&data, const std::vector<internal::Value> &index, const internal::Value &name)
        : Series{internal::Array{data}, index, name} {
    }

    Series::Series(const np::Array<np::string_> &data, const std::vector<internal::Value> &index, const internal::Value &name)
        : Series{internal::Array{data}, index, name} {
    }

    Series::Series(np::Array<np::string_> &&data, const std::vector<internal::Value> &index, const internal::Value &name)
        : Series{internal::Array{data}, index, name} {
    }

    Series::Series(const np::Array<np::unicode_> &data, const std::vector<internal::Value> &index, const internal::Value &name)
        : Series{internal::Array{data}, index, name} {
    }

    Series::Series(np::Array<np::unicode_> &&data, const std::vector<internal::Value> &index, const internal::Value &name)
        : Series{internal::Array{data}, index, name} {
    }

    Series::Series(const internal::Array &data, const std::vector<internal::Value> &index)
        : Series{data, index, internal::Value{}} {
    }

    Series::Series(const np::Array<np::int_> &data, const std::vector<internal::Value> &index)
        : Series{internal::Array{data}, index, internal::Value{}} {
    }

    Series::Series(np::Array<np::int_> &&data, const std::vector<internal::Value> &index)
        : Series{internal::Array{data}, index, internal::Value{}} {
    }

    Series::Series(const np::Array<np::float_> &data, const std::vector<internal::Value> &index)
        : Series{internal::Array{data}, index, internal::Value{}} {
    }

    Series::Series(np::Array<np::float_> &&data, const std::vector<internal::Value> &index)
        : Series{internal::Array{data}, index, internal::Value{}} {
    }

    Series::Series(const np::Array<np::string_> &data, const std::vector<internal::Value> &index)
        : Series{internal::Array{data}, index, internal::Value{}} {
    }

    Series::Series(np::Array<np::string_> &&data, const std::vector<internal::Value> &index)
        : Series{internal::Array{data}, index, internal::Value{}} {
    }

    Series::Series(const np::Array<np::unicode_> &data, const std::vector<internal::Value> &index)
        : Series{internal::Array{data}, index, internal::Value{}} {
    }

    Series::Series(np::Array<np::unicode_> &&data, const std::vector<internal::Value> &index)
        : Series{internal::Array{data}, index, internal::Value{}} {
    }

    Series::Series(const internal::Array &data, const internal::Value &name)
        : Series{data, std::vector<internal::Value>{}, name} {
    }

    Series::Series(internal::Array &&data, const internal::Value &name)
        : Series{std::move(data), std::vector<internal::Value>{}, name} {
    }

    Series::Series(const np::Array<np::int_> &data, const internal::Value &name)
        : Series{internal::Array{data}, std::vector<internal::Value>{}, name} {
    }

    Series::Series(np::Array<np::int_> &&data, const internal::Value &name)
        : Series{internal::Array{data}, std::vector<internal::Value>{}, name} {
    }

    Series::Series(const np::Array<np::float_> &data, const internal::Value &name)
        : Series{internal::Array{data}, std::vector<internal::Value>{}, name} {
    }

    Series::Series(np::Array<np::float_> &&data, const internal::Value &name)
        : Series{internal::Array{data}, std::vector<internal::Value>{}, name} {
    }

    Series::Series(const np::Array<np::string_> &data, const internal::Value &name)
        : Series{internal::Array{data}, std::vector<internal::Value>{}, name} {
    }

    Series::Series(np::Array<np::string_> &&data, const internal::Value &name)
        : Series{internal::Array{data}, std::vector<internal::Value>{}, name} {
    }

    Series::Series(const np::Array<np::unicode_> &data, const internal::Value &name)
        : Series{internal::Array{data}, std::vector<internal::Value>{}, name} {
    }

    Series::Series(np::Array<np::unicode_> &&data, const internal::Value &name)
        : Series{internal::Array{data}, std::vector<internal::Value>{}, name} {
    }

    bool Series::operator==(const Series &other) const {
        if (this != &other) {
            return m_data == other.m_data && m_index == other.m_index && m_name == other.m_name && m_shape == other.m_shape;
        }
        return true;
    }

    internal::Value Series::name() const {
        return m_name;
    }

    internal::Index Series::index() const {
        return m_index;
    }

    np::Size Series::size() const {
        return m_size;
    }

    np::Shape Series::shape() const {
        return m_shape;
    }

    std::string Series::dtype() const {
        if (m_data.isIntArray()) {
            return "int64";
        } else if (m_data.isFloatArray()) {
            return "float64";
        } else if (m_data.isStringArray()) {
            return "str";
        } else if (m_data.isUnicodeArray()) {
            return "unicode";
        }
        return "Unknown";
    }

    const internal::Array &Series::values() const {
        return m_data;
    }

    internal::Array &Series::values() {
        return m_data;
    }

    void Series::set(np::Size row, const internal::Value &value) {
        if (m_data.isIntArray()) {
            np::Array<np::int_> *array = m_data;
            array->set(row, *static_cast<const np::int_ *>(value));
        } else if (m_data.isFloatArray()) {
            np::Array<np::float_> *array = m_data;
            if (value.isFloat()) {
                array->set(row, *static_cast<const np::float_ *>(value));
            } else if (value.isInt()) {
                array->set(row, static_cast<np::float_>(*static_cast<const np::int_ *>(value)));
            } else {
                throw std::runtime_error("Invalid value type");
            }
        } else if (m_data.isStringArray()) {
            np::Array<np::string_> *array = m_data;
            array->set(row, *static_cast<const np::string_ *>(value));
        } else if (m_data.isUnicodeArray()) {
            np::Array<np::unicode_> *array = m_data;
            array->set(row, *static_cast<const np::unicode_ *>(value));
        } else {
            throw std::runtime_error("Invalid value type");
        }
    }

    const internal::Value &Series::at(np::Size row) const {
        if (m_data.isIntArray()) {
            const np::Array<np::int_> *array = m_data;
            m_value = array->get(row);
        } else if (m_data.isFloatArray()) {
            const np::Array<np::float_> *array = m_data;
            m_value = array->get(row);
        } else if (m_data.isStringArray()) {
            const np::Array<np::string_> *array = m_data;
            m_value = array->get(row);
        } else if (m_data.isUnicodeArray()) {
            const np::Array<np::unicode_> *array = m_data;
            m_value = array->get(row);
        } else {
            throw std::runtime_error("Invalid value type");
        }
        return m_value;
    }

    internal::Value &Series::at(np::Size row) {
        if (m_data.isIntArray()) {
            np::Array<np::int_> *array = m_data;
            m_value = array->get(row);
        } else if (m_data.isFloatArray()) {
            np::Array<np::float_> *array = m_data;
            m_value = array->get(row);
        } else if (m_data.isStringArray()) {
            np::Array<np::string_> *array = m_data;
            m_value = array->get(row);
        } else if (m_data.isUnicodeArray()) {
            np::Array<np::unicode_> *array = m_data;
            m_value = array->get(row);
        } else {
            throw std::runtime_error("Invalid value type");
        }
        return m_value;
    }

    internal::Value &Series::iloc(np::Size i) {
        return at(i);
    }

    const internal::Value &Series::iloc(np::Size i) const {
        return at(i);
    }

    Series Series::slicing1(const std::string &cond) const {
        auto colonPos = cond.find(':');
        if (colonPos == std::string::npos) {
            np::Size i = std::stoi(cond);

            if (i >= size()) {
                throw std::runtime_error("Index " + std::to_string(i) + " out of bounds");
            }
            const auto &element = iloc(i);
            if (element.isInt()) {
                auto array = np::Array<np::int_>{*static_cast<const np::int_ *>(element)};
                return Series{array, m_name};
            } else if (element.isFloat()) {
                auto array = np::Array<np::float_>{*static_cast<const np::float_ *>(element)};
                return Series{array, m_name};
            } else if (element.isString()) {
                auto array = np::Array<np::string_>{*static_cast<const np::string_ *>(element)};
                return Series{array, m_name};
            } else if (element.isUnicode()) {
                auto array = np::Array<np::unicode_>{*static_cast<const np::unicode_ *>(element)};
                return Series{array, m_name};
            } else {
                throw std::runtime_error("Invalid value type");
            }
        }

        auto first = cond.substr(0, colonPos);
        np::Size firstIndex = 0;
        try {
            firstIndex = std::stoul(first);
        } catch (std::invalid_argument const &) {
        } catch (std::out_of_range const &) {
        }
        auto last = cond.substr(colonPos + 1, cond.size() - colonPos - 1);
        np::Size lastIndex = size();
        try {
            lastIndex = std::stoul(last);
        } catch (std::invalid_argument const &) {
        } catch (std::out_of_range const &) {
        }
        np::Size length = lastIndex - firstIndex;
        if (length > size()) {
            throw std::runtime_error("Incorrect range");
        }

        np::Shape shape{lastIndex - firstIndex};

        if (dtype() == "float64") {
            np::Array<np::float_> array{shape};
            for (np::Size j = 0; j < array.size(); ++j) {
                const auto &value = at(j + firstIndex);
                array.set(j, *static_cast<const np::float_ *>(value));
            }
            return {array, m_name};
        } else if (dtype() == "int64") {
            np::Array<np::int_> array{shape};
            for (np::Size j = 0; j < array.size(); ++j) {
                auto value = at(j + firstIndex);
                array.set(j, *static_cast<const np::int_ *>(value));
            }
            return {array, m_name};
        } else if (dtype() == "str") {
            np::Array<np::string_> array{shape};
            for (np::Size j = 0; j < array.size(); ++j) {
                auto value = at(j + firstIndex);
                array.set(j, *static_cast<const np::string_ *>(value));
            }
            return {array, m_name};
        } else if (dtype() == "unicode") {
            np::Array<np::unicode_> array{shape};
            for (np::Size j = 0; j < array.size(); ++j) {
                auto value = at(j + firstIndex);
                array.set(j, *static_cast<const np::unicode_ *>(value));
            }
            return {array, m_name};
        } else {
            throw std::runtime_error("Unknown type");
        }
    }

    Series Series::callable1(const std::string &) const {
        throw std::runtime_error("not implemented");
    }

    Series Series::iloc(const std::string &cond) const {
        static constexpr std::size_t kIndexingHandlersSize{static_cast<std::size_t>(np::ndarray::internal::IndexingMode::None)};

        const internal::IndexingHandler<internal::IndexingChecker1, internal::IndexingWorker1<Series>> indexingHandlers[kIndexingHandlersSize] = {
                {internal::IndexingMode::Slicing,
                 internal::isSlicing1,
                 std::bind(&Series::slicing1, this, std::placeholders::_1)},
                {internal::IndexingMode::Callable,
                 internal::isCallable1,
                 std::bind(&Series::callable1, this, std::placeholders::_1)}};

        for (const auto &indexing: indexingHandlers) {
            if (indexing.checker(cond)) {
                return indexing.worker(cond);
            }
        }

        throw std::runtime_error("Invalid operator[] argument");
    }

    Series Series::iloc(const std::vector<bool> &indexes) const {
        auto shape{m_shape};
        if (indexes.size() != size()) {
            throw std::runtime_error("Incorrect range");
        }

        if (dtype() == "float64") {
            np::Array<np::float_> array{shape};
            for (np::Size j = 0; j < indexes.size(); ++j) {
                const auto &value = at(indexes[j]);
                array.set(j, *static_cast<const np::float_ *>(value));
            }
            return {array, m_name};
        } else if (dtype() == "int64") {
            np::Array<np::int_> array{shape};
            for (np::Size j = 0; j < indexes.size(); ++j) {
                const auto &value = at(indexes[j]);
                array.set(j, *static_cast<const np::int_ *>(value));
            }
            return {array, m_name};
        } else if (dtype() == "str") {
            np::Array<np::string_> array{shape};
            for (np::Size j = 0; j < indexes.size(); ++j) {
                const auto &value = at(indexes[j]);
                array.set(j, *static_cast<const np::string_ *>(value));
            }
            return {array, m_name};
        } else if (dtype() == "unicode") {
            np::Array<np::unicode_> array{shape};
            for (np::Size j = 0; j < indexes.size(); ++j) {
                const auto &value = at(indexes[j]);
                array.set(j, *static_cast<const np::unicode_ *>(value));
            }
            return {array, m_name};
        } else {
            throw std::runtime_error("Unknown type");
        }
    }

    np::float_ Series::mean(bool skipna) const {
        if (m_data.isIntArray()) {
            const np::Array<np::int_> *array = m_data;
            if (skipna)
                return array->nanmean();
            else
                return array->mean();
        } else if (m_data.isFloatArray()) {
            const np::Array<np::float_> *array = m_data;
            if (skipna)
                return array->nanmean();
            else
                return array->mean();
        } else {
            throw std::runtime_error("Cannot calculate mean of a non-number array");
        }
    }

    Series Series::replace(internal::Value to_replace, internal::Value value) const {
        if (m_data.isIntArray()) {
            if (!to_replace.isInt()) {
                throw std::runtime_error("Cannot replace to an non-int value in int array");
            }
            const np::int_ *to_replaceIntPtr = to_replace;
            if (value.isInt()) {
                const np::Array<np::int_> *arraySrc = m_data;
                np::Array<np::int_> arrayDst{m_shape};
                for (np::Size row = 0; row < m_shape[0]; ++row) {
                    if (arraySrc->get(row) == *to_replaceIntPtr) {
                        arrayDst.set(row, *static_cast<const np::int_ *>(value));
                    } else {
                        arrayDst.set(row, arraySrc->get(row));
                    }
                }
                return Series{arrayDst, m_name};
            } else if (value.isFloat()) {
                const np::Array<np::int_> *arraySrc = m_data;
                np::Array<np::float_> arrayDst{m_shape};
                for (np::Size row = 0; row < m_shape[0]; ++row) {
                    if (arraySrc->get(row) == *to_replaceIntPtr) {
                        arrayDst.set(row, *static_cast<const np::float_ *>(value));
                    } else {
                        arrayDst.set(row, static_cast<np::float_>(arraySrc->get(row)));
                    }
                }
                return Series{arrayDst, m_name};
            } else if (value.isString() || value.isUnicode()) {
                throw std::runtime_error("Cannot replace value to string in int array");
            } else {
                throw std::runtime_error("Invalid value type");
            }
        } else if (m_data.isFloatArray()) {
            if (!to_replace.isIntC() && !to_replace.isInt() && !to_replace.isFloat()) {
                throw std::runtime_error("Cannot replace a value of different type");
            }
            if (value.isInt()) {
                const np::int_ *valueIntPtr = value;
                const np::Array<np::float_> *arraySrc = m_data;
                np::Array<np::float_> arrayDst{m_shape};
                for (np::Size row = 0; row < m_shape[0]; ++row) {
                    if (to_replace.isFloat()) {
                        auto to_replaceFloatPtr = *static_cast<const np::float_ *>(to_replace);
                        if (np::internal::element_equal(arraySrc->get(row), to_replaceFloatPtr)) {
                            arrayDst.set(row, static_cast<np::float_>(*valueIntPtr));
                        } else {
                            arrayDst.set(row, arraySrc->get(row));
                        }
                    } else if (to_replace.isInt()) {
                        const np::int_ *to_replaceIntPtr = to_replace;
                        if (static_cast<np::int_>(arraySrc->get(row)) == *to_replaceIntPtr) {
                            arrayDst.set(row, static_cast<np::float_>(*valueIntPtr));
                        } else {
                            arrayDst.set(row, arraySrc->get(row));
                        }
                    } else {
                        const np::intc *to_replaceIntPtr = to_replace;
                        if (static_cast<np::int_>(arraySrc->get(row)) == *to_replaceIntPtr) {
                            arrayDst.set(row, static_cast<np::float_>(*valueIntPtr));
                        } else {
                            arrayDst.set(row, arraySrc->get(row));
                        }
                    }
                }
                return Series{arrayDst, m_name};
            } else if (value.isFloat()) {
                const np::Array<np::float_> *arraySrc = m_data;
                np::Array<np::float_> arrayDst{m_shape};
                for (np::Size row = 0; row < m_shape[0]; ++row) {
                    if (to_replace.isFloat()) {
                        auto to_replaceFloatPtr = static_cast<const np::float_ *>(to_replace);
                        if (np::internal::element_equal(arraySrc->get(row), *to_replaceFloatPtr)) {
                            arrayDst.set(row, *static_cast<const np::float_ *>(value));
                        } else {
                            arrayDst.set(row, arraySrc->get(row));
                        }
                    } else if (to_replace.isInt()) {
                        const np::int_ *to_replaceIntPtr = to_replace;
                        if (static_cast<np::int_>(arraySrc->get(row)) == *to_replaceIntPtr) {
                            arrayDst.set(row, *static_cast<const np::float_ *>(value));
                        } else {
                            arrayDst.set(row, arraySrc->get(row));
                        }
                    } else {
                        const np::intc *to_replaceIntPtr = to_replace;
                        if (static_cast<np::int_>(arraySrc->get(row)) == *to_replaceIntPtr) {
                            arrayDst.set(row, *static_cast<const np::float_ *>(value));
                        } else {
                            arrayDst.set(row, arraySrc->get(row));
                        }
                    }
                }
                return Series{arrayDst, m_name};
            } else if (value.isString() || value.isUnicode()) {
                throw std::runtime_error("Cannot replace value to string in float array");
            } else {
                throw std::runtime_error("Invalid value type");
            }
        } else if (m_data.isStringArray()) {
            if (!to_replace.isString()) {
                throw std::runtime_error("Cannot replace a value of different type");
            }
            const np::string_ *to_replaceString = to_replace;
            if (value.isString()) {
                const std::string *valueString = value;
                const np::Array<np::string_> *arraySrc = m_data;
                np::Array<np::string_> arrayDst{m_shape};
                for (np::Size row = 0; row < m_shape[0]; ++row) {
                    if (arraySrc->get(row) == *valueString) {
                        arrayDst.set(row, *to_replaceString);
                    } else {
                        arrayDst.set(row, arraySrc->get(row));
                    }
                }
                return Series{arrayDst, m_name};
            } else if (value.isUnicode()) {
                const np::unicode_ *valueUnicode = value;
                np::Array<np::unicode_> arrayDst{m_shape};
                std::string str(valueUnicode->length(), 0);
                std::transform(valueUnicode->begin(), valueUnicode->end(), str.begin(), [](wchar_t c) {
                    return static_cast<char>(c);
                });
                std::wstring replace_str(to_replaceString->length(), 0);
                std::transform(to_replaceString->begin(), to_replaceString->end(), replace_str.begin(), [](char c) {
                    return static_cast<wchar_t>(c);
                });
                const np::Array<np::string_> *arraySrc = m_data;
                for (np::Size row = 0; row < m_shape[0]; ++row) {
                    if (arraySrc->get(row) == str) {
                        arrayDst.set(row, replace_str);
                    } else {
                        const auto &arrayStr = arraySrc->get(row);
                        std::wstring arrayWStr(arrayStr.length(), 0);
                        std::transform(arrayStr.begin(), arrayStr.end(), arrayWStr.begin(), [](char c) {
                            return static_cast<wchar_t>(c);
                        });
                        arrayDst.set(row, arrayWStr);
                    }
                }
                return Series{arrayDst, m_name};
            } else if (value.isIntC() || value.isInt() || value.isFloat()) {
                throw std::runtime_error("Cannot replace value to number in string array");
            } else {
                throw std::runtime_error("Invalid value type");
            }
        } else if (m_data.isUnicodeArray()) {
            if (!to_replace.isUnicode()) {
                throw std::runtime_error("Cannot replace a value of different type");
            }
            const np::unicode_ *to_replaceUnicode = to_replace;
            if (value.isString()) {
                const np::string_ *valueString = value;
                np::Array<np::string_> arrayDst{m_shape};
                std::wstring str(valueString->length(), 0);
                std::copy(valueString->begin(), valueString->end(), str.begin());
                std::string replace_str(to_replaceUnicode->length(), 0);
                std::transform(to_replaceUnicode->begin(), to_replaceUnicode->end(), replace_str.begin(), [](wchar_t c) {
                    return static_cast<char>(c);
                });
                const np::Array<np::unicode_> *arraySrc = m_data;
                for (np::Size row = 0; row < m_shape[0]; ++row) {
                    if (arraySrc->get(row) == str) {
                        arrayDst.set(row, replace_str);
                    } else {
                        const auto &arrayWStr = arraySrc->get(row);
                        std::string arrayStr(arrayWStr.length(), 0);
                        std::transform(arrayWStr.begin(), arrayWStr.end(), arrayStr.begin(), [](wchar_t c) {
                            return static_cast<char>(c);
                        });
                        arrayDst.set(row, arrayStr);
                    }
                }
                return Series{arrayDst, m_name};
            } else if (value.isUnicode()) {
                const np::unicode_ *valueUnicode = value;
                const np::Array<np::unicode_> *arraySrc = m_data;
                np::Array<np::unicode_> arrayDst{m_shape};
                for (np::Size row = 0; row < m_shape[0]; ++row) {
                    if (arraySrc->get(row) == *valueUnicode) {
                        arrayDst.set(row, *to_replaceUnicode);
                    } else {
                        arrayDst.set(row, arraySrc->get(row));
                    }
                }
                return Series{arrayDst, m_name};
            } else if (value.isIntC() || value.isInt() || value.isFloat()) {
                throw std::runtime_error("Cannot replace value to number in unicode array");
            } else {
                throw std::runtime_error("Invalid value type");
            }
        } else {
            throw std::runtime_error("Invalid value type");
        }
    }

    static void printMemoryUsage(std::size_t bytes) {
        static const constexpr std::uint64_t kBytesInTByte = 1099511627776;
        static const constexpr std::uint64_t kBytesInGByte = 1073741824;
        static const constexpr std::uint64_t kBytesInMByte = 1048576;
        static const constexpr std::uint64_t kBytesInKByte = 1024;

        if (bytes >= kBytesInTByte) {
            std::cout << static_cast<double>(bytes) / static_cast<double>(kBytesInTByte) << " TB";
        } else if (bytes >= kBytesInGByte) {
            std::cout << static_cast<double>(bytes) / static_cast<double>(kBytesInGByte) << " GB";
        } else if (bytes >= kBytesInMByte) {
            std::cout << static_cast<double>(bytes) / static_cast<double>(kBytesInMByte) << " MB";
        } else if (bytes >= kBytesInKByte) {
            std::cout << static_cast<double>(bytes) / static_cast<double>(kBytesInKByte) << " KB";
        } else {
            std::cout << bytes << " bytes";
        }
    }

    void Series::info() const {
        std::cout << "<class 'pandas.core.series.Series'>" << std::endl;
        std::cout << "RangeIndex: " << m_size << " entries";
        if (m_size > 0) {
            std::cout << ", " << 0 << " to " << m_size - 1;
        }
        std::cout << std::endl;
        auto name = m_name.empty() ? "None" : *static_cast<const std::string *>(m_name);
        std::cout << "Series name: " << name << std::endl;
        std::cout << "Non-Null Count\tDtype" << std::endl;
        std::cout << "--------------\t-----" << std::endl;
        std::cout << m_size << " non-null\t" << dtype() << std::endl;
        std::cout << "dtypes: " << dtype() << "(1)" << std::endl;

        std::size_t memoryUsage = sizeof(*this);
        std::cout << "memory usage: ";
        printMemoryUsage(memoryUsage);
        std::cout << std::endl;
    }

}// namespace pd