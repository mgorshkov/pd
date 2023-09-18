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

#ifdef OPENSSL

#include <openssl/err.h>
#include <openssl/ssl.h>

#include <pd/core/internal/httpreader/Error.hpp>
#include <pd/core/internal/httpreader/SslStream.hpp>

namespace pd {
    namespace internal {
        namespace httpreader {
            void SslStream::open() {
                Stream::open();

                const SSL_METHOD *method = TLS_client_method();
                m_ctx = SSL_CTX_new(method);
                if (m_ctx == nullptr) {
                    throw std::runtime_error("Can't create SSL context");
                }
                m_ssl = SSL_new(m_ctx);
                if (m_ssl == nullptr) {
                    throw std::runtime_error("Can't create SSL");
                }
                const char *const PREFERED_CIPHERS = "HIGH:!aNULL:!kRSA:!PSK:!SRP:!MD5:!RC4";
                if (SSL_set_cipher_list(m_ssl, PREFERED_CIPHERS) != 1) {
                    throw std::runtime_error("Cannot set cipher list");
                }
            }

            void SslStream::setHost(const std::string &host) {
                if (SSL_set_tlsext_host_name(m_ssl, host.c_str()) != 1) {
                    throw std::runtime_error("Cannot set tlsext host name");
                }
            }

            void SslStream::connect() {
#ifdef _WIN32
                int sock = static_cast<int>(getSocket().socket());
#else
                int sock = getSocket().socket();
#endif
                if (SSL_set_fd(m_ssl, sock) == 0) {
                    throw std::runtime_error("Error setting fd");
                }
                int connectStatus = SSL_connect(m_ssl);
                int err = SSL_get_error(m_ssl, connectStatus);
                switch (err) {
                    case SSL_ERROR_NONE:
                        //No error, do nothing
                        break;
                    case SSL_ERROR_ZERO_RETURN:
                        throw std::runtime_error("Peer has closed connection");

                    case SSL_ERROR_SSL: {
                        shutdown();
                        std::string errStr = "Error in SSL library: ";
                        throw std::runtime_error(errStr + ERR_error_string(SSL_ERROR_SSL, nullptr));
                    }

                    default: {
                        std::string errStr = "Unknown error: ";
                        throw std::runtime_error(errStr + ERR_error_string(err, nullptr));
                    }
                }
            }

            void SslStream::close() {
                SSL_free(m_ssl);
                SSL_CTX_free(m_ctx);
                Stream::close();
            }

            std::size_t SslStream::read(void *buffer, std::size_t count) const {
                int readSize = SSL_read(m_ssl, buffer, static_cast<int>(count));
                if (readSize < 0) {
                    throw std::runtime_error("Error receiving message");
                }
                return readSize;
            }

            std::size_t SslStream::write(const void *buffer, std::size_t count) const {
                int writeSize = SSL_write(m_ssl, buffer, static_cast<int>(count));
                if (writeSize < 0) {
                    throw std::runtime_error("Error sending SSL message");
                }
                return writeSize;
            }
        }// namespace httpreader
    }    // namespace internal
}// namespace pd

#endif