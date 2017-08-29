#pragma once
#ifndef DATASTORE_MANAGER_H__
#define  DATASTORE_MANAGER_H__
/////////////////////////////
#include "InfoDefs.h"
//////////////////////////////
namespace info {
	class query_filter;
	class blob_data;
	/////////////////////////
	class datastore_manager
	{
	private:
		datastore_manager(const datastore_manager &other) = delete;
		datastore_manager & operator=(const datastore_manager &other) = delete;
	protected:
		datastore_manager();
	public:
		virtual ~datastore_manager();
	public:
		virtual string_t get_base_url(void) = 0;
		virtual std::future<bool> check_field_index_async(const string_t &fieldname) = 0;
		virtual std::future<int> find_documents_count_async(const query_filter &filter) = 0;
		virtual std::future<std::vector<infomap>> query_documents_async(const query_filter &filter) = 0;
		virtual std::future<infomap> find_document_async(const infomap &parameters) = 0;
		virtual std::future<infomap> maintains_document_async(const infomap &doc) = 0;
		virtual std::future<bool> remove_document_async(const infomap &doc) = 0;
		virtual std::future<std::vector<bool>> maintains_documents_async(const std::vector<infomap> &docs) = 0;
		virtual std::future<bool> maintains_blob_async(const blob_data &blob) = 0;
		virtual std::future<blob_data> read_blob_async(const blob_data &blob) = 0;
		virtual std::future<bool> remove_blob_async(const blob_data &blob) = 0;
	};
	//////////////////////////////
}// namespace info
/////////////////////////////////
#endif //  DATASTORE_MANAGER_H__
