#include "index_response.h"
/////////////////////////////////
namespace info {
	namespace couchdb {
		/////////////////////////
		index_response::index_response()
		{
		}
		index_response::index_response(const any &va):infobaseobject(va) {

		}
		index_response::index_response(const index_response &other):infobaseobject(other) {

		}
		index_response & index_response::operator=(const index_response &other) {
			if (this != &other) {
				infobaseobject::operator=(other);
			}
			return (*this);
		}

		index_response::~index_response()
		{
		}
		bool index_response::ok(void) const {
			string_t s = result();
			return ((s == U("exists")) || (s == U("created")));
		}
		int index_response::status_code(void) const {
			int nRet{ 500 };
			get_int_property(U("status"),nRet);
			return (nRet);
		}
		void index_response::status_code(int n) {
			get_int_property(U("status"), n);
		}
		string_t index_response::result(void) const {
			string_t s{};
			get_string_property(U("result"), s);
			return (s);
		}
		string_t index_response::index_id(void) const {
			string_t s{};
			get_string_property(U("id"), s);
			return (s);
		}
		string_t index_response::name(void) const {
			string_t s{};
			get_string_property(U("name"), s);
			return (s);
		}
		///////////////////
	}// namespace couchdb
}// namespace info