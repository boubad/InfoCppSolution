#include "serial_http_client.h"
////////////////////////////////////
namespace info {
	namespace http {
		///////////////////////////
		serial_infohttpclient::serial_infohttpclient(const serverurl &sUrl,
			const username &user /*= username{}*/,
			const password &pass /*= password{}*/) :infohttpclient(sUrl,user,pass) {
		}
		serial_infohttpclient::~serial_infohttpclient() {
		}
		void serial_infohttpclient::head(const dataserviceuri &suri, info_http_response &oRet,
			const query_params &params /*= query_params{}*/,
			const query_params &headers /*= query_params{}*/) {
			info_http_response_ptr o = infohttpclient::head(suri,params,headers).get();
			assert(o.get() != nullptr);
			oRet = *o;
		}
		void serial_infohttpclient::get(const dataserviceuri &suri, 
			info_http_response &oRet,
			const query_params &params /*= query_params{}*/,
			const query_params &headers /*= query_params{}*/) {
			info_http_response_ptr o = infohttpclient::get(suri,params,headers).get();
			assert(o.get() != nullptr);
			oRet = *o;
		}
		void serial_infohttpclient::options(const dataserviceuri &suri, 
			info_http_response &oRet,
			const query_params &params /*= query_params{}*/,
			const query_params &headers /*= query_params{}*/) {
			info_http_response_ptr o = infohttpclient::options(suri,params,headers).get();
			assert(o.get() != nullptr);
			oRet = *o;
		}
		void serial_infohttpclient::post(const dataserviceuri &suri, const value &postData,
			info_http_response &oRet, const query_params &params /*= query_params{}*/,
			const query_params &headers /*= query_params{}*/) {
			info_http_response_ptr o = infohttpclient::post(suri, postData,params,headers).get();
			assert(o.get() != nullptr);
			oRet = *o;
		}
		void serial_infohttpclient::put(const dataserviceuri &suri, const value &putData,
			info_http_response &oRet,
			const query_params &params /*= query_params{}*/,
			const query_params &headers /*= query_params{}*/) {
			info_http_response_ptr o = infohttpclient::put(suri, putData,params,headers).get();
			assert(o.get() != nullptr);
			oRet = *o;
		}
		void serial_infohttpclient::patch(const dataserviceuri &suri, const value &patchData,
			info_http_response &oRet, const query_params &params /*= query_params{}*/,
			const query_params &headers /*= query_params{}*/) {
			info_http_response_ptr o = infohttpclient::patch(suri, patchData,params,headers).get();
			assert(o.get() != nullptr);
			oRet = *o;
		}
		void serial_infohttpclient::del(const dataserviceuri &suri, 
			info_http_response &oRet,
			const query_params &params /*= query_params{}*/,
			const query_params &headers /*= query_params{}*/) {
			info_http_response_ptr o = infohttpclient::del(suri,params,headers).get();
			assert(o.get() != nullptr);
			oRet = *o;
		}
		void serial_infohttpclient::maintains_blob(const dataserviceuri &suri,
			const blob_data &blob,
			info_http_response &oRet,
			const query_params &query /*= query_params{}*/,
			const query_params &headers /*= query_params{}*/,
			const string_t &method /*= U("put")*/) {
			info_http_response_ptr o = infohttpclient::maintains_blob(suri, blob, query, headers, method).get();
			assert(o.get() != nullptr);
			oRet = *o;
		}
		std::shared_ptr<blob_data> serial_infohttpclient::read_blob(const dataserviceuri &suri,
			const query_params &params /*= query_params{}*/,
			const query_params &headers /*= query_params{}*/) {
			return infohttpclient::read_blob(suri, params, headers).get();
		}
		/////////////////////
	}// namespace http
}// namespace info
