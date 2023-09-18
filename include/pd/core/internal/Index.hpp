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

#include <numeric>
#include <vector>

#include <pd/core/internal/Value.hpp>

namespace pd {
    namespace internal {
        class Index {
        public:
            Index() = default;
            Index(const Index &) = default;
            Index(Index &&) = default;

            explicit Index(const std::vector<internal::Value> &index)
                : m_namedIndex{index}, m_count{0} {
                for (np::Size i = 0; i < index.size(); ++i) {
                    m_nameToOffset[index[i]] = i;
                }
            }

            explicit Index(np::Size count)
                : m_namedIndex{}, m_nameToOffset{}, m_count{count} {
            }

            Index(const std::vector<internal::Value> &index, np::Size count)
                : m_namedIndex{index}, m_nameToOffset{}, m_count{count} {
                if (!index.empty() && count > 0) {
                    throw std::runtime_error("Invalid index");
                }
                for (np::Size i = 0; i < index.size(); ++i) {
                    m_nameToOffset[index[i]] = i;
                }
            }

            Index &operator=(const Index &) = default;
            Index &operator=(Index &&) = default;

            bool operator==(const Index &other) const {
                if (this != &other) {
                    return m_namedIndex == other.m_namedIndex &&
                           m_nameToOffset == other.m_nameToOffset &&
                           m_count == other.m_count;
                }
                return true;
            }

            [[nodiscard]] bool empty() const {
                return m_namedIndex.empty() && m_count == 0;
            }

            [[nodiscard]] np::Size size() const {
                return m_namedIndex.empty() ? m_count : static_cast<np::Size>(m_namedIndex.size());
            }

            internal::Value operator[](np::Size offset) const {
                return m_namedIndex.empty() ? internal::Value{offset} : m_namedIndex[offset];
            }

            internal::Value operator[](const internal::Value &offsetOrName) const {
                if (m_namedIndex.empty()) {
                    return offsetOrName;
                }
                auto it = m_nameToOffset.find(offsetOrName);
                if (it != m_nameToOffset.end()) {
                    return m_namedIndex[it->second];
                }
                if (offsetOrName.isInt()) {
                    return m_namedIndex[static_cast<np::int_>(offsetOrName)];
                } else if (offsetOrName.isIntC()) {
                    return m_namedIndex[static_cast<np::intc>(offsetOrName)];
                } else if (offsetOrName.isSize()) {
                    return m_namedIndex[static_cast<np::Size>(offsetOrName)];
                }
                return offsetOrName;
            }

            void addIndex(const Index &index) {
                for (np::Size i = 0; i < index.size(); ++i) {
                    m_nameToOffset[index[i]] = static_cast<np::Size>(m_namedIndex.size()) + i;
                }
                std::copy(index.m_namedIndex.begin(), index.m_namedIndex.end(), std::back_inserter(m_namedIndex));
                m_count += index.m_count;
            }

            void addIndex(const internal::Value &value) {
                m_nameToOffset[value] = static_cast<np::Size>(m_namedIndex.size());
                m_namedIndex.push_back(value);
            }

            void addIndex() {
                ++m_count;
            }

            [[nodiscard]] bool has(const internal::Value &value) const {
                return m_nameToOffset.contains(value) ||
                       (value.isInt() && *static_cast<const np::int_ *>(value) >= 0 && *static_cast<const np::int_ *>(value) < static_cast<np::int_>(m_count));
            }

            [[nodiscard]] std::vector<internal::Value> getIndex() const {
                if (!m_namedIndex.empty())
                    return m_namedIndex;
                std::vector<internal::Value> index(m_count);
                std::iota(index.begin(), index.end(), static_cast<np::Size>(0));
                return index;
            }

        private:
            std::vector<internal::Value> m_namedIndex;
            std::unordered_map<internal::Value, np::Size> m_nameToOffset;
            np::Size m_count{0};
        };

    }// namespace internal
}// namespace pd