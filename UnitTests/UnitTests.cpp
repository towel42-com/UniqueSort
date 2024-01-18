#include "gtest/gtest.h"

#include "../main/Utils.h"
#include "../main/Settings.h"

namespace
{
    TEST( TestUtils, EscapeCode )
    {
        EXPECT_EQ( 0x07, getEscapedChar( "\\a" ) );
        EXPECT_EQ( 0x08, getEscapedChar( "\\b" ) );
        EXPECT_EQ( 0x1b, getEscapedChar( "\\e" ) );
        EXPECT_EQ( 0x0c, getEscapedChar( "\\f" ) );
        EXPECT_EQ( 0x0a, getEscapedChar( "\\n" ) );
        EXPECT_EQ( 0x0d, getEscapedChar( "\\r" ) );
        EXPECT_EQ( 0x09, getEscapedChar( "\\t" ) );
        EXPECT_EQ( 0x0b, getEscapedChar( "\\v" ) );
        EXPECT_EQ( 0x5c, getEscapedChar( "\\\\" ) );
        EXPECT_EQ( 0x27, getEscapedChar( "\\'" ) );
        EXPECT_EQ( 0x22, getEscapedChar( "\\\"" ) );
        EXPECT_EQ( 0x3f, getEscapedChar( "\\?" ) );

        EXPECT_EQ( 012, getEscapedChar( "\\012" ) );
        EXPECT_EQ( 0x012, getEscapedChar( "\\x012" ) );

        EXPECT_EQ( 0, getEscapedChar( "" ) );
        EXPECT_EQ( 'g', getEscapedChar( "\\g" ) );
        EXPECT_EQ( 0, getEscapedChar( "\\018" ) );
        EXPECT_EQ( 0, getEscapedChar( "\\x0K3" ) );
    }

    TEST( TestUtils, GetSeparator )
    {
        {
            auto ii = 0ULL;
            EXPECT_EQ( ' ', getSeparator( ii, { "-t", "'", "'" } ) );
            EXPECT_EQ( 2, ii );
        }
        {
            auto ii = 0ULL;
            EXPECT_EQ( ' ', getSeparator( ii, { "-t'", "'" } ) );
            EXPECT_EQ( 1, ii );
        }
        auto chars = std::vector< std::pair< char, char > >( {
            std::make_pair( '\a', 0x07 ),
            std::make_pair( '\b', 0x08 ),
            //std::make_pair( '\e', 0x1b ),
            std::make_pair( '\f', 0x0c ),
            std::make_pair( '\n', 0x0a ),
            std::make_pair( '\r', 0x0d ),
            std::make_pair( '\t', 0x09 ),
            std::make_pair( '\v', 0x0b ),
            std::make_pair( '\\', 0x5c ),
            std::make_pair( '\'', 0x27 ),
            std::make_pair( '\"', 0x22 ),
            std::make_pair( '\?', 0x3F ),
            std::make_pair( '\012', 10 ),
            std::make_pair( '\x012', 0x12 ),
        } );
        for ( auto && [ key, answer ] : chars )
        {
            std::string keyString = std::string( 1, key );
            auto ii = 0ULL;
            EXPECT_EQ( answer, getSeparator( ii, { "-t" + keyString } ) );
            EXPECT_EQ( 0, ii );

            ii = 0;
            EXPECT_EQ( answer, getSeparator( ii, { "-t'" + keyString + "'" } ) );
            EXPECT_EQ( 0, ii );

            ii = 0;
            EXPECT_EQ( answer, getSeparator( ii, { "-t", "'" + keyString + "'" } ) );
            EXPECT_EQ( 1, ii );

            ii = 0;
            EXPECT_EQ( answer, getSeparator( ii, { "-t", keyString } ) );
            EXPECT_EQ( 1, ii );
        }
    }

    TEST( TestSettings, GetSettings )
    {
        auto args = std::vector< std::string >( { "appName.exe", "-u", "-k", "-3", "-t", " " } );
        auto settings = CSettings( args );
        EXPECT_EQ( 0, settings.numFiles() );
        EXPECT_TRUE( settings.unique() );
        EXPECT_EQ( -3, settings.sortColumn() );
        EXPECT_EQ( ' ', settings.separator() );
    }

    TEST( TestSort, Batch10 )
    {
        auto input = "alluniquecells.txt:batch10/report_cells.txt:  PSEUDOCELL OAOAOAI211111 Ia2.0b2.0c2.0d2.0e2.0f2.0 {>A,0},{>B,1},{>C,2},{>D,3},{>E,4},{>F,5},{>G,6},{<H,7:Ia2.0b2.0c2.0d2.0e2.0f2.0}\n"
                     "alluniquecells.txt:batch10/report_cells.txt:  PSEUDOCELL AOAOAOI211111 a2.1b2.1c2.1d2.1e2.1f2.2 {>E,0},{>H,1},{>D,2},{>A,3},{>F,4},{>B,5},{>C,6},{<G,7:a2.1b2.1c2.1d2.1e2.1f2.2}\n";

        auto args = std::vector< std::string >( { "appName.exe" } );
        auto settings = CSettings( args );
        settings
        
    }

}

int main( int argc, char **argv )
{
    ::testing::InitGoogleTest( &argc, argv );
    int retVal = RUN_ALL_TESTS();

    return retVal;
}
