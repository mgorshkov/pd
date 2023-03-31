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

#include <assert.h>

#ifdef _WIN32

#include <winsock.h>

#include <io.h>

#define SHUT_RD 0x00
#define SHUT_WR 0x01
#define SHUT_RDWR 0x02

#else

#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>

#undef INVALID_SOCKET
#define INVALID_SOCKET (-1)
#undef SOCKET_ERROR
#define SOCKET_ERROR (-1)

#endif

namespace pd {
    namespace internal {
        namespace httpreader {
            class Socket {
            public:
#ifdef _WIN32
                using SocketType = SOCKET;
#else
                using SocketType = int;
#endif
                Socket() = default;

                Socket(const Socket &another) = delete;

                ~Socket() {
                    close();
                }

                Socket &operator=(const Socket &another) = delete;

                [[nodiscard]] SocketType socket() const {
                    return m_socket;
                }

                [[nodiscard]] bool isOpened() const {
                    return m_socket != INVALID_SOCKET;
                }

                void open();
                void close();

            private:
                SocketType m_socket{INVALID_SOCKET};
            };
        }// namespace httpreader
    }    // namespace internal
}// namespace pd