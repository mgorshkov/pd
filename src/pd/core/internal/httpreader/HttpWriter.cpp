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

#include <sstream>
#include <string>

#include <pd/core/internal/httpreader/HttpWriter.hpp>

namespace pd {
    namespace internal {
        namespace httpreader {

            HttpWriter::HttpWriter(
                    const std::string &userAgent,
                    const Uri &uri)
                : m_userAgent{userAgent}, m_uri{uri} {
            }

            void HttpWriter::write(const StreamPtr &stream) {
                assert(stream);
                std::size_t totalSize = m_writeBuffer.size();
                std::size_t totalWritten = 0;
                do {
                    std::size_t written = stream->write(m_writeBuffer.data() + totalWritten, m_writeBuffer.size() - totalWritten);
                    totalWritten += written;
                } while (totalSize - totalWritten > 0);

                m_writeComplete = true;
            }

            void HttpWriter::request(HttpMessage::Type type,
                                     HttpMessage::Method httpMethod,
                                     HttpMessage::ContentType contentType,
                                     const std::string &text, const std::vector<char> &data,
                                     HttpMessage::StatusCodes statusCode) {
                m_httpMessage = std::make_unique<HttpMessage>(httpMethod, m_userAgent, m_uri);
                m_httpMessage->m_type = type;
                m_httpMessage->m_statusCode = statusCode;
                m_httpMessage->m_contentType = contentType;
                if (contentType == HttpMessage::ContentType::kOctetStream) {
                    std::vector<char> v(text.size());
                    std::copy(text.begin(), text.end(), v.begin());
                    m_httpMessage->m_binary = data.empty() ? v : data;
                } else {
                    m_httpMessage->m_text = text.empty() ? std::string{data.begin(), data.end()} : text;
                }

                assert(m_httpMessage->isWellFormed());

                std::ostringstream stream;
                stream << *m_httpMessage;
                m_writeBuffer = stream.str();
            }

            void HttpWriter::request(HttpMessage::Method httpMethod, HttpMessage::ContentType contentType) {
                request(HttpMessage::Type::kRequest, httpMethod, contentType, std::string{}, std::vector<char>{}, HttpMessage::StatusCodes::kNone);
            }

            void HttpWriter::request(HttpMessage::Method httpMethod,
                                     HttpMessage::ContentType contentType,
                                     const std::vector<char> &data) {
                request(HttpMessage::Type::kRequest, httpMethod, contentType, std::string{}, data, HttpMessage::StatusCodes::kNone);
            }

            void HttpWriter::request(HttpMessage::Method httpMethod,
                                     HttpMessage::ContentType contentType,
                                     const std::string &text) {
                request(HttpMessage::Type::kRequest, httpMethod,
                        contentType, text, std::vector<char>{}, HttpMessage::StatusCodes::kNone);
            }

            void HttpWriter::request(HttpMessage::ContentType contentType,
                                     const std::vector<char> &data, HttpMessage::StatusCodes statusCode) {
                request(HttpMessage::Type::kRequest, HttpMessage::HttpMessage::Method::kNone,
                        contentType, std::string{}, data, statusCode);
            }

            void HttpWriter::request(HttpMessage::ContentType contentType,
                                     const std::string &text,
                                     HttpMessage::StatusCodes statusCode) {
                request(HttpMessage::Type::kRequest, HttpMessage::HttpMessage::HttpMessage::Method::kNone,
                        contentType, text, std::vector<char>{}, statusCode);
            }
        }// namespace httpreader
    }    // namespace internal
}// namespace pd