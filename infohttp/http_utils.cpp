#include "http_utils.h"
////////////////////////
namespace info {
	namespace http {
		/////////////////////////////
		extern string_t url_encode(const string_t &url) {
			return web::uri::encode_uri(url);
		}// url_encode
		////////////////////////////////
	}// namespace http
}// namespace info
