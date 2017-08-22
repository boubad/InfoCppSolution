#pragma once
#ifndef INFODEFS_H__
#define INFODEFS_H__
////////////////////////////
#include <cassert>
#include <stdexcept>
#include <memory>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <sstream>
#include <fstream>
//////////////////////////////////
#if defined(_MSC_VER)
#include <any>
#define INFO_ANY_HAS_VALUE(x) ((x).has_value())
#else
#include <boost/any.hpp>
#define INFO_ANY_HAS_VALUE(x) ((!(x).empty()))
#endif // _MSC_VER
////////////////////////////////
namespace info {
	//////////////////////
#if defined(_MSC_VER)
	using any = std::any;
#define INFO_ANY_CAST  std::any_cast
#ifndef _TURN_OFF_PLATFORM_STRING
#define _XPLATSTR(x) L ## x
	using char_t = wchar_t;
	using string_t = std::wstring;
	using istream_t = std::wistream;
	using ostream_t = std::wostream;
	using istringstream_t = std::wistringstream;
	using ostringstream_t = std::wostringstream;
	using stringstream_t = std::wstringstream;
	using ifstream_t = std::wifstream;
	using ofstream_t = std::wofstream;
#define ucin  std::wcin
#define ucout  std::wcout
#define ucerr  std::wcerr
#endif //_TURN_OFF_PLATFORM_STRING
#else
	using any = boost::any;
#define INFO_ANY_CAST  boost::any_cast
#ifndef _TURN_OFF_PLATFORM_STRING
#define _XPLATSTR(x) x
	using char_t = char;
	using string_t = std::string;
	using istream_t = std::istream;
	using ostream_t = std::ostream;
	using istringstream_t = std::istringstream;
	using ostringstream_t = std::ostringstream;
	using stringstream_t = std::stringstream;
	using ifstream_t = std::ifstream;
	using ofstream_t = std::ofstream;
#define ucin  std::cin
#define ucout  std::cout
#define ucerr  std::cerr
#endif //_TURN_OFF_PLATFORM_STRING
#endif // _MSC_VER
	////////////////////////////
#ifndef _TURN_OFF_PLATFORM_STRING
#define U(x) _XPLATSTR(x)
#endif // !_TURN_OFF_PLATFORM_STRING
	/////////////////////////////////
	extern string_t string_to_stringt(const std::string &s);
	extern std::string stringt_to_string(const string_t &s);
	////////////////////////////////
	using infomap = std::map<string_t, any>;
	using infovector = std::vector<any>;
	////////////////////////////
	extern bool info_any_equals(const any &v1, const any &v2);
	//////////////////////////////
	class info_exception : public std::exception {
	private:
		string_t m_message;
		std::string m_smessage;
	public:
		info_exception() noexcept;
		info_exception(const string_t &smess) noexcept;
		info_exception(const info_exception & other) noexcept;
		info_exception(const std::exception &other) noexcept;
		info_exception& operator= (const info_exception&) noexcept;
		virtual ~info_exception();
		const string_t &message(void) const noexcept;
		virtual const char* what() const noexcept;
	}; //class info_exception
	using query_params = std::vector<std::pair<string_t, string_t>>;
///////////////////////////////
}// namespace info
/////////////////////////////
#endif // INFODEFS_H__
