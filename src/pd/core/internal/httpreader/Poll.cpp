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

#include <vector>

#ifdef _WIN32
#include <Winsock2.h>
#else
#include <poll.h>
#endif

#include <pd/core/internal/httpreader/Error.hpp>
#include <pd/core/internal/httpreader/Poll.hpp>

#ifdef _WIN32
static inline int poll(struct pollfd *pfd, int nfds, int timeout) { return WSAPoll(pfd, nfds, timeout); }
#endif

namespace pd {
    namespace internal {
        namespace httpreader {
            static constexpr int kTimeout = 10000;

            void Poll::run() {
                std::vector<pollfd> fds(m_handlers.size());

                while (!m_isStopped && !m_handlers.empty()) {
                    std::size_t i = 0;
                    for (const auto &handler: m_handlers) {
                        if (m_isStopped) {
                            return;
                        }
                        assert(handler);
                        const auto &socket = handler->getSocket();
                        if (!socket.isOpened()) {
                            continue;
                        }

                        auto sock = socket.socket();

                        fds[i].fd = sock;
                        fds[i].events = handler->getEvents();
                        ++i;
                    }
#ifdef _WIN32
                    int res = ::poll(fds.data(), static_cast<int>(m_handlers.size()), kTimeout);
#else
                    int res = ::poll(fds.data(), m_handlers.size(), kTimeout);
#endif

                    if (res == SOCKET_ERROR) {
                        throwError();
                    } else if (res == 0) {
                        continue;
                    }

                    i = 0;
                    for (const auto &handler: m_handlers) {
                        if (m_isStopped) {
                            return;
                        }
                        assert(handler);
                        const auto &socket = handler->getSocket();
                        if (!socket.isOpened()) {
                            continue;
                        }

                        if (fds[i].revents & handler->getEvents()) {
                            handler->handle(fds[i].revents);
                            fds[i].revents = 0;
                        }
                        ++i;
                    }

                    auto handlers{m_handlers};
                    for (const auto &handler: handlers) {
                        if (handler->isFinished()) {
                            removeHandler(handler);
                        }
                    }
                }
            }
        }// namespace httpreader
    }    // namespace internal
}// namespace pd