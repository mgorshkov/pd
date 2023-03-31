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

#include <cerrno>
#include <cstring>

#ifdef _WIN32
#include <Windows.h>
#include <Winsock.h>
#endif

#include <pd/core/internal/httpreader/Error.hpp>

namespace pd {
    namespace internal {
        namespace httpreader {

            static int getLastError() {
#ifdef _WIN32
                int err = WSAGetLastError();
#else
                int err = errno;
#endif
                return err;
            }

            static std::string getErrorMessage(int error) {
#ifdef _WIN32
                std::string res;
                char *s = NULL;
                FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, error,
                               MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR) &s, 0, NULL);
                res = s;
                LocalFree(s);
                return res;
#else
                return std::strerror(error);
#endif
            }

            void throwError(int error) {
                throw std::runtime_error(getErrorMessage(error));
            }

            void throwError() {
                throwError(getLastError());
            }
        }// namespace httpreader
    }    // namespace internal
}// namespace pd
