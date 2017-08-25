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
        class keytype_doc : public couchdb_doc {
        public:
            static const string_t KEY_TYPE;
            static const string_t KEY_SIGLE;
        public:
            keytype_doc();
            keytype_doc(const any &va);
            keytype_doc(const keytype_doc &other);
            keytype_doc & operator=(const keytype_doc &other);
            virtual ~keytype_doc();
        protected:
            void type(const string_t &s);
            virtual void unique_properties(std::vector<string_t> &vec) const;
        public:
            string_t type(void) const;
            any key(void) const;
            void key(const any &as);
        public:
            std::future<bool> load(couchdb_manager &oMan);
            std::future<bool> save(couchdb_manager &oMan);
        };// class sigletype_doc
        /////////////////////////////
    }// namespce couchdb
}// namespace info
///////////////////////////
#endif // SIGLETYPE_DOC_H__

