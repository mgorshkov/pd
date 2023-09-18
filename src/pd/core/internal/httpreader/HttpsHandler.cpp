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

#ifdef _WIN32
#include <Winsock2.h>
#else
#include <poll.h>
#endif

#include <pd/core/internal/httpreader/Connector.hpp>
#include <pd/core/internal/httpreader/HttpsHandler.hpp>

#ifdef OPENSSL
namespace pd {
    namespace internal {
        namespace httpreader {
            HttpsHandler::HttpsHandler(const std::string &userAgent,
                                       const Uri &uri, const HttpReader::Callback &callback, const HttpReader::RedirectCallback &redirectCallback) {
                m_sslStream = std::make_shared<SslStream>();

                connect(m_sslStream, uri.m_address);
                sslConnect(m_sslStream, uri.m_address);

                m_reader = std::make_unique<HttpReader>(callback, redirectCallback);
                m_writer = std::make_unique<HttpWriter>(userAgent, uri);

                m_writer->request();
                m_mode = Mode::kWrite;
            }

            void HttpsHandler::handle(short revents) {
                if (revents & POLLIN) {
                    m_reader->read(m_sslStream);
                    if (m_reader->isReadComplete()) {
                        m_mode = Mode::kFinish;
                        return;
                    }
                }
                if (revents & POLLOUT) {
                    m_writer->write(m_sslStream);
                    if (m_writer->isWriteComplete()) {
                        m_mode = Mode::kRead;
                    }
                }
            }

            short HttpsHandler::getEvents() const {
                return m_mode == Mode::kRead ? POLLIN : m_mode == Mode::kWrite ? POLLOUT
                                                                               : 0;
            }

        }// namespace httpreader
    }    // namespace internal
}// namespace pd
#endif