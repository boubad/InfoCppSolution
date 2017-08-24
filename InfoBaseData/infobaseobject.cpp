#include "infobaseobject.h"
#include "stringutils.h"
////////////////////////////////
namespace info {
/////////////////////////
const string_t infobaseobject::KEY_ID(U("_id"));
const string_t infobaseobject::KEY_OID(U("_oid"));
const string_t infobaseobject::KEY_ETAG(U("_etag"));
const string_t infobaseobject::KEY_VERSION(U("_rev"));
////////////////////////
infobaseobject::infobaseobject()
{
}
infobaseobject::infobaseobject(const any &oVal) {
    if (oVal.type() == typeid(infomap)) {
        m_map = INFO_ANY_CAST<infomap>(oVal);
    }
}
infobaseobject::infobaseobject(const infobaseobject &other) : m_map(other.m_map)
{
}
infobaseobject & infobaseobject::operator=(const infobaseobject &other) {
    if (this != &other) {
        this->m_map = other.m_map;
    }
    return (*this);
}
infobaseobject::~infobaseobject()
{
}
void infobaseobject::clear(void) {
    m_map.clear();
    post_init();
}
void infobaseobject::post_init(void) {
    // do nothing here
}
infomap & infobaseobject::get_map(void) {
    return (m_map);
}
const infomap & infobaseobject::get_map(void) const {
    return (m_map);
}

bool infobaseobject::empty(void) const {
    const infomap &oMap = get_map();
    return (oMap.empty());
}
any  infobaseobject::get(void) const {
    return (any { m_map });
}
void infobaseobject::set(const any &oVal) {
    if (oVal.type() == typeid(infomap)) {
        m_map = m_map = INFO_ANY_CAST<infomap>(oVal);
    }
    else {
        m_map.clear();
    }
    post_init();
}
bool infobaseobject::has_property(const string_t &key) const {
    const infomap &oMap = get_map();
    return (oMap.find(key) != oMap.end());
}
void  infobaseobject::erase_property(const string_t &key) {
    infomap &oMap = get_map();
    auto it = oMap.find(key);
    if (it != oMap.end()) {
        oMap.erase(it);
    }
}
void infobaseobject::set_any_property(const string_t &key, const any &v) {
    if (INFO_ANY_HAS_VALUE(v)) {
        this->erase_property(key);
    }
    else {
        infomap &oMap = get_map();
        any vv { v };
        oMap[key] = vv;
    }
}
bool infobaseobject::get_any_property(const string_t &key, any &v) const {
    const infomap &oMap = get_map();
    auto it = oMap.find(key);
    if (it != oMap.end()) {
        v = (*it).second;
        return (true);
    }
    else {
        return (false);
    }
}
bool infobaseobject::obj_id(string_t &sid) const {
    bool bRet { false };
    const infomap &oMap = get_map();
    auto it = oMap.find(KEY_ID);
    if (it != oMap.end()) {
        const any &v0 = (*it).second;
        if (v0.type() == typeid(string_t)) {
            sid = INFO_ANY_CAST<string_t>(v0);
            return (!sid.empty());
        }
        else if (v0.type() == typeid(infomap)) {
            infomap mx = INFO_ANY_CAST<infomap>(v0);
            auto jt = mx.find(KEY_OID);
            if (jt != mx.end()) {
                const any & v1 = (*jt).second;
                if (v1.type() == typeid(string_t)) {
                    sid = INFO_ANY_CAST<string_t>(v1);
                    return (!sid.empty());
                }
            }//jt
        }
    }// found
    return (bRet);
}
bool infobaseobject::obj_etag(string_t &setag) const {
    bool bRet { false };
    const infomap &oMap = get_map();
    auto it = oMap.find(KEY_ETAG);
    if (it != oMap.end()) {
        const any &v0 = (*it).second;
        if (v0.type() == typeid(string_t)) {
            setag = INFO_ANY_CAST<string_t>(v0);
            return (!setag.empty());
        }
        else if (v0.type() == typeid(infomap)) {
            infomap mx = INFO_ANY_CAST<infomap>(v0);
            auto jt = mx.find(KEY_OID);
            if (jt != mx.end()) {
                const any & v1 = (*jt).second;
                if (v1.type() == typeid(string_t)) {
                    setag = INFO_ANY_CAST<string_t>(v1);
                    return (!setag.empty());
                }
            }//jt
        }
    }// found
    return (bRet);
}
bool infobaseobject::obj_version(string_t &sversion) const {
    bool bRet { false };
    const infomap &oMap = get_map();
    auto it = oMap.find(KEY_VERSION);
    if (it != oMap.end()) {
        const any &v0 = (*it).second;
        if (v0.type() == typeid(string_t)) {
            sversion = INFO_ANY_CAST<string_t>(v0);
            return (!sversion.empty());
        }
    }// found
    return (bRet);
}
bool infobaseobject::has_obj_id(void) const {
    string_t sid {};
    return (this->obj_id(sid));
}
bool infobaseobject::has_obj_etag(void) const {
    string_t sid {};
    return (this->obj_etag(sid));
}
bool infobaseobject::has_obj_version(void) const {
    string_t sid {};
    return (this->obj_version(sid));
}
bool infobaseobject::get_bool_property(const string_t &key, bool &v) const {
    any v0 {};
    if (this->get_any_property(key, v0)) {
        if (v0.type() == typeid(bool)) {
            v = INFO_ANY_CAST<bool>(v0);
            return (true);
        }
    }
    return (false);
}
void infobaseobject::set_bool_property(const string_t &key, bool b) {
    infomap &oMap = get_map();
    oMap[key] = any { b };
}
bool infobaseobject::get_int_property(const string_t &key, int &v) const {
    any v0 {};
    if (this->get_any_property(key, v0)) {
        if (v0.type() == typeid(int)) {
            v = INFO_ANY_CAST<int>(v0);
            return (true);
        }
        else if (v0.type() == typeid(double)) {
            double xv = INFO_ANY_CAST<double>(v0);
            v = static_cast<int>(xv);
            return (true);
        }
        else if (v0.type() == typeid(float)) {
            float xv = INFO_ANY_CAST<float>(v0);
            v = static_cast<int>(xv);
            return (true);
        }
    }
    return (false);
}
void infobaseobject::set_int_property(const string_t &key, int b) {
    infomap &oMap = get_map();
    oMap[key] = any { b };
}
bool infobaseobject::get_float_property(const string_t &key, float &v) const {
    any v0 {};
    if (this->get_any_property(key, v0)) {
        if (v0.type() == typeid(float)) {
            v = INFO_ANY_CAST<float>(v0);
            return (true);
        }
        else if (v0.type() == typeid(double)) {
            double xv = INFO_ANY_CAST<double>(v0);
            v = static_cast<float>(xv);
            return (true);
        }
        else if (v0.type() == typeid(int)) {
            int xv = INFO_ANY_CAST<int>(v0);
            v = static_cast<float>(xv);
            return (true);
        }
    }
    return (false);
}
void infobaseobject::set_float_property(const string_t &key, float b) {
    infomap &oMap = get_map();
    oMap[key] = any { b };
}
bool infobaseobject::get_double_property(const string_t &key, double &v) const {
    any v0 {};
    if (this->get_any_property(key, v0)) {
        if (v0.type() == typeid(double)) {
            v = INFO_ANY_CAST<double>(v0);
            return (true);
        }
        else if (v0.type() == typeid(float)) {
            float xv = INFO_ANY_CAST<float>(v0);
            v = static_cast<double>(xv);
            return (true);
        }
        else if (v0.type() == typeid(int)) {
            int xv = INFO_ANY_CAST<int>(v0);
            v = static_cast<double>(xv);
            return (true);
        }
    }
    return (false);
}
void infobaseobject::set_double_property(const string_t &key, double b) {
    infomap &oMap = get_map();
    oMap[key] = any { b };
}
bool infobaseobject::get_string_property(const string_t &key, string_t &v) const {
    any v0 {};
    if (this->get_any_property(key, v0)) {
        if (v0.type() == typeid(string_t)) {
            v = INFO_ANY_CAST<string_t>(v0);
            return (true);
        }
    }
    return (false);
}
void infobaseobject::set_string_property(const string_t &key, const string_t &b) {
    string_t s = stringutils::trim(b);
    if (s.empty()) {
        erase_property(key);
    }
    else {
        infomap &oMap = get_map();
        oMap[key] = any { s };
    }
}
bool infobaseobject::get_array_property(const string_t &key, infovector &v) const {
    any v0 {};
    if (this->get_any_property(key, v0)) {
        if (v0.type() == typeid(infovector)) {
            v = INFO_ANY_CAST<infovector>(v0);
            return (true);
        }
    }
    return (false);
}
void infobaseobject::set_array_property(const string_t &key, const infovector & b) {
    if (b.empty()) {
        erase_property(key);
    }
    else {
        infomap &oMap = get_map();
        infovector vv { b };
        oMap[key] = any { vv };
    }
}
void infobaseobject::add_item_to_array(const string_t &key, any v) {
    infovector vv {};
    if (this->get_array_property(key, vv)) {
        for (auto x : vv) {
            if (info_any_equals(x, v)) {
                return;
            }
        }
    }
    infomap &oMap = get_map();
    vv.push_back(v);
    oMap[key] = any { vv };
}
void infobaseobject::remove_item_to_array(const string_t &key, any v) {
    infovector vv {};
    if (this->get_array_property(key, vv)) {
        infovector vvz {};
        for (auto x : vv) {
            if (!info_any_equals(x, v)) {
                vvz.push_back(x);
            }
        }
        if (vvz.size() != vv.size()) {
            if (vvz.empty()) {
                erase_property(key);
            }
            else {
                infomap &oMap = get_map();
                oMap[key] = any { vvz };
            }
        }
    }
}
bool infobaseobject::get_obj_property(const string_t &key, infomap &v) const {
    any v0 {};
    if (this->get_any_property(key, v0)) {
        if (v0.type() == typeid(infomap)) {
            v = INFO_ANY_CAST<infomap>(v0);
            return (true);
        }
    }
    return (false);
}
void infobaseobject::set_obj_property(const string_t &key, const infomap & b) {
    if (b.empty()) {
        erase_property(key);
    }
    else {
        infomap vv { b };
        infomap &oMap = get_map();
        oMap[key] = any { vv };
    }
}
bool infobaseobject::get_intvector_property(const string_t &key,
        std::vector<int> &vec) const {
    vec.clear();
    infovector vv {};
    if (get_array_property(key, vv)) {
        for (auto x : vv) {
            if (x.type() == typeid(int)) {
                int z = INFO_ANY_CAST<int>(x);
                vec.push_back(z);
            }
        }// x
    }//ok
    return (!vec.empty());
}
void infobaseobject::set_intvector_property(const string_t &key, const std::vector<int> &vec) {
    infovector vv {};
    for (auto x : vec) {
        vv.push_back(any { x });
    }
    if (vv.empty()) {
        erase_property(key);
    }
    else {
        infomap &oMap = get_map();
        oMap[key] = any { vv };
    }
}
void infobaseobject::add_item_toarray(const string_t &key, int b) {
    any vv { b };
    this->add_item_to_array(key, vv);
}
void infobaseobject::remove_item_fromarray(const string_t &key, int b) {
    any vv { b };
    this->remove_item_to_array(key, vv);
}
//
bool infobaseobject::get_stringvector_property(const string_t &key, std::vector<string_t> &vec) const {
    vec.clear();
    infovector vv {};
    if (get_array_property(key, vv)) {
        for (auto x : vv) {
            if (x.type() == typeid(string_t)) {
                string_t z = INFO_ANY_CAST<string_t>(x);
                vec.push_back(z);
            }
        }// x
    }//ok
    return (!vec.empty());
}
void infobaseobject::set_stringvector_property(const string_t &key, const std::vector<string_t> &vec) {
    infovector vv {};
    for (auto x : vec) {
        vv.push_back(any { x });
    }
    if (vv.empty()) {
        erase_property(key);
    }
    else {
        infomap &oMap = get_map();
        oMap[key] = any { vv };
    }
}
void infobaseobject::add_item_toarray(const string_t &key, const string_t & b) {
    any vv { b };
    this->add_item_to_array(key, vv);
}
void infobaseobject::remove_item_fromarray(const string_t &key, const string_t & b) {
    any vv { b };
    this->remove_item_to_array(key, vv);
}
string_t infobaseobject::toString(void) const {
    any va { m_map };
    return (stringutils::any_to_stringt(va));
}
ostream_t & infobaseobject::write_to(ostream_t &os) const {
    os << toString();
    return (os);
}
}// namespace info
