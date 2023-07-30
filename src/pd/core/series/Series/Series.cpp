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

    Series::Series(const np::Array<np::bool_> &data, const std::vector<internal::Value> &index, const internal::Value &name)
        : Series{internal::Array{data}, index, name} {
    }

    Series::Series(np::Array<np::bool_> &&data, const std::vector<internal::Value> &index, const internal::Value &name)
        : Series{internal::Array{data}, index, name} {
    }

    Series::Series(const np::Array<np::intc> &data, const std::vector<internal::Value> &index, const internal::Value &name)
        : Series{internal::Array{data}, index, name} {
    }

    Series::Series(np::Array<np::intc> &&data, const std::vector<internal::Value> &index, const internal::Value &name)
        : Series{internal::Array{data}, index, name} {
    }

    Series::Series(const np::Array<np::int_> &data, const std::vector<internal::Value> &index, const internal::Value &name)
        : Series{internal::Array{data}, index, name} {
    }

    Series::Series(np::Array<np::int_> &&data, const std::vector<internal::Value> &index, const internal::Value &name)
        : Series{internal::Array{data}, index, name} {
    }

    Series::Series(const np::Array<np::Size> &data, const std::vector<internal::Value> &index, const internal::Value &name)
        : Series{internal::Array{data}, index, name} {
    }

    Series::Series(np::Array<np::Size> &&data, const std::vector<internal::Value> &index, const internal::Value &name)
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

    Series::Series(const np::Array<internal::Value> &data, const std::vector<internal::Value> &index, const internal::Value &name)
        : Series{internal::Array{data}, index, name} {
    }

    Series::Series(np::Array<internal::Value> &&data, const std::vector<internal::Value> &index, const internal::Value &name)
        : Series{internal::Array{data}, index, name} {
    }

    Series::Series(const internal::Array &data, const std::vector<internal::Value> &index)
        : Series{data, index, internal::Value{}} {
    }

    Series::Series(const np::Array<np::bool_> &data, const std::vector<internal::Value> &index)
        : Series{internal::Array{data}, index, internal::Value{}} {
    }

    Series::Series(np::Array<np::bool_> &&data, const std::vector<internal::Value> &index)
        : Series{internal::Array{data}, index, internal::Value{}} {
    }

    Series::Series(const np::Array<np::intc> &data, const std::vector<internal::Value> &index)
        : Series{internal::Array{data}, index, internal::Value{}} {
    }

    Series::Series(np::Array<np::intc> &&data, const std::vector<internal::Value> &index)
        : Series{internal::Array{data}, index, internal::Value{}} {
    }

    Series::Series(const np::Array<np::int_> &data, const std::vector<internal::Value> &index)
        : Series{internal::Array{data}, index, internal::Value{}} {
    }

    Series::Series(np::Array<np::int_> &&data, const std::vector<internal::Value> &index)
        : Series{internal::Array{data}, index, internal::Value{}} {
    }

    Series::Series(const np::Array<np::Size> &data, const std::vector<internal::Value> &index)
        : Series{internal::Array{data}, index, internal::Value{}} {
    }

    Series::Series(np::Array<np::Size> &&data, const std::vector<internal::Value> &index)
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

    Series::Series(const np::Array<internal::Value> &data, const std::vector<internal::Value> &index)
        : Series{internal::Array{data}, index, internal::Value{}} {
    }

    Series::Series(np::Array<internal::Value> &&data, const std::vector<internal::Value> &index)
        : Series{internal::Array{data}, index, internal::Value{}} {
    }

    Series::Series(const internal::Array &data, const internal::Value &name)
        : Series{data, std::vector<internal::Value>{}, name} {
    }

    Series::Series(internal::Array &&data, const internal::Value &name)
        : Series{std::move(data), std::vector<internal::Value>{}, name} {
    }

    Series::Series(const np::Array<np::bool_> &data, const internal::Value &name)
        : Series{internal::Array{data}, std::vector<internal::Value>{}, name} {
    }

    Series::Series(np::Array<np::bool_> &&data, const internal::Value &name)
        : Series{internal::Array{data}, std::vector<internal::Value>{}, name} {
    }

    Series::Series(const np::Array<np::intc> &data, const internal::Value &name)
        : Series{internal::Array{data}, std::vector<internal::Value>{}, name} {
    }

    Series::Series(np::Array<np::intc> &&data, const internal::Value &name)
        : Series{internal::Array{data}, std::vector<internal::Value>{}, name} {
    }

    Series::Series(const np::Array<np::int_> &data, const internal::Value &name)
        : Series{internal::Array{data}, std::vector<internal::Value>{}, name} {
    }

    Series::Series(np::Array<np::int_> &&data, const internal::Value &name)
        : Series{internal::Array{data}, std::vector<internal::Value>{}, name} {
    }

    Series::Series(const np::Array<np::Size> &data, const internal::Value &name)
        : Series{internal::Array{data}, std::vector<internal::Value>{}, name} {
    }

    Series::Series(np::Array<np::Size> &&data, const internal::Value &name)
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

    Series::Series(const np::Array<internal::Value> &data, const internal::Value &name)
        : Series{internal::Array{data}, std::vector<internal::Value>{}, name} {
    }

    Series::Series(np::Array<internal::Value> &&data, const internal::Value &name)
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

    bool Series::empty() const {
        return m_shape.empty();
    }

    np::Size Series::ndim() const {
        return m_shape.size();
    }

    np::Size Series::size() const {
        return m_size;
    }

    np::Shape Series::shape() const {
        return m_shape;
    }

    std::string Series::dtype() const {
        if (m_data.isBoolArray()) {
            return "bool";
        } else if (m_data.isIntCArray()) {
            return "int32";
        } else if (m_data.isIntArray()) {
            return "int64";
        } else if (m_data.isSizeArray()) {
            return "uint64";
        } else if (m_data.isFloatArray()) {
            return "float64";
        } else if (m_data.isStringArray()) {
            return "str";
        } else if (m_data.isUnicodeArray()) {
            return "unicode";
        } else if (m_data.isValueArray()) {
            return "value";
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
        if (m_data.isBoolArray()) {
            auto *array = static_cast<np::Array<np::bool_> *>(m_data);
            array->set(row, *static_cast<const np::bool_ *>(value));
        } else if (m_data.isIntCArray()) {
            auto *array = static_cast<np::Array<np::intc> *>(m_data);
            array->set(row, *static_cast<const np::intc *>(value));
        } else if (m_data.isIntArray()) {
            auto *array = static_cast<np::Array<np::int_> *>(m_data);
            array->set(row, *static_cast<const np::int_ *>(value));
        } else if (m_data.isSizeArray()) {
            auto *array = static_cast<np::Array<np::Size> *>(m_data);
            array->set(row, *static_cast<const np::Size *>(value));
        } else if (m_data.isFloatArray()) {
            auto *array = static_cast<np::Array<np::float_> *>(m_data);
            if (value.isFloat()) {
                array->set(row, *static_cast<const np::float_ *>(value));
            } else if (value.isInt()) {
                array->set(row, static_cast<np::float_>(*static_cast<const np::int_ *>(value)));
            } else {
                throw std::runtime_error("Invalid value type");
            }
        } else if (m_data.isStringArray()) {
            auto *array = static_cast<np::Array<np::string_> *>(m_data);
            array->set(row, *static_cast<const np::string_ *>(value));
        } else if (m_data.isUnicodeArray()) {
            auto *array = static_cast<np::Array<np::unicode_> *>(m_data);
            array->set(row, *static_cast<const np::unicode_ *>(value));
        } else {
            throw std::runtime_error("Invalid value type");
        }
    }

    internal::Value Series::at(np::Size row) const {
        if (m_data.isBoolArray()) {
            const auto *array = static_cast<const np::Array<np::bool_> *>(m_data);
            return internal::Value{array->get(row)};
        } else if (m_data.isIntCArray()) {
            const auto *array = static_cast<const np::Array<np::intc> *>(m_data);
            return internal::Value{array->get(row)};
        } else if (m_data.isIntArray()) {
            const auto *array = static_cast<const np::Array<np::int_> *>(m_data);
            return internal::Value{array->get(row)};
        } else if (m_data.isSizeArray()) {
            const auto *array = static_cast<const np::Array<np::Size> *>(m_data);
            return internal::Value{array->get(row)};
        } else if (m_data.isFloatArray()) {
            const auto *array = static_cast<const np::Array<np::float_> *>(m_data);
            return internal::Value{array->get(row)};
        } else if (m_data.isStringArray()) {
            const auto *array = static_cast<const np::Array<np::string_> *>(m_data);
            return internal::Value{array->get(row)};
        } else if (m_data.isUnicodeArray()) {
            const auto *array = static_cast<const np::Array<np::unicode_> *>(m_data);
            return internal::Value{array->get(row)};
        } else if (m_data.isValueArray()) {
            const auto *array = static_cast<const np::Array<internal::Value> *>(m_data);
            return internal::Value{array->get(row)};
        } else {
            throw std::runtime_error("Invalid value type");
        }
        return internal::Value{};
    }

    [[nodiscard]] internal::Value Series::operator[](np::Size row) const {
        if (m_data.isBoolArray()) {
            const auto *array = static_cast<const np::Array<np::bool_> *>(m_data);
            return internal::Value{array->get(row)};
        } else if (m_data.isIntCArray()) {
            const auto *array = static_cast<const np::Array<np::intc> *>(m_data);
            return internal::Value{array->get(row)};
        } else if (m_data.isIntArray()) {
            const auto *array = static_cast<const np::Array<np::int_> *>(m_data);
            return internal::Value{array->get(row)};
        } else if (m_data.isSizeArray()) {
            const auto *array = static_cast<const np::Array<np::Size> *>(m_data);
            return internal::Value{array->get(row)};
        } else if (m_data.isFloatArray()) {
            const auto *array = static_cast<const np::Array<np::float_> *>(m_data);
            return internal::Value{array->get(row)};
        } else if (m_data.isStringArray()) {
            const auto *array = static_cast<const np::Array<np::string_> *>(m_data);
            return internal::Value{array->get(row)};
        } else if (m_data.isUnicodeArray()) {
            const auto *array = static_cast<const np::Array<np::unicode_> *>(m_data);
            return internal::Value{array->get(row)};
        } else if (m_data.isValueArray()) {
            const auto *array = static_cast<const np::Array<internal::Value> *>(m_data);
            return internal::Value{array->get(row)};
        } else {
            throw std::runtime_error("Invalid value type");
        }
        return internal::Value{};
    }

    internal::Value Series::iloc(np::Size row) const {
        return at(row);
    }

    Series Series::slicing1(const std::string &cond) const {
        auto colonPos = cond.find(':');
        if (colonPos == std::string::npos) {
            np::Size i = std::stoi(cond);

            if (i >= size()) {
                throw std::runtime_error("Index " + std::to_string(i) + " out of bounds");
            }
            const auto &element = iloc(i);
            if (element.isBool()) {
                auto array = np::Array<np::bool_>{*static_cast<const np::bool_ *>(element)};
                return Series{array, m_name};
            } else if (element.isIntC()) {
                auto array = np::Array<np::intc>{*static_cast<const np::intc *>(element)};
                return Series{array, m_name};
            } else if (element.isInt()) {
                auto array = np::Array<np::int_>{*static_cast<const np::int_ *>(element)};
                return Series{array, m_name};
            } else if (element.isSize()) {
                auto array = np::Array<np::Size>{*static_cast<const np::Size *>(element)};
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
        } else if (dtype() == "uint64") {
            np::Array<np::Size> array{shape};
            for (np::Size j = 0; j < array.size(); ++j) {
                auto value = at(j + firstIndex);
                array.set(j, *static_cast<const np::Size *>(value));
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
        static constexpr std::size_t kIndexingHandlersSize{static_cast<std::size_t>(np::ndarray::internal::IndexingMode::Size)};

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
        } else if (dtype() == "int") {
            np::Array<np::intc> array{shape};
            for (np::Size j = 0; j < indexes.size(); ++j) {
                const auto &value = at(indexes[j]);
                array.set(j, *static_cast<const np::intc *>(value));
            }
            return {array, m_name};
        } else if (dtype() == "int64") {
            np::Array<np::int_> array{shape};
            for (np::Size j = 0; j < indexes.size(); ++j) {
                const auto &value = at(indexes[j]);
                array.set(j, *static_cast<const np::int_ *>(value));
            }
            return {array, m_name};
        } else if (dtype() == "uint64") {
            np::Array<np::Size> array{shape};
            for (np::Size j = 0; j < indexes.size(); ++j) {
                const auto &value = at(indexes[j]);
                array.set(j, *static_cast<const np::Size *>(value));
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

    static np::float_ mean_(const np::Array<internal::Value> &array) {
        auto s = array.size();
        if (s == 0)
            return 0;
        np::float_ result{};
        for (np::Size i = 0; i < s; ++i) {
            const auto &element = array.get(i);
            if (element.isInt()) {
                np::ndarray::internal::inc(result, static_cast<np::int_>(element));
            } else if (element.isIntC()) {
                np::ndarray::internal::inc(result, static_cast<np::intc>(element));
            } else if (element.isFloat()) {
                np::ndarray::internal::inc(result, static_cast<np::float_>(element));
            } else if (element.isSize()) {
                np::ndarray::internal::inc(result, static_cast<np::Size>(element));
            } else {
                continue;
            }
        }
        np::float_ resultDiv{};
        np::ndarray::internal::divideBySize(result, s, resultDiv);
        return resultDiv;
    }

    static np::float_ nanmean_(const np::Array<internal::Value> &array) {
        auto s = array.size();
        if (s == 0)
            return 0;
        np::float_ result{};
        np::Size count = 0;
        for (np::Size i = 0; i < s; ++i) {
            const auto &element = array.get(i);
            bool isNaNResult{};
            np::ndarray::internal::isNaN(element, isNaNResult);
            if (isNaNResult)
                continue;
            if (element.isInt()) {
                np::ndarray::internal::inc(result, static_cast<np::int_>(element));
            } else if (element.isIntC()) {
                np::ndarray::internal::inc(result, static_cast<np::intc>(element));
            } else if (element.isFloat()) {
                np::ndarray::internal::inc(result, static_cast<np::float_>(element));
            } else if (element.isSize()) {
                np::ndarray::internal::inc(result, static_cast<np::Size>(element));
            } else {
                continue;
            }
            ++count;
        }
        np::float_ resultDiv{};
        np::ndarray::internal::divideBySize(result, count, resultDiv);
        return resultDiv;
    }

    np::float_ Series::mean(bool skipna) const {
        if (m_data.isIntCArray()) {
            const auto *array = static_cast<const np::Array<np::intc> *>(m_data);
            if (skipna)
                return array->nanmean();
            else
                return array->mean();
        } else if (m_data.isIntArray()) {
            const auto *array = static_cast<const np::Array<np::int_> *>(m_data);
            if (skipna)
                return array->nanmean();
            else
                return array->mean();
        } else if (m_data.isSizeArray()) {
            const auto *array = static_cast<const np::Array<np::Size> *>(m_data);
            if (skipna)
                return array->nanmean();
            else
                return array->mean();
        } else if (m_data.isFloatArray()) {
            const auto *array = static_cast<const np::Array<np::float_> *>(m_data);
            if (skipna)
                return array->nanmean();
            else
                return array->mean();
        } else if (m_data.isValueArray()) {
            const auto *array = static_cast<const np::Array<internal::Value> *>(m_data);
            if (skipna)
                return nanmean_(*array);
            else
                return mean_(*array);
        } else {
            throw std::runtime_error("Cannot calculate mean of a non-number array");
        }
    }

    static np::float_ std__(const np::Array<internal::Value> &array) {
        np::ndarray::array_dynamic::NDArrayDynamic<np::float_> x;
        np::float_ m = mean_(array);
        for (np::Size i = 0; i < array.size(); ++i) {
            const auto &element = array.get(i);
            np::float_ result;
            if (element.isInt()) {
                np::ndarray::internal::subtract<np::float_>(static_cast<np::float_>(element), m, result);
            } else if (element.isIntC()) {
                np::ndarray::internal::subtract<np::float_>(static_cast<np::float_>(element), m, result);
            } else if (element.isFloat()) {
                np::ndarray::internal::subtract<np::float_>(static_cast<np::float_>(element), m, result);
            } else if (element.isSize()) {
                np::ndarray::internal::subtract<np::float_>(static_cast<np::float_>(element), m, result);
            } else {
                continue;
            }
            np::float_ resultAbs;
            np::ndarray::internal::abs(result, resultAbs);
            auto a = resultAbs;
            np::float_ resultMul;
            np::ndarray::internal::multiply(a, a, resultMul);
            x.push_back(resultMul);
        }
        np::float_ resultSqrt;
        np::ndarray::internal::sqrt(x.nanmean(), resultSqrt);
        return resultSqrt;
    }

    static np::float_ nanstd_(const np::Array<internal::Value> &array) {
        np::ndarray::array_dynamic::NDArrayDynamic<np::float_> x;
        np::float_ m = nanmean_(array);
        for (np::Size i = 0; i < array.size(); ++i) {
            const auto &element = array.get(i);
            bool isNaNResult{};
            np::ndarray::internal::isNaN(element, isNaNResult);
            if (isNaNResult)
                continue;
            np::float_ result;
            if (element.isInt()) {
                np::ndarray::internal::subtract<np::float_>(static_cast<np::float_>(element), m, result);
            } else if (element.isIntC()) {
                np::ndarray::internal::subtract<np::float_>(static_cast<np::float_>(element), m, result);
            } else if (element.isFloat()) {
                np::ndarray::internal::subtract<np::float_>(static_cast<np::float_>(element), m, result);
            } else if (element.isSize()) {
                np::ndarray::internal::subtract<np::float_>(static_cast<np::float_>(element), m, result);
            } else {
                continue;
            }
            np::float_ resultAbs;
            np::ndarray::internal::abs(result, resultAbs);
            auto a = resultAbs;
            np::float_ resultMul;
            np::ndarray::internal::multiply(a, a, resultMul);
            x.push_back(resultMul);
        }
        np::float_ resultSqrt;
        np::ndarray::internal::sqrt(x.nanmean(), resultSqrt);
        return resultSqrt;
    }

    static np::float_ var_(const np::Array<internal::Value> &array) {
        np::ndarray::array_dynamic::NDArrayDynamic<np::float_> x;
        np::float_ m = mean_(array);
        for (np::Size i = 0; i < array.size(); ++i) {
            const auto &element = array.get(i);
            np::float_ result;
            if (element.isInt()) {
                np::ndarray::internal::subtract<np::float_>(static_cast<np::float_>(element), m, result);
            } else if (element.isIntC()) {
                np::ndarray::internal::subtract<np::float_>(static_cast<np::float_>(element), m, result);
            } else if (element.isFloat()) {
                np::ndarray::internal::subtract<np::float_>(static_cast<np::float_>(element), m, result);
            } else if (element.isSize()) {
                np::ndarray::internal::subtract<np::float_>(static_cast<np::float_>(element), m, result);
            } else {
                continue;
            }
            np::float_ resultAbs;
            np::ndarray::internal::abs(result, resultAbs);
            auto a = resultAbs;
            np::float_ resultMul;
            np::ndarray::internal::multiply(a, a, resultMul);
            x.push_back(resultMul);
        }
        return x.mean();
    }

    static np::float_ nanvar_(const np::Array<internal::Value> &array) {
        np::ndarray::array_dynamic::NDArrayDynamic<np::float_> x;
        np::float_ m = nanmean_(array);
        for (np::Size i = 0; i < array.size(); ++i) {
            const auto &element = array.get(i);
            bool isNaNResult{};
            np::ndarray::internal::isNaN(element, isNaNResult);
            if (isNaNResult)
                continue;
            np::float_ result;
            if (element.isInt()) {
                np::ndarray::internal::subtract<np::float_>(static_cast<np::float_>(element), m, result);
            } else if (element.isIntC()) {
                np::ndarray::internal::subtract<np::float_>(static_cast<np::float_>(element), m, result);
            } else if (element.isFloat()) {
                np::ndarray::internal::subtract<np::float_>(static_cast<np::float_>(element), m, result);
            } else if (element.isSize()) {
                np::ndarray::internal::subtract<np::float_>(static_cast<np::float_>(element), m, result);
            } else {
                continue;
            }
            np::float_ resultAbs;
            np::ndarray::internal::abs(result, resultAbs);
            auto a = resultAbs;
            np::float_ resultMul;
            np::ndarray::internal::multiply(a, a, resultMul);
            x.push_back(resultMul);
        }
        return x.nanmean();
    }

    np::float_ Series::std_(bool skipna) const {
        if (m_data.isIntCArray()) {
            const auto *array = static_cast<const np::Array<np::intc> *>(m_data);
            if (skipna)
                return array->nanstd();
            else
                return array->std_();
        } else if (m_data.isIntArray()) {
            const auto *array = static_cast<const np::Array<np::int_> *>(m_data);
            if (skipna)
                return array->nanstd();
            else
                return array->std_();
        } else if (m_data.isSizeArray()) {
            const auto *array = static_cast<const np::Array<np::Size> *>(m_data);
            if (skipna)
                return array->nanstd();
            else
                return array->std_();
        } else if (m_data.isFloatArray()) {
            const auto *array = static_cast<const np::Array<np::float_> *>(m_data);
            if (skipna)
                return array->nanstd();
            else
                return array->std_();
        } else if (m_data.isValueArray()) {
            const auto *array = static_cast<const np::Array<internal::Value> *>(m_data);
            if (skipna)
                return nanstd_(*array);
            else
                return std__(*array);
        } else {
            throw std::runtime_error("Cannot calculate mean of a non-number array");
        }
    }

    np::float_ Series::var(bool skipna) const {
        if (m_data.isIntCArray()) {
            const auto *array = static_cast<const np::Array<np::intc> *>(m_data);
            if (skipna)
                return array->nanvar();
            else
                return array->var();
        } else if (m_data.isIntArray()) {
            const auto *array = static_cast<const np::Array<np::int_> *>(m_data);
            if (skipna)
                return array->nanvar();
            else
                return array->var();
        } else if (m_data.isSizeArray()) {
            const auto *array = static_cast<const np::Array<np::Size> *>(m_data);
            if (skipna)
                return array->nanvar();
            else
                return array->var();
        } else if (m_data.isFloatArray()) {
            const auto *array = static_cast<const np::Array<np::float_> *>(m_data);
            if (skipna)
                return array->nanvar();
            else
                return array->var();
        } else if (m_data.isValueArray()) {
            const auto *array = static_cast<const np::Array<internal::Value> *>(m_data);
            if (skipna)
                return nanvar_(*array);
            else
                return var_(*array);
        } else {
            throw std::runtime_error("Cannot calculate mean of a non-number array");
        }
    }

    Series Series::replace(internal::Value to_replace, internal::Value value) const {
        if (m_data.isIntArray()) {
            if (!to_replace.isInt()) {
                throw std::runtime_error("Cannot replace to an non-int value in int array");
            }
            const auto *to_replaceIntPtr = static_cast<const np::int_ *>(to_replace);
            if (value.isInt()) {
                const auto *arraySrc = static_cast<const np::Array<np::int_> *>(m_data);
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
                const auto *arraySrc = static_cast<const np::Array<np::int_> *>(m_data);
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
                const auto *valueIntPtr = static_cast<const np::int_ *>(value);
                const auto *arraySrc = static_cast<const np::Array<np::float_> *>(m_data);
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
                        const auto *to_replaceIntPtr = static_cast<const np::int_ *>(to_replace);
                        if (static_cast<np::int_>(arraySrc->get(row)) == *to_replaceIntPtr) {
                            arrayDst.set(row, static_cast<np::float_>(*valueIntPtr));
                        } else {
                            arrayDst.set(row, arraySrc->get(row));
                        }
                    } else {
                        const auto *to_replaceIntPtr = static_cast<const np::intc *>(to_replace);
                        if (static_cast<np::int_>(arraySrc->get(row)) == *to_replaceIntPtr) {
                            arrayDst.set(row, static_cast<np::float_>(*valueIntPtr));
                        } else {
                            arrayDst.set(row, arraySrc->get(row));
                        }
                    }
                }
                return Series{arrayDst, m_name};
            } else if (value.isFloat()) {
                const auto *arraySrc = static_cast<const np::Array<np::float_> *>(m_data);
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
                        const auto *to_replaceIntPtr = static_cast<const np::int_ *>(to_replace);
                        if (static_cast<np::int_>(arraySrc->get(row)) == *to_replaceIntPtr) {
                            arrayDst.set(row, *static_cast<const np::float_ *>(value));
                        } else {
                            arrayDst.set(row, arraySrc->get(row));
                        }
                    } else {
                        const auto *to_replaceIntPtr = static_cast<const np::intc *>(to_replace);
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
            const auto *to_replaceString = static_cast<const np::string_ *>(to_replace);
            if (value.isString()) {
                const auto *valueString = static_cast<const np::string_ *>(value);
                const auto *arraySrc = static_cast<const np::Array<np::string_> *>(m_data);
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
                const auto *valueUnicode = static_cast<const np::unicode_ *>(value);
                np::Array<np::unicode_> arrayDst{m_shape};
                std::string str(valueUnicode->length(), 0);
                std::transform(valueUnicode->begin(), valueUnicode->end(), str.begin(), [](wchar_t c) {
                    return static_cast<char>(c);
                });
                std::wstring replace_str(to_replaceString->length(), 0);
                std::transform(to_replaceString->begin(), to_replaceString->end(), replace_str.begin(), [](char c) {
                    return static_cast<wchar_t>(c);
                });
                const auto *arraySrc = static_cast<const np::Array<np::string_> *>(m_data);
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
            const auto *to_replaceUnicode = static_cast<const np::unicode_ *>(to_replace);
            if (value.isString()) {
                const auto *valueString = static_cast<const np::string_ *>(value);
                np::Array<np::string_> arrayDst{m_shape};
                std::wstring str(valueString->length(), 0);
                std::copy(valueString->begin(), valueString->end(), str.begin());
                std::string replace_str(to_replaceUnicode->length(), 0);
                std::transform(to_replaceUnicode->begin(), to_replaceUnicode->end(), replace_str.begin(), [](wchar_t c) {
                    return static_cast<char>(c);
                });
                const auto *arraySrc = static_cast<const np::Array<np::unicode_> *>(m_data);
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
                const auto *valueUnicode = static_cast<const np::unicode_ *>(value);
                const auto *arraySrc = static_cast<const np::Array<np::unicode_> *>(m_data);
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

    internal::Value Series::dot(const Series &another) const {
        if (shape().size() != 1 || another.shape().size() != 1 || shape() != another.shape()) {
            throw std::runtime_error("Shapes are different or arguments are not 1D arrays");
        }
        internal::Value result{0};
        for (np::Size i = 0; i < size(); ++i) {
            internal::Value multipleResult{};
            if (m_data.isIntCArray()) {
                if (!another.m_data.isIntCArray()) {
                    throw std::runtime_error("Invalid value type");
                }
                result += static_cast<np::intc>(at(i)) * static_cast<np::intc>(another.at(i));
            } else if (m_data.isIntArray()) {
                if (!another.m_data.isIntArray()) {
                    throw std::runtime_error("Invalid value type");
                }
                result += static_cast<np::int_>(at(i)) * static_cast<np::int_>(another.at(i));
            } else if (m_data.isSizeArray()) {
                if (!another.m_data.isSizeArray()) {
                    throw std::runtime_error("Invalid value type");
                }
                result += static_cast<np::Size>(at(i)) * static_cast<np::Size>(another.at(i));
            } else if (m_data.isFloatArray()) {
                if (!another.m_data.isFloatArray()) {
                    throw std::runtime_error("Invalid value type");
                }
                result += static_cast<np::float_>(at(i)) * static_cast<np::float_>(another.at(i));
            } else {
                throw std::runtime_error("Invalid value type");
            }
        }
        return result;
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