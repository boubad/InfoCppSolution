#pragma once
#ifndef COUCHDB_MANAGER_H__
#define COUCHDB_MANAGER_H__
//////////////////////////////
#include <http_client.h>
////////////////////////////
#include "query_filter.h"
#include "couchdb_doc.h"
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
			string_t	m_dbname;
			//
			void check_databasename(void);
		public:
			couchdb_manager(http_client &client, const databasename &name = databasename{});
			virtual ~couchdb_manager();
			const string_t & database_name(void) const;
			void database_name(const databasename &name);
		public:
			bool get_server_info(server_info &info);
			std::future<std::vector<string_t>> get_all_databases_async(void);
			std::future<std::vector<string_t>> get_uuids_async(int nCount = 1);
			std::future<bool> exists_database_async(const string_t &dbname);
			std::future<bool> create_database_async(const string_t &dbname);
			std::future<bool> delete_database_async(const string_t &dbname);
			std::future<update_response> create_document_async(const couchdb_doc &doc);
			std::future<update_response> delete_document_async(const couchdb_doc &doc);
			std::future<string_t> get_document_version_async(const string_t &docid);
			std::future<index_response> create_index_async(const string_t &field,
				const string_t &ind_name = string_t{},
				const string_t &stype = U("json"),
				const string_t &ddoc = string_t{});
			std::future<couchdb_doc> get_document_by_id_async(const string_t &docid, bool bAttach = true);
			std::future<update_response> update_document_async(const couchdb_doc &doc);
			std::future<int> find_documents_count_async(const query_filter &filter);
			std::future<std::vector<couchdb_doc>> find_documents_async(const query_filter &filter);
			std::future<std::vector<update_response>> maintains_documents_async(const std::vector<couchdb_doc> &vec);
		};// class couchdb_manager
		//////////////////////////////
	}// namespace couchdb
}// namespace info
/////////////////////////////
#endif // COUCHDB_MANAGER_H__
