#pragma once
#ifndef HTTP_CLIENT_H__
#define  HTTP_CLIENT_H__
///////////////////////////////
#include "info_response.h"
#include "info_utils.h"
///////////////////////////////
namespace info {
	///////////////////////
	class http_client
	{
	protected:
		http_client();
	public:
		virtual ~http_client();
	public:
		virtual std::future<info_response_ptr> head(const dataserviceuri &suri,
			const query_params &query = query_params{},
			const query_params &headers = query_params{}) = 0;
		virtual std::future<info_response_ptr> get(const dataserviceuri &suri,
			const query_params &query = query_params{},
			const query_params &headers = query_params{}) = 0;
		virtual std::future<info_response_ptr> options(const dataserviceuri &suri,
			const query_params &query = query_params{},
			const query_params &headers = query_params{}) = 0;
		virtual std::future<info_response_ptr> post(const dataserviceuri &suri,
			const any &postData, const query_params &query = query_params{},
			const query_params &headers = query_params{}) = 0;
		virtual std::future<info_response_ptr> put(const dataserviceuri &suri,
			const any &putData, const query_params &query = query_params{},
			const query_params &headers = query_params{}) = 0;
		virtual std::future<info_response_ptr> patch(const dataserviceuri &suri,
			const any &patchData, const query_params &query = query_params{},
			const query_params &headers = query_params{}) = 0;
		virtual std::future<info_response_ptr> del(const dataserviceuri &suri,
			const query_params &query = query_params{},
			const query_params &headers = query_params{}) = 0;
	}; // class http_client
}// namespace info
//////////////////////////////
#endif // HTTP_CLIENT_H__
