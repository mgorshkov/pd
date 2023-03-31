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

#include <ctype.h>

#include <pd/core/internal/base64/Base64.hpp>

namespace pd {
    namespace internal {
        namespace base64 {

            static const char kConversionStr[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
            static int kModTable[] = {0, 2, 1};

            static bool decodingTableInited = false;
            static char kDecodingTable[256];

            void buildDecodingTable() {
                for (std::size_t i = 0; i < sizeof(kConversionStr) - 1; ++i) {
                    kDecodingTable[static_cast<std::size_t>(static_cast<unsigned char>(kConversionStr[i]))] = static_cast<char>(i);
                }
            }

            std::string encode(const std::vector<char> &input) {
                std::size_t inputSize{input.size()};
                std::size_t outputSize = 4 * (inputSize + 2) / 3;
                std::string output;
                output.reserve(outputSize);

                std::size_t i = 0, j = 0;
                while (i < inputSize) {
                    uint32_t octet_a = i < inputSize ? input[i++] : 0;
                    uint32_t octet_b = i < inputSize ? input[i++] : 0;
                    uint32_t octet_c = i < inputSize ? input[i++] : 0;

                    uint32_t triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;

                    output[j++] = kConversionStr[(triple >> 3 * 6) & 0x3F];
                    output[j++] = kConversionStr[(triple >> 2 * 6) & 0x3F];
                    output[j++] = kConversionStr[(triple >> 1 * 6) & 0x3F];
                    output[j++] = kConversionStr[(triple >> 0 * 6) & 0x3F];
                }

                for (i = 0; i < static_cast<std::size_t>(kModTable[inputSize % 3]); i++)
                    output[outputSize - 1 - i] = '=';

                return output;
            }

            std::vector<char> decode(const std::string &input) {
                if (!decodingTableInited) {
                    buildDecodingTable();
                    decodingTableInited = true;
                }

                std::vector<char> output;
                std::size_t inputSize = input.size();
                if (inputSize % 4 != 0) {
                    return output;
                }

                std::size_t outputSize = inputSize / 4 * 3;
                output.reserve(outputSize);
                if (input[inputSize - 1] == '=') {
                    outputSize--;
                }
                if (input[inputSize - 2] == '=') {
                    outputSize--;
                }

                std::size_t i = 0, j = 0;
                while (i < inputSize) {
                    uint32_t sextet_a = input[i] == '=' ? 0 & i++ : kDecodingTable[static_cast<std::size_t>(input[i++])];
                    uint32_t sextet_b = input[i] == '=' ? 0 & i++ : kDecodingTable[static_cast<std::size_t>(input[i++])];
                    uint32_t sextet_c = input[i] == '=' ? 0 & i++ : kDecodingTable[static_cast<std::size_t>(input[i++])];
                    uint32_t sextet_d = input[i] == '=' ? 0 & i++ : kDecodingTable[static_cast<std::size_t>(input[i++])];

                    uint32_t triple = (sextet_a << 3 * 6) + (sextet_b << 2 * 6) + (sextet_c << 1 * 6) + (sextet_d << 0 * 6);

                    if (j < outputSize) {
                        output[j++] = (triple >> 2 * 8) & 0xFF;
                    }
                    if (j < outputSize) {
                        output[j++] = (triple >> 1 * 8) & 0xFF;
                    }
                    if (j < outputSize) {
                        output[j++] = (triple >> 0 * 8) & 0xFF;
                    }
                }

                return output;
            }
        }// namespace base64
    }    // namespace internal
}// namespace pd