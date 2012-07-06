#pragma once
#include "socket.h"
#include "buffer.h"
#include "message.h"

class front_message : public message
{
	int id;
	int size;
	int type;
public:
	static const int msg_id = 300;
	front_message()
	{
		get_head().command = msg_id;
		id = -1;
		size = -1;
		type = -1;
	}
	void set_id(int i) { id = i; }
	void set_size(int i) { size = i; }
	int get_id() { return id; }
	int get_size() { return size; }
	
	int pack(buffer &buf)
	{
		octet_ostream oos;
		set_length(3 * sizeof(int));
		//oos << get_head() << id << size;
		//char c = 'x';
		int v=400,com=300,t=1,st=2,src=3,des=4,lenn=12;
		unsigned long long sh=12345678900,sl=98765432100;
		oos << v << com ;
		oos << 'x' << 'x' << 'x' << 'x' << 'x' << 'x' << 'x' << 'x' ;
		oos << t << st << src << des << sh << sl << lenn;
		oos << id << size << type;
		buf = oos.get_buffer();
		return 0;
	}

	int unpack(buffer &buf)
	{ 
		octet_istream ois(buf);
		ois >> id >> size >> type;
		return 0;
	}
	int response();
};

class front_response_message : public message
{
	int id;
	int size;
public:
	static const int msg_id = 301;
	front_response_message()
	{
		id = -1;
		size = -1;
		get_head().command = msg_id;
	}
	void set_id(int i) { id = i; }
	void set_size(int i) { size = i; }
	int get_id() { return id; }
	int get_size() { return size; }

	int pack(buffer &buf)
	{
		octet_ostream oos;
		//set_length(2 * sizeof(int));
		//oos << get_head() << id << size;
		oos << 400 << 300 ;
		oos.append_data("1234567", 8); 
		oos << 0 << 0 << 0 << 0;
		unsigned long long sn_high = 0;
		unsigned long long sn_low  = 0;
		int length = 2*sizeof(int);
		oos << sn_high << sn_low << length;
		oos << id << size;
		buf = oos.get_buffer();
		return 0;
	}

	int unpack(buffer &buf)
	{ 
		octet_istream ois(buf);
		ois >> id >> size;
		return 0;
	}
	int response();
	void show(){
		LOG_DEBUG_VA("res id: %d; size: %d", id, size);
		//cout << "id: " << id << "; size: " << size << endl;
	}
};

