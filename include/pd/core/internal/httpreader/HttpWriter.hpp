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

#include <memory>
#include <string>
#include <vector>

#include <pd/core/internal/httpreader/Handler.hpp>
#include <pd/core/internal/httpreader/HttpMessage.hpp>
#include <pd/core/internal/httpreader/UriParser.hpp>

namespace pd {
    namespace internal {
        namespace httpreader {

            class HttpWriter {
            public:
                HttpWriter(const std::string &userAgent, const Uri &uri);

                void request(HttpMessage::Method httpMethod = HttpMessage::Method::kGet,
                             HttpMessage::ContentType contentType = HttpMessage::ContentType::kPlain);

                void request(HttpMessage::Method httpMethod,
                             HttpMessage::ContentType contentType,
                             const std::vector<char> &data);

                void request(HttpMessage::Method httpMethod,
                             HttpMessage::ContentType contentType,
                             const std::string &text);

                void request(HttpMessage::ContentType contentType,
                             const std::vector<char> &data, HttpMessage::StatusCodes statusCode);

                void request(HttpMessage::ContentType contentType,
                             const std::string &text,
                             HttpMessage::StatusCodes statusCode);

                void write(const StreamPtr &stream);

                bool isWriteComplete() const {
                    return m_writeComplete;
                }

            private:
                void request(HttpMessage::Type type,
                             HttpMessage::Method httpMethod,
                             HttpMessage::ContentType contentType,
                             const std::string &text, const std::vector<char> &data,
                             HttpMessage::StatusCodes statusCode);

                std::string m_userAgent;
                Uri m_uri;

                std::unique_ptr<HttpMessage> m_httpMessage;

                std::string m_writeBuffer;
                bool m_writeComplete{false};
            };

            using HttpWriterPtr = std::unique_ptr<HttpWriter>;
        }// namespace httpreader
    }    // namespace internal
}// namespace pd