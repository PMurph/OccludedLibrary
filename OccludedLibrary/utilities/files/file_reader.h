#pragma once

#include <string>
#include <vector>
#include <fstream>

namespace occluded { namespace utilities { namespace files {

class file_reader
{
public:
	static const std::string get_string_from_file( const std::string& filePath );

private:
	file_reader();
	~file_reader();
};

} // end of files namespace 
} // end of utilities namespace
} // end of occluded namespace
