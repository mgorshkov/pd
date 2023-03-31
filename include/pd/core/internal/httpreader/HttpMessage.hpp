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

#include <chrono>
#include <ostream>
#include <string>
#include <unordered_map>
#include <vector>

#include <pd/core/internal/httpreader/HttpVersion.hpp>
#include <pd/core/internal/httpreader/UriParser.hpp>

namespace pd {
    namespace internal {
        namespace httpreader {

            struct HttpMessage {
                enum class Type {
                    kNone,
                    kRequest,
                    kResponse
                };

                /// HTTP methods
                enum class Method {
                    kNone,
                    kOptions,
                    kGet,
                    kHead,
                    kPost,
                    kPut,
                    kDelete,
                    kTrace,
                    kConnect,
                    kLast
                };

                /// HTTP content types
                enum class ContentType {
                    kNone,
                    kOctetStream,
                    kXml,
                    kHtml,
                    kPlain,
                    kNumContextTypes
                };

                enum class ContentTransferEncoding {
                    kNone,
                    k7bit,
                    kQuotedPrintable,
                    kBase64,
                    k8bit,
                    kBinary,
                    kXToken,
                    kNumContextTransferEncodings
                };

                /// Supported status codes for HTTP response according to RFC
                /// (see http://www.ietf.org/rfc/rfc2616.txt)
                enum class StatusCodes {
                    kNone = 0,
                    kContinue = 100,
                    kSwitchingProtocols = 101,
                    kOk = 200,
                    kCreated = 201,
                    kAccepted = 202,
                    kNonAuthInfo = 203,
                    kNoContent = 204,
                    kResetContent = 205,
                    kPartialContent = 206,
                    kMultipleChoices = 300,
                    kMovedPermanently = 301,
                    kFound = 302,
                    kSeeOther = 303,
                    kNotModified = 304,
                    kUseProxy = 305,
                    kTemporaryRedirect = 307,
                    kBadRequest = 400,
                    kUnauthorized = 401,
                    kPaymentRequired = 402,
                    kForbidden = 403,
                    kNotFound = 404,
                    kMethodNotAllowed = 405,
                    kNotAcceptable = 406,
                    kProxyAuthRequired = 407,
                    kRequestTimeout = 408,
                    kConflict = 409,
                    kGone = 410,
                    kLengthRequired = 411,
                    kPreconditionFailed = 412,
                    kEntityTooLarge = 413,
                    kUriTooLarge = 414,
                    kUnsupportedMediaType = 415,
                    kRangeBad = 416,
                    kExpectationFailed = 417,
                    kInternalRrror = 500,
                    kNotImplemented = 501,
                    kBadGateway = 502,
                    kServiceUnavailable = 503,
                    kGatewayTimeout = 504,
                    kVersionNotSupported = 505
                };

                HttpMessage() = default;

                explicit HttpMessage(const std::string &message);

                HttpMessage(HttpMessage::Method httpMethod, const std::string &userAgent, const Uri &uri)
                    : m_type{httpMethod == Method::kNone ? Type::kResponse : Type::kRequest}, m_httpMethod{httpMethod}, m_userAgent{userAgent}, m_uri{uri}, m_contentType(ContentType::kNone), m_wellFormed{true} {
                }

                HttpMessage(const HttpMessage &another) = default;

                HttpMessage &operator=(const HttpMessage &another) = default;

                ~HttpMessage() = default;

                std::string formatMethod() const;

                std::string formatStatusLine() const;

                friend std::ostream &operator<<(std::ostream &stream, const HttpMessage &message);

                bool isWellFormed() const {
                    return m_wellFormed;
                }

                void parseLine(std::string &line);

                void parseCookies(const std::string &string);

                std::string formatDateTime() const;

                std::string formatCookies() const;

                std::string encodeData() const;

                void decodeData(const std::string &encoded);

                static const char *findStatusMessage();

                std::vector<char> getData() const;

                bool m_firstLineParsed{false};
                Type m_type{Type::kNone};
                std::vector<char> m_binary;
                std::string m_text;
                Method m_httpMethod{Method::kNone};
                std::string m_userAgent;
                Uri m_uri;
                std::string m_location;
                HttpVersion m_httpVersion{};
                ContentType m_contentType{ContentType::kNone};
                ContentTransferEncoding m_contentTransferEncoding{ContentTransferEncoding::kNone};
                bool m_wellFormed{false};
                std::size_t m_contentLength{0};
                std::size_t m_headerSize{0};
                std::chrono::time_point<std::chrono::system_clock> m_dateTime{std::chrono::system_clock::now()};
                std::unordered_map<std::string, std::string> m_cookies;

                StatusCodes m_statusCode{StatusCodes::kNone};

                struct ReasonPhrase {
                    StatusCodes code;
                    const char *description;
                };
            };
        }// namespace httpreader
    }    // namespace internal
}// namespace pd