#ifndef __UTILS_H
#define __UTILS_H
// The MIT License( MIT )
//
// Copyright( c ) 2024 Scott Aron Bloom
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files( the "Software" ), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sub-license, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <string>
#include <vector>

char getEscapedChar( const std::string &escaped );
char getSeparator( std::size_t &ii, int argc, char **argv );
char getSeparator( std::size_t &ii, const std::vector< std::string > &args );
std::vector< char * > toCStrings( const std::vector< std::string > &args );

    static const char *kWS = " \t\n\r\f\v";

std::vector< std::string > splitLine( const std::string &line, bool keepEmpty = true, const char *sep = kWS );
std::string getNextToken( const std::string &line, std::size_t &startPos, const char *sep = kWS );

std::vector< std::string > splitLine( const std::string &line, bool keepEmpty = true, const std::string &sep = kWS );
std::string getNextToken( const std::string &line, std::size_t &startPos, const std::string &sep = kWS );

std::vector< std::string > splitLine( const std::string &line, bool keepEmpty = true, char sep = ' ' );
std::string getNextToken( const std::string &line, std::size_t &startPos, char sep );

#endif