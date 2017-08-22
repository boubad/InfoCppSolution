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
		string_t m_id;
		string_t m_name;
		string_t m_mime;
		std::vector<byte> m_data;
	public:
		blob_data();
		blob_data(const string_t &sname, const string_t &smime, const std::vector<byte> &d,
			const string_t &sid = string_t{});
		blob_data(const blob_data &other);
		blob_data & operator=(const blob_data &other);
		~blob_data();
	public:
		const string_t &id(void) const {
			return m_id;
		}
		void id(const string_t &s) {
			m_id = s;
		}
		const string_t &name(void) const {
			return m_name;
		}
		void name(const string_t &s) {
			m_name = s;
		}
		const string_t &mime_type(void) const {
			return m_mime;
		}
		void mime_type(const string_t &s) {
			m_mime = s;
		}
		const std::vector<byte> &data(void) const {
			return m_data;
		}
		void data(const std::vector<byte> &vec) {
			m_data = vec;
		}
	};// class blob_data
}// namespace info
/////////////////////////////
#endif // BLOB_DATA_H__
