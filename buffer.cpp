#include "buffer.h"

using namespace std;

buffer::buffer(){
	begin = new char[128];
	data_begin = begin;
	used_size = 0;
	length = 128;
}

buffer::buffer(const buffer &rhs)
{
	begin = new char[128];
	data_begin = begin;
	used_size = rhs.used_size;
	length  = rhs.length;
	//memcpy(begin, rhs.data_begin, rhs.used_size);
	memcpy(begin, rhs.begin, rhs.used_size);
}

buffer &buffer::operator= (const buffer &rhs)
{
	begin = new char[128];
	data_begin = begin;
	used_size = rhs.used_size;
	length  = rhs.length;
	//memcpy(begin, rhs.data_begin, rhs.used_size);
	memcpy(begin, rhs.begin, rhs.used_size);
	return *this;
}

int buffer::append_data(const void *bgn, int len){
	if(len > length - used_size)
	{
		cout << "system memory lack" << endl;
		return -1;
	}
	memcpy(data_begin, bgn, len);
	data_begin += len;
	used_size += len;
	return 0;
}

int buffer::drain_data(void *data, int len){
	if (used_size < len)
		return -1;
	memcpy(data, data_begin, len);
	used_size -= len;
	data_begin += len;
	if (used_size == 0)
		data_begin = begin;
	return 0;
}

