#pragma once
#ifndef HTTP_MANAGER_H__
#define  HTTP_MANAGER_H__
///////////////////////////////
#include <http_client.h>
////////////////////////////
namespace info {
	namespace http {
		///////////////////////////////
		class serial_infohttpclient;
		////////////////////////////
		class http_manager : public http_client
		{
		private:
			http_manager(const http_manager &other) = delete;
			http_manager & operator=(const http_manager &other) = delete;
			//
			std::shared_ptr<serial_infohttpclient> m_client;
		public:
			http_manager(const serverurl &sUrl,
				const username &user = username{},
				const password &pass = password{});
			virtual ~http_manager();
		public:
			virtual std::future<info_response_ptr> head(const dataserviceuri &suri,
				const query_params &query = query_params{},
				const query_params &headers = query_params{}) override;
			virtual std::future<info_response_ptr> get(const dataserviceuri &suri,
				const query_params &query = query_params{},
				const query_params &headers = query_params{}) override;
			virtual std::future<info_response_ptr> options(const dataserviceuri &suri,
				const query_params &query = query_params{},
				const query_params &headers = query_params{}) override;
			virtual std::future<info_response_ptr> post(const dataserviceuri &suri,
				const any &postData, const query_params &query = query_params{},
				const query_params &headers = query_params{}) override;
			virtual std::future<info_response_ptr> put(const dataserviceuri &suri,
				const any &putData, const query_params &query = query_params{},
				const query_params &headers = query_params{}) override;
			virtual std::future<info_response_ptr> patch(const dataserviceuri &suri,
				const any &patchData, const query_params &query = query_params{},
				const query_params &headers = query_params{}) override;
			virtual std::future<info_response_ptr> del(const dataserviceuri &suri,
				const query_params &query = query_params{},
				const query_params &headers = query_params{}) override;
		};// class http_manager
		///////////////////////////
	}// namespace http
}// namespace info
////////////////////////////
#endif //  HTTP_MANAGER_H__
