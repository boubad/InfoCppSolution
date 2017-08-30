#pragma once
#ifndef INFO_FIXTURE_H
#define INFO_FIXTURE_H
//////////////////////////
#include <InfoDefs.h>
///////////////////////////
namespace info {
	namespace test {
		///////////////////////////////
		class info_fixture
		{
		public:
			string_t  server_url;
			string_t  username;
			string_t  password;
			string_t  dbname;
		public:
			info_fixture();
			~info_fixture();
		}; // class info_fixture
		////////////////////////////
	}// namespace test
}// namespace info
//////////////////////////////////
#endif // INFO_FIXTURE_H
