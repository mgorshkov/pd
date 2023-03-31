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

#include <pd/core/internal/httpreader/Handler.hpp>
#include <pd/core/internal/httpreader/HttpReader.hpp>
#include <pd/core/internal/httpreader/HttpWriter.hpp>

namespace pd {
    namespace internal {
        namespace httpreader {
            class HttpHandler : public Handler {
            public:
                HttpHandler(const std::string &userAgent,
                            const Uri &uri, HttpReader::Callback callback, HttpReader::RedirectCallback redirectCallback);

                void handle(short revents) override;

                [[nodiscard]] short getEvents() const override;

                void shutdown() {
                    m_stream->shutdown();
                }

                void request() {
                    m_writer->request();
                    m_mode = Mode::kWrite;
                }

                [[nodiscard]] bool isFinished() const override {
                    return m_mode == Mode::kFinish;
                }

            protected:
                [[nodiscard]] StreamPtr getStream() const override {
                    return m_stream;
                }

            private:
                StreamPtr m_stream;
                std::unique_ptr<HttpReader> m_reader;
                std::unique_ptr<HttpWriter> m_writer;

                enum class Mode {
                    kNone,
                    kWrite,
                    kRead,
                    kFinish
                };

                Mode m_mode{Mode::kNone};
            };

            using HttpHandlerPtr = std::shared_ptr<HttpHandler>;

        }// namespace httpreader
    }    // namespace internal
}// namespace pd