#include "blob_data.h"
/////////////////////////////
namespace info {
	///////////////////////////
	blob_data::blob_data()
	{
	}
	blob_data::blob_data(const string_t &sname, const string_t &smime, const std::vector<byte> &d,
		const string_t &sid /*= string_t{}*/):m_id(sid),m_name(sname),m_mime(smime),m_data(d) {

	}
	blob_data::blob_data(const blob_data &other):m_id(other.m_id),m_name(other.m_name),
	m_mime(other.m_mime),m_data(other.m_data){

	}
	blob_data & blob_data::operator=(const blob_data &other) {
		if (this != &other) {
			m_id = other.m_id;
			m_name = other.m_name;
			m_mime = other.m_mime;
			m_data = other.m_data;
		}
		return (*this);
	}

	blob_data::~blob_data()
	{
	}
//////////////////////////
}// namespace info
