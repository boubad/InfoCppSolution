#include "couchdb_doc.h"
#include <stringutils.h>
/////////////////////////////
namespace info {
	namespace couchdb {
		///////////////////////////
		static const string_t KEY_ATTACHMENTS(U("_attachments"));
		static const string_t KEY_DELETED(U("_deleted"));
		/////////////////////////
		couchdb_doc::couchdb_doc()
		{
		}
		couchdb_doc::couchdb_doc(const any &va) : infobaseobject(va) {

		}
		couchdb_doc::couchdb_doc(const couchdb_doc &other) : infobaseobject(other) {

		}
		couchdb_doc & couchdb_doc::operator=(const couchdb_doc &other) {
			if (this != &other) {
				infobaseobject::operator=(other);
			}
			return (*this);
		}

		couchdb_doc::~couchdb_doc()
		{
		}
		string_t  couchdb_doc::id(void) const {
			string_t s{};
			obj_id(s);
			return (s);
		}
		void couchdb_doc::id(const string_t &s) {
			set_string_property(KEY_ID, s);
		}
		string_t couchdb_doc::version(void) const {
			string_t s{};
			obj_version(s);
			return (s);
		}
		void couchdb_doc::version(const string_t &s) {
			set_string_property(KEY_VERSION, s);
		}
		bool couchdb_doc::is_persisted(void) const {
			string_t s1 = id();
			string_t s2 = version();
			return ((!s1.empty()) && (!s2.empty()));
		}
		couchdb_doc couchdb_doc::get_deleted_doc(void) const {
			couchdb_doc doc{};
			doc.id(this->id());
			doc.version(this->version());
			doc.is_deleted(true);
			return (doc);
		}
		bool couchdb_doc::is_deleted(void) const {
			bool bRet{ false };
			get_bool_property(KEY_DELETED, bRet);
			return (bRet);
		}
		void couchdb_doc::is_deleted(bool b) {
			set_bool_property(KEY_DELETED, b);
		}
		size_t couchdb_doc::attachments(std::vector<attachment_info> &vec) const {
			vec.clear();
			infomap oMap{};
			get_obj_property(KEY_ATTACHMENTS, oMap);
			for (auto it = oMap.begin(); it != oMap.end(); ++it) {
				string_t key = (*it).first;
				any va = (*it).second;
				attachment_info info{ va };
				info.name(key);
				vec.push_back(info);
			}// it
			return (vec.size());
		}
		size_t couchdb_doc::attachments_names(std::vector<string_t> &vec) const {
			vec.clear();
			infomap oMap{};
			get_obj_property(KEY_ATTACHMENTS, oMap);
			for (auto it = oMap.begin(); it != oMap.end(); ++it) {
				string_t key = (*it).first;
				vec.push_back(key);
			}// it
			return (vec.size());
		}
		void couchdb_doc::update_attachments_urls(const string_t sbase) {
			infomap oMap{};
			if (get_obj_property(KEY_ATTACHMENTS, oMap)) {
				string_t sid = sbase + U("/") +  stringutils::url_encode(this->id()) + U("/");
				infomap xMap{};
				for (auto it = oMap.begin(); it != oMap.end(); ++it) {
					any a = (*it).second;
					string_t key = (*it).first;
					if (a.type() == typeid(infomap)) {
						infomap mx = INFO_ANY_CAST<infomap>(a);
						mx[attachment_info::KEY_NAME] = any{ key };
						string_t sz = sid + stringutils::url_encode(key);
						mx[attachment_info::KEY_URL] = any{ sz };
						a = any{ mx };
					}
					xMap[key] = a;
				}// it
				set_obj_property(KEY_ATTACHMENTS, xMap);
			}//ok
		}
		void couchdb_doc::clean_attachments(bool bAll /*= false*/) {
			if (bAll) {
				erase_property(KEY_ATTACHMENTS);
			}
			else {
				infomap oMap{};
				if (get_obj_property(KEY_ATTACHMENTS, oMap)) {
					infomap xMap{};
					for (auto it = oMap.begin(); it != oMap.end(); ++it) {
						string_t key = (*it).first;
						any  a = (*it).second;
						if (a.type() == typeid(infomap)) {
							infomap mx = INFO_ANY_CAST<infomap>(a);
							{
								auto jt = mx.find(attachment_info::KEY_NAME);
								if (jt != mx.end()) {
									mx.erase(jt);
								}
							}
							{
								auto jt = mx.find(attachment_info::KEY_URL);
								if (jt != mx.end()) {
									mx.erase(jt);
								}
							}
							a = any{ mx };
						}// map
						xMap[key] = a;
					}// it
					set_obj_property(KEY_ATTACHMENTS, xMap);
				}// attachments
			}
		}
		/////////////////////////
	} // namespace couchdb
}// namespace info
