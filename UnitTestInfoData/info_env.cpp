#include "stdafx.h"
#include "info_env.h"
//////////////////////////////
#include <http_manager.h>
#include <info_etudiant.h>
/////////////////////////
namespace info {
	namespace test {
		/////////////////////////////////
		using namespace info::http;
		using namespace info::domain;
		using namespace info::couchdb;
		//////////////////////////////////
		const string_t InfoEnv::m_url{ U("http://localhost:5984") };
		const string_t InfoEnv::m_dbname{ U("xxtest") };
		const string_t InfoEnv::m_username;
		const string_t InfoEnv::m_password;
		const string_t InfoEnv::m_testype{ U("testtype") };
		const string_t InfoEnv::m_testid{ U("testid") };
#ifdef _MSC_VER
		const string_t InfoEnv::m_avatarfilename{ U("D:\\testdata\\test.jpg") };
		const string_t InfoEnv::m_csvfilename{ U("D:\\testdata\\Etuds_s4.csv") };
#else
		const string_t InfoEnv::m_avatarfilename{ U("/home/boubad/testdata/test.jpg") };
		const string_t InfoEnv::m_csvfilename{ U("/home/boubad/testdata/Etuds_s4.csv") };
#endif // _MSC_VER
		//////////////////////////////////
		std::shared_ptr<http_client> InfoEnv::get_http_client(void) {
			return std::shared_ptr<http_client>(new http_manager{ m_url,m_username,m_password });
		}
		std::shared_ptr<couchdb_storemanager> InfoEnv::get_storemanager(http_client &client) {
			return std::shared_ptr<couchdb_storemanager>(new couchdb_storemanager{ client,m_dbname });
		}
		////////////////////////////////
		void InfoEnv::SetUp() {
			std::shared_ptr<http_client> client = get_http_client();
			assert(client.get() != nullptr);
			std::shared_ptr<couchdb_storemanager> oMan = get_storemanager(*client);
			assert(oMan.get() != nullptr);
			if (!oMan->exists_database(m_dbname)) {
				bool b = oMan->create_database(m_dbname);
				assert(b);
			}
			bool bRet = info_etudiant::check_indexes_async(*oMan).get();
			assert(bRet);
		}// SetUp
		void InfoEnv::TearDown() {

		}// TearDown
		const string_t & InfoEnv::test_type(void) {
			return m_testype;
		}
		const string_t & InfoEnv::test_id(void) {
			return m_testid;
		}
		const string_t & InfoEnv::avatar_filename(void) {
			return m_avatarfilename;
		}
		const string_t & InfoEnv::csv_filename(void) {
			return m_csvfilename;
		}
		const string_t & InfoEnv::server_url(void) {
			return m_url;
		}
		const string_t & InfoEnv::username(void) {
			return m_username;
		}
		const string_t & InfoEnv::password(void) {
			return m_password;
		}
		const string_t & InfoEnv::dbname(void) {
			return m_dbname;
		}
		////////////////////////////////////
	}// namespace test
}// namespace info