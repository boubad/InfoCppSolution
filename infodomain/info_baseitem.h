#pragma once
#ifndef INFO_BASEITEM_H__
#define INFO_BASEITEM_H__
//////////////////////////
#include "domainstrings.h"
////////////////////////////
#include <info_document.h>
//////////////////////////////
namespace info {
	namespace domain {
		/////////////////////////
		class info_baseitem : public info_document, public domainstrings
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
			virtual void get_descs(std::set<info_fielddesc> &oSet) const override;
		};
		/////////////////////////
	}// namespace domain
}// namespace info
#endif // INFO_BASEITEM_H__
