#pragma once

#include <string>
#include <vector>
#include <fstream>

namespace occluded { namespace utilities { namespace files {

/**
 * \class file_reader
 * \brief A class that provides ways of reading files.
 *
 * A utility class that provides ways of reading files for processing in the occluded library.
 */
class file_reader
{
public:
	/**
	 * \fn get_string_from_file
	 * \brief Reads a file's contents into a string.
	 *
	 * \param filePath A reference to a string representing the file path of the file.
	 * \return A string containing the contents of the file.
	 *
	 * Reads the contents of the file located at the filePath into a string preserving new line characters.
	 */
	static const std::string get_string_from_file( const std::string& filePath );

private:
	file_reader();
	~file_reader();
};

} // end of files namespace 
} // end of utilities namespace
} // end of occluded namespace
