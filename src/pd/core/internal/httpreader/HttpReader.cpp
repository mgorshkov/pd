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

#include <array>
#include <cstring>

#include <pd/core/internal/httpreader/HttpReader.hpp>

namespace pd {
    namespace internal {
        namespace httpreader {

            static const constexpr char kHeaderSeparator[] = "\r\n\r\n";
            static const constexpr std::size_t kHeaderSeparatorSize = sizeof(kHeaderSeparator) - 1;
            static const constexpr std::size_t kMaxHttpHeaderSize = 100 * 1024;
            static const constexpr std::size_t kMaxHttpMessageSize = 100 * 1024 * 1024;

            void HttpReader::read(const StreamPtr &stream) {
                assert(stream);
                static const constexpr std::size_t kBufferSize = 10 * 1024;
                std::array<char, kBufferSize> buffer{};
                std::size_t count = stream->read(buffer.data(), kBufferSize);

                assert(count <= kBufferSize);

                if (count + m_readBuffer.size() > kMaxHttpMessageSize) {
                    throw std::runtime_error("Too big message");
                }

                if (count) {
                    m_readBuffer.insert(m_readBuffer.end(), buffer.data(), buffer.data() + count);

                    if (!m_headerFound) {
                        m_headerFound = findHeader();
                        if (!m_headerFound && m_readBuffer.size() > kMaxHttpHeaderSize) {
                            throw std::runtime_error("Too big header");
                        }
                    }
                }

                if (m_totalLength > 0 && m_readBuffer.size() < m_totalLength) {
                    return;
                }

                onMessageComplete();
            }

            bool HttpReader::findHeader() {
                for (std::size_t i = 0; i < m_readBuffer.size() - kHeaderSeparatorSize + 1; ++i) {
                    if (kHeaderSeparator[0] == m_readBuffer.at(i)) {
                        if (std::strncmp(kHeaderSeparator + 1, &m_readBuffer.at(i + 1), kHeaderSeparatorSize - 1) != 0) {
                            continue;
                        }
                        std::size_t headerSize = i + 2;
                        if (headerSize > kMaxHttpHeaderSize) {
                            throw std::runtime_error("Too big header");
                        }
                        std::string header;
                        header.reserve(headerSize);
                        std::copy(m_readBuffer.data(), m_readBuffer.data() + headerSize, std::back_inserter(header));
                        m_httpMessage = std::make_unique<HttpMessage>(header);
                        m_totalLength = m_httpMessage->m_contentLength + headerSize;
                        if (m_totalLength > kMaxHttpMessageSize) {
                            throw std::runtime_error("Too big message");
                        }
                        return true;
                    }
                }
                return false;
            }

            void HttpReader::onMessageComplete() {
                // message is completely read, construct the message object and further process the message
                std::string message(m_readBuffer.data(), m_readBuffer.size());
                m_httpMessage = std::make_unique<HttpMessage>(message);
                if (!m_httpMessage->isWellFormed()) {
                    throw std::runtime_error("Message is malformed");
                }
                if (m_httpMessage->m_statusCode == HttpMessage::StatusCodes::kTemporaryRedirect ||
                    m_httpMessage->m_statusCode == HttpMessage::StatusCodes::kMovedPermanently) {
                    m_redirectCallback(m_httpMessage->m_location);
                } else {
                    const auto &data = m_httpMessage->getData();
                    m_callback(data);
                }
                m_complete = true;
            }

        }// namespace httpreader
    }    // namespace internal
}// namespace pd