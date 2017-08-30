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
#include "info_env.h"
/////////////////////////////
//
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTestInfoData
{
	using namespace info;
	using namespace info::http;
	using namespace info::couchdb;
	using namespace info::domain;
	using namespace info::test;
	///////////////////////////
	TEST_CLASS(EtudManagerTest)
	{
	protected:
		std::shared_ptr<http_client> m_client;
		std::shared_ptr<couchdb_storemanager> m_man;
	public:
		
	public:
		TEST_METHOD_INITIALIZE(SetUp)
		{
			m_client = InfoEnv::get_http_client();
			Assert::IsTrue(m_client.get() != nullptr);
			m_man = InfoEnv::get_storemanager(*m_client);
			Assert::IsTrue(m_man.get() != nullptr);
			//
		}// SetUp
		TEST_METHOD_CLEANUP(TearDown)
		{
			m_man.reset();
			m_client.reset();
		}// TearDown
	public:

		TEST_METHOD(EtudManagerTest_ImportEtuds)
		{
			string_t filename(InfoEnv::csv_filename());
			ifstream_t in{ filename };
			Assert::IsTrue(in.is_open());
			etud_importer oImport{ in };
			bool b = oImport.import();
			Assert::IsTrue(b);
			std::vector<any> vv = oImport.get_items();
			Assert::IsTrue(!vv.empty());
		}//EtudManagerTest_ImportEtuds
		TEST_METHOD(EtudManagerTest_SaveImportEtuds)
		{
			string_t filename(InfoEnv::csv_filename());
			ifstream_t in{ filename };
			Assert::IsTrue(in.is_open());
			etud_importer oImport{ in };
			bool b = oImport.import();
			Assert::IsTrue(b);
			std::vector<any> vv = oImport.get_items();
			Assert::IsTrue(!vv.empty());
			//
			couchdb_storemanager *pMan = m_man.get();
			Assert::IsNotNull(pMan);
			std::vector<info_etudiant> etuds{};
			for (auto va : vv) {
				info_etudiant oEtud{ va };
				b = oEtud.load(*pMan).get();
				if (!b) {
					b = oEtud.save(*pMan).get();
					Assert::IsTrue(b);
				}
				etuds.push_back(oEtud);
			}// va
			query_filter filter{};
			int nPageSize{ 10 };
			filter.set_limit(nPageSize);
			//filter.add_sort(info_etudiant::KEY_FULLNAME);
			//	filter.add_projection_field(info_etudiant::KEY_FULLNAME);
			//filter.add_projection_field(info_etudiant::KEY_DOSSIER);
			//filter.add_projection_field(info_etudiant::KEY_ID);
			int nTotal = info_etudiant::get_count_async(*pMan, filter).get();
			int nOffset{ 0 };
			while (nOffset < nTotal) {
				filter.set_skip(nOffset);
				std::vector<info_etudiant> xx = info_etudiant::get_async(*pMan, filter).get();
				int nx = static_cast<int>(xx.size());
				nOffset += nx;
				for (auto x : xx) {
					string_t s = x.toString();
					Logger::WriteMessage(s.c_str());
				}// x
				if (nx < nPageSize) {
					break;
				}
			}// nOffset
		}//EtudManagerTest_SaveImportEtuds
		TEST_METHOD(EtudManagerTest_ReadEtuds)
		{
			couchdb_storemanager *pMan = m_man.get();
			Assert::IsNotNull(pMan);
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
					string_t s = x.toString();
					Logger::WriteMessage(s.c_str());
				}// x
				if (nx < nPageSize) {
					break;
				}
			}// nOffset
		}//EtudManagerTest_ReadEtuds

	};
	/////////////////////////
}