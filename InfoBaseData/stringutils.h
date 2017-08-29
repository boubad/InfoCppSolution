#pragma once
#ifndef STRINGUTILS_H__
#define STRINGUTILS_H__
////////////////////////
#include "InfoDefs.h"
////////////////////////
namespace info {
	////////////////////////
	class stringutils
	{
	private:
		stringutils() = delete;
		stringutils(const stringutils &other) = delete;
		stringutils & operator=(const stringutils &other) = delete;
	public:
		static bool get_num_value(const any &v, double &dRet);
		static bool get_num_value(const any &v, int &dRet);
		static bool info_read_any_value(const string_t &sx, any &v);
		static string_t any_to_stringt(const any &v);
		static void check_string(string_t &s);
		static string_t trim(const string_t &s);
		static string_t tolower(const string_t &s);
		static string_t toupper(const string_t &s);
		static size_t split_tokens(const string_t &s, std::vector<string_t> &vec);
		static size_t split_vector(const string_t &s, std::vector<string_t> &vec,
			const string_t &delim = U(";,\t "), bool bCheck = true);
        static string_t url_encode(const string_t &s);
	};
}// namespace info
#endif // STRINGUTILS_H__
