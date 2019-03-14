/*
 * vertex_index_minimizer.cpp
 *
 *  Created on: Sep 21, 2015
 *      Author: Farzad Khorasani
 */

#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <algorithm>
#include <stdexcept>
#include <unordered_map>
#include <stdlib.h>


template <typename intT>
intT mapNumber( std::unordered_map<intT, intT>& mapper, const intT key, const unsigned int offset ) {

	auto pos_index = mapper.find( key );
	if( pos_index == mapper.end() ) {	// The key does not exist. We need to add it.
		intT val = mapper.size() + offset;
		std::pair<intT, intT> kvp( key, val );
		mapper.insert( kvp );
		return val;
	}
	else {	// The key does exist.
		return pos_index->second;
	}

}

template <typename idxT>
void vertex_index_minimizer( std::ifstream& in_file, std::ofstream& out_file, const unsigned int offset ) {

	std::unordered_map<idxT, idxT> mapper;
	std::string line;

	// Read the input graph line-by-line.
	while( std::getline( in_file, line ) ) {

		if( line[0] < '0' || line[0] > '9' ) {	// Skipping any line blank or starting with a character rather than a number.
			out_file << line << "\n";
			continue;
		}

		std::size_t pos = 2015;
		const auto first_index = static_cast<idxT>( std::stoull( line, &pos ) );
		std::string sub_line;
		sub_line = line.substr( ++pos );
		const auto second_index = static_cast<idxT>( std::stoull( sub_line, &pos ) );
		sub_line = sub_line.substr( pos );

		out_file << mapNumber<idxT>( std::ref(mapper), first_index, offset ) << "\t";
		out_file << mapNumber<idxT>( std::ref(mapper), second_index, offset ) << sub_line << "\n";

	}

}


// Opening files safely.
template <typename T_file>
void openFileToAccess( T_file& input_file, const char* file_name ) {
	input_file.open( file_name );
	if( !input_file )
		throw std::runtime_error( "Failed to open specified file." );
}


int main( int argc, char** argv ) {

	std::string usage =
	"\tRequired command line arguments:\n\
		-Input file: E.g., --input in.txt\n\
	Additional arguments:\n\
		-Starting index non-negative offset (default: 0): E.g., --offset 1\n\
		-Output file (default: out.txt). E.g., --output myout.txt\n";

	std::ofstream out_file;
	std::ifstream in_file;
	unsigned int offset = 0;

	try{

		// Getting required input parameters.
		for( int iii = 1; iii < argc; ++iii )
			if( !strcmp( argv[iii], "--input" ) && iii != argc-1 /*is not the last one*/)
				openFileToAccess< std::ifstream >( in_file, argv[iii+1] );
			else if( !strcmp( argv[iii], "--output" ) && iii != argc-1 /*is not the last one*/)
				openFileToAccess< std::ofstream >( out_file, argv[iii+1] );
			else if( !strcmp( argv[iii], "--offset" ) && iii != argc-1 /*is not the last one*/)
				offset = std::stoul( std::string( argv[iii+1] ) );

		if( !in_file.is_open() )
			throw std::runtime_error( "Initialization Error: The input file has not been specified." );
		if( !out_file.is_open() )
			openFileToAccess( out_file, "out.txt" );

		// Start the work.
		std::cout << "Minimizing vertex indices for the specified graph ...\n";
		using vertexIndexType = unsigned int;
		vertex_index_minimizer<vertexIndexType>( in_file, out_file, offset );
		std::cout << "Done." << std::endl;

	}
	catch( const std::exception& strException ) {
		std::cerr << strException.what() << "\n";
		std::cerr << "Usage: " << usage << std::endl << "Exiting." << std::endl;
		return( EXIT_FAILURE );
	}
	catch(...) {
		std::cerr << "An exception has occurred." << std::endl;
		return( EXIT_FAILURE );
	}

	return( EXIT_SUCCESS );
}
