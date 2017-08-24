#pragma once
#ifndef SERVER_INFO_H__
#define SERVER_INFO_H__
//////////////////////////////
#include <infobaseobject.h>
////////////////////////////
namespace info {
	namespace couchdb {
		///////////////////
		class server_info : public infobaseobject
		{
		public:
			server_info();
			server_info(const any &val);
			server_info(const server_info &other);
			server_info & operator=(const server_info &other);
			virtual ~server_info();
			operator bool(void) const { return ok(); }
		public:
			bool ok(void) const;
			string_t welcome_string(void) const;
			string_t uuid(void) const;
			string_t version(void) const;
			void features(std::vector<string_t> &vv) const;
			string_t vendor_name(void) const;
			string_t vendor_version(void) const;
		};
	}// namespace couchdb
}// namespace info
////////////////////////////
#endif // SERVER_INFO_H__
