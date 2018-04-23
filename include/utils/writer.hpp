/**
 * file: utils/writer.hpp
 * Provides some helper functions for writing to text and to binary file.
 *
 */
#ifndef _WRITER_HPP_
#define _WRITER_HPP_

#include <fstream>

#include <assert.h>
#include <exception>

namespace MC {
/**
 * generic writer for files in text file format.
 *
 */
template <typename ForwardIt>
void write_contiguous_array(ForwardIt first, ForwardIt last, unsigned int cols,
			    const char* filename, char mode = 'w') {
	assert(first != last && "No data to write. Cannot continue.");
	assert(cols != 0 && "number of columns = 0. Cannot continue.");

	std::fstream::ios_base::openmode _mode =
	    (mode == 'w') ? std::ios_base::in : std::ios_base::app;

	std::ofstream file{};
	file.open(filename, _mode);
	auto dist = std::distance(first, last);
	auto rows = dist / cols;
	ForwardIt it = first;
	try {
		for (unsigned int i = 0U; i < rows; ++i) {
			for (unsigned int j = 0U; j < cols; ++j) {
				file << *it << " ";
				++it;
			}
			file << std::endl;
		}

		// last row;
		while (it != last) {
			file << *it << " ";
			++it;
		}
		file.flush();
		file.close();
	} catch (std::exception const& e) {
		// do some exception handling here
		file.close();
		throw e;
	}
}
/***
 * write up a name-value pair in the file specified by "filename"
 */
template <typename T>
void write_meta_data(const char* name, T val, const char* filename,
		     char mode = 'a') {
	std::ofstream file{};
	using base = std::fstream::ios_base;
	base::openmode _mode = mode == 'a' ? base::app : base::out;

	try {
		file.open(filename, _mode);
		file << name << " " << val << std::endl;
		file.flush();
		file.close();
	} catch (std::exception const& e) {
		file.close();
		throw e;
	}
}

};  // namespace MC

#endif
