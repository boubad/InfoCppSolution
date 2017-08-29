#pragma once
#ifndef BLOB_DATA_H__
#define BLOB_DATA_H__
//////////////////////////
#include "InfoDefs.h"
///////////////////////////
namespace info {
	//////////////////////////////
	class blob_data 
	{
	public:
		using byte = unsigned char;
		static string_t fetch_mime_type(const string_t &sext);
	private:
		bool	m_persisted;
		size_t  m_size;
		string_t m_id;
		string_t m_name;
		string_t m_url;
		string_t m_mime;
		std::shared_ptr<byte> m_data;
	public:
		blob_data();
		blob_data(const string_t &filename);
		blob_data(size_t n);
		blob_data(const string_t &sid, const string_t &sname, const any &info);
		blob_data(const blob_data &other);
		blob_data & operator=(const blob_data &other);
		virtual ~blob_data();
	public:
		bool is_persisted(void) const;
		bool is_storable(void) const;
		bool has_url(void) const;
		bool has_data(void) const;
		size_t size(void) const;
		const byte *data(size_t & nLength) const;
		byte *data(size_t & nLength);
		const string_t &id(void) const;
		void id(const string_t &s);
		const string_t &name(void) const;
		void name(const string_t &s);
		string_t mime_type(void) const;
		void mime_type(const string_t &s);
		const string_t &url(void) const;
		void url(const string_t &s);
		void data(const byte *pData, size_t nLength);
		void data(const std::vector<byte> &vec);
	};// class blob_data
}// namespace info
/////////////////////////////
#endif // BLOB_DATA_H__
