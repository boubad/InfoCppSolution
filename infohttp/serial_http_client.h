#pragma once
#ifndef SERIAL_HTTP_CLIENT_H__
#define SERIAL_HTTP_CLIENT_H__
///////////////////////////
#include "infohttpclient.h"
///////////////////////
namespace info {
	namespace http {
		////////////////////////////
		class serial_infohttpclient : public infohttpclient {
		private:
			serial_infohttpclient(const infohttpclient &other) = delete;
			serial_infohttpclient & operator=(const infohttpclient &other) = delete;
		public:
			serial_infohttpclient(const serverurl &sUrl,
				const username &user = username{},
				const password &pass = password{});
			virtual ~serial_infohttpclient();
		public:
			void head(const dataserviceuri &suri, info_http_response &oRet,
				const query_params &params = query_params{},
				const query_params &headers = query_params{});
			void get(const dataserviceuri &suri, info_http_response &oRet,
				const query_params &params = query_params{},
				const query_params &headers = query_params{});
			void options(const dataserviceuri &suri, info_http_response &oRet,
				const query_params &params = query_params{},
				const query_params &headers = query_params{});
			void post(const dataserviceuri &suri, const value &postData,
				info_http_response &oRet, const query_params &params = query_params{},
				const query_params &headers = query_params{});
			void put(const dataserviceuri &suri, const value &putData,
				info_http_response &oRet, const query_params &params = query_params{},
				const query_params &headers = query_params{});
			void patch(const dataserviceuri &suri, const value &patchData,
				info_http_response &oRet, const query_params &params = query_params{},
				const query_params &headers = query_params{});
			void del(const dataserviceuri &suri, info_http_response &oRet,
				const query_params &params = query_params{},
				const query_params &headers = query_params{});
		};
		// class infohttpclient
		////////////////////////////////
	}// namespace http
	 /////////////////////////////////
}// namespace info
 //////////////////////////////
//////////////////////////////
#endif // SERIAL_HTTP_CLIENT_H__
