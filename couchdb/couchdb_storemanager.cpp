#include "couchdb_storemanager.h"
#include "server_info.h"
#include "index_response.h"
/////////////////////////////
#include <info_document.h>
#include <stringutils.h>
#include <query_filter.h>
/////////////////////////////
namespace info {
	namespace couchdb {
		////////////////////////////////
		static const string_t STRING_SLASH(U("/"));
		static const string_t URI_ALL_DBS(U("/_all_dbs"));
		static const string_t URI_UUIDS(U("/_uuids"));
		static const string_t QUERY_COUNT(U("count"));
		static const string_t KEY_UUIDS(U("uuids"));
		static const string_t KEY_OK(U("ok"));
		static const string_t STRING_UNDERSCORE(U("_"));
		static const string_t QUERY_REV(U("rev"));
		static const string_t QUERY_ETAG(U("ETag"));
		static const string_t KEY_FIELDS(U("fields"));
		static const string_t KEY_INDEX(U("index"));
		static const string_t KEY_NAME(U("name"));
		static const string_t KEY_DDOC(U("ddoc"));
		static const string_t KEY_TYPE(U("type"));
		static const string_t URI_INDEX(U("/_index"));
		static const string_t QUERY_ATTACHMENTS(U("attachments"));
		static const string_t STRING_TRUE(U("true"));
		static const string_t URI_FIND(U("/_find"));
		static const string_t KEY_DOCS(U("docs"));
		static const string_t URI_BULK_DOCS(U("/_bulk_docs"));
		/////////////////////////////////
		static const string_t ERROR_COUCHDB_VERSION(U("Invalid CoudchDB version."));
		static const string_t ERROR_DATABASE_NAME(U("Invalid database name."));
		static const string_t ERROR_DOCUMENT_ID(U("Invalid document id."));
		static const string_t ERROR_DOCUMENT_REV(U("Invalid document revision."));
		static const string_t ERROR_DOCUMENT_NOT_PERSISTED(U("Invalid document id and (or) revision."));
		static const string_t ERROR_INVALID_ATTACHMENT(U("Invalid attachment."));
		static const string_t ERROR_INVALID_ATTACHMENT_NAME(U("Invalid attachment name."));
		/////////////////////////////
		couchdb_storemanager::couchdb_storemanager(http_client &client, const databasename &name /*= databasename{}*/) :
			m_client(client)
		{
			if (!name.empty()) {
				string_t sx = stringutils::tolower(stringutils::trim(name));
				if (sx.empty()) {
					throw info_exception{ ERROR_DATABASE_NAME };
				}
				m_dbname = sx;
			}
			post_create();
		}
		couchdb_storemanager::~couchdb_storemanager()
		{
		}
		bool couchdb_storemanager::can_use_mango(void) const {
			if (!m_version.empty()) {
				string_t s = m_version.substr(0, 1);
				istringstream_t in{ s };
				int n{ 0 };
				in >> n;
				return (n >= 2);
			}
			return (false);
		}
		void couchdb_storemanager::post_create(void) {
			server_info info{};
			get_server_info(info);
			m_version = info.version();
		}
		const string_t & couchdb_storemanager::version(void) const {
			return m_version;
		}
		void couchdb_storemanager::check_databasename(void) {
			if (m_dbname.empty()) {
				throw info_exception{ ERROR_DATABASE_NAME };
			}
		}
		const string_t & couchdb_storemanager::database_name(void) const {
			return (m_dbname);
		}
		void couchdb_storemanager::database_name(const databasename &name) {
			string_t sx = stringutils::tolower(stringutils::trim(name));
			if (sx.empty()) {
				throw info_exception{ ERROR_DATABASE_NAME };
			}
			m_dbname = sx;
		}
		///////////////////////////////////////////
		bool couchdb_storemanager::get_server_info(server_info &info) {
			info.clear();
			bool bRet{ false };
			dataserviceuri uri{ STRING_SLASH };
			info_response_ptr rsp = m_client.get(uri).get();
			info_response *pRsp = rsp.get();
			assert(pRsp != nullptr);
			if (!pRsp->has_error()) {
				info = server_info{ pRsp->jsonval };
				bRet = info.ok();
			}
			return (bRet);
		}//get_server_info
		bool couchdb_storemanager::get_all_databases(std::vector<string_t> &vRet) {
			bool bRet{ false };
			vRet.clear();
			dataserviceuri uri{ URI_ALL_DBS };
			info_response_ptr rsp = m_client.get(uri).get();
			info_response *pRsp = rsp.get();
			assert(pRsp != nullptr);
			if (!pRsp->has_error()) {
				bRet = true;
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
			return (bRet);
		}//get_all_databases
		bool couchdb_storemanager::get_uuids(std::vector<string_t> &vRet, int nCount /*= 1*/) {
			bool bRet{ false };
			vRet.clear();
			dataserviceuri uri{ URI_UUIDS };
			int nc = (nCount > 0) ? nCount : 1;
			query_params query{};
			string_t sx{};
			{
				ostringstream_t os{};
				os << nc;
				sx = os.str();
			}
			query.push_back(std::make_pair(QUERY_COUNT, sx));
			info_response_ptr rsp = m_client.get(uri, query).get();
			info_response *pRsp = rsp.get();
			assert(pRsp != nullptr);
			if (!pRsp->has_error()) {
				bRet = true;
				const any &va = pRsp->jsonval;
				if (va.type() == typeid(infomap)) {
					infomap oMap = INFO_ANY_CAST<infomap>(va);
					auto it = oMap.find(KEY_UUIDS);
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
			return (bRet);
		}//get_uuids
		bool couchdb_storemanager::exists_database(const string_t &dbname) {
			string_t sx = stringutils::tolower(stringutils::trim(dbname));
			if (sx.empty()) {
				throw info_exception{ ERROR_DATABASE_NAME };
			}
			bool bRet{ false };
			string_t suri{ STRING_SLASH };
			suri += sx;
			dataserviceuri uri{ suri };
			info_response_ptr rsp = m_client.head(uri).get();
			info_response *pRsp = rsp.get();
			assert(pRsp != nullptr);
			if (!pRsp->has_error()) {
				bRet = (pRsp->statuscode == 200);
			}// ok
			return (bRet);
		}//exists_database
		bool couchdb_storemanager::create_database(const string_t &dbname) {
			string_t sx = stringutils::tolower(stringutils::trim(dbname));
			if (sx.empty()) {
				throw info_exception{ ERROR_DATABASE_NAME };
			}
			bool bRet{ false };
			string_t suri{ STRING_SLASH };
			suri += sx;
			dataserviceuri uri{ suri };
			any dummy{};
			info_response_ptr rsp = m_client.put(uri, dummy).get();
			info_response *pRsp = rsp.get();
			assert(pRsp != nullptr);
			if (!pRsp->has_error()) {
				const any &va = pRsp->jsonval;
				if (va.type() == typeid(infomap)) {
					infomap oMap = INFO_ANY_CAST<infomap>(va);
					auto it = oMap.find(KEY_OK);
					if (it != oMap.end()) {
						any vx = (*it).second;
						if (vx.type() == typeid(bool)) {
							bRet = INFO_ANY_CAST<bool>(vx);
						}
					}// it
				}// map
			}// ok
			return (bRet);
		}//exists_database
		bool couchdb_storemanager::delete_database(const string_t &dbname) {
			string_t sx = stringutils::tolower(stringutils::trim(dbname));
			if (sx.empty()) {
				throw info_exception{ ERROR_DATABASE_NAME };
			}
			bool bRet{ false };
			string_t suri{ STRING_SLASH };
			suri += sx;
			dataserviceuri uri{ suri };
			info_response_ptr rsp = m_client.del(uri).get();
			info_response *pRsp = rsp.get();
			assert(pRsp != nullptr);
			if (!pRsp->has_error()) {
				const any &va = pRsp->jsonval;
				if (va.type() == typeid(infomap)) {
					infomap oMap = INFO_ANY_CAST<infomap>(va);
					auto it = oMap.find(KEY_OK);
					if (it != oMap.end()) {
						any vx = (*it).second;
						if (vx.type() == typeid(bool)) {
							bRet = INFO_ANY_CAST<bool>(vx);
						}
					}// it
				}// map
			}// ok
			return (bRet);
		}//delete_database
		bool couchdb_storemanager::create_index(const string_t &field,
			index_response &bRet,
			const string_t &ind_name /*= string_t{}*/,
			const string_t &stype /*= U("json")*/,
			const string_t &ddoc /*= string_t{}*/) {
			assert(!field.empty());
			check_databasename();
			infovector vx{};
			vx.push_back(any{ field });
			infomap xMap{};
			xMap[KEY_FIELDS] = any{ vx };
			infomap oMap{};
			oMap[KEY_INDEX] = any{ xMap };
			if (!ind_name.empty()) {
				oMap[KEY_NAME] = any{ ind_name };
			}
			else {
				string_t sx = field + U("_index");
				oMap[KEY_NAME] = any{ sx };
			}
			if (!ddoc.empty()) {
				oMap[KEY_DDOC] = any{ ddoc };
			}
			if (!stype.empty()) {
				oMap[KEY_TYPE] = any{ stype };
			}
			else {
				string_t sx(U("json"));
				oMap[KEY_TYPE] = any{ sx };
			}
			bool xRet{ false };
			any doc{ oMap };
			string_t suri{ STRING_SLASH };
			suri += this->m_dbname;
			suri += URI_INDEX;
			dataserviceuri uri{ suri };
			info_response_ptr rsp = m_client.post(uri, doc).get();
			info_response *pRsp = rsp.get();
			assert(pRsp != nullptr);
			bRet.status_code(pRsp->statuscode);
			if (!pRsp->has_error()) {
				xRet = true;
				const any &va = pRsp->jsonval;
				bRet.set(va);
			}// ok
			return (xRet);
		}//create_index
		bool couchdb_storemanager::get_document_version(const string_t &docid, string_t &res) {
			check_databasename();
			res.clear();
			if (docid.empty()) {
				throw info_exception{ ERROR_DOCUMENT_ID };
			}
			string_t suri{ STRING_SLASH };
			suri += this->m_dbname;
			suri += STRING_SLASH + stringutils::url_encode(docid);
			dataserviceuri uri{ suri };
			info_response_ptr rsp = m_client.head(uri).get();
			info_response *pRsp = rsp.get();
			bool bRet{ false };
			assert(pRsp != nullptr);
			if (!pRsp->has_error()) {
				const std::map<string_t, string_t> &oMap = pRsp->headers;
				auto it = oMap.find(QUERY_ETAG);
				if (it != oMap.end()) {
					string_t s = (*it).second;
					size_t n = s.length();
					if (n > 2) {
						res = s.substr(1, n - 2);
					}
				}
			}
			return (!res.empty());
		}
		bool couchdb_storemanager::get_document_by_id(const string_t &sid, info_document &doc) {
			bool bRet{ false };
			check_databasename();
			doc.clear();
			string_t suri{ STRING_SLASH };
			suri += this->m_dbname;
			suri += STRING_SLASH + stringutils::url_encode(sid);
			dataserviceuri uri{ suri };
			info_response_ptr rsp = m_client.get(uri).get();
			info_response *pRsp = rsp.get();
			assert(pRsp != nullptr);
			if (!pRsp->has_error()) {
				bRet = true;
				any va = pRsp->jsonval;
				doc.set(va);
				doc.check_blobs_urls(get_base_url());
			}
			return (bRet);
		}
		bool couchdb_storemanager::delete_document(const info_document &doc,
			update_response &res) {
			res.clear();
			bool bRet{ false };
			check_databasename();
			if (!doc.is_persisted()) {
				return (false);
			}
			string_t sid = doc.id();
			string_t srev{};
			if (!get_document_version(sid, srev)) {
				return (false);
			}
			string_t suri{ STRING_SLASH };
			suri += this->m_dbname;
			suri += STRING_SLASH + stringutils::url_encode(sid);
			query_params query{};
			query.push_back(std::make_pair(QUERY_REV, srev));
			dataserviceuri uri{ suri };
			info_response_ptr rsp = m_client.del(uri, query).get();
			info_response *pRsp = rsp.get();
			assert(pRsp != nullptr);
			res.status_code(pRsp->statuscode);
			if (!pRsp->has_error()) {
				bRet = true;
				const any &va = pRsp->jsonval;
				res.set(va);
			}// ok
			return (bRet);
		}//delete_document_async
		bool couchdb_storemanager::create_document(info_document &doc, update_response &res) {
			res.clear();
			bool bRet{ false };
			check_databasename();
			string_t suri{ STRING_SLASH };
			suri += this->m_dbname;
			dataserviceuri uri{ suri };
			any vdoc{ doc.get() };
			info_response_ptr rsp = m_client.post(uri, vdoc).get();
			info_response *pRsp = rsp.get();
			assert(pRsp != nullptr);
			res.status_code(pRsp->statuscode);
			if (!pRsp->has_error()) {
				bRet = true;
				const any &va = pRsp->jsonval;
				res.set(va);
			}// ok
			return (bRet);
		}//create_document_async
		bool couchdb_storemanager::update_document(info_document &doc, update_response &res) {
			res.clear();
			bool bRet{ false };
			check_databasename();
			if (!doc.is_persisted()) {
				throw info_exception{ ERROR_DOCUMENT_NOT_PERSISTED };
			}
			string_t sid = doc.id();
			string_t srev = doc.version();
			string_t suri{ STRING_SLASH };
			suri += this->m_dbname;
			suri += STRING_SLASH + stringutils::url_encode(sid);
			query_params query{};
			query.push_back(std::make_pair(QUERY_REV, srev));
			dataserviceuri uri{ suri };
			any vdoc{ doc.get() };
			info_response_ptr rsp = m_client.put(uri, vdoc, query).get();
			info_response *pRsp = rsp.get();
			assert(pRsp != nullptr);
			res.status_code(pRsp->statuscode);
			if (!pRsp->has_error()) {
				bRet = true;
				const any &va = pRsp->jsonval;
				res.set(va);
			}// ok
			return (bRet);
		}//update_document
		bool couchdb_storemanager::maintains_document(info_document &doc,
			update_response &res) {
			if (!doc.is_storable()) {
				return (false);
			}
			bool bRet{ false };
			string_t sempty{};
			string_t docid = doc.id();
			string_t srev{};
			if (docid.empty()) {
				doc.id(sempty);
				doc.version(sempty);
			}
			else {
				get_document_version(docid, srev);
				if (srev.empty()) {
					doc.version(sempty);
				}
				else {
					info_document old{};
					if (get_document_by_id(docid, old)) {
						infomap cur{};
						const infomap &src = doc.get_map();
						for (auto it = src.begin(); it != src.end(); ++it) {
							string_t key = (*it).first;
							if (key.find_first_of(STRING_UNDERSCORE) != 0) {
								cur[key] = (*it).second;
							}
						}// it
						const infomap &oldMap = old.get_map();
						for (auto it = oldMap.begin(); it != oldMap.end(); ++it) {
							string_t key = (*it).first;
							if (key.find_first_of(STRING_UNDERSCORE) == 0) {
								cur[key] = (*it).second;
							}
						}// it
						info_document  dCur{ any{cur} };
						return update_document(dCur, res);
					}// not empty
				}
			}
			infomap cur{};
			const infomap &src = doc.get_map();
			for (auto it = src.begin(); it != src.end(); ++it) {
				string_t key = (*it).first;
				if (key.find_first_of(STRING_UNDERSCORE) != 0) {
					cur[key] = (*it).second;
				}
				else if (key == info_document::KEY_ID) {
					cur[key] = (*it).second;
				}
			}// it
			info_document dCur{ any{cur} };
			return create_document(dCur, res);
		}//maintains_document_async
		bool couchdb_storemanager::find_documents_count(const query_filter &filter, int &nRes) {
			if (!can_use_mango()) {
				throw info_exception(ERROR_COUCHDB_VERSION);
			}
			check_databasename();
			query_filter oFilter{ filter };
			oFilter.clear_sort();
			oFilter.clear_projection();
			oFilter.set_skip(0);
			oFilter.set_limit(std::numeric_limits<int>::max());
			oFilter.add_projection_field(info_document::KEY_ID);
			nRes = 0;
			bool bRet{ false };
			string_t suri{ STRING_SLASH };
			suri += this->m_dbname + URI_FIND;
			dataserviceuri uri{ suri };
			any a = oFilter.get();
			info_response_ptr rsp = m_client.post(uri, a).get();
			info_response *pRsp = rsp.get();
			assert(pRsp != nullptr);
			if (!pRsp->has_error()) {
				const any &va = pRsp->jsonval;
				if (va.type() == typeid(infomap)) {
					infomap oMap = INFO_ANY_CAST<infomap>(va);
					auto it = oMap.find(KEY_DOCS);
					if (it != oMap.end()) {
						any &vx = (*it).second;
						if (vx.type() == typeid(infovector)) {
							infovector vv = INFO_ANY_CAST<infovector>(vx);
							nRes = static_cast<int>(vv.size());
							bRet = true;
						}// vector
					}// docs
				}// map
			}// ok
			return (bRet);
		}// find_documents_count
		bool couchdb_storemanager::find_documents(const query_filter &filter, std::vector<info_document> &vec) {
			if (!can_use_mango()) {
				throw info_exception(ERROR_COUCHDB_VERSION);
			}
			vec.clear();
			check_databasename();
			bool bRet{ false };
			string_t suri{ STRING_SLASH };
			suri += this->m_dbname + URI_FIND;
			dataserviceuri uri{ suri };
			any a = filter.get();
			info_response_ptr rsp = m_client.post(uri, a).get();
			info_response *pRsp = rsp.get();
			assert(pRsp != nullptr);
			if (!pRsp->has_error()) {
				const any &va = pRsp->jsonval;
				if (va.type() == typeid(infomap)) {
					infomap oMap = INFO_ANY_CAST<infomap>(va);
					auto it = oMap.find(KEY_DOCS);
					if (it != oMap.end()) {
						any &vx = (*it).second;
						if (vx.type() == typeid(infovector)) {
							bRet = true;
							infovector vv = INFO_ANY_CAST<infovector>(vx);
							for (auto jt = vv.begin(); jt != vv.end(); ++jt) {
								info_document x{ *jt };
								x.check_blobs_urls(get_base_url());
								vec.push_back(x);
							}// jt
						}// vector
					}// docs
				}// map
			}// ok
			return (bRet);
		}//find_documents
		bool couchdb_storemanager::maintains_documents(const std::vector<info_document> &vec,
			std::vector<update_response> &vecRes) {
			bool bRet{ false };
			vecRes.clear();
			check_databasename();
			string_t suri{ STRING_SLASH };
			suri += this->m_dbname + URI_BULK_DOCS;
			dataserviceuri uri{ suri };
			infovector vx{};
			for (auto it = vec.begin(); it != vec.end(); ++it) {
				const info_document &doc = *it;
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
					bRet = true;
					infovector vxx = INFO_ANY_CAST<infovector>(va);
					for (auto it = vxx.begin(); it != vxx.end(); ++it) {
						update_response r{ *it };
						vecRes.push_back(r);
					}// it
				}// vector
			}// ok
			return (bRet);
		}//maintains_documents_async
		//////////////////////////////////////////
		string_t couchdb_storemanager::form_attachment_url(const string_t &docid, const string_t &name) {
			string_t sx = (const string_t &)m_client.get_serverurl();
			string_t s0 = sx + STRING_SLASH;
			s0 += this->m_dbname + STRING_SLASH + stringutils::url_encode(docid) + STRING_SLASH + stringutils::url_encode(name);
			return (s0);
		}//form_attachment_url
		bool couchdb_storemanager::read_attachment(const string_t &docid,
			const string_t &name, blob_data &blob) {
			if (name.empty()) {
				throw info_exception{ ERROR_INVALID_ATTACHMENT_NAME };
			}
			if (docid.empty()) {
				throw info_exception{ ERROR_DOCUMENT_ID };
			}
			string_t srev{};
			if (!get_document_version(docid, srev)) {
				throw info_exception{ ERROR_DOCUMENT_REV };
			}
			bool bRet{ false };
			string_t suri{ STRING_SLASH };
			suri += this->m_dbname;
			suri += STRING_SLASH + stringutils::url_encode(docid) + STRING_SLASH + stringutils::url_encode(name);
			dataserviceuri uri{ suri };
			std::shared_ptr<blob_data> oRet = m_client.read_blob(uri).get();
			if (oRet.get() != nullptr) {
				bRet = true;
				oRet->id(docid);
				oRet->name(name);
				string_t surl = form_attachment_url(docid, name);
				oRet->url(surl);
				blob = *oRet;
			}
			return (bRet);
		}
		bool couchdb_storemanager::update_attachment(blob_data &blob, update_response &res) {
			if (!blob.is_storable()) {
				throw info_exception{ ERROR_INVALID_ATTACHMENT };
			}
			string_t docid = blob.id();
			string_t srev{};
			if (!get_document_version(docid, srev)) {
				throw info_exception{ ERROR_DOCUMENT_REV };
			}
			bool bRet{ false };
			string_t name = blob.name();
			string_t suri{ STRING_SLASH };
			suri += this->m_dbname;
			suri += STRING_SLASH + stringutils::url_encode(docid) + STRING_SLASH + stringutils::url_encode(name);
			dataserviceuri uri{ suri };
			query_params query{};
			query.push_back(std::make_pair(QUERY_REV, srev));
			info_response_ptr rsp = m_client.maintains_blob(uri, blob, query).get();
			info_response *pRsp = rsp.get();
			assert(pRsp != nullptr);
			res.status_code(pRsp->statuscode);
			if (!pRsp->has_error()) {
				bRet = true;
				const any &va = pRsp->jsonval;
				res.set(va);
			}// ok
			return (bRet);
		}
		bool couchdb_storemanager::delete_attachment(blob_data &blob, update_response &res) {
			string_t xname = blob.name();
			if (xname.empty()) {
				throw info_exception{ ERROR_INVALID_ATTACHMENT_NAME };
			}
			string_t docid = blob.id();
			if (docid.empty()) {
				throw info_exception{ ERROR_DOCUMENT_ID };
			}
			string_t srev{};
			if (!get_document_version(docid, srev)) {
				throw info_exception{ ERROR_DOCUMENT_REV };
			}
			bool bRet{ false };
			string_t suri{ STRING_SLASH };
			suri += this->m_dbname;
			suri += STRING_SLASH + stringutils::url_encode(docid) + STRING_SLASH + stringutils::url_encode(xname);
			dataserviceuri uri{ suri };
			query_params query{};
			query.push_back(std::make_pair(QUERY_REV, srev));
			info_response_ptr rsp = m_client.del(uri, query).get();
			info_response *pRsp = rsp.get();
			assert(pRsp != nullptr);
			res.status_code(pRsp->statuscode);
			if (!pRsp->has_error()) {
				bRet = true;
				const any &va = pRsp->jsonval;
				res.set(va);
			}// ok
			return (bRet);
		}
		/////////////////////////////////////////
		string_t couchdb_storemanager::get_base_url(void) {
			string_t sx = (const string_t &)m_client.get_serverurl();
			string_t s0 = sx + STRING_SLASH;
			s0 += this->m_dbname + STRING_SLASH;
			return (s0);
		}//get_base_url
		std::future<bool> couchdb_storemanager::check_field_index_async(const string_t &fieldname) {
			std::shared_ptr<string_t> field = std::make_shared<string_t>(fieldname);
			return std::async([this, field]()->bool {
				bool bRet{ false };
				index_response r{};
				if (this->create_index(*field, r)) {
					bRet = r.ok();
				}
				return (bRet);
			});
		}//check_field_index_async
		std::future<int> couchdb_storemanager::find_documents_count_async(const query_filter &filter) {
			std::shared_ptr<query_filter> pf = std::make_shared<query_filter>(filter);
			return std::async([this, pf]()->int {
				int nRet{ 0 };
				this->find_documents_count(*pf, nRet);
				return (nRet);
			});
		}// find_documents_count_async
		std::future<std::vector<infomap>> couchdb_storemanager::query_documents_async(const query_filter &filter) {
			std::shared_ptr<query_filter> pf = std::make_shared<query_filter>(filter);
			return std::async([this, pf]()->std::vector<infomap> {
				std::vector<infomap> oRet{};
				std::vector<info_document> vec{};
				if (this->find_documents(*pf, vec)) {
					for (auto doc : vec) {
						infomap m = doc.get_map();
						oRet.push_back(m);
					}// doc
				}// ok
				return (oRet);
			});
		}//query_documents_async
		std::future<infomap> couchdb_storemanager::find_document_async(const infomap &parameters) {
			std::shared_ptr<infomap> pv = std::make_shared<infomap>(parameters);
			return std::async([this, pv]()->infomap {
				infomap oRet{};
				auto jt = pv->find(info_document::KEY_ID);
				if (jt != pv->end()) {
					any va = (*jt).second;
					if (!INFO_ANY_HAS_VALUE(va)) {
						return (oRet);
					}
					if (va.type() != typeid(string_t)) {
						return (oRet);
					}
					string_t docid = INFO_ANY_CAST<string_t>(va);
					info_document doc{};
					if (get_document_by_id(docid, doc)) {
						oRet = doc.get_map();
					}
					return (oRet);
				}
				query_filter filter{};
				for (auto it = pv->begin(); it != pv->end(); ++it) {
					any va = (*it).second;
					if (INFO_ANY_HAS_VALUE(va)) {
						string_t key = (*it).first;
						filter.add_equals(key, va);
					}
					if (!filter.empty()) {
						filter.set_limit(1);
						filter.set_skip(0);
						std::vector<info_document> vv{};
						if (this->find_documents(filter, vv)) {
							if (!vv.empty()) {
								oRet = (vv[0]).get_map();
							}// vv
						}// vv
					}// filter
				}// it
				return (oRet);
			});
		}//find_document_async
		std::future<infomap> couchdb_storemanager::maintains_document_async(const infomap &doc) {
			std::shared_ptr<infomap> pv = std::make_shared<infomap>(doc);
			return std::async([this, pv]()->infomap {
				any va{ *pv };
				info_document xdoc{ va };
				update_response r{};
				bool bRet{ false };
				if (this->maintains_document(xdoc, r)) {
					string_t docid = r.id();
					string_t srev = r.rev();
					(*pv)[info_document::KEY_ID] = any{ docid };
					(*pv)[info_document::KEY_VERSION] = any{ srev };
				}
				return (*pv);
			});
		}//maintains_document_async
		std::future<bool> couchdb_storemanager::remove_document_async(const infomap &doc) {
			std::shared_ptr<infomap> pv = std::make_shared<infomap>(doc);
			return std::async([this, pv]()->bool {
				any va{ *pv };
				info_document xdoc{ va };
				update_response r{};
				bool bRet{ false };
				if (this->delete_document(xdoc, r)) {
					bRet = r.ok();
				}
				return (bRet);
			});
		}//remove_document_async
		std::future<std::vector<bool>>couchdb_storemanager::maintains_documents_async(const std::vector<infomap> &docs) {
			std::shared_ptr<std::vector<infomap>> pv = std::make_shared<std::vector<infomap>>(docs);
			return std::async([this, pv]()->std::vector<bool> {
				std::vector<bool> vRet{};
				std::vector<info_document> vec{};
				for (auto it = pv->begin(); it != pv->end(); ++it) {
					any va{ *it };
					info_document x{ va };
					vec.push_back(x);
				}// it
				std::vector<update_response> rr{};
				if (this->maintains_documents(vec, rr)) {
					for (auto jt = rr.begin(); jt != rr.end(); ++jt) {
						bool b = (*jt).ok();
						vRet.push_back(b);
					}// jt
				}//
				return (vRet);
			});
		}//maintains_documents_async
		std::future<bool> couchdb_storemanager::maintains_blob_async(const blob_data &blob) {
			std::shared_ptr<blob_data> pv = std::make_shared<blob_data>(blob);
			return std::async([this, pv]()->bool {
				update_response r{};
				bool bRet{ false };
				if (this->update_attachment(*pv, r)) {
					bRet = r.ok();
				}
				return (bRet);
			});
		}//maintains_blob_async
		std::future<blob_data> couchdb_storemanager::read_blob_async(const blob_data &blob) {
			std::shared_ptr<blob_data> pv = std::make_shared<blob_data>(blob);
			return std::async([this, pv]()->blob_data {
				blob_data r{};
				this->read_attachment(pv->id(), pv->name(), r);
				return (r);
			});
		}//read_blob_async
		std::future<bool> couchdb_storemanager::remove_blob_async(const blob_data &blob) {
			std::shared_ptr<blob_data> pv = std::make_shared<blob_data>(blob);
			return std::async([this, pv]()->bool {
				update_response r{};
				bool bRet{ false };
				if (this->delete_attachment(*pv, r)) {
					bRet = r.ok();
				}
				return (bRet);
			});
		}// remove_blob_async
		/////////////////////////////////////////////
	}// namespace couchdb
}// namespace info