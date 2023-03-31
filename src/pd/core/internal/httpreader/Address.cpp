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

#include <cassert>
#include <sstream>

#include <pd/core/internal/httpreader/Address.hpp>

#ifdef _WIN32
#include <Winsock2.h>
#include <ws2tcpip.h>
#else
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#endif

namespace pd {
    namespace internal {
        namespace httpreader {

            static const char kIpSeparator = '.';
            static const char kPortDelimiter = ':';

            Address::Address(const std::string &host, bool resolve)
                : m_port{0}, m_ipAddress{0}, m_resolved{true}, m_resolveName{resolve} {
                auto pos = host.find(kPortDelimiter);
                if (pos == std::string::npos) {
                    m_ipAddressOrName = host;
                    m_port = 80;
                } else {
                    m_ipAddressOrName = host.substr(pos, host.size() - pos);
                    m_port = static_cast<std::uint16_t>(std::stoi(m_ipAddressOrName));
                }
            }

            std::string Address::textIpAddress() const {
                resolveIfNotResolved();

                std::ostringstream sstream;
                sstream << ((m_ipAddress >> 24) & 0xFF)
                        << kIpSeparator
                        << ((m_ipAddress >> 16) & 0xFF)
                        << kIpSeparator
                        << ((m_ipAddress >> 8) & 0xFF)
                        << kIpSeparator
                        << (m_ipAddress & 0xFF);
                return sstream.str();
            }

            std::string Address::ipAddressAndPort() const {
                std::ostringstream sstream;
                sstream << textIpAddress() << kPortDelimiter << m_port;
                return sstream.str();
            }

            void Address::resolve() const {
                addrinfo *info = nullptr;
                addrinfo hints{};
                hints.ai_family = AF_INET;
                hints.ai_socktype = SOCK_STREAM;
                hints.ai_protocol = IPPROTO_TCP;

                m_resolved = false;

                if (getaddrinfo(
                            m_ipAddressOrName.c_str(),
                            std::to_string(m_port).c_str(),
                            &hints,
                            &info) == 0) {
                    assert(info);
                    auto *addr = reinterpret_cast<sockaddr_in *>(info->ai_addr);
                    m_ipAddress = htonl(addr->sin_addr.s_addr);
                    freeaddrinfo(info);
                    m_resolved = true;
                }
            }

            void Address::resolveIfNotResolved() const {
                if (m_ipAddress == 0 && !m_ipAddressOrName.empty()) {
                    if (m_resolveName) {
                        resolve();
                    } else {
                        strToIp();
                    }
                }
            }

            void Address::strToIp() const {
                // we should make a conversion from "127.0.0.1" to 0x7F000001
                m_ipAddress = 0;
                std::size_t prevPos = 0;
                for (auto i = 4; i--;) {
                    auto pos = m_ipAddressOrName.find(kIpSeparator, prevPos);
                    char *endptr = nullptr;
                    std::uint8_t part = static_cast<std::uint8_t>(std::strtoul(m_ipAddressOrName.substr(prevPos, pos - prevPos).c_str(), &endptr, 10));
                    m_ipAddress += (part << i * 8);
                    prevPos = pos;
                }
            }

        }// namespace httpreader
    }    // namespace internal
}// namespace pd