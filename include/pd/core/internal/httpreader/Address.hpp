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

#include <string>

namespace pd {
    namespace internal {
        namespace httpreader {
            class Address {
            public:
                explicit Address(std::uint16_t port = 0, std::uint32_t ipAddress = 0x7F000001)// Default ip = 127.0.0.1
                    : m_port{port}, m_ipAddress{ipAddress} {
                }

                Address(std::uint16_t port, const std::string &ipAddressOrName, bool resolve = true)
                    : m_port{port}, m_ipAddress{0}, m_ipAddressOrName{ipAddressOrName}, m_resolved{true}, m_resolveName{resolve} {
                }

                explicit Address(const std::string &host, bool resolve = true);

                Address(const Address &addr) = default;
                Address &operator=(const Address &addr) = default;

                bool operator==(const Address &addr) const {
                    return m_ipAddress == addr.m_ipAddress && m_port == addr.m_port && m_resolved == addr.m_resolved &&
                           m_ipAddressOrName == addr.m_ipAddressOrName && m_resolveName == addr.m_resolveName;
                }

                ~Address() = default;

                std::uint32_t ipAddress() const {
                    resolveIfNotResolved();
                    return m_ipAddress;
                }

                // returns ip as text. makes DNS request if needed
                std::string textIpAddress() const;

                std::string ipAddressOrName() const {
                    return m_ipAddressOrName;
                }

                // returns ip in form 'host:port'. makes DNS request if needed
                std::string ipAddressAndPort() const;

                std::uint16_t port() const {
                    return m_port;
                }

                // returns true if name resolution was successful, otherwise false
                bool ok() const {
                    resolveIfNotResolved();
                    return m_resolved;
                }

                bool initialized() const {
                    return m_port != 0;
                }

                void strToIp() const;

            private:
                void resolve() const;

                void resolveIfNotResolved() const;

                std::uint16_t m_port;
                mutable std::uint32_t m_ipAddress;
                std::string m_ipAddressOrName;
                mutable bool m_resolved{true};
                bool m_resolveName{false};
            };
        }// namespace httpreader
    }    // namespace internal
}// namespace pd