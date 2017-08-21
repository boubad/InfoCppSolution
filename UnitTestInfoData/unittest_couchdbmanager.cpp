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
		string_t m_username;
		string_t m_password;
		std::shared_ptr<http_client> m_httpclient;
		std::shared_ptr<couchdb_manager> m_man;
		any m_doc;
	public:
		TEST_METHOD_INITIALIZE(SetUp)
		{
			m_serverurl = serverurl{ U("http://localhost:5984") };
			m_httpclient.reset(new http_manager{ m_serverurl,m_username,m_password });
			http_client *pClient = m_httpclient.get();
			Assert::IsNotNull(pClient);
			m_man.reset(new couchdb_manager{ *pClient });
			Assert::IsNotNull(m_man.get());
			//
			infomap oMap{};
			string_t textval(U("string testvalue"));
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
			string_t dbname(U("xxtest"));
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
			string_t dbname(U("xxtest"));
			bool b = pMan->exists_database_async(dbname).get();
			if (!b) {
				b = pMan->create_database_async(dbname).get();
				Assert::IsTrue(b);
			}
			any doc{ m_doc };
			//
			update_response rsp = pMan->create_document_async(dbname, doc).get();
			if (rsp.ok()) {
				string_t sid = rsp.id();
				Assert::IsFalse(sid.empty());
				string_t srev = rsp.rev();
				Assert::IsFalse(srev.empty());
				//
				string_t sRev2 = pMan->get_document_version_async(dbname, sid).get();
				Assert::AreEqual(srev, sRev2);
				update_response rsp2 = pMan->delete_document_async(dbname, sid, srev).get();
				b = rsp2.ok();
				Assert::IsTrue(b);
			}
		}//CouchDBManager_CreateDeleteDocumente
	};
}