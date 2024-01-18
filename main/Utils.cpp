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

#include "Utils.h"

#include <iostream>
#include <string>
#include <vector>

char getEscapedChar( const std::string &escaped )
{
    if ( escaped.empty() )
        return 0;

    if ( ( escaped[ 0 ] == '\\' ) && ( escaped.length() > 1 ) )
        return getEscapedChar( escaped.substr( 1 ) );

    bool singleChar = true;
    char retVal = 0;
    switch ( escaped[ 0 ] )
    {
        case 'a':
            retVal = 0x07;
            break;
        case 'b':
            retVal = 0x08;
            break;
        case 'e':
            retVal = 0x1b;
            break;
        case 'f':
            retVal = 0x0c;
            break;
        case 'n':
            retVal = 0x0a;
            break;
        case 'r':
            retVal = 0x0d;
            break;
        case 't':
            retVal = 0x09;
            break;
        case 'v':
            retVal = 0x0b;
            break;
        case '\\':
            retVal = 0x5c;
            break;
        case '\'':
            retVal = 0x27;
            break;
        case '\"':
            retVal = 0x22;
            break;
        case '?':
            retVal = 0x3f;
            break;
        case 'x':
            {
                singleChar = false;
                std::size_t pos = -1;
                try
                {
                    retVal = std::stoi( escaped.substr( 1 ), &pos, 16 );
                }
                catch ( ... )
                {
                    std::cerr << "Invalid hex escape code: " << escaped << "\n";
                    retVal = 0;
                }
                if ( ( pos != -1 ) && ( pos != ( escaped.length() - 1 ) ) )
                {
                    std::cerr << "Invalid hex escape code: " << escaped << "\n";
                    retVal = 0;
                }
                break;
            }
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
            {
                singleChar = false;
                // octal;
                std::size_t pos = -1;
                try
                {
                    retVal = std::stoi( escaped, &pos, 8 );
                }
                catch ( ... )
                {
                    std::cerr << "Invalid octal escape code: " << escaped << "\n";
                    retVal = 0;
                }
                if ( ( pos != -1 ) && ( pos != escaped.length() ) )
                {
                    std::cerr << "Invalid octal escape code: " << escaped << "\n";
                    retVal = 0;
                }
                break;
            }
        default:
            retVal = escaped[ 0 ];
    };
    if ( singleChar && ( escaped.length() != 1 ) )
        return 0;
    return retVal;
}

char getSeparator( std::size_t &ii, const std::string &currArg, const std::string &nextArg, const std::string &nextNextArg, const std::string &nextNextNextArg, bool nested )
{
    char separator = 0;
    if ( nested && currArg.length() == 1 )
    {
        separator = currArg[ 0 ];
    }
    else if ( currArg.length() == 2 )
    {
        if ( nextArg == "\'" && ( nextNextArg == "\'" ) )
        {
            separator = ' ';
            ii += 2;
        }
        else
        {
            ii++;
            return getSeparator( ii, nextArg, nextNextArg, nextNextNextArg, "", true );
        }
    }
    else if ( currArg.length() == 3 )
    {
        if ( ( currArg[ 2 ] == '\'' ) && ( nextArg == "\'" ) )
        {
            separator = ' ';
            ii++;
        }
        else if ( nested && ( currArg[ 0 ] == '\'' ) && ( currArg[ 2 ] == '\'' ) )
        {
            separator = currArg[ 1 ];
        }
        else if ( currArg.substr( 0, 2 ) == "-t" )
        {
            separator = currArg[ 2 ];
        }
    }
    else if ( ( currArg.length() == 4 ) && ( currArg[ 2 ] == '\\' ) )
    {
        separator = getEscapedChar( currArg.substr( 3 ) );
    }
    else if ( ( currArg.length() == 5 ) && ( currArg[ 2 ] == '\'' ) && ( currArg[ 4 ] == '\'' ) )
    {
        separator = currArg[ 3 ];
    }
    return separator;
}

char getSeparator( std::size_t &ii, int argc, char **argv )
{
    std::vector< std::string > args;
    for ( auto ii = 0ULL; ii < argc; ++ii )
        args.emplace_back( argv[ ii ] );

    return getSeparator( ii, args );
}

char getSeparator( std::size_t &ii, const std::vector< std::string > &args )
{
    auto argc = args.size();
    auto currArg = args.empty() ? std::string() : args[ ii ];
    auto nextArg = ( ii >= ( argc - 1 ) ) ? std::string() : args[ ii + 1 ];
    auto nextNextArg = ( ( argc < 2 ) || ( ii >= ( argc - 2 ) ) ) ? std::string() : args[ ii + 2 ];
    auto nextNextNextArg = ( ( argc < 3 ) || ( ii >= ( argc - 3 ) ) ) ? std::string() : args[ ii + 3 ];
    auto retVal = getSeparator( ii, currArg, nextArg, nextNextArg, nextNextNextArg, false );
    return retVal;
}

std::vector< char * > toCStrings( const std::vector< std::string > &args )
{
    std::vector< char * > cstrings;
    cstrings.reserve( args.size() );
    for ( auto &&curr : args )
    {
        cstrings.push_back( const_cast< char * >( curr.c_str() ) );
    }
    return cstrings;
}

std::string getNextToken( const std::string &line, std::size_t &startPos, const char *sep /*= kWS */ )
{
    auto tokenStartPos = line.find_first_not_of( sep, startPos );
    if ( tokenStartPos == std::string::npos )
    {
        startPos = tokenStartPos;
        return {};   // its all whitespace
    }

    auto endTokenPos = line.find_first_of( sep, tokenStartPos );
    auto token = line.substr( tokenStartPos, endTokenPos - tokenStartPos );
    startPos = endTokenPos;
    return token;
}

std::vector< std::string > splitLine( const std::string &line, bool keepEmpty /*=true*/, const char *sep /*= kWS */ )
{
    std::vector< std::string > retVal;
    std::size_t startPos = 0;
    do
    {
        auto token = getNextToken( line, startPos, sep );
        if ( keepEmpty || !token.empty() )
            retVal.emplace_back( token );
    }
    while ( startPos != std::string::npos );

    return retVal;
}

std::vector< std::string > splitLine( const std::string &line, bool keepEmpty /*=true*/, const std::string &sep /*= kWS */ )
{
    return splitLine( line, keepEmpty, sep.c_str() );
}

std::string getNextToken( const std::string &line, std::size_t &startPos, const std::string &sep /*= kWS*/ )
{
    return getNextToken( line, startPos, sep.c_str() );
}

std::vector< std::string > splitLine( const std::string &line, bool keepEmpty, char sep )
{
    return splitLine( line, keepEmpty, std::string( 1, sep ) );
}

std::string getNextToken( const std::string &line, std::size_t &startPos, char sep )
{
    return getNextToken( line, startPos, std::string( 1, sep ) );
}
