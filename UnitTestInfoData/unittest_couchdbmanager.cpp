#include "stdafx.h"
#include "CppUnitTest.h"
////////////////////////
#include <stringutils.h>
#include <http_manager.h>
#include <couchdb_manager.h>
#include <server_info.h>
#include <update_response.h>
/////////////////////////////
//
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTestInfoData
{
	using namespace info;
	using namespace info::http;
	using namespace info::couchdb;
	///////////////////////////
	TEST_CLASS(CouchDBManagerTest)
	{
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
	public:
		TEST_CLASS_INITIALIZE(ClassInitialize)
		{
			size_t n{ 50 };
			std::vector<couchdb_doc> &vec = st_m_docs;
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
				couchdb_doc doc{ a };
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
			m_man.reset(new couchdb_manager{ *pClient,m_dbname });
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
			couchdb_manager *pMan = m_man.get();
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
			couchdb_manager *pMan = m_man.get();
			Assert::IsNotNull(pMan);
			std::vector<string_t> vv = pMan->get_all_databases_async().get();
			Assert::IsFalse(vv.empty());
		}//CouchDBManager_GetAllDatabases
		TEST_METHOD(CouchDBManager_GetUuids)
		{
			couchdb_manager *pMan = m_man.get();
			Assert::IsNotNull(pMan);
			size_t nCount = 10;
			std::vector<string_t> vv = pMan->get_uuids_async((int)nCount).get();
			size_t ns = vv.size();
			Assert::AreEqual(nCount, ns);
		}//CouchDBManager_GetUuids
		TEST_METHOD(CouchDBManager_CreateDeleteDatabase)
		{
			couchdb_manager *pMan = m_man.get();
			Assert::IsNotNull(pMan);
			string_t dbname = m_dbname;
			bool b = pMan->exists_database_async(dbname).get();
			if (!b) {
				b = pMan->create_database_async(dbname).get();
				Assert::IsTrue(b);
			}
			b = pMan->delete_database_async(dbname).get();
			Assert::IsTrue(b);
			b = pMan->exists_database_async(dbname).get();
			Assert::IsFalse(b);
		}//CouchDBManager_CreateDeleteDatabase
		TEST_METHOD(CouchDBManager_CreateDeleteDocument)
		{
			couchdb_manager *pMan = m_man.get();
			Assert::IsNotNull(pMan);
			string_t dbname = m_dbname;
			bool b = pMan->exists_database_async(dbname).get();
			if (!b) {
				b = pMan->create_database_async(dbname).get();
				Assert::IsTrue(b);
			}
			string_t sid(U("testid"));
			couchdb_doc doc = pMan->get_document_by_id_async(sid).get();
			if (doc.empty()) {
				update_response r = pMan->maintains_document_async(m_doc).get();
				Assert::IsTrue(r.ok());
				doc = pMan->get_document_by_id_async(sid).get();
				Assert::IsFalse(doc.empty());
			}
			Assert::IsTrue(doc.is_persisted());
			string_t sid2 = doc.id();
			Assert::AreEqual(sid, sid2);
			string_t srev = doc.version();
			Assert::IsFalse(srev.empty());
			string_t sRev2 = pMan->get_document_version_async(sid).get();
			Assert::AreEqual(srev, sRev2);
			infomap oMap{ doc.get_map() };
			oMap[U("ival")] = any{ (int)7890 };
			couchdb_doc xdoc{ oMap };
			update_response rsp1 = pMan->maintains_document_async(xdoc).get();
			Assert::IsTrue(rsp1.ok());
			update_response rsp2 = pMan->delete_document_async(doc).get();
			b = rsp2.ok();
			Assert::IsTrue(b);
			sRev2 = pMan->get_document_version_async(sid).get();
			Assert::IsTrue(sRev2.empty());
			doc = pMan->get_document_by_id_async(sid).get();
			Assert::IsTrue(doc.empty());
			//
		}//CouchDBManager_CreateDeleteDocument
		TEST_METHOD(CouchDBManager_CreateIndex)
		{
			couchdb_manager *pMan = m_man.get();
			Assert::IsNotNull(pMan);
			string_t dbname = m_dbname;
			bool b = pMan->exists_database_async(dbname).get();
			if (!b) {
				b = pMan->create_database_async(dbname).get();
				Assert::IsTrue(b);
			}
			string_t field(U("ival"));
			index_response rsp = pMan->create_index_async(dbname, field).get();
			if (rsp.ok()) {
				string_t sid = rsp.index_id();
				Assert::IsFalse(sid.empty());
				string_t srev = rsp.name();
				Assert::IsFalse(srev.empty());
				string_t srev2 = rsp.result();
				Assert::IsFalse(srev2.empty());
			}
		}//CouchDBManager_CreateIndex
		TEST_METHOD(CouchDBManager_MaintainsDocs)
		{
			couchdb_manager *pMan = m_man.get();
			Assert::IsNotNull(pMan);
			string_t dbname = m_dbname;
			bool b = pMan->exists_database_async(dbname).get();
			if (!b) {
				b = pMan->create_database_async(dbname).get();
				Assert::IsTrue(b);
			}
			string_t stype(U("testtype"));
			string_t keysigle(U("sigle"));
			query_filter filter{};
			filter.add_equals(U("type"), any{ stype });
			int nCount = pMan->find_documents_count_async(filter).get();
			Assert::IsTrue(nCount >= 0);
			if (nCount < 1) {
				const std::vector<couchdb_doc> &vec = st_m_docs;
				int nc = static_cast<int>(vec.size());
				Assert::IsTrue(nc > 0);
				std::vector<update_response> rr = pMan->maintains_documents_async(vec).get();
				nCount = pMan->find_documents_count_async(filter).get();
				Assert::AreEqual(nc, nCount);
			}
			std::vector<couchdb_doc> alldocs{};
			int pagesize = 10;
			int offset = 0;
			filter.set_limit(pagesize);
			while (offset < nCount) {
				filter.set_skip(offset);
				std::vector<couchdb_doc> docs = pMan->find_documents_async(filter).get();
				int nx = static_cast<int>(docs.size());
				offset += nx;
				for (auto p : docs) {
					ostringstream_t os{};
					os << p;
					string_t s = os.str();
					Logger::WriteMessage(s.c_str());
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
			Assert::AreEqual(nt, nx);
			int nr = pMan->find_documents_count_async(filter).get();
			Assert::IsTrue(nr == 0);
		}//CouchDBManager_MaintainsDocs
		TEST_METHOD(CouchDBManager_Attachment)
		{
			couchdb_manager *pMan = m_man.get();
			Assert::IsNotNull(pMan);
			string_t dbname = m_dbname;
			bool b = pMan->exists_database_async(dbname).get();
			if (!b) {
				b = pMan->create_database_async(dbname).get();
				Assert::IsTrue(b);
			}
			string_t sid(U("testid"));
			couchdb_doc doc = pMan->get_document_by_id_async(sid).get();
			if (doc.empty()) {
				update_response r = pMan->maintains_document_async(m_doc).get();
				Assert::IsTrue(r.ok());
				doc = pMan->get_document_by_id_async(sid).get();
				Assert::IsFalse(doc.empty());
			}
			Assert::IsTrue(doc.is_persisted());
			string_t filename(U("D:\\testdata\\test.jpg"));
			blob_data blob{ filename };
			b = blob.ok();
			Assert::IsTrue(b);
			string_t attname = blob.name();
			update_response rsp = pMan->update_document_attachment_async(doc, blob).get();
			Assert::IsTrue(rsp.ok());
			doc = pMan->get_document_by_id_async(sid).get();
			std::vector<attachment_info> vec{};
			size_t nu = doc.attachments(vec);
			Assert::IsTrue(nu > 0);
			for (auto p : vec) {
				string_t xu = p.url();
				Assert::IsTrue(!xu.empty());
				string_t xc = p.content_type();
				string_t s = p.toString();
				Logger::WriteMessage(s.c_str());
			}//p
			std::shared_ptr<blob_data> bb = pMan->read_document_attachment_async(doc, attname).get();
			blob_data *pb = bb.get();
			Assert::IsNotNull(pb);
			bool bz = pb->ok();
			Assert::IsTrue(pb->ok());
			rsp = pMan->delete_document_attachment_async(doc, attname).get();
			Assert::IsTrue(rsp.ok());
			std::shared_ptr<blob_data> bb2 = pMan->read_document_attachment_async(doc, attname).get();
			blob_data *pb2 = bb2.get();
			Assert::IsFalse(pb2->ok());
		}// CouchDBManager_Attachment
	};
	////////////////////////////
	std::vector<couchdb_doc> CouchDBManagerTest::st_m_docs;
	/////////////////////////
}