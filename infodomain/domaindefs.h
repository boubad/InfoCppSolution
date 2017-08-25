#pragma once
#ifndef DOMAINSDEFS_H__
#define DOMAINSDEFS_H__
/////////////////////////////
#include <InfoDefs.h>
//////////////////////////////////
#ifdef _MSC_VER
#include <optional>
#else
#if __GNUC__ > 6
#include <optional>
#else
#include <boost/optional.hpp>
#endif
#endif // _MSC_VER
//////////////////////////////////
namespace info {
	namespace domain {
		/////////////////////////////////////
#ifdef _MSC_VER
		using std::optional;
#define INFO_OPTIONAL_HAS_VALUE(x) ((x).has_value())
#else
#if __GNUC__ > 6
        using std::optional;
#define INFO_OPTIONAL_HAS_VALUE(x) ((x).has_value())
#else
		using boost::optional;
#define INFO_OPTIONAL_HAS_VALUE(x) ((bool)(x))
#endif
#endif // _MSC_VER
		//////////////////////////////////////
		using nullable_int = std::optional<int>;
		using nullable_double = std::optional<double>;
		using nullable_bool = std::optional<bool>;
		using nullable_string = std::optional<string_t>;
		using nullable_any = std::optional<any>;
		/////////////////////////////
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
		///////////////////////////
	}// namespace domain
}// namespace info
////////////////////////////////
#endif // DOMAINSDEFS_H__
