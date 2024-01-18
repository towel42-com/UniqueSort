#ifndef __SETTINGS_H
#define __SETTINGS_H
#include <widemath.h>

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
#include <iostream>
#include <cstdint>

class CSettings
{
public:
    CSettings( int argc, char **argv );
    CSettings( const std::vector< std::string > &args );
    CSettings( const std::vector< const char * > &args );

    static void showHelp();
    bool process() const;

    bool aOK() const { return fAOK; }
    void dump() const;

    std::size_t numFiles() const { return fFileNames.size(); }
    bool unique() const { return fUnique; }
    uint64_t sortColumn() const { return fSortColumn; }
    char separator() const { return fSeparator; }

    private:
    void init( const std::vector< std::string > &args );
    bool fAOK{ false };
    void createStreams();
    bool deleteStreams() const;

    char fSeparator{ ' ' };
    uint64_t fSortColumn{ -1*1ULL };
    bool fUnique{ false };
    std::vector< std::string > fFileNames;
    std::vector< std::istream * > fStreams;

};

#endif