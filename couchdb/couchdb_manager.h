#pragma once
#ifndef COUCHDB_MANAGER_H__
#define COUCHDB_MANAGER_H__
//////////////////////////////
#include <http_client.h>
#include "update_response.h"
#include "index_response.h"
/////////////////////////////
namespace info {
	namespace couchdb {
		/////////////////////////
		class server_info;
		/////////////////////////
		class couchdb_manager
		{
		private:
			couchdb_manager(const couchdb_manager &other) = delete;
			couchdb_manager & operator=(const couchdb_manager &other) = delete;
			//
			http_client  &m_client;
		public:
			couchdb_manager(http_client &client);
			~couchdb_manager();
		public:
			bool get_server_info(server_info &info);
			std::future<std::vector<string_t>> get_all_databases_async(void);
			std::future<std::vector<string_t>> get_uuids_async(int nCount = 1);
			std::future<bool> exists_database_async(const string_t &dbname);
			std::future<bool> create_database_async(const string_t &dbname);
			std::future<bool> delete_database_async(const string_t &dbname);
			std::future<update_response> create_document_async(const string_t &dbname, const any &doc);
			std::future<update_response> delete_document_async(const string_t &dbname, 
				const string_t &sid, const string_t &srev);
			std::future<string_t> get_document_version_async(const string_t &dbname, const string_t &sid);
			std::future<index_response> create_index_async(const string_t &dbname, const string_t &field,
				const string_t &ind_name = string_t{},
				const string_t &stype = U("json"),
				const string_t &ddoc = string_t{});
		};// class couchdb_manager
		//////////////////////////////
	}// namespace couchdb
}// namespace info
/////////////////////////////
#endif // COUCHDB_MANAGER_H__
