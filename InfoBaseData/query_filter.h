#pragma once
#ifndef QUERY_FILTER_H__
#define QUERY_FILTER_H__
///////////////////////////////
#include "infobaseobject.h"
////////////////////////////
namespace info {
	////////////////////////
	class query_filter : public infobaseobject
	{
	private:
		void add_operation(const string_t &field, const string_t &op, any v);
	public:
		query_filter();
		query_filter(const query_filter &other);
		query_filter & operator=(const query_filter &other);
		virtual ~query_filter();
	public:
		//
		void clear_sort(void);
		void clear_projection(void);
		//
		void set_limit(int n);
		void set_skip(int n);
		void add_useindex(const string_t &s);
		void  set_read_quorum(int n);
		void set_bookmark(const string_t &s);
		void set_update(bool b);
		void set_stable(bool b);
		void add_sort(const string_t &field, bool bAsc = true);
		void add_projection_field(const string_t &field);
		//
		void add_less(const string_t &field, const any &v);
		void add_lessequals(const string_t &field, const any &v);
		void add_equals(const string_t &field, const any &v);
		void add_notequals(const string_t &field, const any &v);
		void add_greater(const string_t &field, const any &v);
		void add_greaterequals(const string_t &field, const any &v);
		void add_exists(const string_t &field, bool bExists = true);
		void add_type(const string_t &field, const string_t &stype);
		void add_in(const string_t &field, const infovector &vv);
		void add_notin(const string_t &field, const infovector &vv);
		void add_mod(const string_t &field, int divisor, int remainder);
		void add_regex(const string_t &field, const string_t &s);
	}; // class query_filter
		//
}// namespace info
//////////////////////////
#endif // QUERY_FILTER_H__
