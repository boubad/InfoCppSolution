#pragma once
#ifndef INFO_ENV_H__
#define INFO_ENV_H__
///////////////////////////
#include <InfoDefs.h>
#include <http_client.h>
#include <couchdb_storemanager.h>
/////////////////////////////////
namespace info {
	namespace test {
		//////////////////////////////
		class InfoEnv {
		private:
			static const string_t m_url;
			static const string_t m_dbname;
			static const string_t m_avatarfilename;
			static const string_t m_csvfilename;
			static const string_t m_username;
			static const string_t m_password;
			static const string_t m_testype;
			static const string_t m_testid;
		public:
			static void SetUp(void);
			static void TearDown(void);
		public:
			static std::shared_ptr<info::http_client> get_http_client(void);
			static std::shared_ptr<info::couchdb::couchdb_storemanager> get_storemanager(http_client &client);
		public:
			static const string_t & test_type(void);
			static const string_t & test_id(void);
			static const string_t & server_url(void);
			static const string_t & username(void);
			static const string_t & password(void);
			static const string_t & dbname(void);
			static const string_t & avatar_filename(void);
			static const string_t & csv_filename(void);
		}; //class InfoEnv
	}// namespace info
}// namespace info
///////////////////////////////
#endif // INFO_ENV_H__
