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

#include <atomic>
#include <unordered_set>

#include <pd/core/internal/httpreader/Handler.hpp>

namespace pd {
    namespace internal {
        namespace httpreader {
            class Poll {
            public:
                Poll() = default;
                ~Poll() = default;

                Poll(const Poll &) = delete;
                Poll(Poll &&) = delete;
                Poll &operator=(const Poll &) = delete;
                Poll &operator=(Poll &&) = delete;

                void run();

                void addHandler(HandlerPtr handler) {
                    m_handlers.insert(std::move(handler));
                }

                void removeHandler(HandlerPtr handler) {
                    m_handlers.erase(handler);
                }

                void shutdown() {
                    m_isStopped = true;
                }

            private:
                std::unordered_set<HandlerPtr> m_handlers;
                std::atomic_bool m_isStopped{false};
            };
        }// namespace httpreader
    }    // namespace internal
}// namespace pd
