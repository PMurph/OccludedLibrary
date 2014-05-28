#include "stdafx.h"
#include "CppUnitTest.h"

#include <cstdio>
#include "utilities/files/file_reader.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace occluded::utilities::files;

namespace OccludedLibraryUnitTests
{
	static const string testFilePath( "test_file.txt" );
	
	static fstream fileStream;

	TEST_CLASS( file_reader_test )
	{
	public:

		TEST_CLASS_INITIALIZE( file_reader_test_init ) 
		{
			std::string testContent( "This is a standard test file.\n This file contains multiple\nlines." );

			fileStream.open( testFilePath.c_str(), ios::out );

			if( fileStream.is_open() ) {
				fileStream.write( testContent.c_str(), testContent.size() );
				fileStream.flush();
				fileStream.close();
			}
		}

		TEST_CLASS_CLEANUP( file_reader_test_clean )
		{
			remove( testFilePath.c_str() );
		}
		
		TEST_METHOD( file_reader_get_string_from_file_valid_file_test )
		{
			const string readString = file_reader::get_string_from_file( testFilePath );

			// Test to make sure the correct string is returned for an existing file
			Assert::AreEqual( string( "This is a standard test file.\n This file contains multiple\nlines." ), readString );

			const string anotherPath( "test_file2.txt" );

			if( !fileStream.is_open() ) {
				string message( "Test to see if stream is properly closed when function exits." );

				fileStream.open( anotherPath.c_str(), ios::out );
				fileStream.write( message.c_str(), message.size() );
				fileStream.flush();
				fileStream.close();

				const string testString2 = file_reader::get_string_from_file( anotherPath );
				
				// Test to make sure the file stream that reads the file in the get_string_from_file function is properly closed
				Assert::AreEqual( remove( anotherPath.c_str() ), 0 );
			} else {
				// Fails the test to indicate there is a problem in the setup of the test
				Assert::Fail();
			}
		}

		TEST_METHOD( file_reader_get_string_from_file_invalid_file_test )
		{
			const string anotherPath( "not_exist.txt" );

			try {
				file_reader::get_string_from_file( anotherPath );

				// Test to make sure an exception is thrown if a file that doesn't exist is attempted to be opened
				Assert::Fail();
			} catch( const std::exception& ) {
			}
		}
	};
}