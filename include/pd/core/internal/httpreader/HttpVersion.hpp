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

#include <ostream>
#include <string>

namespace pd {
    namespace internal {
        namespace httpreader {

            /// HTTP version as described in RFC2616
            class HttpVersion {
            public:
                HttpVersion() = default;

                // example: 'HTTP/1.1'
                HttpVersion(const std::string &protocol, int major, int minor)
                    : m_protocol{protocol}, m_major{major}, m_minor{minor} {
                }

                explicit HttpVersion(const std::string &version) {
                    auto pos = version.find(kProtocolDelimiter);
                    if (pos == std::string::npos) {
                        throw std::runtime_error("Invalid version format");
                    }
                    m_protocol = version.substr(0, pos);
                    auto rest = version.substr(pos + 1);
                    auto versionDelimPos = rest.find(kVersionDelimiter);
                    if (versionDelimPos == std::string::npos) {
                        throw std::runtime_error("Invalid version format");
                    }
                    m_major = std::stoi(rest.substr(0, versionDelimPos));
                    m_minor = std::stoi(rest.substr(versionDelimPos + 1));
                }

                HttpVersion(const HttpVersion &another) = default;
                HttpVersion(HttpVersion &&another) = default;

                ~HttpVersion() = default;

                HttpVersion &operator=(const HttpVersion &version) = default;
                HttpVersion &operator=(HttpVersion &&version) = default;

                bool operator==(const HttpVersion &version) const {
                    return m_protocol == version.m_protocol &&
                           m_major == version.m_major &&
                           m_minor == version.m_minor;
                }

                friend std::ostream &operator<<(std::ostream &stream, const HttpVersion &version) {
                    stream << version.m_protocol << kProtocolDelimiter << version.m_major << kVersionDelimiter << version.m_minor;
                    return stream;
                }

            private:
                std::string m_protocol = "HTTP";//< protocol name
                int m_major{1};                 //< major protocol version
                int m_minor{1};                 //< minor protocol version

                static const char kProtocolDelimiter = '/';// protocol and version separator
                static const char kVersionDelimiter = '.'; // major and minor version separator
            };
        }// namespace httpreader
    }    // namespace internal
}// namespace pd