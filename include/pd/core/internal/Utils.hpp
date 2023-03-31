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

#include <algorithm>
#include <cctype>
#include <locale>

namespace pd {
    namespace internal {
        inline std::string &ltrim(std::string &s) {
            s.erase(s.begin(), std::find_if(s.begin(), s.end(),
                                            [](unsigned char ch) {
                                                return !std::isspace(ch);
                                            }));
            return s;
        }

        inline std::string &rtrim(std::string &s) {
            s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
                        return !std::isspace(ch);
                    }).base(),
                    s.end());
            return s;
        }

        inline std::string &trim(std::string &s) {
            return ltrim(rtrim(s));
        }

        inline bool strcompci(const std::string &s1, const std::string &s2) {
            std::string s1Upper;
            std::transform(s1.cbegin(), s1.cend(), std::back_inserter(s1Upper), [](char c) { return static_cast<char>(std::toupper(c)); });
            std::string s2Upper;
            std::transform(s2.cbegin(), s2.cend(), std::back_inserter(s2Upper), [](char c) { return static_cast<char>(std::toupper(c)); });
            return s1Upper == s2Upper;
        }
    }// namespace internal
}// namespace pd