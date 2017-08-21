#pragma once
#ifndef COUCHDB_DOC_H__
#define COUCHDB_DOC_H__
///////////////////////////////
#include <infobaseobject.h>
#include "attachment_info.h"
//////////////////////////////////
namespace info {
	namespace couchdb {
		////////////////////////////
		class couchdb_doc : public infobaseobject
		{
		public:
			couchdb_doc();
			couchdb_doc(const info::any &va);
			couchdb_doc(const couchdb_doc &other);
			couchdb_doc & operator=(const couchdb_doc &other);
			virtual ~couchdb_doc();
		public:
			bool is_deleted(void) const;
			void is_deleted(bool b);
			size_t attachments(std::vector<attachment_info> &vec) const;
		};
	}// namespace couchdb
}// namespace info
/////////////////////////////
#endif // COUCHDB_DOC_H__
