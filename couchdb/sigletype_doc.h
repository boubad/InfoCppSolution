#pragma once
#ifndef SIGLETYPE_DOC_H__
#define SIGLETYPE_DOC_H__
//////////////////////////
#include "couchdb_doc.h"
//////////////////////////
#include <future>
/////////////////////////
namespace info {
    namespace couchdb {
        //////////////////////////////
        class couchdb_manager;
        /////////////////////////////
        class type_doc : public couchdb_doc {
        public:
            static const string_t KEY_TYPE;
        public:
            type_doc();
            type_doc(const any &va);
            type_doc(const type_doc &other);
            type_doc & operator=(const type_doc &other);
            virtual ~type_doc();
        protected:
            void type(const string_t &s);
            virtual void unique_properties(std::vector<std::vector<string_t>> &vvec) const;
        public:
            string_t type(void) const;
			virtual bool is_storable(void) const;
        public:
			bool load_by_id(couchdb_manager &oMan, couchdb_doc &doc) const;
			bool load_unique(couchdb_manager &oMan, couchdb_doc &doc) const;
			//
            virtual std::future<bool> load(couchdb_manager &oMan);
            virtual std::future<bool> save(couchdb_manager &oMan);
			virtual std::future<bool> remove(couchdb_manager &oMan);
        };// class sigletype_doc
        /////////////////////////////
    }// namespce couchdb
}// namespace info
///////////////////////////
#endif // SIGLETYPE_DOC_H__

