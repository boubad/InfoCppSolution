#pragma once
#ifndef HTTP_UTILS_H__
#define  HTTP_UTILS_H__
/////////////////////////////
#include "infoconv.h"
/////////////////////////////
namespace info {
	namespace http {
		///////////////////////////
		extern string_t url_encode(const string_t &url);
		
		///////////////////////////////
	}// namespace http
}// namespace info
///////////////////////////
#endif //  HTTP_UTILS_H__
