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

#include <pd/core/internal/httpreader/Address.hpp>
#include <pd/core/internal/httpreader/Socket.hpp>

namespace pd {
    namespace internal {
        namespace httpreader {
            class Stream {
            public:
                Stream() = default;

                virtual ~Stream() = default;

                const Socket &getSocket() const {
                    return m_socket;
                }

                Socket &getSocket() {
                    return m_socket;
                }

                void init(const std::string &) {}

                virtual void open() {
                    m_socket.open();
                }

                virtual void close() {
                    m_socket.close();
                }

                [[nodiscard]] bool isOpened() const {
                    return m_socket.isOpened();
                }

                virtual std::size_t read(void *buffer, std::size_t count) const;
                virtual std::size_t write(const void *buffer, std::size_t count) const;

                void setAddress(const Address &address) {
                    m_address = address;
                }

                virtual void shutdown();

            private:
                Socket m_socket;
                Address m_address;///< address of peer socket
            };

            using StreamPtr = std::shared_ptr<Stream>;
        }// namespace httpreader
    }    // namespace internal
}// namespace pd
