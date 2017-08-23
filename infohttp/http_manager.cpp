#include "http_manager.h"
#include "serial_http_client.h"
#include "infoconv.h"
/////////////////////////////////
namespace info {
	namespace http {
		///////////////////////////////////
		static void convert_rsp(const info_http_response &src, info_response &dest) {
			dest.contentlength = src.contentlength;
			dest.statuscode = static_cast<int>(src.statuscode);
			dest.reason = src.reason;
			dest.errorstring = src.errorstring;
			dest.byteval = src.byteval;
			dest.stringval = src.stringval;
			convert_any(src.jsonval, dest.jsonval);
			std::map<string_t, string_t> &oMap = dest.headers;
			oMap.clear();
			for (auto it = src.headers.begin(); it != src.headers.end(); ++it) {
				string_t key = (*it).first;
				string_t val = (*it).second;
				oMap[key] = val;
			}// it
		}//convert_rsp
		///////////////////////////////
		http_manager::http_manager(const serverurl &sUrl,
			const username &user /*= username{}*/,
			const password &pass /*= password{}*/):
			m_client(new serial_infohttpclient{sUrl,user,pass})
		{
			assert(m_client.get() != nullptr);
		}
		http_manager::~http_manager()
		{
		}
		serverurl http_manager::get_serverurl(void) {
			return m_client->get_serverurl();
		}
		std::future<info_response_ptr> http_manager::head(const dataserviceuri &suri,
			const query_params &query /*= query_params{}*/,
			const query_params &headers /*= query_params{}*/) {
			std::shared_ptr<dataserviceuri> xuri = std::make_shared<dataserviceuri>(suri);
			std::shared_ptr<query_params> xquery = std::make_shared<query_params>(query);
			std::shared_ptr<query_params> xheaders = std::make_shared<query_params>(headers);
			return std::async( [this,xuri,xquery,xheaders]()->info_response_ptr {
				info_response_ptr oRet = std::make_shared<info_response>();
				info_http_response rsp{};
				this->m_client->head(*xuri, rsp, *xquery, *xheaders);
				convert_rsp(rsp, *oRet);
				return (oRet);
			});
		}// head
		std::future<info_response_ptr> http_manager::get(const dataserviceuri &suri,
			const query_params &query /*= query_params{}*/,
			const query_params &headers /*= query_params{}*/) {
			std::shared_ptr<dataserviceuri> xuri = std::make_shared<dataserviceuri>(suri);
			std::shared_ptr<query_params> xquery = std::make_shared<query_params>(query);
			std::shared_ptr<query_params> xheaders = std::make_shared<query_params>(headers);
			return std::async( [this, xuri, xquery, xheaders]()->info_response_ptr {
				info_response_ptr oRet = std::make_shared<info_response>();
				info_http_response rsp{};
				this->m_client->get(*xuri, rsp, *xquery, *xheaders);
				convert_rsp(rsp, *oRet);
				return (oRet);
			});
		}// get
		std::future<info_response_ptr> http_manager::options(const dataserviceuri &suri,
			const query_params &query /*= query_params{}*/,
			const query_params &headers /*= query_params{}*/) {
			std::shared_ptr<dataserviceuri> xuri = std::make_shared<dataserviceuri>(suri);
			std::shared_ptr<query_params> xquery = std::make_shared<query_params>(query);
			std::shared_ptr<query_params> xheaders = std::make_shared<query_params>(headers);
			return std::async( [this, xuri, xquery, xheaders]()->info_response_ptr {
				info_response_ptr oRet = std::make_shared<info_response>();
				info_http_response rsp{};
				this->m_client->options(*xuri, rsp, *xquery, *xheaders);
				convert_rsp(rsp, *oRet);
				return (oRet);
			});
		}
		std::future<info_response_ptr> http_manager::post(const dataserviceuri &suri,
			const any &postData, const query_params &query/* = query_params{}*/,
			const query_params &headers /*= query_params{}*/) {
			std::shared_ptr<dataserviceuri> xuri = std::make_shared<dataserviceuri>(suri);
			std::shared_ptr<query_params> xquery = std::make_shared<query_params>(query);
			std::shared_ptr<query_params> xheaders = std::make_shared<query_params>(headers);
			std::shared_ptr<any> xa = std::make_shared<any>(postData);
			return std::async( [this, xuri, xquery, xheaders,xa]()->info_response_ptr {
				info_response_ptr oRet = std::make_shared<info_response>();
				info_http_response rsp{};
				value val{};
				convert_any(*xa, val);
				this->m_client->post(*xuri, val, rsp, *xquery, *xheaders);
				convert_rsp(rsp, *oRet);
				return (oRet);
			});
		}
		std::future<info_response_ptr> http_manager::put(const dataserviceuri &suri,
			const any &putData, const query_params &query /*= query_params{}*/,
			const query_params &headers /*= query_params{}*/) {
			std::shared_ptr<dataserviceuri> xuri = std::make_shared<dataserviceuri>(suri);
			std::shared_ptr<query_params> xquery = std::make_shared<query_params>(query);
			std::shared_ptr<query_params> xheaders = std::make_shared<query_params>(headers);
			std::shared_ptr<any> xa = std::make_shared<any>(putData);
			return std::async( [this, xuri, xquery, xheaders, xa]()->info_response_ptr {
				info_response_ptr oRet = std::make_shared<info_response>();
				info_http_response rsp{};
				value val{};
				convert_any(*xa, val);
				this->m_client->put(*xuri, val, rsp, *xquery, *xheaders);
				convert_rsp(rsp, *oRet);
				return (oRet);
			});
		}
		std::future<info_response_ptr> http_manager::patch(const dataserviceuri &suri,
			const any &patchData, const query_params &query /*= query_params{}*/,
			const query_params &headers /*= query_params{}*/) {
			std::shared_ptr<dataserviceuri> xuri = std::make_shared<dataserviceuri>(suri);
			std::shared_ptr<query_params> xquery = std::make_shared<query_params>(query);
			std::shared_ptr<query_params> xheaders = std::make_shared<query_params>(headers);
			std::shared_ptr<any> xa = std::make_shared<any>(patchData);
			return std::async( [this, xuri, xquery, xheaders, xa]()->info_response_ptr {
				info_response_ptr oRet = std::make_shared<info_response>();
				info_http_response rsp{};
				value val{};
				convert_any(*xa, val);
				this->m_client->patch(*xuri, val, rsp, *xquery, *xheaders);
				convert_rsp(rsp, *oRet);
				return (oRet);
			});
		}
		std::future<info_response_ptr> http_manager::del(const dataserviceuri &suri,
			const query_params &query /*= query_params{}*/,
			const query_params &headers /*= query_params{}*/) {
			std::shared_ptr<dataserviceuri> xuri = std::make_shared<dataserviceuri>(suri);
			std::shared_ptr<query_params> xquery = std::make_shared<query_params>(query);
			std::shared_ptr<query_params> xheaders = std::make_shared<query_params>(headers);
			return std::async( [this, xuri, xquery, xheaders]()->info_response_ptr {
				info_response_ptr oRet = std::make_shared<info_response>();
				info_http_response rsp{};
				this->m_client->del(*xuri, rsp, *xquery, *xheaders);
				convert_rsp(rsp, *oRet);
				return (oRet);
			});
		}
		std::future<info_response_ptr> http_manager::maintains_blob(const dataserviceuri &suri,
			const blob_data &blob,
			const query_params &query /* = query_params{}*/,
			const query_params &headers /*= query_params{}*/,
			const string_t &method /*= U("put")*/) {
			std::shared_ptr<dataserviceuri> xuri = std::make_shared<dataserviceuri>(suri);
			std::shared_ptr<blob_data> pv = std::make_shared<blob_data>(blob);
			std::shared_ptr<query_params> xquery = std::make_shared<query_params>(query);
			std::shared_ptr<query_params> xheaders = std::make_shared<query_params>(headers);
			std::shared_ptr<string_t> sm = std::make_shared<string_t>(method);
			return std::async([this, xuri, pv, xquery, xheaders,sm]()->info_response_ptr {
				info_response_ptr oRet = std::make_shared<info_response>();
				info_http_response rsp{};
				this->m_client->maintains_blob(*xuri, *pv, rsp, *xquery, *xheaders, *sm);
				convert_rsp(rsp, *oRet);
				return (oRet);
			});
		}
		std::future<std::shared_ptr<blob_data>> http_manager::read_blob(const dataserviceuri &suri,
			const query_params &params /*= query_params{}*/,
			const query_params &headers /*= query_params{}*/) {
			std::shared_ptr<dataserviceuri> xuri = std::make_shared<dataserviceuri>(suri);
			std::shared_ptr<query_params> xquery = std::make_shared<query_params>(params);
			std::shared_ptr<query_params> xheaders = std::make_shared<query_params>(headers);
			return std::async([this, xuri,xquery, xheaders]()->std::shared_ptr<blob_data> {
				return this->m_client->read_blob(*xuri, *xquery, *xheaders);
			});
		}
		//////////////////////////////////
	}// namespace http
}// namespace info
