#include "infohttpclient.h"
#include "http_utils.h"
#include <stringutils.h>
////////////////////////////
namespace info {
	namespace http {
		////////////////////
		using namespace utility;                    // Common utilities like string conversions
		using namespace web;                        // Common features like URIs.
		using namespace web::http;                  // Common HTTP functionality
		using namespace web::json;
		using namespace web::http::client;
		using namespace concurrency::streams;
		using namespace pplx;
		using namespace std;
		///////////////////////////////////////
		const string_t SKEY_ERROR(U("error"));
		//////////////////////////////////////
		static const string_t ACCEPT_STRING(U("accept"));
		static const string_t ACCEPTS_ALL_STRING(U("*/*"));
		static const string_t APPLICATION_JSON(U("application/json"));
		static const string_t HEADER_CONTENT_TYPE(U("Content-Type"));
		static const string_t JSTRING_JSON(U("json"));
		static const string_t JSTRING_TEXT(U("text"));
		static const string_t STRING_QUESTION(U("?"));
		static const string_t STRING_AND(U("&"));
		static const string_t STRING_EQUAL(U("="));
		static const string_t STRING_SLASH(U("/"));
		static const char_t CHAR_SLASH(U('/'));
		 ////////////////////////////////////////////////
		infohttpclient::infohttpclient(const serverurl &sUrl, const username &user, const password &pass) {
			string_t s0 = sUrl;
			string_t sx = stringutils::trim(s0);
			assert(!sx.empty());
			size_t n = sx.length();
			if (sx[n - 1] == CHAR_SLASH) {
				sx = sx.substr(0, n - 1);
			}
			if ((!user.empty()) && (!pass.empty())) {
				m_credentials.reset(new credentials{ user,pass });
				credentials *pCred = m_credentials.get();
				assert(pCred != nullptr);
				m_config.reset(new http_client_config{});
				http_client_config *pConfig = m_config.get();
				assert(pConfig != nullptr);
				pConfig->set_credentials(*pCred);
				m_client.reset(new http_client{ sx,*pConfig });
				m_url = sx;
			}
			else {
				m_client.reset(new http_client{ (const string_t &)sUrl });
			}
			assert(m_client.get() != nullptr);
		}
		infohttpclient::~infohttpclient() {
			m_client.reset();
			m_config.reset();
			m_credentials.reset();
		}
		string_t infohttpclient::form_url(const string_t &baseUri, 
			const query_params &params /*= query_params{}*/) {
			string_t sQuery{};
			bool bFirst{ true };
			if (!params.empty()) {
				sQuery = STRING_QUESTION;
				for (auto it = params.begin(); it != params.end(); ++it) {
					string_t skey = url_encode((*it).first);
					string_t val = url_encode((*it).second);
					if (!bFirst) {
						sQuery += STRING_AND;
					}
					else {
						bFirst = false;
					}
					sQuery += skey;
					if (!val.empty()) {
						sQuery += STRING_EQUAL + val;
					}
				}	// it
			}	// not empty
			string_t sx = baseUri;
			if (!sQuery.empty()) {
				sx += sQuery;
			}
			return (sx);
		}// form_url
		task<info_http_response_ptr> infohttpclient::head(const dataserviceuri &suri, 
			const query_params &params /*= query_params{}*/,
			const query_params &headers /*= query_params{}*/) {
			std::shared_ptr<string_t> ps = std::make_shared<string_t>(form_url(suri,params));
			assert(ps.get() != nullptr);
			std::shared_ptr<query_params> ph = std::make_shared<query_params>(headers);
			return task<info_http_response_ptr>{[this, ps, ph]()->info_http_response_ptr {
				http_request request(methods::HEAD);
				request.headers().add(ACCEPT_STRING, ACCEPTS_ALL_STRING);
				for (auto it = ph->begin(); it != ph->end(); ++it) {
					request.headers().add((*it).first, (*it).second);
				}//it
				request.set_request_uri(*ps);
				return this->m_client->request(request).then([](http_response response) {
					info_http_response_ptr rsp = std::make_shared<info_http_response>();
					info_http_response *p = rsp.get();
					assert(p != nullptr);
					status_code code = response.status_code();
					p->statuscode = code;
					p->headers = response.headers();
					return (rsp);
				}).get();
			}};
		}// head
		task<info_http_response_ptr> infohttpclient::options(const dataserviceuri &suri,
			const query_params &params /*= query_params{}*/,
			const query_params &headers /*= query_params{}*/) {
			std::shared_ptr<string_t> ps = std::make_shared<string_t>(form_url(suri, params));
			assert(ps.get() != nullptr);
			std::shared_ptr<query_params> ph = std::make_shared<query_params>(headers);
			return task<info_http_response_ptr>{[this, ps, ph]()->info_http_response_ptr {
				http_request request(methods::OPTIONS);
				request.headers().add(ACCEPT_STRING, ACCEPTS_ALL_STRING);
				for (auto it = ph->begin(); it != ph->end(); ++it) {
					request.headers().add((*it).first, (*it).second);
				}//it
				request.set_request_uri(*ps);
				info_http_response_ptr rsp = std::make_shared<info_http_response>();
				assert(rsp.get() != nullptr);
				return this->m_client->request(request).then([rsp](http_response response) {
					info_http_response *p = rsp.get();
					assert(p != nullptr);
					status_code code = response.status_code();
					p->statuscode = code;
					p->headers = response.headers();
					return (rsp);
				}).get();
			}};
		}// options
		task<info_http_response_ptr> infohttpclient::get(const dataserviceuri &suri, 
			const query_params &params /*= query_params{}*/,
			const query_params &headers /*= query_params{}*/) {
			std::shared_ptr<string_t> ps = std::make_shared<string_t>(form_url(suri, params));
			assert(ps.get() != nullptr);
			std::shared_ptr<query_params> ph = std::make_shared<query_params>(headers);
			return task<info_http_response_ptr>{[this, ps, ph]()->info_http_response_ptr {
				http_request request(methods::GET);
				request.headers().add(ACCEPT_STRING, ACCEPTS_ALL_STRING);
				for (auto it = ph->begin(); it != ph->end(); ++it) {
					request.headers().add((*it).first, (*it).second);
				}//it
				request.set_request_uri(*ps);
				info_http_response_ptr rsp = std::make_shared<info_http_response>();
				assert(rsp.get() != nullptr);
				return this->m_client->request(request).then([rsp](http_response response) {
					info_http_response *p = rsp.get();
					assert(p != nullptr);
					status_code code = response.status_code();
					p->statuscode = code;
					p->headers = response.headers();
					string_t stype = response.headers().content_type();
					if (stype == APPLICATION_JSON) {
						return response.extract_json();
					}
					else {
						value v{};
						return task_from_result(v);
					}
				}).then([rsp](task<value> previousTask) {
					info_http_response *p = rsp.get();
					assert(p != nullptr);
					try {
						value val = previousTask.get();
						p->jsonval = val;
					}
					catch (std::exception & ex) {
						std::string s = ex.what();
						p->errorstring = utility::conversions::to_string_t(s);
					}
					return (rsp);
				}).get();
			}};
		}// get
		task<info_http_response_ptr> infohttpclient::post(const dataserviceuri &suri, 
			const value &postData, const query_params &params /*= query_params{}*/,
			const query_params &headers /*= query_params{}*/) {
			std::shared_ptr<string_t> ps = std::make_shared<string_t>(form_url(suri, params));
			assert(ps.get() != nullptr);
			std::shared_ptr<value> pv = std::make_shared<value>(postData);
			assert(pv.get() != nullptr);
			std::shared_ptr<query_params> ph = std::make_shared<query_params>(headers);
			return task<info_http_response_ptr>{[this, ps, pv, ph]()->info_http_response_ptr {
				http_request request(methods::POST);
				request.headers().add(ACCEPT_STRING, ACCEPTS_ALL_STRING);
				for (auto it = ph->begin(); it != ph->end(); ++it) {
					request.headers().add((*it).first, (*it).second);
				}//it
				request.set_request_uri(*ps);
				request.set_body(*pv);
				info_http_response_ptr rsp = std::make_shared<info_http_response>();
				assert(rsp.get() != nullptr);
				return this->m_client->request(request).then([rsp](http_response response) {
					info_http_response *p = rsp.get();
					assert(p != nullptr);
					status_code code = response.status_code();
					p->statuscode = code;
					p->headers = response.headers();
					string_t stype = response.headers().content_type();
					if (stype == APPLICATION_JSON) {
						return response.extract_json();
					}
					else {
						value v{};
						return task_from_result(v);
					}
				}).then([rsp](task<value> previousTask) {
					info_http_response *p = rsp.get();
					assert(p != nullptr);
					try {
						value val = previousTask.get();
						p->jsonval = val;
					}
					catch (std::exception & ex) {
						std::string s = ex.what();
						p->errorstring = utility::conversions::to_string_t(s);
					}
					return (rsp);
				}).get();
			}};
		}// post
		task<info_http_response_ptr> infohttpclient::put(const dataserviceuri &suri,
			const value &putData, const query_params &params /*= query_params{}*/,
			const query_params &headers /*= query_params{}*/) {
			std::shared_ptr<string_t> ps = std::make_shared<string_t>(form_url(suri, params));
			assert(ps.get() != nullptr);
			std::shared_ptr<value> pv = std::make_shared<value>(putData);
			assert(pv.get() != nullptr);
			std::shared_ptr<query_params> ph = std::make_shared<query_params>(headers);
			return task<info_http_response_ptr>{[this, ps, pv, ph]()->info_http_response_ptr {
				http_request request(methods::PUT);
				request.headers().add(ACCEPT_STRING, ACCEPTS_ALL_STRING);
				for (auto it = ph->begin(); it != ph->end(); ++it) {
					request.headers().add((*it).first, (*it).second);
				}//it
				request.set_request_uri(*ps);
				if (!pv->is_null()) {
					request.set_body(*pv);
				}
				info_http_response_ptr rsp = std::make_shared<info_http_response>();
				assert(rsp.get() != nullptr);
				return this->m_client->request(request).then([rsp](http_response response) {
					info_http_response *p = rsp.get();
					assert(p != nullptr);
					status_code code = response.status_code();
					p->statuscode = code;
					p->headers = response.headers();
					string_t stype = response.headers().content_type();
					if (stype == APPLICATION_JSON) {
						return response.extract_json();
					}
					else {
						value v{};
						return task_from_result(v);
					}
				}).then([rsp](task<value> previousTask) {
					info_http_response *p = rsp.get();
					assert(p != nullptr);
					try {
						value val = previousTask.get();
						p->jsonval = val;
					}
					catch (std::exception & ex) {
						std::string s = ex.what();
						p->errorstring = utility::conversions::to_string_t(s);
					}
					return (rsp);
				}).get();
			}};
		}// put
		task<info_http_response_ptr> infohttpclient::patch(const dataserviceuri &suri, 
			const value &patchData, const query_params &params /*= query_params{}*/,
			const query_params &headers /*= query_params{}*/) {
			std::shared_ptr<string_t> ps = std::make_shared<string_t>(form_url(suri, params));
			assert(ps.get() != nullptr);
			std::shared_ptr<value> pv = std::make_shared<value>(patchData);
			assert(pv.get() != nullptr);
			std::shared_ptr<query_params> ph = std::make_shared<query_params>(headers);
			return task<info_http_response_ptr>{[this, ps, pv, ph]()->info_http_response_ptr {
				http_request request(methods::PATCH);
				request.headers().add(ACCEPT_STRING, ACCEPTS_ALL_STRING);
				for (auto it = ph->begin(); it != ph->end(); ++it) {
					request.headers().add((*it).first, (*it).second);
				}//it
				request.set_request_uri(*ps);
				if (!pv->is_null()) {
					request.set_body(*pv);
				}
				info_http_response_ptr rsp = std::make_shared<info_http_response>();
				assert(rsp.get() != nullptr);
				return this->m_client->request(request).then([rsp](http_response response) {
					info_http_response *p = rsp.get();
					assert(p != nullptr);
					status_code code = response.status_code();
					p->statuscode = code;
					p->headers = response.headers();
					string_t stype = response.headers().content_type();
					if (stype == APPLICATION_JSON) {
						return response.extract_json();
					}
					else {
						value v{};
						return task_from_result(v);
					}
				}).then([rsp](task<value> previousTask) {
					info_http_response *p = rsp.get();
					assert(p != nullptr);
					try {
						value val = previousTask.get();
						p->jsonval = val;
					}
					catch (std::exception & ex) {
						std::string s = ex.what();
						p->errorstring = utility::conversions::to_string_t(s);
					}
					return (rsp);
				}).get();
			}};
		}// patch
		task<info_http_response_ptr> infohttpclient::del(const dataserviceuri &suri,
			const query_params &params /*= query_params{}*/,
			const query_params &headers /*= query_params{}*/) {
			std::shared_ptr<string_t> ps = std::make_shared<string_t>(form_url(suri, params));
			assert(ps.get() != nullptr);
			std::shared_ptr<query_params> ph = std::make_shared<query_params>(headers);
			return task<info_http_response_ptr>{[this, ps, ph]()->info_http_response_ptr {
				http_request request(methods::DEL);
				request.headers().add(ACCEPT_STRING, ACCEPTS_ALL_STRING);
				for (auto it = ph->begin(); it != ph->end(); ++it) {
					request.headers().add((*it).first, (*it).second);
				}//it
				request.set_request_uri(*ps);
				info_http_response_ptr rsp = std::make_shared<info_http_response>();
				assert(rsp.get() != nullptr);
				return this->m_client->request(request).then([rsp](http_response response) {
					info_http_response *p = rsp.get();
					assert(p != nullptr);
					status_code code = response.status_code();
					p->statuscode = code;
					p->headers = response.headers();
					string_t stype = response.headers().content_type();
					if (stype == APPLICATION_JSON) {
						return response.extract_json();
					}
					else {
						value v{};
						return task_from_result(v);
					}
				}).then([rsp](task<value> previousTask) {
					info_http_response *p = rsp.get();
					assert(p != nullptr);
					try {
						value val = previousTask.get();
						p->jsonval = val;
					}
					catch (std::exception & ex) {
						std::string s = ex.what();
						p->errorstring = utility::conversions::to_string_t(s);
					}
					return (rsp);
				}).get();
			}};
		}// del
		/////////////////////////////////
		task<info_http_response_ptr> infohttpclient::maintains_blob(const dataserviceuri &suri,
			const blob_data &blob,
			const query_params &query /*= query_params{}*/,
			const query_params &headers /*= query_params{}*/,
			const string_t &method /*= U("put")*/) {
			std::shared_ptr<string_t> ps = std::make_shared<string_t>(form_url(suri, query));
			assert(ps.get() != nullptr);
			std::shared_ptr<blob_data> pv = std::make_shared<blob_data>(blob);
			assert(pv.get() != nullptr);
			std::shared_ptr<string_t> sm = std::make_shared<string_t>(method);
			assert(sm.get() != nullptr);
			std::shared_ptr<query_params> ph = std::make_shared<query_params>(headers);
			assert(ph.get() != nullptr);
			return task<info_http_response_ptr>{[this, ps, pv,sm, ph]()->info_http_response_ptr {
				std::shared_ptr<http_request> req{};
				if (*sm == U("put")) {
					req.reset(new http_request{ methods::PUT });
				}
				else {
					req.reset(new http_request{ methods::POST });
				}
				req->headers().add(ACCEPT_STRING, ACCEPTS_ALL_STRING);
				for (auto it = ph->begin(); it != ph->end(); ++it) {
					req->headers().add((*it).first, (*it).second);
				}//it
				req->set_request_uri(*ps);
				size_t n{ 0 };
				const byte *pData = pv->data(n);
				std::vector<byte> vec{ pData, pData + n };
				req->set_body(vec);
				req->headers().add(HEADER_CONTENT_TYPE, pv->mime_type());
				info_http_response_ptr rsp = std::make_shared<info_http_response>();
				assert(rsp.get() != nullptr);
				return this->m_client->request(*req).then([rsp](http_response response) {
					info_http_response *p = rsp.get();
					assert(p != nullptr);
					status_code code = response.status_code();
					p->statuscode = code;
					p->headers = response.headers();
					string_t stype = response.headers().content_type();
					if (stype == APPLICATION_JSON) {
						return response.extract_json();
					}
					else {
						value v{};
						return task_from_result(v);
					}
				}).then([rsp](task<value> previousTask) {
					info_http_response *p = rsp.get();
					assert(p != nullptr);
					try {
						value val = previousTask.get();
						p->jsonval = val;
					}
					catch (std::exception & ex) {
						std::string s = ex.what();
						p->errorstring = utility::conversions::to_string_t(s);
					}
					return (rsp);
				}).get();
			}};
		}//maintains_blob
		task<std::shared_ptr<blob_data>> infohttpclient::read_blob(const dataserviceuri &suri,
			const query_params &params /*= query_params{}*/,
			const query_params &headers /*= query_params{}*/) {
			std::shared_ptr<string_t> ps = std::make_shared<string_t>(form_url(suri, params));
			assert(ps.get() != nullptr);
			std::shared_ptr<query_params> ph = std::make_shared<query_params>(headers);
			return task<std::shared_ptr<blob_data>>{[this, ps, ph]()->std::shared_ptr<blob_data> {
				http_request request(methods::GET);
				request.headers().add(ACCEPT_STRING, ACCEPTS_ALL_STRING);
				for (auto it = ph->begin(); it != ph->end(); ++it) {
					request.headers().add((*it).first, (*it).second);
				}//it
				request.set_request_uri(*ps);
				std::shared_ptr<blob_data> oRet{ new blob_data{} };
				blob_data *px = oRet.get();
				assert(px != nullptr);
				px->url(*ps);
				return this->m_client->request(request).then([oRet](http_response response) {
					std::vector<unsigned char >  vec{};
					blob_data *p = oRet.get();
					assert(p != nullptr);
					int code = static_cast<int>(response.status_code());
					if (code < 400) {
						auto headers = response.headers();
						if (headers.has(HEADER_CONTENT_TYPE)) {
							string_t sx = headers[HEADER_CONTENT_TYPE];
							p->mime_type(sx);
						}
						return response.extract_vector();
					}
					else if (code < 500) {
						return task_from_result(vec);
					}
					else {
						throw http_exception(code);
					}
				}).then([oRet](task<std::vector<unsigned char > > previousTask) {
					try {
						blob_data *p = oRet.get();
						std::vector<byte> vec = previousTask.get();
						p->data(vec);
					}
					catch (std::exception & /*ex*/) {
					}
					return (oRet);
				}).get();
			}};
		}//read_blob
		////////////////////////////
	}// namespace http
}// namespace info
