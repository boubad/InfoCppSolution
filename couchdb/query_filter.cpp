#include "query_filter.h"
#include <stringutils.h>
///////////////////////////////
namespace info {
	namespace couchdb {
		//////////////////////////////////////
		static const string_t KEY_SELECTOR(U("selector"));
		static const string_t KEY_LIMIT(U("limit"));
		static const string_t KEY_SKIP(U("skip"));
		static const string_t KEY_SORT(U("sort"));
		static const string_t KEY_FIELDS(U("fields"));
		static const string_t KEY_USEINDEX(U("use_index"));
		static const string_t KEY_R(U("r"));
		static const string_t KEY_BOOKMARK(U("bookmark"));
		static const string_t KEY_UPDATE(U("update"));
		static const string_t KEY_STABLE(U("stable"));
		//
		static const string_t KEY_ASC(U("asc"));
		static const string_t KEY_DESC(U("desc"));
		////////////////////////////////
		query_filter::query_filter()
		{
		}
		query_filter::query_filter(const query_filter &other):infobaseobject(other) {

		}
		query_filter & query_filter::operator=(const query_filter &other) {
			if (this != &other) {
				infobaseobject::operator=(other);
			}
			return (*this);
		}
		query_filter::~query_filter()
		{
		}
		void query_filter::clear_sort(void) {
			erase_property(U("sort"));
		}
		void query_filter::clear_projection(void) {
			erase_property(U("fields"));
		}
		void query_filter::add_operation(const string_t &field, const string_t &op, any v) {
			infomap xMap{};
			xMap[op] = v;
			infomap oMap{};
			get_obj_property(KEY_SELECTOR, oMap);
			oMap[field] = any{ xMap };
			set_obj_property(KEY_SELECTOR, oMap);
		}// add_operation
		void query_filter::set_limit(int n) {
			int nx = (n > 0) ? n : 25;
			set_int_property(KEY_LIMIT, nx);
		}
		void query_filter::set_skip(int n) {
			int nx = (n >= 0) ? n : 0;
			set_int_property(KEY_SKIP, nx);
		}
		void query_filter::add_useindex(const string_t &s) {
			add_item_toarray(KEY_USEINDEX, s);
		}
		void  query_filter::set_read_quorum(int n) {
			int nx = (n > 0) ? n : 1;
			set_int_property(KEY_R, nx);
		}
		void query_filter::set_bookmark(const string_t &s) {
			set_string_property(KEY_BOOKMARK, s);
		}
		void query_filter::set_update(bool b) {
			set_bool_property(KEY_UPDATE, b);
		}
		void query_filter::set_stable(bool b) {
			set_bool_property(KEY_STABLE, b);
		}
		
		void query_filter::add_projection_field(const string_t &field) {
			add_item_toarray(KEY_FIELDS, field);
		}//add_projection_field
		void query_filter::add_sort(const string_t &field, bool bAsc /*= true*/) {
			string_t ss = (bAsc) ? KEY_ASC : KEY_DESC;
			infomap oMap{};
			oMap[field] = any{ ss };
			infovector vv{};
			get_array_property(KEY_SORT, vv);
			vv.push_back(any{ oMap });
			set_array_property(KEY_SORT, vv);
		}//add_sort
		void query_filter::add_less(const string_t &field, const any &v) {
			add_operation(field, U("$lt"), v);
		}
		void query_filter::add_lessequals(const string_t &field, const any &v) {
			add_operation(field, U("$lte"), v);
		}
		void query_filter::add_equals(const string_t &field, const any &v) {
			add_operation(field, U("$eq"), v);
		}
		void query_filter::add_notequals(const string_t &field, const any &v) {
			add_operation(field, U("$ne"), v);
		}
		void query_filter::add_greater(const string_t &field, const any &v) {
			add_operation(field, U("$gt"), v);
		}
		void query_filter::add_greaterequals(const string_t &field, const any &v) {
			add_operation(field, U("$gte"), v);
		}
		void query_filter::add_exists(const string_t &field, bool bExists /*= true*/) {
			add_operation(field, U("$exists"), any{bExists});
		}
		void query_filter::add_type(const string_t &field, const string_t &stype) {
			string_t s = stringutils::tolower(stringutils::trim(stype));
			if ((s == U("null")) || (s == U("boolean")) || (s == U("number")) ||
				(s == U("string")) || (s == U("array")) || (s == U("onject"))) {
				add_operation(field, U("$type"), any{ s });
			}
		}
		void query_filter::add_in(const string_t &field, const infovector &vv) {
			infovector vx{ vv };
			add_operation(field, U("$in"), any{vx});
		}
		void query_filter::add_notin(const string_t &field, const infovector &vv) {
			infovector vx{ vv };
			add_operation(field, U("$in"), any{ vx });
		}
		void query_filter::add_mod(const string_t &field, int divisor, int remainder) {
			if ((divisor > 0) && (remainder >= 0)) {
				infovector vv{ any{divisor},any{remainder} };
				add_operation(field, U("$mod"), any{ vv });
			}
		}
		void query_filter::add_regex(const string_t &field, const string_t &s) {
			add_operation(field, U("$regex"), s);
		}
		//////////////////////////////////
	}// namespace couchdb
}// namespace info
