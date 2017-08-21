#pragma once
#ifndef INFO_RESPONSE_H__
#define INFO_RESPONSE_H__
///////////////////////////////
#include "InfoDefs.h"
////////////////////////////
#include <future>
////////////////////////////
namespace info {
	using byte = unsigned char;
	using byte_vector = std::vector<byte>;
	////////////////////////
	class info_response {
	public:
		unsigned long contentlength;
		int statuscode;
		std::map<string_t,string_t> headers;
		string_t stringval;
		any jsonval;
		byte_vector byteval;
		string_t errorstring;
		string_t reason;
	public:
		info_response();
		info_response(const info_response &other);
		info_response & operator=(const info_response &other);
		~info_response();
		operator bool(void) const { return (!has_error()); }
	public:
		string_t content_type(void) const;
		bool has_error(void) const;
	};
	/////////////////////////////
	using info_response_ptr = std::shared_ptr<info_response>;
	//////////////////////////////
}// namespace info
/////////////////////////
#endif // INFO_RESPONSE_H__
