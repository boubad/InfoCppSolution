#include "stdafx.h"
#include "CppUnitTest.h"
////////////////////////
#include <http_manager.h>
#include <couchdb_storemanager.h>
#include <info_etudiant.h>
//
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTestInfoData
{		
	using namespace info;
	using namespace info::http;
	using namespace info::couchdb;
	using namespace info::domain;
	///////////////////////////////
	static const serverurl m_url{ U("http://localhost:5984") };
	static const databasename m_dbname{U("xxtest")};
	const string_t m_username{};
	const string_t m_password{};
	//////////////////////////////////////////
	TEST_MODULE_INITIALIZE(ModuleInitialize)
	{
		//Logger::WriteMessage("In Module Initialize");
		http_manager client{m_url,m_username,m_password };
		couchdb_storemanager oMan{ client,m_dbname };
		bool b = oMan.exists_database(m_dbname);
		if (!b) {
			b = oMan.create_database(m_dbname);
			Assert::IsTrue(b);
		}
		bool bRet = info_etudiant::check_indexes_async(oMan).get();
		Assert::IsTrue(bRet);
	}

	TEST_MODULE_CLEANUP(ModuleCleanup)
	{
		//Logger::WriteMessage("In Module Cleanup");
	}
}