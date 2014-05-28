#include "file_reader.h"

namespace occluded { namespace utilities { namespace files { 

const std::string file_reader::get_string_from_file( const std::string& filePath ) {
	std::vector<char> rawString;
	std::ifstream fileStream( filePath );

	if( !fileStream.is_open() ) {
		throw std::runtime_error( "file_reader.get_string_from_file: Failed to get string from file(" + filePath + 
			") because there was an error opening the file." );
	}

	fileStream.seekg( 0, std::ios::end );
	rawString.resize( static_cast<std::size_t>( fileStream.tellg( ) ) + 1 );
	
	fileStream.seekg( 0, std::ios::beg );
	fileStream.read( &rawString[0], rawString.size() - 1 );
	rawString[rawString.size() - 1] = '\0';

	fileStream.close();

	return std::string( &rawString[0] );
}

// private functions

file_reader::file_reader()
{
}


file_reader::~file_reader()
{
}


} // end of files namespace
} // end of utilities namespace
} // end of occluded namespace
