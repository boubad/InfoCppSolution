#include "info_response.h"
////////////////////////
namespace info {
	////////////////////////
	static const string_t HEADER_CONTENT_TYPE(U("Content-Type"));
	/////////////////////////
	info_response::info_response() :contentlength(0), statuscode(500) {
	}
	info_response::info_response(const info_response &other) : contentlength(other.contentlength),
		statuscode(other.statuscode), headers(other.headers), stringval(other.stringval),
		jsonval(other.jsonval), byteval(other.byteval), errorstring(other.errorstring), reason(other.reason) {

	}
	info_response & info_response::operator=(const info_response &other) {
		if (this != &other) {
			this->contentlength = other.contentlength;
			this->statuscode = other.statuscode;
			this->headers = other.headers;
			this->stringval = other.stringval;
			this->jsonval = other.jsonval;
			this->byteval = other.byteval;
			this->errorstring = other.errorstring;
			this->reason = other.reason;
		}
		return (*this);
	}
	info_response::~info_response() {

	}
	string_t info_response::content_type(void) const {
		string_t sRet{};
		auto it = headers.find(HEADER_CONTENT_TYPE);
		if (it != headers.end()) {
			sRet = (*it).second;
		}
		return (sRet);
	}
	bool info_response::has_error(void) const {
		int n = static_cast<int>(statuscode);
		return ((!errorstring.empty()) || (n >= 400));
	}
	
	///////////////////////
}// info
