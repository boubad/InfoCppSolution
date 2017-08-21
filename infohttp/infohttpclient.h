#pragma once
#ifndef INFOHTTPCLIENT_H__
#define INFOHTTPCLIENT_H__
//////////////////////////////
#include "info_http_utils.h"
///////////////////////////
#include <info_utils.h>
////////////////////////////////
namespace info {
	namespace http {
		////////////////////////////
		using pplx::task;
		//////////////////////////////
		class infohttpclient {
		private:
			infohttpclient(const infohttpclient &other) = delete;
			infohttpclient & operator=(const infohttpclient &other) = delete;
			//
			string_t form_url(const string_t &baseUri, const query_params &params = query_params{});
		private:
			std::shared_ptr<web::credentials> m_credentials;
			std::shared_ptr<web::http::client::http_client_config> m_config;
			std::shared_ptr<web::http::client::http_client> m_client;
		public:
			infohttpclient(const serverurl &sUrl,
				const username &user = username{},
				const password &pass = password{});
			virtual ~infohttpclient();
		public:
			task<info_http_response_ptr> head(const dataserviceuri &suri,
				const query_params &params = query_params{},
				const query_params &headers = query_params{});
			task<info_http_response_ptr> get(const dataserviceuri &suri,
				const query_params &params = query_params{},
				const query_params &headers = query_params{});
			task<info_http_response_ptr> options(const dataserviceuri &suri,
				const query_params &params = query_params{},
				const query_params &headers = query_params{});
			task<info_http_response_ptr> post(const dataserviceuri &suri,
				const value &postData, const query_params &params = query_params{},
				const query_params &headers =
				query_params{});
			task<info_http_response_ptr> put(const dataserviceuri &suri,
				const value &putData, const query_params &params = query_params{},
				const query_params &headers = query_params{});
			task<info_http_response_ptr> patch(const dataserviceuri &suri,
				const value &patchData, const query_params &params = query_params{},
				const query_params &headers =
				query_params{});
			task<info_http_response_ptr> del(const dataserviceuri &suri,
				const query_params &params = query_params{},
				const query_params &headers = query_params{});
		};
		// class infohttpclient
		/////////////////////////////////
	}// namespace http
	/////////////////////////////////
}// namespace info
 ///////////////////////////////
#endif // INFOHTTPCLIENT_H__
