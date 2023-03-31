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

#include <pd/core/internal/httpreader/Connector.hpp>
#include <pd/core/internal/httpreader/Error.hpp>

namespace pd {
    namespace internal {
        namespace httpreader {

            void connect(const StreamPtr &stream, const Address &address) {
                assert(stream && !stream->isOpened());

                sockaddr_in addr{};
                addr.sin_family = AF_INET;
                addr.sin_port = htons(address.port());
                addr.sin_addr.s_addr = htonl(address.ipAddress());

                stream->setAddress(address);
                stream->open();

                if (::connect(stream->getSocket().socket(), reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) == SOCKET_ERROR) {
                    throwError();
                }
            }
        }// namespace httpreader
    }    // namespace internal
}// namespace pd