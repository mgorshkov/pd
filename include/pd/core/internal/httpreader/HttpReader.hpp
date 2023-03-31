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
#include <memory>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include <pd/core/internal/httpreader/Handler.hpp>
#include <pd/core/internal/httpreader/HttpMessage.hpp>

namespace pd {
    namespace internal {
        namespace httpreader {

            class HttpReader {
            public:
                using Callback = std::function<void(const std::vector<char> &buffer)>;
                using RedirectCallback = std::function<void(const std::string &redirectUrl)>;
                explicit HttpReader(Callback callback, RedirectCallback redirectCallback)
                    : m_callback{std::move(callback)}, m_redirectCallback{std::move(redirectCallback)} {
                }

                void read(const StreamPtr &stream);
                [[nodiscard]] bool isReadComplete() const {
                    return m_complete;
                }

            private:
                bool findHeader();
                void onMessageComplete();

                bool m_complete{false};

                Callback m_callback;
                RedirectCallback m_redirectCallback;
                std::unique_ptr<HttpMessage> m_httpMessage;

                std::vector<char> m_readBuffer;
                bool m_headerFound{false};
                std::size_t m_totalLength{0};
            };

            using HttpReadHandlerPtr = std::shared_ptr<HttpReader>;
        }// namespace httpreader
    }    // namespace internal
}// namespace pd