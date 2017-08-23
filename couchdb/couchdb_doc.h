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
			couchdb_doc(const any &va);
			couchdb_doc(const couchdb_doc &other);
			couchdb_doc & operator=(const couchdb_doc &other);
			virtual ~couchdb_doc();
		public:
			string_t  id(void) const;
			void id(const string_t &s);
			string_t version(void) const;
			void version(const string_t &s);
			bool is_persisted(void) const;
			couchdb_doc get_deleted_doc(void) const;
			//
			bool is_deleted(void) const;
			void is_deleted(bool b);
			size_t attachments(std::vector<attachment_info> &vec) const;
			size_t attachments_names(std::vector<string_t> &vec) const;
			void update_attachments_urls(const string_t sbase);
			void clean_attachments(bool bAll = false);
		};
	}// namespace couchdb
}// namespace info
/////////////////////////////
#endif // COUCHDB_DOC_H__
