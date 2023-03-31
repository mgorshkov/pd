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

#include <pd/core/internal/AllToNumberConvertor.hpp>
#include <pd/core/internal/Utils.hpp>

namespace pd {
    namespace internal {
        enum class IndexingMode {
            Callable,
            Slicing,
            None
        };

        using IndexingChecker1 = std::function<bool(const std::string &cond)>;
        using IndexingChecker2 = std::function<bool(const std::string &cond1, const std::string &cond2)>;

        template<typename Out>
        using IndexingWorker1 = std::function<Out(const std::string &cond)>;

        template<typename Out>
        using IndexingWorker2 = std::function<Out(const std::string &cond1, const std::string &cond2)>;

        template<typename Checker, typename Worker>
        struct IndexingHandler {
            IndexingMode mode{IndexingMode::None};
            Checker checker;
            Worker worker;
        };

        inline bool isCallable1(const std::string &) {
            return false;
        }

        inline bool isCallable2(const std::string &, const std::string &) {
            return false;
        }

        inline bool isSlicing1(const std::string &cond) {
            return std::all_of(cond.begin(), cond.end(), [](const auto &c) {
                return std::isdigit(c) || c == ',' || c == ':';
            });
        }

        inline bool isSlicing2(const std::string &cond1, const std::string &cond2) {
            return isSlicing1(cond1) && isSlicing1(cond2);
        }
    }// namespace internal
}// namespace pd
