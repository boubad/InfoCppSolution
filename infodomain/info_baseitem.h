#pragma once
#ifndef INFO_BASEITEM_H__
#define INFO_BASEITEM_H__
//////////////////////////
#include "domainstrings.h"
////////////////////////////
#include <sigletype_doc.h>
#include <query_filter.h>
#include <info_fielddesc.h>
#include <set>
//////////////////////////////
namespace info {
	namespace couchdb {
		class couchdb_manager;
	}// namespace couchdb
}// namespace info;
/////////////////////////////
namespace info {
	namespace domain {
		/////////////////////////
		class info_baseitem : public info::couchdb::type_doc, public domainstrings
		{
		protected:
			info_baseitem();
			info_baseitem(const any &v);
			info_baseitem(const info_baseitem &other);
			info_baseitem & operator=(const info_baseitem &other);
		public:
			virtual ~info_baseitem();
		public:
			info_status status(void) const;
			void status(info_status s);
			nullable_string observations(void) const;
			void observations(const string_t &s);
			virtual const std::set<info_fielddesc>  get_descs(void) const;
		};
		/////////////////////////
	}// namespace domain
}// namespace info
#endif // INFO_BASEITEM_H__
