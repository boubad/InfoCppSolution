#pragma once
#ifndef INFOBASEOBJECT_H__
#define INFOBASEOBJECT_H__
///////////////////////////////////
#include "InfoDefs.h"
////////////////////////
namespace info {
	/////////////////////////////////
	class infobaseobject
	{
	public:
		static const string_t KEY_ID;
		static const string_t KEY_ETAG;
		static const string_t KEY_VERSION;
	private:
		static const string_t KEY_OID;
		infomap m_map;
	protected:
		infobaseobject();
		infobaseobject(const any &oVal);
		infobaseobject(const infobaseobject &other);
		infobaseobject & operator=(const infobaseobject &other);
		infomap & get_map(void);
		const infomap & get_map(void) const;
	public:
		virtual ~infobaseobject();
		operator any (void) const { return get(); }
	public:
		bool empty(void) const;
		any  get(void) const;
		void set(const any &oMap);
		bool obj_id(string_t &sid) const;
		bool obj_etag(string_t &setag) const;
		bool obj_version(string_t &sversion) const;
		bool has_obj_id(void) const;
		bool has_obj_etag(void) const;
		bool has_obj_version(void) const;
		string_t toString(void) const;
		ostream_t & write_to(ostream_t &os) const;
		void clear(void);
	protected:
		virtual void post_init(void);
		void erase_property(const string_t &key);
		void set_any_property(const string_t &key, const any &v);
		void set_bool_property(const string_t &key, bool b);
		void set_int_property(const string_t &key, int b);
		void set_float_property(const string_t &key, float b);
		void set_double_property(const string_t &key, double b);
		void set_string_property(const string_t &key, const string_t &b);
		void set_array_property(const string_t &key, const infovector & b);
		void add_item_to_array(const string_t &key, any v);
		void remove_item_to_array(const string_t &key, any v);
		void set_obj_property(const string_t &key, const infomap & b);
		void set_intvector_property(const string_t &key, const std::vector<int> &vec);
		void add_item_toarray(const string_t &key, int b);
		void remove_item_fromarray(const string_t &key, int b);
		void set_stringvector_property(const string_t &key, const std::vector<string_t> &vec);
		void add_item_toarray(const string_t &key, const string_t & b);
		void remove_item_fromarray(const string_t &key, const string_t & b);
	public:
		bool has_property(const string_t &key) const;
		bool get_any_property(const string_t &key, any &v) const;
		bool get_bool_property(const string_t &key, bool &v) const;
		bool get_int_property(const string_t &key, int &v) const;
		bool get_float_property(const string_t &key, float &v) const;
		bool get_double_property(const string_t &key, double &v) const;
		bool get_string_property(const string_t &key, string_t &v) const;
		bool get_array_property(const string_t &key, infovector &v) const;
		bool get_obj_property(const string_t &key, infomap &v) const;
		bool get_intvector_property(const string_t &key, std::vector<int> &vec) const;
		bool get_stringvector_property(const string_t &key, std::vector<string_t> &vec) const;
	};// class infobaseobject
	///////////////////////////////
	inline ostream_t & operator<<(ostream_t &os, const infobaseobject &d) {
		return (d.write_to(os));
	}
	///////////////////////////////////
}// namespace info
#endif // INFOBASEOBJECT_H__
