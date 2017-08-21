#include "couchdb_manager.h"
#include <stringutils.h>
#include <http_utils.h>
#include "query_filter.h"
#include "server_info.h"
////////////////////////////
namespace info {
	namespace couchdb {
		using namespace info::http;
		////////////////////////////////
		static const string_t STRING_SLASH(U("/"));
		/////////////////////////////
		couchdb_manager::couchdb_manager(http_client &client) :m_client(client)
		{
		}
		couchdb_manager::~couchdb_manager()
		{
		}
		bool couchdb_manager::get_server_info(server_info &info) {
			info.clear();
			bool bRet{ false };
			dataserviceuri uri{};
			info_response_ptr rsp = m_client.get(uri).get();
			info_response *pRsp = rsp.get();
			assert(pRsp != nullptr);
			if (!pRsp->has_error()) {
				info = server_info{ pRsp->jsonval };
				bRet = info.ok();
			}
			return (bRet);
		}//get_server_info
		std::future<std::vector<string_t>> couchdb_manager::get_all_databases_async(void) {
			return std::async([this]()->std::vector<string_t> {
				std::vector<string_t> vRet{};
				dataserviceuri uri{ U("/_all_dbs") };
				info_response_ptr rsp = m_client.get(uri).get();
				info_response *pRsp = rsp.get();
				assert(pRsp != nullptr);
				if (!pRsp->has_error()) {
					const any &va = pRsp->jsonval;
					if (va.type() == typeid(infovector)) {
						infovector vv = INFO_ANY_CAST<infovector>(va);
						for (auto it = vv.begin(); it != vv.end(); ++it) {
							const any &x = *it;
							if (x.type() == typeid(string_t)) {
								vRet.push_back(INFO_ANY_CAST<string_t>(x));
							}
						}// it
					}// vector
				}// ok
				return (vRet);
			});
		}//get_all_databases_async
		std::future<std::vector<string_t>> couchdb_manager::get_uuids_async(int nCount /*= 1*/) {
			return std::async([this, nCount]()->std::vector<string_t> {
				std::vector<string_t> vRet{};
				dataserviceuri uri{ U("/_uuids") };
				int nc = (nCount > 0) ? nCount : 1;
				query_params query{};
				string_t sx{};
				{
					ostringstream_t os{};
					os << nc;
					sx = os.str();
				}
				query.push_back(std::make_pair(U("count"), sx));
				info_response_ptr rsp = m_client.get(uri, query).get();
				info_response *pRsp = rsp.get();
				assert(pRsp != nullptr);
				if (!pRsp->has_error()) {
					const any &va = pRsp->jsonval;
					if (va.type() == typeid(infomap)) {
						infomap oMap = INFO_ANY_CAST<infomap>(va);
						auto it = oMap.find(U("uuids"));
						if (it != oMap.end()) {
							any vx = (*it).second;
							if (vx.type() == typeid(infovector)) {
								infovector vv = INFO_ANY_CAST<infovector>(vx);
								for (auto jt = vv.begin(); jt != vv.end(); ++jt) {
									const any &x = *jt;
									if (x.type() == typeid(string_t)) {
										vRet.push_back(INFO_ANY_CAST<string_t>(x));
									}
								}// it
							}// vector
						}// uuids
					}// map
				}// ok
				return (vRet);
			});
		}//get_uuids_async
		std::future<bool> couchdb_manager::exists_database_async(const string_t &dbname) {
			string_t sx = stringutils::tolower(stringutils::trim(dbname));
			if (sx.empty()) {
				throw info_exception{ U("Invalid database name.") };
			}
			std::shared_ptr<string_t> name = std::make_shared<string_t>(sx);
			return std::async([this, name]()->bool {
				bool bRet{ false };
				string_t suri{ STRING_SLASH };
				suri += *name;
				dataserviceuri uri{ suri};
				info_response_ptr rsp = m_client.head(uri).get();
				info_response *pRsp = rsp.get();
				assert(pRsp != nullptr);
				if (!pRsp->has_error()) {
					bRet = (pRsp->statuscode == 200);
				}// ok
				return (bRet);
			});
		}//exists_database_async
		std::future<bool> couchdb_manager::create_database_async(const string_t &dbname) {
			string_t sx = stringutils::tolower(stringutils::trim(dbname));
			if (sx.empty()) {
				throw info_exception{ U("Invalid database name.") };
			}
			std::shared_ptr<string_t> name = std::make_shared<string_t>(sx);
			return std::async([this, name]()->bool {
				bool bRet{ false };
				string_t suri{ STRING_SLASH };
				suri += *name;
				dataserviceuri uri{ suri };
				any dummy{};
				info_response_ptr rsp = m_client.put(uri,dummy).get();
				info_response *pRsp = rsp.get();
				assert(pRsp != nullptr);
				if (!pRsp->has_error()) {
					const any &va = pRsp->jsonval;
					if (va.type() == typeid(infomap)) {
						infomap oMap = INFO_ANY_CAST<infomap>(va);
						auto it = oMap.find(U("ok"));
						if (it != oMap.end()) {
							any vx = (*it).second;
							if (vx.type() == typeid(bool)) {
								bRet = INFO_ANY_CAST<bool>(vx);
							}
						}// it
					}// map
				}// ok
				return (bRet);
			});
		}//exists_database_async
		std::future<bool> couchdb_manager::delete_database_async(const string_t &dbname) {
			string_t sx = stringutils::tolower(stringutils::trim(dbname));
			if (sx.empty()) {
				throw info_exception{ U("Invalid database name.") };
			}
			std::shared_ptr<string_t> name = std::make_shared<string_t>(sx);
			return std::async([this, name]()->bool {
				bool bRet{ false };
				string_t suri{ STRING_SLASH };
				suri += *name;
				dataserviceuri uri{ suri };
				info_response_ptr rsp = m_client.del(uri).get();
				info_response *pRsp = rsp.get();
				assert(pRsp != nullptr);
				if (!pRsp->has_error()) {
					const any &va = pRsp->jsonval;
					if (va.type() == typeid(infomap)) {
						infomap oMap = INFO_ANY_CAST<infomap>(va);
						auto it = oMap.find(U("ok"));
						if (it != oMap.end()) {
							any vx = (*it).second;
							if (vx.type() == typeid(bool)) {
								bRet = INFO_ANY_CAST<bool>(vx);
							}
						}// it
					}// map
				}// ok
				return (bRet);
			});
		}//delete_database_async
		std::future<update_response> couchdb_manager::create_document_async(const string_t &dbname, const any &doc) {
			if (doc.type() != typeid(infomap)) {
				throw info_exception{ U("Invalid document.") };
			}
			string_t sx = stringutils::tolower(stringutils::trim(dbname));
			if (sx.empty()) {
				throw info_exception{ U("Invalid database name.") };
			}
			std::shared_ptr<string_t> name = std::make_shared<string_t>(sx);
			std::shared_ptr<any> pa = std::make_shared<any>(doc);
			return std::async([this, name, pa]()->update_response {
				infomap oMap = INFO_ANY_CAST<infomap>(*pa);
				auto it = oMap.find(U("_rev"));
				if (it != oMap.end()) {
					oMap.erase(it);
				}
				if (oMap.empty()) {
					throw info_exception{ U("Empty document.") };
				}
				update_response bRet{};
				string_t suri{ STRING_SLASH };
				suri += *name;
				dataserviceuri uri{ suri };
				any vdoc{ oMap };
				info_response_ptr rsp = m_client.post(uri,vdoc).get();
				info_response *pRsp = rsp.get();
				assert(pRsp != nullptr);
				bRet.status_code(pRsp->statuscode);
				if (!pRsp->has_error()) {
					const any &va = pRsp->jsonval;
					bRet.set(va);
				}// ok
				return (bRet);
			});
		}//create_document_async
		std::future<update_response> couchdb_manager::delete_document_async(const string_t &dbname,
			const string_t &sid, const string_t &srev) {
			string_t sx = stringutils::tolower(stringutils::trim(dbname));
			if (sx.empty() || sid.empty() || srev.empty()) {
				throw info_exception{ U("Invalid parameter(s).") };
			}
			std::shared_ptr<string_t> name = std::make_shared<string_t>(sx);
			std::shared_ptr<string_t> si = std::make_shared<string_t>(sid);
			std::shared_ptr<string_t> sr = std::make_shared<string_t>(srev);
			return std::async([this, name, si,sr]()->update_response {
				update_response bRet{};
				string_t suri{ STRING_SLASH };
				suri += *name;
				suri += STRING_SLASH + url_encode(*si);
				query_params query{};
				query.push_back(std::make_pair(U("rev"), *sr));
				dataserviceuri uri{ suri };
				info_response_ptr rsp = m_client.del(uri, query).get();
				info_response *pRsp = rsp.get();
				assert(pRsp != nullptr);
				bRet.status_code(pRsp->statuscode);
				if (!pRsp->has_error()) {
					const any &va = pRsp->jsonval;
					bRet.set(va);
				}// ok
				return (bRet);
			});
		}//delete_document_async
		std::future<string_t> couchdb_manager::get_document_version_async(const string_t &dbname, const string_t &sid) {
			string_t sx = stringutils::tolower(stringutils::trim(dbname));
			if (sx.empty() || sid.empty()) {
				throw info_exception{ U("Invalid parameter(s).") };
			}
			std::shared_ptr<string_t> name = std::make_shared<string_t>(sx);
			std::shared_ptr<string_t> si = std::make_shared<string_t>(sid);
			return std::async([this, name, si]()->string_t {
				string_t bRet{};
				string_t suri{ STRING_SLASH };
				suri += *name;
				suri += STRING_SLASH + url_encode(*si);
				dataserviceuri uri{ suri };
				info_response_ptr rsp = m_client.head(uri).get();
				info_response *pRsp = rsp.get();
				assert(pRsp != nullptr);
				if (!pRsp->has_error()) {
					const std::map<string_t, string_t> &oMap = pRsp->headers;
					string_t key(U("ETag"));
					auto it = oMap.find(key);
					if (it != oMap.end()) {
						string_t s = (*it).second;
						size_t n = s.length();
						if (n > 2) {
							bRet = s.substr(1, n - 2);
						}
					}
				}
				return (bRet);
			});
		}//get_document_version_async
		////////////////////////////////
	}// namespace couchdb
}// namespace info
