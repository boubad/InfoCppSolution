#pragma once
#ifndef INFO_HPP_UTILS_H__
#define INFO_HPP_UTILS_H__
////////////////////////////////
#include "infoconv.h"
/////////////////////////////
#include <info_response.h>
////////////////////////////
namespace info {
	namespace http {
		using web::json::value;
		using attachment_data = std::pair<byte_vector, string_t>;
		////////////////////////////
		class info_http_response {
		public:
			unsigned long contentlength;
			web::http::status_code statuscode;
			web::http::http_headers headers;
			string_t stringval;
			value jsonval;
			byte_vector byteval;
			string_t errorstring;
			string_t reason;
		public:
			info_http_response();
			info_http_response(const info_http_response &other);
			info_http_response & operator=(const info_http_response &other);
			~info_http_response();
			operator bool(void) const { return (!has_error()); }
		public:
			string_t content_type(void) const;
			bool has_error(void) const;
			string_t to_string(void) const;
			string_t toString(void) const;
			ostream_t & write_to(ostream_t &os) const;
		};
		// class info_http_response
		inline ostream_t & operator<<(ostream_t &os, const info_http_response &d) {
			return (d.write_to(os));
		}
		using info_http_response_ptr = std::shared_ptr<info_http_response>;
		/////////////////////////////
	}// namsespace http
}// namespace info
/////////////////////////////
#endif // INFO_HPP_UTILS_H__
