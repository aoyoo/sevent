#pragma once 
#include "public.h"

class buffer{
	char *begin;
	char *data_begin;

	int length;
	int used_size;
public:
	buffer();
	buffer(const buffer &rhs);
	buffer &operator= (const buffer &rhs);

	~buffer(){if(begin) delete [] begin;}
	void set_buffer_length(int len){length = len;}
	void set_used_size(int len){used_size = len;}

	int get_buffer_length(){return length;}
	int get_size(){return used_size;}

	char *get_buffer(){return begin;}
	char *get_data_buffer(){return begin;}
	//i didn't think it's good
	//maybe vecter is better?

	int get_data_length(){return used_size;}

	int append_data(const void *bgn,int len);
	int drain_data(void *data, int length);
};


