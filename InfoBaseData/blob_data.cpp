#include "blob_data.h"
/////////////////////////////
namespace info {
	///////////////////////////
	blob_data::blob_data():m_size(0)
	{
	}
	blob_data::blob_data(const string_t &sname, const string_t &smime, const std::vector<byte> &d,
		const string_t &sid /*= string_t{}*/):m_size(0),m_id(sid),m_name(sname),m_mime(smime) {
		data(d);
	}
	blob_data::blob_data(const blob_data &other):m_size(other.m_size),m_id(other.m_id),m_rev(other.m_rev),m_name(other.m_name),
	m_mime(other.m_mime),m_data(other.m_data){

	}
	blob_data & blob_data::operator=(const blob_data &other) {
		if (this != &other) {
			m_size = other.m_size;
			m_id = other.m_id;
			m_rev = other.m_rev;
			m_name = other.m_name;
			m_mime = other.m_mime;
			m_data = other.m_data;
		}
		return (*this);
	}

	blob_data::~blob_data()
	{
	}
	void blob_data::data(const byte *pData, size_t nLength) {
		if ((pData == nullptr) || (nLength < 1)) {
			m_size = 0;
			m_data.reset();
		}
		else {
			m_size = nLength;
			byte *px = const_cast<byte *>(pData);
			m_data.reset(px);
		}
	}
	void blob_data::data(const std::vector<byte> &vec) {
		size_t n = vec.size();
		if (n < 1) {
			m_size = 0;
			m_data.reset();
		}
		else {
			m_data.reset(new byte[n]);
			byte *p = m_data.get();
			if (p != nullptr) {
				m_size = n;
				for (size_t i = 0; i < n; ++i) {
					p[i] = vec[i];
				}
			}
			else {
				m_size = 0;
				m_data.reset();
			}
		}
	}
//////////////////////////
}// namespace info
