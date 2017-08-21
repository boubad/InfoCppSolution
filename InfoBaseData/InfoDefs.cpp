//InfoDefs.cpp
#include "InfoDefs.h"
///////////////////////
#include <codecvt>
/////////////////////
namespace info {
	////////////////////////////////////////////
	std::wstring utf8ToUtf16(const std::string& utf8Str)
	{
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
		return conv.from_bytes(utf8Str);
	}

	std::string utf16ToUtf8(const std::wstring& utf16Str)
	{
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
		return conv.to_bytes(utf16Str);
	}
	string_t string_to_stringt(const std::string &s) {
#ifdef _MSC_VER
		return utf8ToUtf16(s);
#else
		string_t r{ s };
		return (s);
#endif
	}
	std::string stringt_to_string(const string_t &s) {
#ifdef _MSC_VER
		return utf16ToUtf8(s);
#else
		string_t r{ s };
		return (s);
#endif
	}
	//////////////////////////////////////////
	info_exception::info_exception() noexcept {

	}
	info_exception::info_exception(const string_t &smess) noexcept {
		m_message = smess;
		m_smessage = stringt_to_string(smess);
	}
	info_exception::info_exception(const info_exception & other) noexcept {
		m_message = other.m_message;
		m_smessage = other.m_smessage;
	}
	info_exception::info_exception(const std::exception &other) noexcept {
		const char *ps = other.what();
		if (ps != nullptr) {
			m_smessage = ps;
			m_message = string_to_stringt(m_smessage);
		}
	}
	info_exception& info_exception::operator= (const info_exception& other) noexcept {
		if (this != &other) {
			m_message = other.m_message;
			m_smessage = other.m_smessage;
		}
		return (*this);
	}
	info_exception::~info_exception() {
	}
	const string_t &info_exception::message(void) const noexcept {
		return (m_message);
	}
	const char* info_exception::what() const noexcept {
		return (m_smessage.c_str());
	}
	//////////////////////////////////////////
	bool info_any_equals(const any &v1, const any &v2) {
		if ((!INFO_ANY_HAS_VALUE(v1)) && (!INFO_ANY_HAS_VALUE(v2))) {
			return (true);
		}
		if (v1.type() != v2.type()) {
			return (false);
		}
	    if (v1.type() == typeid(string_t)) {
			string_t r1 = INFO_ANY_CAST<string_t>(v1);
			string_t r2 = INFO_ANY_CAST<string_t>(v2);
			return (r1 == r2);
		}
		else if (v1.type() == typeid(bool)) {
			bool r1 = INFO_ANY_CAST<bool>(v1);
			bool r2 = INFO_ANY_CAST<bool>(v2);
			return (r1 == r2);
		}
		else if (v1.type() == typeid(int)) {
			int r1 = INFO_ANY_CAST<int>(v1);
			int r2 = INFO_ANY_CAST<int>(v2);
			return (r1 == r2);
		}
		else if (v1.type() == typeid(float)) {
			float r1 = INFO_ANY_CAST<float>(v1);
			float r2 = INFO_ANY_CAST<float>(v2);
			return (r1 == r2);
		}
		else if (v1.type() == typeid(double)) {
			double r1 = INFO_ANY_CAST<double>(v1);
			double r2 = INFO_ANY_CAST<double>(v2);
			return (r1 == r2);
		}
		else if (v1.type() == typeid(infovector)) {
			infovector r1 = INFO_ANY_CAST<infovector>(v1);
			infovector r2 = INFO_ANY_CAST<infovector>(v2);
			if (r1.size() != r2.size()) {
				return (false);
			}
			for (auto x : r1) {
				bool bFound{ false };
				for (auto y : r2) {
					if (info_any_equals(x, y)) {
						bFound = true;
						break;
					}
				}// y
				if (!bFound) {
					return (false);
				}
			}// x
			return (true);
		}
		else if (v1.type() == typeid(infomap)) {
			infomap r1 = INFO_ANY_CAST<infomap>(v1);
			infomap r2 = INFO_ANY_CAST<infomap>(v2);
			if (r1.size() != r2.size()) {
				return (false);
			}
			for (auto it = r1.begin(); it != r1.end(); ++it) {
				const string_t &key = (*it).first;
				auto jt = r2.find(key);
				if (jt == r2.end()) {
					return false;
				}
				const any &v = (*it).second;
				const any &vx = (*jt).second;
				if (!info_any_equals(v, vx)) {
					return (false);
				}
			}// it
		}
		return (false);
	}//info_any_equals
///////////////////////////////////////////////
}// namespace info
