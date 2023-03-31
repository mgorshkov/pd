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
#include <pd/core/internal/httpreader/Stream.hpp>

namespace pd {
    namespace internal {
        namespace httpreader {
            std::size_t Stream::read(void *buffer, std::size_t count) const {
#ifdef _WIN32
                int res = ::recv(m_socket.socket(), static_cast<char *>(buffer), static_cast<int>(count), 0);
#else
                ssize_t res = ::recv(m_socket.socket(), buffer, count, 0);
#endif
                if (res == SOCKET_ERROR) {
                    throwError();
                }
                return res;
            }

            std::size_t Stream::write(const void *buffer, std::size_t count) const {
#ifdef _WIN32
                int res = ::send(m_socket.socket(), static_cast<const char *>(buffer), static_cast<int>(count), 0);
#else
                ssize_t res = ::send(m_socket.socket(), buffer, count, MSG_NOSIGNAL);
#endif
                if (res == SOCKET_ERROR) {
                    throwError();
                }
                return res;
            }

            void Stream::shutdown() {
                if (::shutdown(m_socket.socket(), SHUT_RDWR) == SOCKET_ERROR) {
                    throwError();
                }
            }
        }// namespace httpreader
    }    // namespace internal
}// namespace pd