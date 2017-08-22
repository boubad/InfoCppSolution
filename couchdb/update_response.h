#pragma once
#ifndef UPDATE_RESPONSE_H__
#define UPDATE_RESPONSE_H__
/////////////////////////
#include <infobaseobject.h>
//////////////////////////////
namespace info {
	namespace couchdb {
		/////////////////////////
		class update_response : public infobaseobject
		{
		public:
			update_response();
			update_response(const any &val);
			update_response(const update_response &other);
			update_response & operator=(const update_response &other);
			virtual ~update_response();
		public:
			bool ok(void) const;
			int status_code(void) const;
			void status_code(int n);
			string_t id(void) const;
			string_t rev(void) const;
			string_t error(void) const;
			string_t reason(void) const;
		};// class update_response
		///////////////////////////
	}// namespace couchdb
}// namespace info
/////////////////////////////
#endif // UPDATE_RESPONSE_H__
