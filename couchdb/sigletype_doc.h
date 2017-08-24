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
        class sigletype_doc : public couchdb_doc {
        public:
            static const string_t KEY_TYPE;
            static const string_t KEY_SIGLE;
        public:
            sigletype_doc();
            sigletype_doc(const any &va);
            sigletype_doc(const sigletype_doc &other);
            sigletype_doc & operator=(const sigletype_doc &other);
            virtual ~sigletype_doc();
        protected:
            void type(const string_t &s);
            virtual void unique_properties(std::vector<string_t> &vec) const;
        public:
            string_t type(void) const;
            string_t sigle(void) const;
            void sigle(const string_t &s);
        public:
            std::future<bool> load(couchdb_manager &oMan);
            std::future<bool> save(couchdb_manager &oMan);
        };// class sigletype_doc
        /////////////////////////////
    }// namespce couchdb
}// namespace info
///////////////////////////
#endif // SIGLETYPE_DOC_H__

