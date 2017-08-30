#include <gtest/gtest.h>
////////////////////
#include <stringutils.h>
#include <http_manager.h>
#include <couchdb_storemanager.h>
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
    std::shared_ptr<couchdb_storemanager> m_man;
    any m_doc;
    //
    static std::vector<info_document> st_m_docs;
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
        //
    }// SetUpTestCase
    static void TearDownTestCase() {
        st_m_docs.clear();
    }//TearDownTestCase
    virtual void SetUp() {
        m_serverurl = serverurl { U("http://localhost:5984") };
        m_dbname = databasename { U("xxtest") };
        m_httpclient.reset(new http_manager { m_serverurl,m_username,m_password });
        http_client *pClient = m_httpclient.get();
        ASSERT_TRUE(pClient != nullptr);
        m_man.reset(new couchdb_manager { *pClient,m_dbname });
        ASSERT_TRUE(m_man.get() != nullptr);
        //
        couchdb_manager *pMan = m_man.get();
        ASSERT_TRUE(pMan != nullptr);
         string_t dbname = m_dbname;
         bool b = pMan->exists_database_async(dbname).get();
         if (!b) {
             b = pMan->create_database_async(dbname).get();
             ASSERT_TRUE(b);
         }
         bool bRet = info_etudiant::check_indexes_async(*pMan).get();
         ASSERT_TRUE(bRet);
        //
        infomap oMap {};
        string_t sid(U("testid"));
        string_t textval(U("string testvalue"));
        oMap[U("_id")] = any { sid };
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
std::vector<info_document> CouchDBManagerTest::st_m_docs;
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
    std::vector<string_t> vv = pMan->get_all_databases_async().get();
    ASSERT_TRUE(!vv.empty());
    bool b = pMan->delete_database_async(m_dbname).get();
    ASSERT_TRUE(b);
    b = pMan->exists_database_async(m_dbname).get();
    ASSERT_TRUE(!b);
}// Databases
TEST_F(CouchDBManagerTest,Documents)
{
    couchdb_manager *pMan = m_man.get();
    ASSERT_TRUE(pMan != nullptr);
    string_t sid(U("testid"));
    couchdb_doc doc = pMan->get_document_by_id_async(sid).get();
    if (doc.empty()) {
        update_response r = pMan->maintains_document_async(m_doc).get();
        ASSERT_TRUE(r.ok());
        doc = pMan->get_document_by_id_async(sid).get();
        ASSERT_TRUE(!doc.empty());
    }
    ASSERT_TRUE(doc.is_persisted());
    string_t sid2 = doc.id();
    ASSERT_EQ(sid, sid2);
    string_t srev = doc.version();
    ASSERT_TRUE(!srev.empty());
    string_t sRev2 = pMan->get_document_version_async(sid).get();
    ASSERT_EQ(srev, sRev2);
    infomap oMap { doc.get_map() };
    oMap[U("ival")] = any { (int)7890 };
    couchdb_doc xdoc { oMap };
    update_response rsp1 = pMan->maintains_document_async(xdoc).get();
    ASSERT_TRUE(rsp1.ok());
    update_response rsp2 = pMan->delete_document_async(doc).get();
    bool b = rsp2.ok();
    ASSERT_TRUE(b);
    sRev2 = pMan->get_document_version_async(sid).get();
    ASSERT_TRUE(sRev2.empty());
    doc = pMan->get_document_by_id_async(sid).get();
    ASSERT_TRUE(doc.empty());
}// Documents
TEST_F(CouchDBManagerTest,Index)
{
    couchdb_manager *pMan = m_man.get();
    ASSERT_TRUE(pMan != nullptr);
    string_t field(U("ival"));
    index_response rsp = pMan->create_index_async(field).get();
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
TEST_F(CouchDBManagerTest,Attachments)
{
    couchdb_manager *pMan = m_man.get();
    ASSERT_TRUE(pMan != nullptr);
    string_t sid(U("testid"));
    couchdb_doc doc = pMan->get_document_by_id_async(sid).get();
    if (doc.empty()) {
        update_response r = pMan->maintains_document_async(m_doc).get();
        ASSERT_TRUE(r.ok());
        doc = pMan->get_document_by_id_async(sid).get();
        ASSERT_TRUE(!doc.empty());
    }
    ASSERT_TRUE(doc.is_persisted());
    string_t filename(U("/home/boubad/testdata/test.jpg"));
    blob_data blob { filename };
    bool b = blob.ok();
    ASSERT_TRUE(b);
    string_t attname = blob.name();
    update_response rsp = pMan->update_document_attachment_async(doc, blob).get();
    ASSERT_TRUE(rsp.ok());
    doc = pMan->get_document_by_id_async(sid).get();
    std::vector<attachment_info> vec {};
    size_t nu = doc.attachments(vec);
    ASSERT_TRUE(nu > 0);
    for (auto p : vec) {
        string_t xu = p.url();
        ASSERT_TRUE(!xu.empty());
        string_t xc = p.content_type();
        ASSERT_TRUE(!xc.empty());
    }//p
    std::shared_ptr<blob_data> bb = pMan->read_document_attachment_async(doc, attname).get();
    blob_data *pb = bb.get();
    ASSERT_TRUE(pb != nullptr);
    bool bz = pb->ok();
    ASSERT_TRUE(bz);
    rsp = pMan->delete_document_attachment_async(doc, attname).get();
    ASSERT_TRUE(rsp.ok());
    std::shared_ptr<blob_data> bb2 = pMan->read_document_attachment_async(doc, attname).get();
    blob_data *pb2 = bb2.get();
    ASSERT_TRUE(!pb2->ok());
}// Databases
TEST_F(CouchDBManagerTest,ImportEtuds)
{
    string_t filename(U("/home/boubad/testdata/Etuds_s4.csv"));
    ifstream_t in{filename};
    ASSERT_TRUE(in.is_open());
    etud_importer oImport{ in };
    bool b = oImport.import();
    ASSERT_TRUE(b);
    std::vector<any> vv = oImport.get_items();
    ASSERT_TRUE(!vv.empty());
}//CouchDBManager_CreateIndex
TEST_F(CouchDBManagerTest,SaveImportEtuds)
{
    string_t filename(U("/home/boubad/testdata/Etuds_s4.csv"));
    ifstream_t in{filename};
    ASSERT_TRUE(in.is_open());
    etud_importer oImport{ in };
    bool b = oImport.import();
    ASSERT_TRUE(b);
    std::vector<any> vv = oImport.get_items();
    ASSERT_TRUE(!vv.empty());
    //
    couchdb_manager *pMan = m_man.get();
   ASSERT_TRUE(pMan != nullptr);
    string_t dbname = m_dbname;
    b = pMan->exists_database_async(dbname).get();
    if (!b) {
        b = pMan->create_database_async(dbname).get();
        ASSERT_TRUE(b);
    }
    string_t field(info_etudiant::KEY_FULLNAME);
    index_response rsp = pMan->create_index_async(field).get();
    ASSERT_TRUE(rsp.ok());
    std::vector<info_etudiant> etuds{};
    for (auto va : vv) {
        info_etudiant oEtud{ va };
        b = oEtud.load(*pMan).get();
        if (!b) {
            b = oEtud.save(*pMan).get();
        }
        if (oEtud.is_persisted()){
        etuds.push_back(oEtud);
        }
    }// va
    query_filter filter{};
    int nPageSize{ 10 };
    filter.set_limit(nPageSize);
    filter.add_sort(info_etudiant::KEY_FULLNAME);
//	filter.add_projection_field(info_etudiant::KEY_FULLNAME);
//	filter.add_projection_field(info_etudiant::KEY_DOSSIER);
//	filter.add_projection_field(info_etudiant::KEY_VERSION);
    int nTotal = info_etudiant::get_count_async(*pMan,filter).get();
    int nOffset{ 0 };
    while (nOffset < nTotal) {
        filter.set_skip(nOffset);
        std::vector<info_etudiant> xx = info_etudiant::get_async(*pMan, filter).get();
        int nx = static_cast<int>(xx.size());
        nOffset += nx;
        for (auto x : xx) {
            ucout << x << std::endl;
        }// x
        if (nx < nPageSize) {
            break;
        }
    }// nOffset
}//CouchDBManager_CreateIndex
TEST_F(CouchDBManagerTest,ReadEtuds)
{
    couchdb_manager *pMan = m_man.get();
    ASSERT_TRUE(pMan != nullptr);
    query_filter filter{};
    int nPageSize{ 10 };
    filter.set_limit(nPageSize);
    
    filter.add_projection_field(info_etudiant::KEY_FULLNAME);
    filter.add_projection_field(info_etudiant::KEY_FIRSTNAME);
    filter.add_projection_field(info_etudiant::KEY_LASTNAME);
    //filter.add_sort(info_etudiant::KEY_LASTNAME);
    filter.add_sort(info_etudiant::KEY_FIRSTNAME);
    int nTotal = info_etudiant::get_count_async(*pMan, filter).get();
    int nOffset{ 0 };
    while (nOffset < nTotal) {
        filter.set_skip(nOffset);
        std::vector<info_etudiant> xx = info_etudiant::get_async(*pMan, filter).get();
        int nx = static_cast<int>(xx.size());
        nOffset += nx;
        for (auto x : xx) {
            ucout << x << std::endl;
        }// x
        if (nx < nPageSize) {
            break;
        }
    }// nOffset
}//CouchDBManager_ReadEtuds
////////////////////////////
}// namespace
