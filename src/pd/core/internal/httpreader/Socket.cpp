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

#include <pd/core/internal/httpreader/Error.hpp>
#include <pd/core/internal/httpreader/Socket.hpp>

#ifdef _WIN32
#define socklen_t int
#endif


namespace pd {
    namespace internal {
        namespace httpreader {

            void Socket::open() {
                assert(m_socket == INVALID_SOCKET);

                SocketType socket = ::socket(AF_INET, SOCK_STREAM, 0);
                if (socket == INVALID_SOCKET) {
                    throwError();
                }

                m_socket = socket;
            }

            void Socket::close() {
                if (m_socket != INVALID_SOCKET) {
#ifdef _WIN32
                    closesocket(m_socket);
#else
                    ::close(m_socket);
#endif

                    m_socket = INVALID_SOCKET;
                }
            }

        }// namespace httpreader
    }    // namespace internal
}// namespace pd
