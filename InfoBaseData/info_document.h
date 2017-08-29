#pragma once
#ifndef INFO_DOCUMENT_H__
#define INFO_DOCUMENT_H__
/////////////////////////////
#include "infobaseobject.h"
#include "info_fielddesc.h"
#include "blob_data.h"
//////////////////////////////
namespace info {
	/////////////////////////////////////
	class datastore_manager;
	//////////////////////////
	class info_document : public infobaseobject
	{
	public:
		static const string_t KEY_TYPE;
	public:
		info_document();
		info_document(const infomap &oMap);
		info_document(const any &va);
		info_document(const info_document &other);
		info_document & operator=(const info_document &other);
		virtual ~info_document();
	protected:
		std::vector<blob_data> m_blobs;
		void type(const string_t &s);
	protected:
		virtual void post_init(void);
		virtual void unique_properties(std::vector<std::vector<string_t>> &vvec) const;
	public:
		virtual bool is_storable(void) const;
		virtual std::future<bool> load(datastore_manager &oMan);
		virtual std::future<bool> save(datastore_manager &oMan);
		virtual std::future<bool> remove(datastore_manager &oMan);
		virtual void get_descs(std::set<info_fielddesc> &oSet) const;
		//
	public:
		void set_deleted(bool b = true);
		virtual void set(const any &oMap) override;
		void check_blobs_urls(const string_t &baseurl);
		const std::vector<blob_data> & blobs(void) const;
		std::vector<blob_data> & blobs(void);
		string_t  id(void) const;
		void id(const string_t &s);
		string_t version(void) const;
		void version(const string_t &s);
		bool is_persisted(void) const;
		string_t type(void) const;
		bool load_by_id(datastore_manager &oMan, info_document &doc) const;
		bool load_unique(datastore_manager &oMan, info_document &doc) const;
	};// class info_document
}// namespace info
/////////////////////////
#endif // INFO_DOCUMENT_H__
