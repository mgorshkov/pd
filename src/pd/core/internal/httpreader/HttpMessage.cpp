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
#include <cstring>
#include <iomanip>
#include <sstream>

#include <pd/core/internal/Utils.hpp>
#include <pd/core/internal/base64/Base64.hpp>
#include <pd/core/internal/httpreader/HttpMessage.hpp>

namespace pd {
    namespace internal {
        namespace httpreader {
            static constexpr const char kCookieFieldSeparator = ';';
            static constexpr const char kCookieValueSeparator = '=';
            static constexpr const char kStringSeparator[] = "\r\n";
            static constexpr const char *kMethodTag[] = {"", "OPTIONS", "GET", "HEAD", "POST", "PUT", "DELETE", "TRACE", "CONNECT"};
            static constexpr const std::size_t kSeparatorLen = sizeof(kStringSeparator) - 1;
            static constexpr const char *kUserAgentTag = "User-Agent";
            static constexpr const char *kHostTag = "Host";
            static constexpr const char *kContentLengthTag = "Content-Length";
            static constexpr const char *kContentTypeTag = "Content-Type";
            static constexpr const char *kContentTypeStrs[] = {"", "application/octet-stream", "text/xml", "text/html", "text/plain"};
            static constexpr const char *kContentTransferEncodingTag = "Content-Transfer-Encoding";
            static constexpr const char *kContentTransferEncodingStrs[] = {"", "7bit", "quoted-printable", "base64", "8bit", "binary", "x-token"};
            static constexpr const char *kDateTag = "Date";
            static constexpr const char *kDateTimeFormat = "%b, %d %Y %H:%M:%S GMT";
            static constexpr const char *kCookieTag = "Cookie";
            static constexpr const char *kSetCookieTag = "Set-Cookie";
            static constexpr const char *kLocationTag = "Location";

            HttpMessage::HttpMessage(const std::string &message)
                : HttpMessage{} {
                std::size_t prevPos = 0;
                std::size_t pos = message.find(kStringSeparator);
                while (pos != std::string::npos) {
                    std::string line = message.substr(prevPos, pos - prevPos);
                    if (line.empty()) {
                        break;// header ended
                    }

                    prevPos = pos + kSeparatorLen;
                    parseLine(line);
                    pos = message.find(kStringSeparator, prevPos);
                }

                if (pos != std::string::npos) {
                    m_headerSize = pos + kSeparatorLen;

                    std::string encoded = message.substr(m_headerSize);

                    m_wellFormed = encoded.length() == m_contentLength;

                    if (m_wellFormed && !encoded.empty()) {
                        decodeData(encoded);
                    }
                }
            }

            std::string HttpMessage::formatMethod() const {
                std::stringstream stream;
                stream << kMethodTag[static_cast<std::size_t>(m_httpMethod)] << " " << m_uri.m_url << " " << m_httpVersion;
                return stream.str();
            }

            std::string HttpMessage::formatStatusLine() const {
                std::stringstream stream;
                stream << m_httpVersion << " " << static_cast<std::size_t>(m_statusCode) << " " << findStatusMessage();
                return stream.str();
            }

            void HttpMessage::parseLine(std::string &line) {
                static const char *kFieldSeparator = ":";
                static const char *kStatusSeparator = " \t";

                if (!m_firstLineParsed) {
                    auto method = std::strtok(line.data(), kStatusSeparator);
                    if (method == nullptr) {
                        throw std::runtime_error("Invalid header format");
                    }
                    m_type = Type::kResponse;
                    for (std::size_t i = 0; i < static_cast<std::size_t>(Method::kLast); ++i) {
                        if (strcompci(method, kMethodTag[i])) {
                            m_httpMethod = static_cast<Method>(i);
                            m_type = Type::kRequest;
                            break;
                        }
                    }
                    std::string version;
                    if (m_type == Type::kRequest) {
                        auto status = std::strtok(nullptr, kStatusSeparator);
                        if (status == nullptr) {
                            throw std::runtime_error("Invalid header format");
                        }
                        m_uri.m_url = status;
                        version = std::strtok(nullptr, kStatusSeparator);
                    } else {
                        version = method;
                        std::string code = std::strtok(nullptr, kStatusSeparator);
                        m_statusCode = code.empty() ? StatusCodes::kNone : static_cast<StatusCodes>(std::stoul(code));
                    }
                    m_httpVersion = HttpVersion{version};
                    m_firstLineParsed = true;
                    return;
                }

                std::size_t pos = line.find(kFieldSeparator);
                if (pos == std::string::npos) {
                    throw std::runtime_error("Invalid header format");
                }
                std::string field = line.substr(0, pos);
                std::string value = line.substr(pos + strlen(kFieldSeparator));
                internal::trim(value);

                if (strcompci(field, kUserAgentTag)) {
                    m_userAgent = value;
                } else if (strcompci(field, kHostTag)) {
                    m_uri.m_address = Address(value);
                } else if (strcompci(field, kContentLengthTag)) {
                    m_contentLength = std::stoul(value);
                } else if (strcompci(field, kContentTypeTag)) {
                    for (std::size_t i = 0; i < static_cast<std::size_t>(ContentType::kNumContextTypes); ++i) {
                        if (strcompci(value, kContentTypeStrs[i])) {
                            m_contentType = static_cast<ContentType>(i);
                        }
                    }
                } else if (strcompci(field, kContentTransferEncodingTag)) {
                    for (std::size_t i = 0; i < static_cast<std::size_t>(ContentTransferEncoding::kNumContextTransferEncodings); ++i) {
                        if (strcompci(value, kContentTransferEncodingStrs[i])) {
                            m_contentTransferEncoding = static_cast<ContentTransferEncoding>(i);
                        }
                    }
                } else if (strcompci(field, kDateTag)) {
                    std::tm tm = {};
                    std::stringstream sstream(value);
                    sstream >> std::get_time(&tm, kDateTimeFormat);
                    m_dateTime = std::chrono::system_clock::from_time_t(std::mktime(&tm));
                } else if ((strcompci(field, kCookieTag) && m_type == Type::kRequest) ||
                           (strcompci(field, kSetCookieTag) && m_type == Type::kResponse)) {
                    parseCookies(value);
                } else if (strcompci(field, kLocationTag) && m_type == Type::kResponse) {
                    m_location = value;
                }
            }

            void HttpMessage::parseCookies(const std::string &string) {
                m_cookies.clear();
                std::size_t posOld = 0;
                while (posOld != std::string::npos) {
                    std::size_t pos = string.find(kCookieFieldSeparator, posOld);

                    auto cookie = string.substr(posOld, pos - posOld);

                    std::size_t cookieValuePos = cookie.find(kCookieFieldSeparator);
                    std::string value{cookie.substr(0, cookieValuePos)};
                    trim(value);
                    std::size_t attrPos = value.find(kCookieValueSeparator);
                    std::string attr = value.substr(attrPos);
                    trim(attr);
                    m_cookies[attr] = value;
                    posOld = pos;
                }
            }

            const char *HttpMessage::findStatusMessage() {
                static const ReasonPhrase kReasonPhrases[] =
                        {
                                {StatusCodes::kNone, ""},
                                {StatusCodes::kContinue, "Continue"},
                                {StatusCodes::kSwitchingProtocols, "Switching Protocols"},
                                {StatusCodes::kOk, "OK"},
                                {StatusCodes::kCreated, "Created"},
                                {StatusCodes::kAccepted, "Accepted"},
                                {StatusCodes::kNonAuthInfo, "Non-Authoritative Information"},
                                {StatusCodes::kNoContent, "No Content"},
                                {StatusCodes::kResetContent, "Reset Content"},
                                {StatusCodes::kPartialContent, "Partial Content"},
                                {StatusCodes::kMultipleChoices, "Multiple Choices"},
                                {StatusCodes::kMovedPermanently, "Moved Permanently"},
                                {StatusCodes::kFound, "Found"},
                                {StatusCodes::kSeeOther, "See Other"},
                                {StatusCodes::kNotModified, "Not Modified"},
                                {StatusCodes::kUseProxy, "Use Proxy"},
                                {StatusCodes::kTemporaryRedirect, "Temporary Redirect"},
                                {StatusCodes::kBadRequest, "Bad Request"},
                                {StatusCodes::kUnauthorized, "Unauthorized"},
                                {StatusCodes::kPaymentRequired, "Payment Required"},
                                {StatusCodes::kForbidden, "Forbidden"},
                                {StatusCodes::kNotFound, "Not Found"},
                                {StatusCodes::kMethodNotAllowed, "Method Not Allowed"},
                                {StatusCodes::kNotAcceptable, "Not Acceptable"},
                                {StatusCodes::kProxyAuthRequired, "Proxy Authentication Required"},
                                {StatusCodes::kRequestTimeout, "Request Time-out"},
                                {StatusCodes::kConflict, "Conflict"},
                                {StatusCodes::kGone, "Gone"},
                                {StatusCodes::kLengthRequired, "Length Required"},
                                {StatusCodes::kPreconditionFailed, "Precondition Failed"},
                                {StatusCodes::kEntityTooLarge, "Request Entity Too Large"},
                                {StatusCodes::kUriTooLarge, "Request-URI Too Large"},
                                {StatusCodes::kUnsupportedMediaType, "Unsupported Media Type"},
                                {StatusCodes::kRangeBad, "Requested range not satisfiable"},
                                {StatusCodes::kExpectationFailed, "Expectation Failed"},
                                {StatusCodes::kInternalRrror, "Internal Server Error"},
                                {StatusCodes::kNotImplemented, "Not Implemented"},
                                {StatusCodes::kBadGateway, "Bad Gateway"},
                                {StatusCodes::kServiceUnavailable, "Service Unavailable"},
                                {StatusCodes::kGatewayTimeout, "Gateway Time-out"},
                                {StatusCodes::kVersionNotSupported, "HTTP Version not supported"}};
                for (std::size_t i = 0; i < sizeof(StatusCodes); ++i) {
                    if (i == static_cast<std::size_t>(kReasonPhrases[i].code)) {
                        return kReasonPhrases[i].description;
                    }
                }

                assert(false);
                return "";
            }

            std::vector<char> HttpMessage::getData() const {
                if (m_contentType == ContentType::kOctetStream && m_contentTransferEncoding == ContentTransferEncoding::kBase64) {
                    return m_binary;
                }
                std::vector<char> text;
                std::copy(m_text.cbegin(), m_text.end(), std::back_inserter(text));
                return text;
            }

            std::string HttpMessage::formatDateTime() const {
                auto time = std::chrono::system_clock::to_time_t(m_dateTime);
                std::tm *tm = std::localtime(&time);
                std::stringstream sstream;
                sstream << std::put_time(tm, kDateTimeFormat);
                return sstream.str();
            }

            std::string HttpMessage::formatCookies() const {
                std::stringstream cookiesFormatted;

                if (!m_cookies.empty()) {
                    cookiesFormatted << (m_type == Type::kRequest ? kCookieTag : kSetCookieTag),
                            cookiesFormatted << ": ";
                    for (const auto &m_cookie: m_cookies) {
                        cookiesFormatted << m_cookie.first << kCookieValueSeparator << m_cookie.second << kCookieFieldSeparator;
                    }
                }

                return cookiesFormatted.str();
            }

            std::string HttpMessage::encodeData() const {
                return m_contentType == ContentType::kOctetStream ? base64::encode(m_binary) : m_text;
            }

            void HttpMessage::decodeData(const std::string &encoded) {
                if (m_contentType == ContentType::kOctetStream && m_contentTransferEncoding == ContentTransferEncoding::kBase64) {
                    m_binary = base64::decode(encoded);
                } else {
                    m_text = encoded;
                }
            }

            std::ostream &operator<<(std::ostream &stream, const HttpMessage &message) {
                assert(message.m_type == HttpMessage::Type::kResponse || message.m_statusCode == HttpMessage::StatusCodes::kNone);

                if (message.m_type == HttpMessage::Type::kRequest) {
                    stream << message.formatMethod();
                } else {
                    stream << message.formatStatusLine();
                }

                stream << kStringSeparator;

                stream << kHostTag << ": " << message.m_uri.m_address.ipAddressOrName();
                stream << kStringSeparator;

                stream << kUserAgentTag << ": " << message.m_userAgent;
                stream << kStringSeparator;

                std::string encodedData = message.encodeData();
                if (!encodedData.empty()) {
                    stream << kContentLengthTag << ": " << encodedData.size();
                    stream << kStringSeparator;

                    stream << kContentTypeTag << ": " << kContentTypeStrs[static_cast<std::size_t>(message.m_contentType)];
                    stream << kStringSeparator;

                    if (message.m_contentType == HttpMessage::ContentType::kOctetStream) {
                        stream << kContentTransferEncodingTag << ": " << kContentTransferEncodingStrs[static_cast<std::size_t>(HttpMessage::ContentTransferEncoding::kBase64)];
                        stream << kStringSeparator;
                    }
                }

                stream << kDateTag << ": " << message.formatDateTime();
                stream << kStringSeparator;

                std::string cookiesFormatted = message.formatCookies();
                if (!cookiesFormatted.empty()) {
                    stream << cookiesFormatted;
                    stream << kStringSeparator;
                }

                stream << kStringSeparator;
                stream << encodedData;

                return stream;
            }
        }// namespace httpreader
    }    // namespace internal
}// namespace pd