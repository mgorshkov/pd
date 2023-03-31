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

#include <cstring>
#include <pd/core/internal/httpreader/UriParser.hpp>

namespace pd {
    namespace internal {
        namespace httpreader {
            struct SchemeStrs {
                Scheme m_scheme;
                const char *m_str;
                std::uint16_t m_port;
            } schemeStrs[] = {
                    {Scheme::kHttp, "http://", 80},
                    {Scheme::kHttps, "https://", 443},
                    {Scheme::kFtp, "ftp://", 20}};

            Uri parseUri(const std::string &uri) {
                // URI = scheme ":" ["//" authority] path ["?" query] ["#" fragment]
                // authority = [userinfo "@"] host [":" port]
                for (const auto &schemeStr: schemeStrs) {
                    if (uri.find(schemeStr.m_str) == 0) {
                        Uri retUri;
                        retUri.m_scheme = schemeStr.m_scheme;
                        std::size_t schemeLen = std::strlen(schemeStr.m_str);
                        std::size_t urlPos = uri.find('/', schemeLen);
                        std::string host = uri.substr(schemeLen, urlPos - schemeLen);
                        std::uint16_t port = schemeStr.m_port;
                        std::size_t portPos = uri.find(':', urlPos);
                        if (portPos != std::string::npos) {
                            port = static_cast<std::uint16_t>(std::stoi(uri.substr(portPos + 1)));
                        }
                        retUri.m_url = uri.substr(urlPos);
                        retUri.m_address = Address(port, host);
                        return retUri;
                    }
                }
                return {};
            }

        }// namespace httpreader
    }    // namespace internal
}// namespace pd