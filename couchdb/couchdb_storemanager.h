#pragma once
#ifndef COUCHDB_STOREMANAGER_H__
#define COUCHDB_STOREMANAGER_H__
////////////////////////////////
#include <datastore_manager.h>
#include <http_client.h>
#include <update_response.h>
/////////////////////////////////
namespace info {
	class info_document;
	namespace couchdb {
		///////////////////////
		class server_info;
		class index_response;
		////////////////////////
		class couchdb_storemanager : public datastore_manager
		{
		private:
			couchdb_storemanager(const couchdb_storemanager &other) = delete;
			couchdb_storemanager & operator=(const couchdb_storemanager &other) = delete;
			void post_create(void);
			void check_databasename(void);
			string_t form_attachment_url(const string_t &docid, const string_t &name);
		private:
			http_client &m_client;
			string_t m_dbname;
			string_t m_version;
		public:
			couchdb_storemanager(http_client &client, const databasename &name = databasename{});
			~couchdb_storemanager();
			const string_t & database_name(void) const;
			void database_name(const databasename &name);
			const string_t & version(void) const;
			bool can_use_mango(void) const;
		public:
			bool get_server_info(server_info &info);
			//
			bool get_all_databases(std::vector<string_t> &vec);
			bool get_uuids(std::vector<string_t> &vec,int nCount = 1);
			bool exists_database(const string_t &dbname);
			bool create_database(const string_t &dbname);
			bool delete_database(const string_t &dbname);
			//
			bool create_index(const string_t &field, index_response &res,
				const string_t &ind_name = string_t{},
				const string_t &stype = U("json"),
				const string_t &ddoc = string_t{});
			//
			bool get_document_version(const string_t &docid, string_t &sver);
			bool get_document_by_id(const string_t &docid, info_document &res);
			bool delete_document(const info_document &doc,update_response &res);
			bool maintains_document(info_document &doc,update_response &res);
			bool create_document(info_document &doc, update_response &res);
			bool update_document(info_document &doc, update_response &res);
			//
			bool find_documents_count(const query_filter &filter,int &nRes);
			bool find_documents(const query_filter &filter, std::vector<info_document> &vec);
			bool maintains_documents(const std::vector<info_document> &vec,
				std::vector<update_response> &vecRes);
			bool read_attachment(const string_t &docid, 
				const string_t &name, blob_data &res);
			bool update_attachment(blob_data &blob, update_response &res);
			bool delete_attachment(blob_data &blob, update_response &res);
		public:
			virtual string_t get_base_url(void) override;
			virtual std::future<bool> check_field_index_async(const string_t &fieldname) override;
			virtual std::future<int> find_documents_count_async(const query_filter &filter) override;
			virtual std::future<std::vector<infomap>> query_documents_async(const query_filter &filter) override;
			virtual std::future<infomap> find_document_async(const infomap &parameters) override;
			virtual std::future<infomap> maintains_document_async(const infomap &doc) override;
			virtual std::future<bool> remove_document_async(const infomap &doc) override;
			virtual std::future<std::vector<bool>> maintains_documents_async(const std::vector<infomap> &docs) override;
			virtual std::future<bool> maintains_blob_async(const blob_data &blob) override;
			virtual std::future<blob_data> read_blob_async(const blob_data &blob) override;
			virtual std::future<bool> remove_blob_async(const blob_data &blob) override;
		};
		//////////////////////////
	}// namespace couchdb
}// namespace info
////////////////////////////
#endif // COUCHDB_STOREMANAGER_H__
