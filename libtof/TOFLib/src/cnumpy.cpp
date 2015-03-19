/*******************************************************
*
*  Copyright (c) 2014 Texas Instruments Incorporated
*
*******************************************************/

//Copyright (C) 2011  Carl Rogers
//Released under MIT License
//license available at http://www.opensource.org/licenses/mit-license.php
//original source can be downloaded at https://github.com/rogersce/cnpy

#include"cnumpy.h"
#include<complex>
#include<cstdlib>
#include<algorithm>
#include<cstring>
#include<iomanip>

char cnpy::BigEndianTest() {
	unsigned char x[] = { 1, 0 };
	short y = *(short*)x;
	return y == 1 ? '<' : '>';
}

char cnpy::map_type(const std::type_info& t)
{
	if (t == typeid(float)) return 'f';
	if (t == typeid(double)) return 'f';
	if (t == typeid(long double)) return 'f';

	if (t == typeid(int)) return 'i';
	if (t == typeid(char)) return 'i';
	if (t == typeid(short)) return 'i';
	if (t == typeid(long)) return 'i';
	if (t == typeid(long long)) return 'i';

	if (t == typeid(unsigned char)) return 'u';
	if (t == typeid(unsigned short)) return 'u';
	if (t == typeid(unsigned long)) return 'u';
	if (t == typeid(unsigned long long)) return 'u';
	if (t == typeid(unsigned int)) return 'u';

	if (t == typeid(bool)) return 'b';

	if (t == typeid(std::complex<float>)) return 'c';
	if (t == typeid(std::complex<double>)) return 'c';
	if (t == typeid(std::complex<long double>)) return 'c';

	else return 'u';
}

template<> std::vector<char>& cnpy::operator+=(std::vector<char>& lhs, const std::string rhs) {
	lhs.insert(lhs.end(), rhs.begin(), rhs.end());
	return lhs;
}

template<> std::vector<char>& cnpy::operator+=(std::vector<char>& lhs, const char* rhs) {
	//write in little endian
	size_t len = strlen(rhs);
	lhs.reserve(len);
	for (size_t byte = 0; byte < len; byte++) {
		lhs.push_back(rhs[byte]);
	}
	return lhs;
}
