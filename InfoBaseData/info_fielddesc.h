#pragma once
#ifndef INFOFIELDDESC_H__
#define INFOFIELDDESC_H__
////////////////////////////////////
#include "infobaseobject.h"
///////////////////////////////////
namespace info {
		class info_fielddesc : public  infobaseobject
		{
		public:
			info_fielddesc();
			info_fielddesc(const string_t &pname, info_datatype t = info_datatype::text);
			info_fielddesc(const string_t &pname, const string_t &impname, info_datatype t = info_datatype::text);
			info_fielddesc(const string_t &pname, const string_t &dispname, const string_t &impname, info_datatype t = info_datatype::text);
			info_fielddesc(const info_fielddesc &other);
			info_fielddesc & operator=(const info_fielddesc &other);
			virtual ~info_fielddesc();
			bool operator<(const info_fielddesc &other) const;
			bool operator==(const info_fielddesc &other) const;
		public:
			static bool read_any_value(const string_t &sx, any &v);
			nullable_any get_field_value(const string_t &sx) const;
			bool apply_string_value(const string_t &sx, infomap &obj) const;
		public:
			info_datatype data_type(void) const;
			void data_type(info_datatype t);
			string_t property_name(void) const;
			void property_name(const string_t &s);
			string_t import_name(void) const;
			void import_name(const string_t &s);
			string_t display_name(void) const;
			void display_name(const string_t &s);
			bool required(void) const;
			void required(bool b);
			any min_value(void) const;
			void min_value(const any &v);
			any max_value(void) const;
			void max_value(const any &v);
			size_t maxlength(void) const;
			void maxlength(size_t n);
			bool validate(const any &v) const;
			nullable_any get_value(const string_t &sval);
			//
			info_filteroptype filteroptype(void) const;
			void filteroptype(info_filteroptype t);
			nullable_any filter_val(void) const;
			void filter_val(const any &val);
			nullable_any filter(void) const;
		};// class info_fielddesc
}// namespace info
 //////////////////////////////////
#endif // INFOFIELDDESC_H__
