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

#include <pd/core/internal/Value.hpp>

namespace pd {
    namespace internal {

        template<typename DType>
        class AllToNumberConvertor;

        template<>
        class AllToNumberConvertor<std::string> {
        public:
            Value operator()(std::string &val) {
                return Value{val};
            }
        };

        template<>
        class AllToNumberConvertor<std::wstring> {
        public:
            Value operator()(const std::string &val) {
                std::string str(val.length(), 0);
                std::wstring replace_str(val.length(), 0);
                std::transform(replace_str.begin(), replace_str.end(), str.begin(), [](wchar_t c) {
                    return static_cast<char>(c);
                });
                return Value{replace_str};
            }
        };

        template<>
        class AllToNumberConvertor<np::float_> {
        public:
            Value operator()(const std::string &val) {
                return Value{std::stod(val)};
            }
        };


        template<>
        class AllToNumberConvertor<np::int_> {
        public:
            Value operator()(const std::string &val) {
                return Value{std::stol(val)};
            }
        };

    }// namespace internal
}// namespace pd
