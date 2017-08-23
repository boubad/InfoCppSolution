#include <gtest/gtest.h>
////////////////////
#include <stringutils.h>
#include <http_manager.h>
#include <couchdb_manager.h>
#include <server_info.h>
#include <update_response.h>
/////////////////////////
namespace {
using namespace info;
using namespace info::http;
using namespace info::couchdb;
///////////////////////////
class CouchDBManagerTest : public ::testing::Test {
protected:
    serverurl m_serverurl;
    databasename m_dbname;
    string_t m_username;
    string_t m_password;
    std::shared_ptr<http_client> m_httpclient;
    std::shared_ptr<couchdb_manager> m_man;
    any m_doc;
    //
    static std::vector<couchdb_doc> st_m_docs;
protected:
    CouchDBManagerTest() {}
    virtual ~CouchDBManagerTest() {}
protected:
    static void SetUpTestCase() {
        size_t n { 20 };
        std::vector<couchdb_doc> &vec = st_m_docs;
        vec.clear();
        string_t stype(U("testtype"));
        for (size_t i = 0; i < n; ++i) {
            int nRand = std::rand() % 150;
            infomap oMap {};
            oMap[U("type")] = any { stype };
            {
                ostringstream_t os {};
                os << U("sigle") << (i + 1);
                string_t s = os.str();
                oMap[U("sigle")] = any { s };
            }
            bool b = ((std::rand() % 3) == 1) ? true : false;
            oMap[U("bval")] = any { b };
            oMap[U("ival")] = any { nRand };
            oMap[U("dval")] = any { 3.14159 * nRand };
            {
                ostringstream_t os {};
                os << U("test val ") << nRand;
                string_t s = os.str();
                oMap[U("sval")] = any { s };
            }
            any a { oMap };
            couchdb_doc doc { a };
            vec.push_back(doc);
        }// i
    }// SetUpTestCase
    static void TearDownTestCase() {
        st_m_docs.clear();
    }//TearDownTestCase
    virtual void SetUp() {
        m_serverurl = serverurl { U("http://192.168.1.17:5984") };
        m_dbname = databasename { U("xxtest") };
        m_httpclient.reset(new http_manager { m_serverurl,m_username,m_password });
        http_client *pClient = m_httpclient.get();
        ASSERT_TRUE(pClient != nullptr);
        m_man.reset(new couchdb_manager { *pClient,m_dbname });
        ASSERT_TRUE(m_man.get() != nullptr);
        //
        infomap oMap {};
        string_t textval(U("string testvalue"));
        oMap[U("bval")] = any { true };
        oMap[U("ival")] = any { (int)1234 };
        oMap[U("dval")] = any { 3.14159 };
        oMap[U("sval")] = any { textval };
        infovector v1 { any{ (int)5 },any{ (int)6 },any{ (int)7 },any{ (int)8 },any{ (int)9 } };
        oMap[U("ivec")] = any { v1 };
        string_t s1(U("toto1")), s2(U("toto2")), s3(U("toto3"));
        infovector v2 { any{ s1 },any{ s2 },any{ s3 } };
        oMap[U("svec")] = any { v2 };
        infomap xMap {};
        xMap[U("first")] = any { 65.89 };
        xMap[U("second")] = any { true };
        oMap[U("oval")] = any { xMap };
        m_doc = any { oMap };
    }// SetUp
    virtual void TearDown() {
        m_man.reset();
        m_httpclient.reset();
        m_password.clear();
        m_username.clear();
    }// TearDown
}; // class CouchDBManagerTest
std::vector<couchdb_doc> CouchDBManagerTest::st_m_docs;
/////////////////////////////
TEST_F(CouchDBManagerTest,ServerInfo)
{
    couchdb_manager *pMan = m_man.get();
    ASSERT_TRUE(pMan != nullptr);
    server_info info {};
    bool b = pMan->get_server_info(info);
    ASSERT_TRUE(b);
    string_t s1 = info.welcome_string();
    ASSERT_TRUE(!s1.empty());
    string_t welcome(U("Welcome"));
    ASSERT_EQ(welcome, s1);
    string_t s2 = info.uuid();
    string_t s3 = info.version();
    ASSERT_TRUE(!s3.empty());
    string_t s4 = info.vendor_name();
    ASSERT_TRUE(!s4.empty());
    string_t s5 = info.vendor_version();
    if (s3 == U("2.1.0")) {
        std::vector<string_t> vv {};
        info.features(vv);
        ASSERT_TRUE(!vv.empty());
    }
}// ServerInfo
TEST_F(CouchDBManagerTest,Uuids)
{
    couchdb_manager *pMan = m_man.get();
    ASSERT_TRUE(pMan != nullptr);
    size_t nCount = 10;
    std::vector<string_t> vv = pMan->get_uuids_async((int)nCount).get();
    size_t ns = vv.size();
    ASSERT_EQ(nCount, ns);
}// Uuids
TEST_F(CouchDBManagerTest,Databases)
{
    couchdb_manager *pMan = m_man.get();
    ASSERT_TRUE(pMan != nullptr);
    string_t dbname = m_dbname;
    bool b = pMan->exists_database_async(dbname).get();
    if (!b) {
        b = pMan->create_database_async(dbname).get();
        ASSERT_TRUE(b);
    }
    std::vector<string_t> vv = pMan->get_all_databases_async().get();
    ASSERT_TRUE(!vv.empty());
    b = pMan->delete_database_async(dbname).get();
    ASSERT_TRUE(b);
    b = pMan->exists_database_async(dbname).get();
    ASSERT_TRUE(!b);
}// Databases
TEST_F(CouchDBManagerTest,Documents)
{
    couchdb_manager *pMan = m_man.get();
    ASSERT_TRUE(pMan != nullptr);
    string_t dbname = m_dbname;
    bool b = pMan->exists_database_async(dbname).get();
    if (!b) {
        b = pMan->create_database_async(dbname).get();
        ASSERT_TRUE(b);
    }
    couchdb_doc doc { m_doc };
    //
    update_response rsp = pMan->create_document_async(doc).get();
    if (rsp.ok()) {
        string_t sid = rsp.id();
        ASSERT_TRUE(!sid.empty());
        string_t srev = rsp.rev();
        ASSERT_TRUE(!srev.empty());
        //
        string_t sRev2 = pMan->get_document_version_async(sid).get();
        ASSERT_EQ(srev, sRev2);
        //
        couchdb_doc xdoc = pMan->get_document_by_id_async(sid).get();
        string_t sid2 {};
        bool bx = xdoc.obj_id(sid2);
        ASSERT_TRUE(bx);
        ASSERT_EQ(sid, sid2);
        //
        update_response rsp2 = pMan->delete_document_async(xdoc).get();
        b = rsp2.ok();
        ASSERT_TRUE(b);
    }
}// Documents
TEST_F(CouchDBManagerTest,Index)
{
    couchdb_manager *pMan = m_man.get();
    ASSERT_TRUE(pMan != nullptr);
    string_t dbname = m_dbname;
    bool b = pMan->exists_database_async(dbname).get();
    if (!b) {
        b = pMan->create_database_async(dbname).get();
        ASSERT_TRUE(b);
    }
    string_t field(U("ival"));
    index_response rsp = pMan->create_index_async(dbname, field).get();
    if (rsp.ok()) {
        string_t sid = rsp.index_id();
        ASSERT_TRUE(!sid.empty());
        string_t srev = rsp.name();
        ASSERT_TRUE(!srev.empty());
        string_t srev2 = rsp.result();
        ASSERT_TRUE(!srev2.empty());
    }
}// Index
TEST_F(CouchDBManagerTest,Bulk)
{
    couchdb_manager *pMan = m_man.get();
    ASSERT_TRUE(pMan != nullptr);
    string_t dbname = m_dbname;
    bool b = pMan->exists_database_async(dbname).get();
    if (!b) {
        b = pMan->create_database_async(dbname).get();
        ASSERT_TRUE(b);
    }
    string_t stype(U("testtype"));
    string_t keysigle(U("sigle"));
    query_filter filter {};
    filter.add_equals(U("type"), any { stype });
    int nCount = pMan->find_documents_count_async(filter).get();
    ASSERT_TRUE(nCount >= 0);
    if (nCount < 1) {
        const std::vector<couchdb_doc> &vec = st_m_docs;
        int nc = static_cast<int>(vec.size());
        ASSERT_TRUE(nc > 0);
        std::vector<update_response> rr = pMan->maintains_documents_async(vec).get();
        nCount = pMan->find_documents_count_async(filter).get();
        ASSERT_EQ(nc, nCount);
    }
    std::vector<couchdb_doc> alldocs {};
    int pagesize = 10;
    int offset = 0;
    filter.set_limit(pagesize);
    while (offset < nCount) {
        filter.set_skip(offset);
        std::vector<couchdb_doc> docs = pMan->find_documents_async(filter).get();
        int nx = static_cast<int>(docs.size());
        offset += nx;
        for (auto p : docs) {
            p.is_deleted(true);
            alldocs.push_back(p);
        }// p
        if (nx < pagesize) {
            break;
        }
    }// offsert
    int nt = static_cast<int>(alldocs.size());
    std::vector<update_response> rr = pMan->maintains_documents_async(alldocs).get();
    int nx = static_cast<int>(rr.size());
    ASSERT_EQ(nt, nx);
    int nr = pMan->find_documents_count_async(filter).get();
    ASSERT_TRUE(nr == 0);
}// Bulk
////////////////////////////
}// namespace
