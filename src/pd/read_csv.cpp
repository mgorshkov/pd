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

#include <atomic>
#include <filesystem>
#include <string>

#include <pd/core/internal/httpreader/HttpHandler.hpp>
#include <pd/core/internal/httpreader/HttpsHandler.hpp>
#include <pd/core/internal/httpreader/Initializer.hpp>
#include <pd/core/internal/httpreader/Poll.hpp>
#include <pd/core/internal/httpreader/SslInitializer.hpp>
#include <pd/core/internal/httpreader/UriParser.hpp>
#include <pd/read_csv.hpp>

namespace pd {
    struct ReadCsvContext {
        explicit ReadCsvContext(const ReadCsvSettings &settings)
            : m_settings{settings} {
        }
        ReadCsvSettings m_settings;
        bool m_firstLine{true};
        enum class ColumnType {
            kNone,
            kInt,
            kFloat,
            kString,
            kUnicode
        };
        std::vector<std::pair<internal::Value, ColumnType>> m_headers;
        std::string m_buffer;
        DataFrame m_dataFrame{};
        np::Size m_totalSamples{0};
        np::Size m_row{0};
    };

    static void processLine(ReadCsvContext *context, const std::string &currentLine) {
        // Pregnancies,Glucose,BloodPressure,SkinThickness,Insulin,BMI,DiabetesPedigreeFunction,Age,Outcome
        // 6,148,72,35,0,33.6,0.627,50,1
        // 1,85,66,29,0,26.6,0.351,31,0
        // 8,183,64,0,0,23.3,0.672,32,1
        const ReadCsvSettings &settings = context->m_settings;

        std::vector<internal::Value> columns;
        auto wordStart = currentLine.begin();
        auto wordEnd = wordStart;
        bool hasDigitInLine = false;
        bool hasDotInLine = false;
        while (wordEnd < currentLine.end() && *wordEnd != '\r') {
            const char separator = (settings.separator == Separator::kComma ? ',' : settings.separator == Separator::kTab ? '\t'
                                                                                                                          : '\0');
            bool hasDigitInWord = false;
            bool hasDotInWord = false;

            while (wordEnd != currentLine.end() && *wordEnd != separator && *wordEnd != '\r') {
                if (!hasDigitInLine && std::isdigit(*wordEnd)) {
                    hasDigitInLine = true;
                }
                if (!hasDigitInWord && std::isdigit(*wordEnd)) {
                    hasDigitInWord = true;
                }
                if (!hasDotInLine && *wordEnd == '.') {
                    hasDotInLine = true;
                }
                if (!hasDotInWord && *wordEnd == '.') {
                    hasDotInWord = true;
                }

                ++wordEnd;
            }
            std::string word{wordStart, wordEnd};
            if (hasDigitInWord && hasDotInWord)
                columns.emplace_back(std::stod(word));
            else if (hasDigitInWord)
                columns.emplace_back(std::stol(word));
            else
                columns.emplace_back(word);
            if (wordEnd == currentLine.end()) {
                break;
            }
            wordStart = wordEnd + 1;
            wordEnd = wordStart;
        }
        if (columns.empty())
            return;

        bool needToAddData = true;
        if (context->m_firstLine) {
            if (settings.header == Header::kInfer) {
                if (!hasDigitInLine && !hasDotInLine) {
                    for (const auto &column: columns) {
                        context->m_headers.emplace_back(*static_cast<const std::string *>(column), ReadCsvContext::ColumnType::kNone);
                    }
                    needToAddData = false;
                    --context->m_totalSamples;
                } else {
                    for (std::size_t i = 0; i < columns.size(); ++i) {
                        auto columnType = ReadCsvContext::ColumnType::kNone;
                        if (columns[i].isInt()) {
                            columnType = ReadCsvContext::ColumnType::kInt;
                        } else if (columns[i].isFloat()) {
                            columnType = ReadCsvContext::ColumnType::kFloat;
                        } else if (columns[i].isString()) {
                            columnType = ReadCsvContext::ColumnType::kString;
                        } else if (columns[i].isUnicode()) {
                            columnType = ReadCsvContext::ColumnType::kUnicode;
                        } else {
                            throw std::runtime_error("Invalid column type");
                        }
                        context->m_headers.emplace_back(static_cast<np::intc>(i), columnType);
                    }
                }
            } else if (settings.header == Header::kNo) {
                for (std::size_t i = 0; i < columns.size(); ++i) {
                    auto columnType = ReadCsvContext::ColumnType::kNone;
                    if (columns[i].isInt()) {
                        columnType = ReadCsvContext::ColumnType::kInt;
                    } else if (columns[i].isFloat()) {
                        columnType = ReadCsvContext::ColumnType::kFloat;
                    } else if (columns[i].isString()) {
                        columnType = ReadCsvContext::ColumnType::kString;
                    } else if (columns[i].isUnicode()) {
                        columnType = ReadCsvContext::ColumnType::kUnicode;
                    } else {
                        throw std::runtime_error("Invalid column type");
                    }
                    context->m_headers.emplace_back(static_cast<np::intc>(i), columnType);
                }
            } else {
                throw std::runtime_error("Invalid settings.header value");
            }
        }
        if (needToAddData) {
            for (std::size_t i = 0; i < columns.size(); ++i) {
                auto &[columnName, columnType] = context->m_headers[i];
                if (columnType == ReadCsvContext::ColumnType::kNone) {
                    if (columns[i].isInt()) {
                        columnType = ReadCsvContext::ColumnType::kInt;
                    } else if (columns[i].isFloat()) {
                        columnType = ReadCsvContext::ColumnType::kFloat;
                    } else if (columns[i].isString()) {
                        columnType = ReadCsvContext::ColumnType::kString;
                    } else if (columns[i].isUnicode()) {
                        columnType = ReadCsvContext::ColumnType::kUnicode;
                    } else {
                        throw std::runtime_error("Invalid column type");
                    }
                }
                if (!context->m_dataFrame.hasColumn(columnName)) {
                    switch (columnType) {
                        case ReadCsvContext::ColumnType::kInt: {
                            auto array = np::Array<np::int_>{np::Shape{context->m_totalSamples}};
                            auto series = Series{array, columnName};
                            context->m_dataFrame.add(series);
                            break;
                        }
                        case ReadCsvContext::ColumnType::kFloat: {
                            auto array = np::Array<np::float_>{np::Shape{context->m_totalSamples}};
                            auto series = Series{array, columnName};
                            context->m_dataFrame.add(series);
                            break;
                        }
                        case ReadCsvContext::ColumnType::kString: {
                            auto array = np::Array<np::string_>{np::Shape{context->m_totalSamples}};
                            auto series = Series{array, columnName};
                            context->m_dataFrame.add(series);
                            break;
                        }
                        case ReadCsvContext::ColumnType::kUnicode: {
                            auto array = np::Array<np::unicode_>{np::Shape{context->m_totalSamples}};
                            auto series = Series{array, columnName};
                            context->m_dataFrame.add(series);
                            break;
                        }
                        case ReadCsvContext::ColumnType::kNone:
                            throw std::runtime_error("Invalid column type");
                    }
                }
                context->m_dataFrame.at(context->m_row, columnName) = columns.at(i);
            }
        }
        if (needToAddData) {
            ++context->m_row;
        }
        if (context->m_firstLine) {
            context->m_firstLine = false;
        }
    }

    using namespace internal::httpreader;

    template<typename TInitializer, typename THandler>
    static void readWeb(const Uri &uri, ReadCsvContext *context) {
        TInitializer initializer;
        Uri uriLocal{uri};
        int redirectCount = 0;
        int kMaxRedirectCount = 5;
        bool redirect = false;
        do {
            Poll poll;
            auto handler = std::make_shared<THandler>(
                    "pd",
                    uriLocal,
                    [context](const std::vector<char> &buffer) {
                        context->m_buffer += std::string(buffer.data(), buffer.size());
                    },
                    [&uriLocal, &redirect](const std::string &url) {
                        uriLocal.m_url = url;
                        redirect = true;
                    });
            poll.addHandler(handler);
            poll.run();
        } while (redirect && redirectCount++ < kMaxRedirectCount);
    }

    static void readFtp(const Uri &uri, ReadCsvContext *context) {
        Initializer initializer;
        Poll poll;
        auto handler = std::make_shared<HttpHandler>(
                "pd",
                uri,
                [context](const std::vector<char> &buffer) {
                    context->m_buffer += std::string(buffer.data(), buffer.size());
                },
                [](const std::string &) {
                });
        poll.addHandler(handler);
        poll.run();
    }

    static void readLocal(const std::string &filepath, ReadCsvContext *context) {
        std::filesystem::path path{filepath};
        std::fstream stream;
        stream.open(filepath, std::ios::in);
        if (!stream.is_open()) {
            throw std::runtime_error("Cannot open file " + filepath);
        }
        stream.seekg(0, std::ios::end);
        auto size = stream.tellg();
        context->m_buffer.resize(size);
        stream.seekg(0);
        stream.read(&context->m_buffer[0], size);
    }

    DataFrame read_csv(const std::string &filepath, const ReadCsvSettings &settings) {
        if (filepath.empty()) {
            return DataFrame{};
        }
        ReadCsvContext context{settings};

        using namespace internal::httpreader;
        Uri uri = parseUri(filepath);
        if (uri.m_scheme == Scheme::kHttp) {
            readWeb<Initializer, HttpHandler>(uri, &context);
        } else if (uri.m_scheme == Scheme::kHttps) {
            readWeb<SslInitializer, HttpsHandler>(uri, &context);
        } else if (uri.m_scheme == Scheme::kFtp) {
            readFtp(uri, &context);
        } else {
            readLocal(filepath, &context);
        }

        std::string currentLine;
        {
            std::stringstream stream(context.m_buffer);
            context.m_totalSamples = 0;
            while (std::getline(stream, currentLine)) {
                bool whiteSpacesOnly = std::all_of(currentLine.begin(), currentLine.end(), isspace);
                if (!whiteSpacesOnly)
                    ++context.m_totalSamples;
            }
        }
        {
            std::stringstream stream(context.m_buffer);
            while (std::getline(stream, currentLine)) {
                processLine(&context, currentLine);
            }
        }
        return context.m_dataFrame;
    }
}// namespace pd
