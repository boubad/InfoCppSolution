#pragma once
#ifndef BLOB_DATA_H__
#define BLOB_DATA_H__
//////////////////////////
#include "InfoDefs.h"
///////////////////////////
namespace info {
	class blob_data
	{
	public:
		using byte = unsigned char;
	private:
		size_t  m_size;
		string_t m_id;
		string_t m_rev;
		string_t m_name;
		string_t m_url;
		std::shared_ptr<byte> m_data;
		std::vector<string_t> m_mimes;
	public:
		blob_data();
		blob_data(const string_t &filename);
		blob_data(size_t n);
		blob_data(const string_t &sname, const string_t &smime, const std::vector<byte> &d,
			const string_t &sid = string_t{});
		blob_data(const blob_data &other);
		blob_data & operator=(const blob_data &other);
		virtual ~blob_data();
		operator bool(void) const { return ok(); }
	public:
		bool ok(void) const {
			return (!m_name.empty()) && (!m_mimes.empty()) && (m_size > 0) && (m_data.get() != nullptr);
		}// ok
		size_t size(void) const {
			return m_size;
		}
		const byte *data(size_t & nLength) const {
			nLength = m_size;
			return (m_data.get());
		}
		byte *data(size_t & nLength)  {
			nLength = m_size;
			return (m_data.get());
		}
		const string_t &id(void) const {
			return m_id;
		}
		void id(const string_t &s) {
			m_id = s;
		}
		const string_t &version(void) const {
			return m_rev;
		}
		void version(const string_t &s) {
			m_rev = s;
		}
		const string_t &name(void) const {
			return m_name;
		}
		void name(const string_t &s) {
			m_name = s;
		}
		string_t mime_type(void) const {
			return (m_mimes.empty()) ? string_t{} : m_mimes[0];
		}
		void mime_type(const string_t &s);
		const string_t &url(void) const {
			return m_url;
		}
		void url(const string_t &s) {
			m_url = s;
		}
		void data(const byte *pData, size_t nLength);
		void data(const std::vector<byte> &vec);
	};// class blob_data
}// namespace info
/////////////////////////////
#endif // BLOB_DATA_H__
