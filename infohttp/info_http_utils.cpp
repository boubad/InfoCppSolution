#include "info_http_utils.h"
////////////////////////////////
namespace info {
	namespace http {
		using namespace web::http;                  // Common HTTP functionality
		using namespace web::json;
		using namespace web::http::client;
		////////////////////////////////
		static const string_t HEADER_CONTENT_TYPE(U("Content-Type"));
		/////////////////////////////////
		info_http_response::info_http_response() :contentlength(0), statuscode(status_codes::NotImplemented) {
		}
		info_http_response::info_http_response(const info_http_response &other) : contentlength(other.contentlength),
			statuscode(other.statuscode), headers(other.headers), stringval(other.stringval),
			jsonval(other.jsonval), byteval(other.byteval), errorstring(other.errorstring), reason(other.reason) {

		}
		info_http_response & info_http_response::operator=(const info_http_response &other) {
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
		info_http_response::~info_http_response() {

		}
		string_t info_http_response::content_type(void) const {
			string_t sRet{};
			if (headers.has(HEADER_CONTENT_TYPE)) {
				sRet = headers.content_type();
			}
			return (sRet);
		}
		bool info_http_response::has_error(void) const {
			int n = static_cast<int>(statuscode);
			return ((!errorstring.empty()) || (n >= 400));
		}
		string_t info_http_response::to_string(void) const {
			stringstream_t os{};
			write_to(os);
			string_t s = os.str();
			return (s);
		}
		string_t info_http_response::toString(void) const {
			return to_string();
		}
		ostream_t & info_http_response::write_to(ostream_t &os) const {
			os << U("Status Code:\t") << statuscode << std::endl;
			if (!errorstring.empty()) {
				os << U("Error string:\t") << errorstring << std::endl;
			}
			for (auto it = headers.begin(); it != headers.end(); ++it) {
				os << (*it).first << U(":\t") << (*it).second << std::endl;
			}// it
			if (!stringval.empty()) {
				os << U("String Content:\t") << stringval << std::endl;
			}
			if (!jsonval.is_null()) {
				os << U("Json Content:\t") << jsonval.serialize() << std::endl;
			}
			if (!byteval.empty()) {
				os << U("Byte Content:\t") << byteval.size() << U(" bytes") << std::endl;
			}
			return (os);
		}// write_to
	}// namespace http
}// namespace info