#pragma once
#ifndef INDEX_RESPONSE_H__
#define INDEX_RESPONSE_H__
//////////////////////////////
#include <infobaseobject.h>
//////////////////////////////
namespace info {
	namespace couchdb {
		//////////////////////////////
		class index_response : public infobaseobject
		{
		public:
			index_response();
			index_response(const any &va);
			index_response(const index_response &other);
			index_response & operator=(const index_response &other);
			virtual ~index_response();
		public:
			bool ok(void) const;
			int status_code(void) const;
			void status_code(int n);
			string_t result(void) const;
			string_t index_id(void) const;
			string_t name(void) const;
		};// class index_response
	}// namespace couchdb
}// namespace info
//////////////////////////////
#endif // INDEX_RESPONSE_H__
