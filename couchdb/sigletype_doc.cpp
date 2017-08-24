#include "sigletype_doc.h"
#include "couchdb_manager.h"
////////////////////////////
namespace info {
namespace couchdb {
///////////////////////
const string_t sigletype_doc::KEY_TYPE(U("type"));
const string_t sigletype_doc::KEY_SIGLE(U("sigle"));
/////////////////////////////
sigletype_doc::sigletype_doc() {

}
sigletype_doc::sigletype_doc(const any &va):couchdb_doc(va) {

}
sigletype_doc:: sigletype_doc(const sigletype_doc &other):couchdb_doc(other) {

}
sigletype_doc & sigletype_doc::operator=(const sigletype_doc &other) {
    if (this != &other) {
        couchdb_doc::operator=(other);
    }
    return (*this);
}
sigletype_doc::~sigletype_doc() {

}
void sigletype_doc::type(const string_t &s) {
    set_string_property(KEY_TYPE,s);
}
void sigletype_doc::unique_properties(std::vector<string_t> &vec) const {
    vec.clear();
    vec.push_back(KEY_SIGLE);
    vec.push_back(KEY_TYPE);
}

string_t sigletype_doc::type(void) const {
    string_t s {};
    get_string_property(KEY_TYPE,s);
    return (s);
}
string_t sigletype_doc::sigle(void) const {
    string_t s {};
    get_string_property(KEY_TYPE,s);
    return (s);
}
void sigletype_doc::sigle(const string_t &s) {
    set_string_property(KEY_TYPE,s);
}
std::future<bool> sigletype_doc::load(couchdb_manager &oMan) {
    couchdb_manager *pMan = &oMan;
    return std::async([this,pMan]()->bool {
        string_t docid = this->id();
        if (!docid.empty()) {
            couchdb_doc doc = pMan->get_document_by_id_async(docid).get();
            if (!doc.empty()) {
                this->set(doc.get());
                return (true);
            }
        }
        std::vector<string_t> vec{};
        this->unique_properties(vec);
        query_filter filter{};
        for (auto key : vec) {
            any va {};
            if (!this->get_any_property(key,va)) {
                return (false);
            }
            filter.add_equals(key, va);
        }// key
        filter.set_limit(1);
        filter.set_skip(0);
        std::vector<couchdb_doc> vv = pMan->find_documents_async(filter).get();
        if (!vv.empty()) {
            const couchdb_doc &doc = vv[0];
            if (!doc.empty()) {
                this->set(doc.get());
                return (true);
            }
        }
        return (false);
    });
}// load
std::future<bool> sigletype_doc::save(couchdb_manager &oMan) {
    couchdb_manager *pMan = &oMan;
    return std::async([this,pMan]()->bool {
        update_response r = pMan->maintains_document_async(*this).get();
        if (r.ok()) {
            return this->load(*pMan).get();
        }
        return (false);
    });
}// save
/////////////////
}// namespace couchdb
} // namespace info
