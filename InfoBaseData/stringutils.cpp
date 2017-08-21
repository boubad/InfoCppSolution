#include "stringutils.h"
///////////////////
#if defined(_MSC_VER)
#include <cctype>
#include <algorithm>
#include <locale>
#else
#include <boost/algorithm/string.hpp>
#endif // _MSC_VER
/////////////////////////
//
namespace info {
	/////////////////////////////////////
	static const char_t CHAR_QUOTE(U('\"'));
	static const string_t STRING_QUOTE(U("\""));
	static const string_t STRING_COMMA(U(","));
	static const string_t STRING_QUOTE_SEMICOLON(U("\":"));
	static const string_t STRING_BEGIN_ARRAY(U("["));
	static const string_t STRING_END_ARRAY(U("]"));
	static const string_t STRING_BEGIN_OBJECT(U("{"));
	static const string_t STRING_END_OBJECT(U("}"));
	static const string_t STRING_TRUE(U("true"));
	static const string_t STRING_FALSE(U("false"));
	static const string_t STRING_VRAI(U("vrai"));
	static const string_t STRING_FAUX(U("faux"));
	static const string_t STRING_YES(U("yes"));
	static const string_t STRING_NO(U("no"));
	static const string_t STRING_OUI(U("oui"));
	static const string_t STRING_NON(U("non"));
	static const string_t STRING_NAN1(U("n/a"));
	static const string_t STRING_NAN2(U("na"));
	static const string_t STRING_NAN3(U("nan"));
	////////////////////////////////////////
	bool stringutils::info_read_any_value(const string_t &sx, any &v) {
		v = any{};
		string_t s{ stringutils::trim(sx) };
		if (s.empty()) {
			return (false);
		}
		bool bString{ false };
		auto jt = s.begin();
		if (*jt == CHAR_QUOTE) {
			size_t n = s.length();
			if (n < 3) {
				return (false);
			}
			s = stringutils::trim(s.substr(1, n - 2));
			bString = true;
		}
		string_t sv = stringutils::tolower(s);
		if ((sv == STRING_TRUE) || (sv == STRING_YES) || (sv == STRING_VRAI)) {
			v = any{ true };
			return (true);
		}
		else if ((sv == STRING_FALSE) || (sv == STRING_NO) || (sv == STRING_FAUX)) {
			v = any{ false };
			return (true);
		}
		if ((sv == STRING_NAN1) || (sv == STRING_NAN2) || (sv == STRING_NAN3)) {
			return (false);
		}
		if (bString) {
			v = any{ s };
			return true;
		}
		{
			try {
				size_t cur{ 0 };
				int nx = std::stoi(s, &cur);
				v = any{ nx };
				return (true);
			}
			catch (std::exception & /* ex*/) {}
		}// int
		{
			try {
				size_t cur{ 0 };
				double nx = std::stod(s, &cur);
				v = any{ nx };
				return (true);
			}
			catch (std::exception & /* ex*/) {}
		}// int
		v = any{ s };
		return (true);
	}// read_any_value
	//////////////////////////////////////
	string_t stringutils::any_to_stringt(const any &v) {
		if (!INFO_ANY_HAS_VALUE(v)) {
			return (string_t{});
		}
		if (v.type() == typeid(string_t)) {
			string_t sRet = INFO_ANY_CAST<string_t>(v);
			string_t sx = STRING_QUOTE + sRet + STRING_QUOTE;
			return (sx);
		}
		else if (v.type() == typeid(int)) {
			ostringstream_t os{};
			int n = INFO_ANY_CAST<int>(v);
			os << n;
			return (os.str());
		}
		else if (v.type() == typeid(double)) {
			ostringstream_t os{};
			double n = INFO_ANY_CAST<double>(v);
			os << n;
			return (os.str());
		}
		else if (v.type() == typeid(float)) {
			ostringstream_t os{};
			float n = INFO_ANY_CAST<float>(v);
			os << n;
			return (os.str());
		}
		else if (v.type() == typeid(bool)) {
			bool n = INFO_ANY_CAST<bool>(v);
			return (n) ? STRING_TRUE : STRING_FALSE;
		}
		else if (v.type() == typeid(infovector)) {
			infovector vv = INFO_ANY_CAST<infovector>(v);
			string_t sRet(STRING_BEGIN_ARRAY);
			for (auto it = vv.begin(); it != vv.end(); ++it) {
				if (it != vv.begin()) {
					sRet += STRING_COMMA;
				}
				string_t s = any_to_stringt(*it);
				sRet += s;
			}// it
			sRet += STRING_END_ARRAY;
			return (sRet);
		}
		else if (v.type() == typeid(infomap)) {
			infomap vv = INFO_ANY_CAST<infomap>(v);
			string_t sRet(STRING_BEGIN_OBJECT);
			for (auto it = vv.begin(); it != vv.end(); ++it) {
				if (it != vv.begin()) {
					sRet += STRING_COMMA;
				}
				string_t key = (*it).first;
				string_t s = any_to_stringt((*it).second);
				sRet += STRING_QUOTE + key + STRING_QUOTE_SEMICOLON + s;
			}// it
			sRet += STRING_END_OBJECT;
			return (sRet);
		}
		return (string_t{});
	}// any_to_stringt
	//////////////////////////////////
	void stringutils::check_string(string_t &s) {
		if (s.empty()) {
			return;
		}
		auto it = s.begin();
		if (*it == CHAR_QUOTE) {
			++it;
			auto jt = it;
			while ((jt != s.end()) && (*jt != CHAR_QUOTE)) {
				++jt;
			}// jt
			if (jt == s.end()) {
				s.clear();
			}
			string_t ss{ it,jt };
			s = ss;
		}// it
	}// check_strings
	//////////////////////////////////
#if defined(_MSC_VER)
	string_t stringutils::trim(const string_t &s) {
		if (s.empty()) {
			return string_t{};
		}
		auto it = s.begin();
		while ((it != s.end()) && std::isblank(*it)) {
			++it;
		}// it
		if (it == s.end()) {
			return string_t{};
		}
		auto jt = s.end();
		--jt;
		while ((jt != it) && std::isblank(*jt)) {
			--jt;
		}// jt
		++jt;
		return string_t{ it,jt };
	}// trim
	//////////////////////////////////////////
	string_t stringutils::tolower(const string_t &s) {
		if (s.empty()) {
			return string_t{};
		}
		string_t sRet{ s };
		std::locale loc{};
		std::transform(s.begin(), s.end(), sRet.begin(), [loc](char_t c)->char_t {
			return static_cast<char_t>(std::tolower(c));
		});
		return (sRet);
	}// tolower
	/////////////////////////////////////////
	string_t stringutils::toupper(const string_t &s) {
		if (s.empty()) {
			return string_t{};
		}
		string_t sRet{ s };
		std::locale loc{};
		std::transform(s.begin(), s.end(), sRet.begin(), [loc](char_t c)->char_t {
			return static_cast<char_t>(std::toupper(c));
		});
		return (sRet);
	}// toupper
#else
	//////////////////////////////////
	string_t stringutils::trim(const string_t &s) {
		return boost::trim_copy(s);
	}// trim
	string_t stringutils::tolower(const string_t &s) {
		return boost::to_lower_copy(s);
	}// tolower
	string_t stringutils::toupper(const string_t &s) {
		return boost::to_upper_copy(s);
	}// toupper
	 //////////////////////////////////////
#endif // _MSC_VER
	/////////////////////////////////////
	size_t stringutils::split_tokens(const string_t &s, std::vector<string_t> &vec) {
		vec.clear();
		string_t ss{ trim(s) };
		while (!ss.empty()) {
			string_t cur{};
			auto it = ss.begin();
			auto lf = ss.end();
			if (*it == CHAR_QUOTE) {
				++it;
				while ((it != lf) && std::isblank(*it)) {
					++it;
				}
				if (it == lf) {
					break;
				}
				auto jt = it;
				while ((jt != lf) && (*jt != CHAR_QUOTE)) {
					++jt;
				}// jt
				auto kt = jt;
				--kt;
				while ((kt != it) && std::isblank(*kt)) {
					--kt;
				}
				if (kt != it) {
					++kt;
					cur = string_t{ it,kt };
				}
				if (jt != lf) {
					++jt;
					if (jt != lf) {
						ss = string_t{ jt,lf };
					}
					else {
						ss.clear();
					}
				}
			}
			else {
				while ((it != lf) && std::isblank(*it)) {
					++it;
				}
				if (it == lf) {
					break;
				}
				auto jt = it;
				while ((jt != ss.end()) && (!std::isblank(*jt))) {
					++jt;
				}// jt
				if (it != jt) {
					cur = string_t{ it,jt };
				}
				if (jt == lf) {
					ss.clear();
				}
				else {
					++jt;
					if (jt == lf) {
						ss.clear();
					}
					else {
						ss = string_t{ jt,lf };
					}
				}
			}
			if (!cur.empty()) {
				vec.push_back(cur);
			}
		}// ss
		return (vec.size());
	}//split_tokens
	 //////////////////////////////////////
	size_t stringutils::split_vector(const string_t &s, std::vector<string_t> &vec,
		const string_t &delim /*= ";,\t "*/, bool bCheck /* = true */) {
		vec.clear();
		string_t ss{ s };
		while (!ss.empty()) {
			size_t pos = ss.find_first_of(delim);
			if (pos == string_t::npos) {
				if (bCheck) {
					check_string(ss);
				}
				vec.push_back(ss);
				break;
			}
			string_t cur = ss.substr(0, pos++);
			if (bCheck) {
				check_string(ss);
			}
			vec.push_back(cur);
			const size_t total = ss.length();
			if (pos >= total) {
				break;
			}
			ss = ss.substr(pos, total - pos);
		}// ss
		return (vec.size());
	}// split_vector
	/////////////////////////////////
}// info