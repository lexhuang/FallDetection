#pragma once

#ifndef CNUMPY_H
#define CNUMPY_H

#ifndef LIBCNPY_H_
#define LIBCNPY_H_

#include<string>
#include<stdexcept>
#include<sstream>
#include<vector>
#include<cstdio>
#include<typeinfo>
#include<iostream>
#include<cassert>
#include<map>

#ifdef PLAT_LINUX
#include <stdint.h>
uint32_t crc32(uint32_t crc, const void *address, unsigned int length);
#endif
#define FILE_NEW	0
#define FILE_APPEND	1


namespace cnpy {

	struct NpyArray {
		char* data;
		std::vector<unsigned int> shape;
		unsigned int word_size;
		bool fortran_order;
		void destruct() { delete[] data; }
	};

	struct npz_t : public std::map<std::string, NpyArray>
	{
		void destruct()
		{
			npz_t::iterator it = this->begin();
			for (; it != this->end(); ++it) (*it).second.destruct();
		}
	};

	char BigEndianTest();
	char map_type(const std::type_info& t);
	template<typename T> std::vector<char> create_npy_header(const T* data, const unsigned int* shape, const unsigned int ndims);

	template<typename T> std::vector<char>& operator+=(std::vector<char>& lhs, const T rhs) {
		//write in little endian
		for (unsigned char byte = 0; byte < sizeof(T); byte++) {
			char val = *((char*)&rhs + byte);
			lhs.push_back(val);
		}
		return lhs;
	}

	template<> std::vector<char>& operator+=(std::vector<char>& lhs, const std::string rhs);
	template<> std::vector<char>& operator+=(std::vector<char>& lhs, const char* rhs);


	template<typename T> std::string tostring(T i) {
		std::stringstream s;
		s << i;
		return s.str();
	}

	template<typename T> void npy_save(std::string fname, const T* data, const unsigned int* shape, const unsigned int ndims, int mode = FILE_NEW) {
		FILE* fp = NULL;

		if (mode == FILE_APPEND) {
			fp = fopen(fname.c_str(), "ab");
			std::vector<char> header1 = create_npy_header(data, shape, ndims);
			fwrite(&header1[0], sizeof(char), header1.size(), fp);
		}
		else {
			fp = fopen(fname.c_str(), "wb");
			std::vector<char> header = create_npy_header(data, shape, ndims);
			fwrite(&header[0], sizeof(char), header.size(), fp);
		}

		unsigned int nels = 1;
		for (unsigned int i = 0; i < ndims; i++) nels *= shape[i];
		fwrite(data, 4, nels, fp); //TODO: 4 indicates 32 bit integer. Ideally need to find size of datatype
		fclose(fp);
	}

	template<typename T> std::vector<char> create_npy_header(const T* data, const unsigned int* shape, const unsigned int ndims) {

		std::vector<char> dict;
		dict += "{'descr': '";
		dict += BigEndianTest();
		dict += map_type(typeid(T));
		dict += "4";    //tostring(sizeof(T)); //TODO: 4 indicates 32 bits. Ideally should be size of datatype
		dict += "', 'fortran_order': False, 'shape': (";
		dict += tostring(shape[0]);
		for (unsigned int i = 1; i < ndims; i++) {
			dict += "L, ";
			dict += tostring(shape[i]);
		}
		dict += "L";
		if (ndims == 1) dict += ",";
		dict += "), }";
		//pad with spaces so that preamble+dict is modulo 16 bytes. preamble is 10 bytes. dict needs to end with \n
		int remainder = 16 - (10 + dict.size()) % 16;
		dict.insert(dict.end(), remainder, ' ');
		dict.back() = '\n';

		std::vector<char> header;
		header += (char)0x93;
		header += "NUMPY";
		header += (char)0x01; //major version of numpy format
		header += (char)0x00; //minor version of numpy format
		header += (unsigned short)dict.size();
		header.insert(header.end(), dict.begin(), dict.end());
		return header;
	}
}

#endif


#endif // CNUMPY_H
