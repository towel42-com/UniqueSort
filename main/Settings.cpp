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

#include "Settings.h"
#include "Utils.h"

#include <fstream>
#include <map>
#include <set>
#include< list >

CSettings::CSettings( int argc, char **argv )
{
    std::vector< std::string > args;
    for ( auto ii = 0; ii < argc; ++ii )
        args.emplace_back( argv[ ii ] );

    init( args );
}

CSettings::CSettings( const std::vector< std::string > &args )
{
    init( args );
}

CSettings::CSettings( const std::vector< const char * > &args )
{
    std::vector< std::string > stringArgs;
    for ( auto &&ii : args )
        stringArgs.emplace_back( ii );
    init( stringArgs );
}

void CSettings::init( const std::vector< std::string > &args )
{
    fAOK = true;
    for ( auto ii = 1ULL; ii < args.size(); ++ii )
    {
        auto lastArg = ( ii == ( args.size() - 1 ) );
        auto currArg = args[ ii ];
        auto nextArg = lastArg ? "" : args[ ii + 1 ];

        //std::cout << "lastArg: " << ( lastArg ? "true" : "false" ) << "\n";
        //std::cout << "currArg: " << ( currArg ) << "\n";
        //std::cout << "nextArg: " << ( nextArg ) << "\n";

        if ( currArg.compare( 0, 2, "-u" ) == 0 )
        {
            fUnique = true;
        }
        else if ( currArg.compare( 0, 2, "-t" ) == 0 )
        {
            if ( ( currArg.length() == 2 ) && lastArg )
            {
                showHelp();
                fAOK = false;
                return;
            }
            ////std::cout << "ii = " << ii << "\n";
            ////std::cout << "args.size() = " << args.size() << "\n";
            //for ( auto ii = 0; ii < args.size(); ++ii )
            //{
            //    std::cout << "args[" << ii << "] = {" << args[ ii ] << "}\n";
            //}
            fSeparator = getSeparator( ii, args );
            if ( fSeparator == 0 )
            {
                showHelp();
                fAOK = false;
                return;
            }
        }
        else if ( currArg.compare( 0, 2, "-k" ) == 0 )
        {
            if ( ( currArg.length() == 2 ) && lastArg )
            {
                showHelp();
                fAOK = false;
                return;
            }
            std::string strColumn;
            if ( currArg.length() == 2 )
            {
                strColumn = nextArg;
                ii++;
            }
            else
            {
                strColumn = currArg.substr( 2 );
            }

            try
            {
                fSortColumn = std::stoll( strColumn );
            }
            catch ( ... )
            {
                std::cerr << "Invalid argument: column must be an integer." << '\n';
                showHelp();
            }
        }
        else
            fFileNames.emplace_back( currArg );
    }
    createStreams();
}

void CSettings::showHelp()
{
    std::cout << "Usage unique_sort [-t char] [-k column] [-u] inputfile" << std::endl;
}

void CSettings::createStreams()
{
    if ( fFileNames.empty() )
    {
        fStreams.push_back( &std::cin );
    }
    else
    {
        for ( auto &&fileName : fFileNames )
        {
            auto stream = new std::ifstream( fileName );
            if ( !stream->is_open() )
            {
                std::cerr << "Could not open file '" << fileName << "'" << std::endl;
                showHelp();
                fAOK = false;
                return;
            }
            fStreams.push_back( stream );
        }
    }
}

bool CSettings::deleteStreams() const
{
    return !fFileNames.empty();
}

void CSettings::dump() const
{
    return;

    std::cout << "Files: ";
    if ( fFileNames.empty() )
        std::cout << "<stdin>";
    auto first = true;
    for ( auto &&ii : fFileNames )
    {
        if ( !first )
            std::cout << ", ";
        first = false;
        std::cout << ii;
    }
    std::cout << "\n";
}

bool CSettings::process() const
{
    if ( !aOK() )
        return false;

    std::map< std::string, std::set< std::string > > lines;
    for ( auto &&stream : fStreams )
    {
        std::size_t lineNum = 0;
        for ( std::string line; std::getline( *stream, line, '\n' ); ++lineNum )
        {
            if ( fSortColumn == -1 )
            {
                lines[ line ].emplace( line );
            }
            else
            {
                auto split = splitLine( line, false, fSeparator );
                if ( split.empty() )
                    continue;

                auto columnToSort = fSortColumn;
                if ( fSortColumn > split.size() )
                    columnToSort = 0;
                auto key = split[ columnToSort ];
                if ( !fUnique )
                    lines[ key ].emplace( line );
                else
                {
                    auto pos = lines.find( key );
                    if ( pos == lines.end() )
                        lines[ key ].emplace( line );
                }
            }
        };
        if ( deleteStreams() )
            delete stream;
    }
    for ( auto &&[ key, currLines ] : lines )
    {
        for ( auto &&line : currLines )
        {
            std::cout << line << "\n";
        }
    }
    return true;
}
