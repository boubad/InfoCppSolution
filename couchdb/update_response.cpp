#include "update_response.h"
//////////////////////////////////
namespace info {
	namespace couchdb {
		////////////////////////////////////
		update_response::update_response()
		{
		}
		update_response::update_response(const any &val):infobaseobject(val) {

		}
		update_response::update_response(const update_response &other):infobaseobject(other) {

		}
		update_response & update_response::operator=(const update_response &other) {
			if (this != &other) {
				infobaseobject::operator=(other);
			}
			return (*this);
		}

		update_response::~update_response()
		{
		}
		bool update_response::ok(void) const {
			bool bRet{ false };
			get_bool_property(U("ok"), bRet);
			return (bRet);
		}
		int update_response::status_code(void) const {
			int bRet{ 500 };
			get_int_property(U("status"), bRet);
			return (bRet);
		}
		void update_response::status_code(int n) {
			set_int_property(U("status"), n);
		}
		string_t update_response::id(void) const {
			string_t bRet{ };
			get_string_property(U("id"), bRet);
			return (bRet);
		}
		string_t update_response::rev(void) const {
			string_t bRet{};
			get_string_property(U("rev"), bRet);
			return (bRet);
		}
		string_t update_response::error(void) const {
			string_t bRet{};
			get_string_property(U("error"), bRet);
			return (bRet);
		}
		string_t update_response::reason(void) const {
			string_t bRet{};
			get_string_property(U("reason"), bRet);
			return (bRet);
		}
		///////////////////////////
	}// namespace couchdb
}// namespace info
