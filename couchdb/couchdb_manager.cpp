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
		couchdb_manager::couchdb_manager(http_client &client, const databasename &name /*= databasename{}*/) :
			m_client(client)
		{
			if (!name.empty()) {
				string_t sx = stringutils::tolower(stringutils::trim(name));
				if (sx.empty()) {
					throw info_exception{ U("Invalid database name.") };
				}
				m_dbname = sx;
			}
		}
		couchdb_manager::~couchdb_manager()
		{
		}
		void couchdb_manager::check_databasename(void) {
			if (m_dbname.empty()) {
				throw info_exception{ U("Invalid database name.") };
			}
		}
		const string_t & couchdb_manager::database_name(void) const {
			return (m_dbname);
		}
		void couchdb_manager::database_name(const databasename &name) {
			string_t sx = stringutils::tolower(stringutils::trim(name));
			if (sx.empty()) {
				throw info_exception{ U("Invalid database name.") };
			}
			m_dbname = sx;
		}
		string_t couchdb_manager::form_attachment_url(const string_t &docid, const string_t &name) {
			string_t sx = (const string_t &)m_client.get_serverurl();
			string_t s0 = sx + STRING_SLASH;
			s0 += this->m_dbname + STRING_SLASH + url_encode(docid) + STRING_SLASH + url_encode(name);
			return (s0);
		}//form_attachment_url
		void couchdb_manager::check_attachments_url(couchdb_doc &doc) {
			string_t docid = doc.id();
			string_t sx = (const string_t &)m_client.get_serverurl();
			string_t sbase = sx + STRING_SLASH + this->m_dbname;
			doc.update_attachments_urls(sbase);
		}//check_attachments_url
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
		std::future<update_response> couchdb_manager::create_document_async(const couchdb_doc &doc) {
			std::shared_ptr<couchdb_doc> pa = std::make_shared<couchdb_doc>(doc);
			return std::async([this, pa]()->update_response {
				check_databasename();
				couchdb_doc *pDoc = pa.get();
				update_response bRet{};
				string_t suri{ STRING_SLASH };
				suri += this->m_dbname;
				dataserviceuri uri{ suri };
				any vdoc{ pDoc->get() };
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
		std::future<update_response> couchdb_manager::delete_document_async(const couchdb_doc &doc) {
			std::shared_ptr<couchdb_doc> pa = std::make_shared<couchdb_doc>(doc);
			return std::async([this, pa]()->update_response {
				check_databasename();
				couchdb_doc *pDoc = pa.get();
				string_t sid{}, srev{};
				pDoc->obj_id(sid);
				pDoc->obj_version(srev);
				if (sid.empty() || srev.empty()) {
					throw info_exception{ U("Invalid document.") };
				}
				update_response bRet{};
				string_t suri{ STRING_SLASH };
				suri += this->m_dbname;
				suri += STRING_SLASH + url_encode(sid);
				query_params query{};
				query.push_back(std::make_pair(U("rev"), srev));
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
		std::future<string_t> couchdb_manager::get_document_version_async(const string_t &docid) {
			std::shared_ptr<string_t> si = std::make_shared<string_t>(docid);
			return std::async([this, si]()->string_t {
				check_databasename();
				string_t sid = *si;
				if (sid.empty()) {
					throw info_exception{ U("Invalid document id.") };
				}
				string_t bRet{};
				string_t suri{ STRING_SLASH };
				suri += this->m_dbname;
				suri += STRING_SLASH + url_encode(sid);
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
		std::future<index_response> couchdb_manager::create_index_async(const string_t &field,
			const string_t &ind_name /*= string_t{}*/,
			const string_t &stype /*= U("json")*/,
			const string_t &ddoc /*= string_t{}*/) {
			infovector vx{};
			vx.push_back( any{field} );
			infomap xMap{};
			xMap[U("fields")] = any{ vx };
			infomap oMap{};
			oMap[U("index")] = any{ xMap };
			if (!ind_name.empty()) {
				oMap[U("name")] = any{ ind_name };
			}
			if (!ddoc.empty()) {
				oMap[U("ddoc")] = any{ ddoc };
			}
			if (!stype.empty()) {
				oMap[U("type")] = any{ stype };
			}
			any doc{ oMap };
			std::shared_ptr<any> pa = std::make_shared<any>(doc);
			return std::async([this, pa]()->index_response {
				check_databasename();
				index_response bRet{};
				string_t suri{ STRING_SLASH };
				suri += this->m_dbname;
				suri += STRING_SLASH + U("_index");
				dataserviceuri uri{ suri };
				info_response_ptr rsp = m_client.post(uri, *pa).get();
				info_response *pRsp = rsp.get();
				assert(pRsp != nullptr);
				bRet.status_code(pRsp->statuscode);
				if (!pRsp->has_error()) {
					const any &va = pRsp->jsonval;
					bRet.set(va);
				}// ok
				return (bRet);
			});
		}//create_index_async
		std::future<couchdb_doc> couchdb_manager::get_document_by_id_async(const string_t &sid, bool bAttach /*= true*/) {
			std::shared_ptr<string_t> si = std::make_shared<string_t>(sid);
			return std::async([this, si,bAttach]()->couchdb_doc {
				check_databasename();
				couchdb_doc bRet{};
				string_t suri{ STRING_SLASH };
				suri += this->m_dbname;
				suri += STRING_SLASH + url_encode(*si);
				dataserviceuri uri{ suri };
				query_params query{};
				if (bAttach) {
					query.push_back(std::make_pair(U("attachments"), U("true")));
				}
				info_response_ptr rsp = m_client.get(uri,query).get();
				info_response *pRsp = rsp.get();
				assert(pRsp != nullptr);
				if (!pRsp->has_error()) {
					any va = pRsp->jsonval;
					bRet.set(va);
					this->check_attachments_url(bRet);
				}
				return (bRet);
			});
		}//get_document_by_id_as
		std::future<update_response> couchdb_manager::update_document_async(couchdb_doc &doc) {
			std::shared_ptr<couchdb_doc> pa = std::make_shared<couchdb_doc>(doc);
			return std::async([this, pa]()->update_response {
				check_databasename();
				string_t sid{}, srev{};
				couchdb_doc *pDoc = pa.get();
				if (!pDoc->is_persisted()) {
					throw info_exception{ U("Invalid document.") };
				}
				pDoc->clean_attachments();
				update_response bRet{};
				string_t suri{ STRING_SLASH };
				suri += this->m_dbname;
				suri += STRING_SLASH + url_encode(sid);
				query_params query{};
				query.push_back(std::make_pair(U("rev"), srev));
				dataserviceuri uri{ suri };
				any vdoc{ pDoc->get() };
				info_response_ptr rsp = m_client.put(uri, vdoc,query).get();
				info_response *pRsp = rsp.get();
				assert(pRsp != nullptr);
				bRet.status_code(pRsp->statuscode);
				if (!pRsp->has_error()) {
					const any &va = pRsp->jsonval;
					bRet.set(va);
				}// ok
				return (bRet);
			});
		}//update_document_async
		std::future<int>  couchdb_manager::find_documents_count_async(const query_filter &filter) {
			std::shared_ptr<query_filter> pf = std::make_shared<query_filter>(filter);
			return std::async([this, pf]()->int {
				check_databasename();
				query_filter oFilter{ *pf };
				oFilter.clear_sort();
				oFilter.clear_projection();
				oFilter.set_skip(0);
				oFilter.set_limit(INT_MAX);
				string_t ff(U("_id"));
				oFilter.add_projection_field(ff);
				int bRet{-1};
				string_t suri{ STRING_SLASH };
				suri += this->m_dbname + U("/_find");
				dataserviceuri uri{ suri };
				any a = oFilter.get();
				info_response_ptr rsp = m_client.post(uri, a).get();
				info_response *pRsp = rsp.get();
				assert(pRsp != nullptr);
				if (!pRsp->has_error()) {
					const any &va = pRsp->jsonval;
					if (va.type() == typeid(infomap)) {
						infomap oMap = INFO_ANY_CAST<infomap>(va);
						auto it = oMap.find(U("docs"));
						if (it != oMap.end()) {
							any &vx = (*it).second;
							if (vx.type() == typeid(infovector)) {
								infovector vv = INFO_ANY_CAST<infovector>(vx);
								bRet = static_cast<int>(vv.size());
							}// vector
						}// docs
					}// map
				}// ok
				return (bRet);
			});
		}// find_documents_count_async
		std::future<std::vector<couchdb_doc>> couchdb_manager::find_documents_async(const query_filter &filter) {
			std::shared_ptr<query_filter> pf = std::make_shared<query_filter>(filter);
			return std::async([this, pf]()->std::vector<couchdb_doc> {
				check_databasename();
				std::vector<couchdb_doc> bRet{};
				string_t suri{ STRING_SLASH };
				suri += this->m_dbname + U("/_find");
				dataserviceuri uri{ suri };
				any a = (*pf).get();
				info_response_ptr rsp = m_client.post(uri, a).get();
				info_response *pRsp = rsp.get();
				assert(pRsp != nullptr);
				if (!pRsp->has_error()) {
					const any &va = pRsp->jsonval;
					if (va.type() == typeid(infomap)) {
						infomap oMap = INFO_ANY_CAST<infomap>(va);
						auto it = oMap.find(U("docs"));
						if (it != oMap.end()) {
							any &vx = (*it).second;
							if (vx.type() == typeid(infovector)) {
								infovector vv = INFO_ANY_CAST<infovector>(vx);
								for (auto jt = vv.begin(); jt != vv.end(); ++jt) {
									couchdb_doc x{ *jt };
									bRet.push_back(x);
								}// jt
							}// vector
						}// docs
					}// map
				}// ok
				return (bRet);
			});
		}//find_documents_async
		std::future<std::vector<update_response>> couchdb_manager::maintains_documents_async(const std::vector<couchdb_doc> &vec) {
			std::shared_ptr<std::vector<couchdb_doc>> pf = std::make_shared<std::vector<couchdb_doc>>(vec);
			return std::async([this, pf]()->std::vector<update_response> {
				check_databasename();
				std::vector<update_response> bRet{};
				string_t suri{ STRING_SLASH };
				suri += this->m_dbname + U("/_bulk_docs");
				dataserviceuri uri{ suri };
				infovector vx{};
				for (auto it = pf->begin(); it != pf->end(); ++it) {
					couchdb_doc &doc = (*it);
					doc.clean_attachments();
					vx.push_back(doc.get());
				}
				infomap oMap{};
				oMap[U("docs")] = any{ vx };
				any a{ oMap };
				info_response_ptr rsp = m_client.post(uri, a).get();
				info_response *pRsp = rsp.get();
				assert(pRsp != nullptr);
				if (!pRsp->has_error()) {
					const any &va = pRsp->jsonval;
					if (va.type() == typeid(infovector)) {
						infovector vxx = INFO_ANY_CAST<infovector>(va);
						for (auto it = vxx.begin(); it != vxx.end(); ++it) {
							update_response r{ *it };
							bRet.push_back(r);
						}// it
					}// vector
				}// ok
				return (bRet);
			});
		}//maintains_documents_async
		///////////////////////////////////
		std::future<std::shared_ptr<blob_data>> couchdb_manager::read_document_attachment_async(const couchdb_doc &doc,
			const string_t &name) {
			std::shared_ptr<couchdb_doc> pf = std::make_shared<couchdb_doc>(doc);
			std::shared_ptr<string_t> xname = std::make_shared<string_t>(name);
			return std::async([this, pf, xname]()->std::shared_ptr<blob_data> {
				if (!pf->is_persisted()) {
					throw info_exception{ U("Invalid document.") };
				}
				string_t suri{ STRING_SLASH };
				suri += this->m_dbname;
				suri += STRING_SLASH + url_encode(pf->id()) + STRING_SLASH + url_encode(*xname);
				dataserviceuri uri{ suri };
				std::shared_ptr<blob_data> oRet = m_client.read_blob(uri).get();
				return (oRet);
			});
		}
		std::future<update_response> couchdb_manager::update_document_attachment_async(const couchdb_doc &doc,
			const blob_data &blob) {
			std::shared_ptr<couchdb_doc> pf = std::make_shared<couchdb_doc>(doc);
			std::shared_ptr<blob_data> pv = std::make_shared<blob_data>(blob);
			return std::async([this, pf, pv]()->update_response {
				if (!pf->is_persisted()) {
					throw info_exception{ U("Invalid document.") };
				}
				string_t suri{ STRING_SLASH };
				suri += this->m_dbname;
				suri += STRING_SLASH + url_encode(pf->id()) + STRING_SLASH + url_encode(pv->name());
				dataserviceuri uri{ suri };
				query_params query{};
				string_t srev = pf->version();
				query.push_back(std::make_pair(U("rev"), srev));
				info_response_ptr rsp = m_client.maintains_blob(uri, *pv, query).get();
				info_response *pRsp = rsp.get();
				assert(pRsp != nullptr);
				update_response r{};
				r.status_code(pRsp->statuscode);
				if (!pRsp->has_error()) {
					const any &va = pRsp->jsonval;
					r.set(va);
				}// ok
				return (r);
			});
		}
		std::future<update_response> couchdb_manager::delete_document_attachment_async(const couchdb_doc &doc,
			const string_t &name) {
			std::shared_ptr<couchdb_doc> pf = std::make_shared<couchdb_doc>(doc);
			std::shared_ptr<string_t> xname = std::make_shared<string_t>(name);
			return std::async([this, pf, xname]()->update_response {
				if (!pf->is_persisted()) {
					throw info_exception{ U("Invalid document.") };
				}
				string_t suri{ STRING_SLASH };
				suri += this->m_dbname;
				suri += STRING_SLASH + url_encode(pf->id()) + STRING_SLASH + url_encode(*xname);
				dataserviceuri uri{ suri };
				query_params query{};
				string_t srev = pf->version();
				query.push_back(std::make_pair(U("rev"), srev));
				info_response_ptr rsp = m_client.del(uri, query).get();
				info_response *pRsp = rsp.get();
				assert(pRsp != nullptr);
				update_response r{};
				r.status_code(pRsp->statuscode);
				if (!pRsp->has_error()) {
					const any &va = pRsp->jsonval;
					r.set(va);
				}// ok
				return (r);
			});
		}
		////////////////////////////////
	}// namespace couchdb
}// namespace info
