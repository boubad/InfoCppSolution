#pragma once
#ifndef HTTP_CLIENT_H__
#define  HTTP_CLIENT_H__
///////////////////////////////
#include "info_response.h"
#include "blob_data.h"
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
		virtual serverurl get_serverurl(void) = 0;
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
		//
		virtual std::future<info_response_ptr> maintains_blob(const dataserviceuri &suri,
			const blob_data &blob,
			const query_params &query = query_params{},
			const query_params &headers = query_params{},
			const string_t &method = U("put")) = 0;
		virtual std::future<std::shared_ptr<blob_data>> read_blob(const dataserviceuri &suri,
			const query_params &params = query_params{},
			const query_params &headers = query_params{}) = 0;
	}; // class http_client
}// namespace info
//////////////////////////////
#endif // HTTP_CLIENT_H__
