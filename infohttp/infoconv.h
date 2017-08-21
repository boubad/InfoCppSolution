#pragma once
#ifndef INFOCONV_H__
#define INFOCONV_H__
//////////////////////////////
#include "info_restsdk.h"
////////////////////////////
#include <InfoDefs.h>
////////////////////////////
namespace info {
	namespace http {
		/////////////////////////////
		using pplx::task;
		//////////////////////////////////
		using value = web::json::value;
		using object = web::json::object;
		using array = web::json::array;
		/////////////////////////////////
		extern void convert_any(const any &src, value &dest);
		extern void convert_any(const value &src, any &dest);
		//////////////////////////////
	}// namespace http
}// namesmape info
//////////////////////////
#endif // INFOCONV_H__
