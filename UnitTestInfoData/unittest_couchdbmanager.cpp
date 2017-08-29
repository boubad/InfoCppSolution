#include "stdafx.h"
#include "CppUnitTest.h"
////////////////////////
#include <stringutils.h>
#include <http_manager.h>
#include <couchdb_storemanager.h>
#include <server_info.h>
#include <update_response.h>
#include <etud_importer.h>
#include <info_etudiant.h>
/////////////////////////////
//
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTestInfoData
{
	using namespace info;
	using namespace info::http;
	using namespace info::couchdb;
	using namespace info::domain;
	///////////////////////////
	TEST_CLASS(CouchDBManagerTest)
	{
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
	public:
		TEST_CLASS_INITIALIZE(ClassInitialize)
		{
			size_t n{ 50 };
			std::vector<info_document> &vec = st_m_docs;
			vec.clear();
			string_t stype(U("testtype"));
			for (size_t i = 0; i < n; ++i) {
				int nRand = std::rand() % 150;
				infomap oMap{};
				oMap[U("type")] = any{ stype };
				{
					ostringstream_t os{};
					os << U("sigle") << (i + 1);
					string_t s = os.str();
					oMap[U("sigle")] = any{ s };
				}
				bool b = ((std::rand() % 3) == 1) ? true : false;
				oMap[U("bval")] = any{ b };
				oMap[U("ival")] = any{ nRand };
				oMap[U("dval")] = any{ 3.14159 * nRand };
				{
					ostringstream_t os{};
					os << U("test val ") << nRand;
					string_t s = os.str();
					oMap[U("sval")] = any{ s };
				}
				any a{ oMap };
				info_document doc{ a };
				vec.push_back(doc);
			}// i
		}//ClassInitialize
		TEST_CLASS_CLEANUP(ClassCleanup)
		{
			st_m_docs.clear();
		}//ClassCleanup
	public:
		TEST_METHOD_INITIALIZE(SetUp)
		{
			m_serverurl = serverurl{ U("http://localhost:5984") };
			m_dbname = databasename{ U("xxtest") };
			m_httpclient.reset(new http_manager{ m_serverurl,m_username,m_password });
			http_client *pClient = m_httpclient.get();
			Assert::IsNotNull(pClient);
			m_man.reset(new couchdb_storemanager{ *pClient,m_dbname });
			Assert::IsNotNull(m_man.get());
			//
			infomap oMap{};
			string_t sid(U("testid"));
			string_t textval(U("string testvalue"));
			oMap[U("_id")] = any{ sid };
			oMap[U("bval")] = any{ true };
			oMap[U("ival")] = any{ (int)1234 };
			oMap[U("dval")] = any{ 3.14159 };
			oMap[U("sval")] = any{ textval };
			infovector v1{ any{ (int)5 },any{ (int)6 },any{ (int)7 },any{ (int)8 },any{ (int)9 } };
			oMap[U("ivec")] = any{ v1 };
			string_t s1(U("toto1")), s2(U("toto2")), s3(U("toto3"));
			infovector v2{ any{ s1 },any{ s2 },any{ s3 } };
			oMap[U("svec")] = any{ v2 };
			infomap xMap{};
			xMap[U("first")] = any{ 65.89 };
			xMap[U("second")] = any{ true };
			oMap[U("oval")] = any{ xMap };
			m_doc = any{ oMap };
			//
		}// SetUp
		TEST_METHOD_CLEANUP(TearDown)
		{
			m_man.reset();
			m_httpclient.reset();
			m_password.clear();
			m_username.clear();
		}// TearDown
	public:

		TEST_METHOD(CouchDBManager_ServerInfo)
		{
			couchdb_storemanager *pMan = m_man.get();
			Assert::IsNotNull(pMan);
			server_info info{};
			bool b = pMan->get_server_info(info);
			Assert::IsTrue(b);
			string_t s = info.toString();
			Logger::WriteMessage(s.c_str());
			string_t s1 = info.welcome_string();
			Assert::IsFalse(s1.empty());
			string_t welcome(U("Welcome"));
			Assert::AreEqual(welcome, s1);
			string_t s2 = info.uuid();
			string_t s3 = info.version();
			Assert::IsFalse(s3.empty());
			string_t s4 = info.vendor_name();
			Assert::IsFalse(s4.empty());
			string_t s5 = info.vendor_version();
			if (s3 == U("2.1.0")) {
				std::vector<string_t> vv{};
				info.features(vv);
				Assert::IsFalse(vv.empty());
			}
		}//CouchDBManager_ServerInfo
		TEST_METHOD(CouchDBManager_GetAllDatabases)
		{
			couchdb_storemanager *pMan = m_man.get();
			Assert::IsNotNull(pMan);
			std::vector<string_t> vv{};
			bool bRet = pMan->get_all_databases(vv);
			Assert::IsTrue(bRet);
			Assert::IsFalse(vv.empty());
		}//CouchDBManager_GetAllDatabases
		TEST_METHOD(CouchDBManager_GetUuids)
		{
			couchdb_storemanager *pMan = m_man.get();
			Assert::IsNotNull(pMan);
			size_t nCount = 10;
			std::vector<string_t> vv{};
			bool bRet = pMan->get_uuids(vv,(int)nCount);
			Assert::IsTrue(bRet);
			size_t ns = vv.size();
			Assert::AreEqual(nCount, ns);
		}//CouchDBManager_GetUuids
		TEST_METHOD(CouchDBManager_CreateDeleteDatabase)
		{
			couchdb_storemanager *pMan = m_man.get();
			Assert::IsNotNull(pMan);
			string_t dbname(U("yytest"));
			bool b = pMan->exists_database(dbname);
			if (!b) {
				b = pMan->create_database(dbname);
				Assert::IsTrue(b);
			}
			b = pMan->delete_database(dbname);
			Assert::IsTrue(b);
			b = pMan->exists_database(dbname);
			Assert::IsFalse(b);
		}//CouchDBManager_CreateDeleteDatabase
		TEST_METHOD(CouchDBManager_CreateDeleteDocument)
		{
			couchdb_storemanager *pMan = m_man.get();
			Assert::IsNotNull(pMan);
			string_t sid(U("testid"));
			info_document doc{};
			if (!pMan->get_document_by_id(sid,doc)){
				update_response r{};
				info_document d{ m_doc };
				bool b = pMan->maintains_document(d,r);
				Assert::IsTrue(b);
				b = pMan->get_document_by_id(sid,doc);
				Assert::IsTrue(b);
			}
			Assert::IsTrue(doc.is_persisted());
			string_t sid2 = doc.id();
			Assert::AreEqual(sid, sid2);
			string_t srev = doc.version();
			Assert::IsFalse(srev.empty());
			string_t sRev2{};
			bool bRet = pMan->get_document_version(sid,sRev2);
			Assert::IsTrue(bRet);
			Assert::AreEqual(srev, sRev2);
			infomap oMap{ doc.get_map() };
			oMap[U("ival")] = any{ (int)7890 };
			info_document xdoc{ any{oMap} };
			update_response rsp1{};
			bRet = pMan->maintains_document(xdoc,rsp1);
			Assert::IsTrue(bRet);
			update_response rsp2{};
			bRet = pMan->delete_document(doc,rsp2);
			Assert::IsTrue(bRet);
			//
		}//CouchDBManager_CreateDeleteDocument
		
		TEST_METHOD(CouchDBManager_MaintainsDocs)
		{
			couchdb_storemanager *pMan = m_man.get();
			Assert::IsNotNull(pMan);
			string_t stype(U("testtype"));
			string_t keysigle(U("sigle"));
			query_filter filter{};
			filter.add_equals(U("type"), any{ stype });
			int nCount{ -1 };
			bool bRet = pMan->find_documents_count(filter,nCount);
			Assert::IsTrue(bRet);
			Assert::IsTrue(nCount >= 0);
			if (nCount < 1) {
				const std::vector<info_document> &vec = st_m_docs;
				int nc = static_cast<int>(vec.size());
				Assert::IsTrue(nc > 0);
				std::vector<update_response> rr{};
				bRet = pMan->maintains_documents(vec,rr);
				Assert::IsTrue(bRet);
				bRet = pMan->find_documents_count(filter, nCount);
				Assert::IsTrue(bRet);
				Assert::AreEqual(nc, nCount);
			}
			std::vector<info_document> alldocs{};
			int pagesize = 10;
			int offset = 0;
			filter.set_limit(pagesize);
			while (offset < nCount) {
				filter.set_skip(offset);
				std::vector<info_document> docs{};
				bRet = pMan->find_documents(filter,docs);
				Assert::IsTrue(bRet);
				int nx = static_cast<int>(docs.size());
				offset += nx;
				for (auto p : docs) {
					ostringstream_t os{};
					os << p;
					string_t s = os.str();
					Logger::WriteMessage(s.c_str());
					p.set_deleted(true);
					alldocs.push_back(p);
				}// p
				if (nx < pagesize) {
					break;
				}
			}// offsert
			int nt = static_cast<int>(alldocs.size());
			std::vector<update_response> rr{};
			bRet = pMan->maintains_documents(alldocs,rr);
			Assert::IsTrue(bRet);
			int nx = static_cast<int>(rr.size());
			Assert::AreEqual(nt, nx);
			int nr{ -1 };
			bRet = pMan->find_documents_count(filter,nr);
			Assert::IsTrue(bRet);
			Assert::IsTrue(nr == 0);
		}//CouchDBManager_MaintainsDocs
		TEST_METHOD(CouchDBManager_Attachment)
		{
			couchdb_storemanager *pMan = m_man.get();
			Assert::IsNotNull(pMan);
			string_t sid(U("testid"));
			info_document doc{};
			if (!pMan->get_document_by_id(sid,doc)) {
				update_response r{};
				info_document x{ m_doc };
				bool b = pMan->maintains_document(x,r);
				Assert::IsTrue(b);
				b = pMan->get_document_by_id(sid,doc);
				Assert::IsTrue(b);
			}
			Assert::IsTrue(doc.is_persisted());
			string_t filename(U("D:\\testdata\\test.jpg"));
			blob_data blob{ filename };
			blob.id(sid);
			update_response rsp{};
			bool bRet = pMan->update_attachment(blob,rsp);
			Assert::IsTrue(bRet);
			Assert::IsTrue(rsp.ok());
			bRet = pMan->get_document_by_id(sid, doc);
			Assert::IsTrue(bRet);
			const std::vector<blob_data> &vec = doc.blobs();
			Assert::IsTrue(!vec.empty());
			for (auto p : vec) {
				string_t sname = p.name();
				Assert::IsTrue(!sname.empty());
				string_t xu = p.url();
				Assert::IsTrue(!xu.empty());
				string_t xc = p.mime_type();
				Assert::IsTrue(!xc.empty());
				blob_data blob{};
				bRet = pMan->read_attachment(sid, sname, blob);
				Assert::IsTrue(bRet);
				update_response rr{};
				bRet = pMan->delete_attachment(blob, rr);
				Assert::IsTrue(bRet);
			}//p
			bRet = pMan->get_document_by_id(sid, doc);
			Assert::IsTrue(bRet);
			const std::vector<blob_data> &vec2 = doc.blobs();
			Assert::IsTrue(vec2.empty());
		}// CouchDBManager_Attachment
		
	};
	////////////////////////////
	std::vector<info_document> CouchDBManagerTest::st_m_docs;
	/////////////////////////
}