#include "server_info.h"
////////////////////////////
namespace info {
	namespace couchdb {
		/////////////////////////////
		server_info::server_info()
		{
		}
		server_info::server_info(const any &val) :infobaseobject(val) {

		}
		server_info::server_info(const server_info &other) : infobaseobject(other) {

		}
		server_info & server_info::operator=(const server_info &other) {
			if (this != &other) {
				infobaseobject::operator=(other);
			}
			return (*this);
		}
		server_info::~server_info()
		{
		}
		bool server_info::ok(void) const {
			return (welcome_string() == U("Welcome"));
		}
		string_t server_info::welcome_string(void) const {
			string_t s{};
			get_string_property(U("couchdb"), s);
			return (s);
		}
		string_t server_info::uuid(void) const {
			string_t s{};
			get_string_property(U("uuid"), s);
			return (s);
		}
		string_t server_info::version(void) const {
			string_t s{};
			get_string_property(U("version"), s);
			return (s);
		}
		void server_info::features(std::vector<string_t> &vv) const {
			get_stringvector_property(U("features"), vv);
		}
		string_t server_info::vendor_name(void) const {
			string_t sRet{};
			infomap oMap{};
			if (get_obj_property(U("vendor"), oMap)) {
				auto it = oMap.find(U("name"));
				if (it != oMap.end()) {
					any va = (*it).second;
					if (va.type() == typeid(string_t)) {
						sRet = INFO_ANY_CAST<string_t>(va);
					}
				}
			}
			return (sRet);
		}
		string_t server_info::vendor_version(void) const {
			string_t sRet{};
			infomap oMap{};
			if (get_obj_property(U("vendor"), oMap)) {
				auto it = oMap.find(U("version"));
				if (it != oMap.end()) {
					any va = (*it).second;
					if (va.type() == typeid(string_t)) {
						sRet = INFO_ANY_CAST<string_t>(va);
					}
				}
			}
			return (sRet);
		}
		////////////////////
	}// namespace couchdb
}// namespace info
