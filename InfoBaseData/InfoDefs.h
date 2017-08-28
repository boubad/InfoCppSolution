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
#include <optional>
#define INFO_ANY_HAS_VALUE(x) ((x).has_value())
#define INFO_OPTIONAL_HAS_VALUE(x) ((x).has_value())
#else
#if __GNUC__ > 6
#include <any>
#include <optional>
#define INFO_ANY_HAS_VALUE(x) ((x).has_value())
#define INFO_OPTIONAL_HAS_VALUE(x) ((x).has_value())
#else
#include <boost/any.hpp>
#include <boost/optional.hpp>
#define INFO_ANY_HAS_VALUE(x) ((!(x).empty()))
#define INFO_OPTIONAL_HAS_VALUE(x) ((bool)(x))
#endif
#endif // _MSC_VER
////////////////////////////////
namespace info {
	//////////////////////
#if defined(_MSC_VER)
	using any = std::any;
	using std::optional;
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
#if __GNUC__ > 6
    using any = std::any;
	using std::optional;
#define INFO_ANY_CAST  std::any_cast
#else
	using any = boost::any;
	using boost::optional;
#define INFO_ANY_CAST  boost::any_cast
#endif
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
	///////////////////////////////////
	using nullable_int = optional<int>;
	using nullable_double = optional<double>;
	using nullable_bool = optional<bool>;
	using nullable_string = optional<string_t>;
	using nullable_any = optional<any>;
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
	//////////////////////////////
	enum class info_resulttype : int {
		unkown = 0, val = 1, valc = 2,
		ajac = 4, nar = 8, misc = 16
	};
	enum class info_status
		: int {
		unknown = 0,
		persisted = 1,
		tail = 2,
		disabled = 4,
		inserted = 8,
		updated = 16,
		deleted = 32,
		info = 64,
		normal = 128
	};
	enum class info_datatype
		: int {
		unknown = 0,
		real = 1,
		integer = 2,
		text = 4,
		boolean = 8,
		date = 16,
		datetime = 32,
		enumtype = 64,
		normal = 128,
		other = 256,
		status = 512,
		kind = 1024,
		datatype = 2048,
		any = 4096,
		vector = 8192,
		result = 16384
	};
	enum class info_kindtype
		: int {
		unknown = 0, normal = 1, info = 2, modal = 4, ordinal = 8
	};
	enum class info_filteroptype
	{
		ignore = 0,
		exists = 1,
		notexists = 2,
		equals = 4,
		notequals = 8,
		less = 16,
		lessequals = 32,
		greater = 64,
		greaterequals = 128,
		like = 256
	};
	enum class info_storeoptype
		: int {
		unknown = 0,
		findonedoc = 1,
		insertonedoc = 2,
		replaceonedoc = 4,
		maintainsonedoc = 8,
		deleteonedoc = 16,
		deletemanydocs = 32,
		maintainsmanydocs = 64,
		querydocscount = 128,
		querydocs = 256,
		fetchdistinct = 512,
		checkindex = 1024
	};
///////////////////////////////
}// namespace info
/////////////////////////////
#endif // INFODEFS_H__
