#include "couchdb_doc.h"
/////////////////////////////
namespace info {
	namespace couchdb {
		/////////////////////////
		couchdb_doc::couchdb_doc()
		{
		}
		couchdb_doc::couchdb_doc(const any &va) : infobaseobject (va){

		}
		couchdb_doc::couchdb_doc(const couchdb_doc &other):infobaseobject(other) {

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
		bool couchdb_doc::is_deleted(void) const {
			bool bRet{ false };
			get_bool_property(U("_deleted"), bRet);
			return (bRet);
		}
		void couchdb_doc::is_deleted(bool b) {
			set_bool_property(U("_deleted"), b);
		}
		size_t couchdb_doc::attachments(std::vector<attachment_info> &vec) const {
			vec.clear();
			infomap oMap{};
			get_obj_property(U("_attachments"), oMap);
			for (auto it = oMap.begin(); it != oMap.end(); ++it) {
				string_t key = (*it).first;
				any va = (*it).second;
				attachment_info info{ va };
				info.name(key);
				vec.push_back(info);
			}// it
			return (vec.size());
		}
		/////////////////////////
	} // namespace couchdb
}// namespace info
