#include "blob_data.h"
#include "stringutils.h"
#include "attachment_info.h"
/////////////////////////////
namespace info {
	///////////////////////////////
#ifdef _MSC_VER
	static const string_t STRING_FILEDELIM(U("\\"));
#else
	static const string_t STRING_FILEDELIM(U("/"));
#endif
	static const string_t APPLICATION_OCTETSTREAM(U("application/octet-stream"));
	///////////////////////////
	blob_data::blob_data() :m_persisted(false),m_size(0)
	{
	}
	blob_data::blob_data(const string_t &filename) : m_persisted(false),m_size(0) {
		try {
			std::string sfile = stringt_to_string(filename);
			std::ifstream file{ sfile,std::ios::in | std::ios::binary | std::ios::ate };
			if (file.is_open()) {
				size_t n = static_cast<size_t>(file.tellg());
				if (n > 0) {
					std::shared_ptr<byte> xdata{ new byte[n] };
					char *p = (char *)xdata.get();
					if (p != nullptr) {
						file.seekg(0, std::ios::beg);
						file.read(p, n);
						file.close();
						m_data = xdata;
						m_size = n;
						string_t s = stringutils::tolower(filename);
						size_t nl = s.length();
						size_t pos = s.find_last_of(U("."));
						string_t sext{}, sname{};
						if (pos != string_t::npos) {
							sext = s.substr(pos + 1, nl - pos - 1);
							size_t xpos = s.find_last_of(STRING_FILEDELIM);
							if (xpos != string_t::npos) {
								sname = s.substr(xpos + 1, pos - xpos - 1);
							}
							else {
								sname = s.substr(0, pos);
							}
						}
						m_name = sname;
						mime_type(fetch_mime_type(sext));
					}// p
				}// n
			}// file
		}
		catch (std::exception & /*ex*/) {}
	}
	blob_data::blob_data(const string_t &sid, const string_t &sname, const any &info):m_persisted(false),m_size(0) {
		attachment_info o{ info };
		string_t sc = o.content_type();
		int nLen = o.length();
		if ((nLen > 0) && (!sc.empty())) {
			m_persisted = true;
			m_id = sid;
			m_mime = sc;
			m_size = nLen;
			m_name = sname;
		}
	}
	blob_data::blob_data(size_t n) :m_persisted(false),m_size(0) {
		if (n > 0) {
			m_data.reset(new byte[n]);
		}
	}
	
	blob_data::blob_data(const blob_data &other) :m_persisted(other.m_persisted), m_size(other.m_size), m_id(other.m_id), m_name(other.m_name),
		m_url(other.m_url),m_mime(other.m_mime),m_data(other.m_data) {

	}
	blob_data & blob_data::operator=(const blob_data &other) {
		if (this != &other) {
			m_persisted = other.m_persisted;
			m_size = other.m_size;
			m_id = other.m_id;
			m_name = other.m_name;
			m_url = other.m_url;
			m_data = other.m_data;
		}
		return (*this);
	}

	blob_data::~blob_data()
	{
	}
	string_t blob_data::fetch_mime_type(const string_t &s) {
		static std::vector<std::pair<string_t, string_t>> stMap{
			std::make_pair(U("jpg"),U("image/jpeg")),
			std::make_pair(U("jpeg"),U("image/jpeg")),
			std::make_pair(U("png"),U("image/png")),
			std::make_pair(U("svg"),U("image/svg+xml")),
			std::make_pair(U("pdf"),U("application/pdf")),
			std::make_pair(U("json"),U("application/json")),
			std::make_pair(U("csv"),U("text/csv")),
			std::make_pair(U("txt"),U("text/plain")),
			std::make_pair(U("rtf"),U("application/rtf")),
			std::make_pair(U("html"),U("text/html")),
			std::make_pair(U("htm"),U("text/html")),
			std::make_pair(U("doc"),U("application/msword")),
			std::make_pair(U("ppt"),U("application/vnd.ms-powerpoint")),
			std::make_pair(U("xls"),U("application/vnd.ms-excel")),
			std::make_pair(U("xlsx"),U("application/vnd.openxmlformats-officedocument.spreadsheetml.sheet")),
			std::make_pair(U("odt"),U("application / vnd.oasis.opendocument.text")),
			std::make_pair(U("ods"),U("application / vnd.oasis.opendocument.spreadsheet")),
			std::make_pair(U("odp"),U("application / vnd.oasis.opendocument.presentation")),
			std::make_pair(U("xml"),U("application/xml")),
			std::make_pair(U("zip"),U("application/zip")),
			std::make_pair(U("rar"),U("application/x-rar-compressed"))
		};
		string_t sRet{};
		string_t sx = stringutils::tolower(stringutils::trim(s));
		if (sx.length() > 1) {
			if (sx[0] == U('.')) {
				sx = sx.substr(1, sx.length() - 1);
			}
			for (auto p : stMap) {
				if (p.first == sx) {
					sRet = p.second;
					break;
				}
			}// p
		}
		if (sRet.empty()) {
			sRet = APPLICATION_OCTETSTREAM;
		}
		return (sRet);
	}//fetch_mime_type
	string_t blob_data::mime_type(void) const {
		string_t sRet{ m_mime };
		if (sRet.empty()) {
			sRet = APPLICATION_OCTETSTREAM;
		}
		return (sRet);
	}
	void blob_data::mime_type(const string_t &s) {
		static string_t delim(U(","));
		std::vector<string_t> vec{};
		stringutils::split_vector(s, vec, delim, false);
		m_mime.clear();
		for (auto p : vec) {
			string_t sk = stringutils::trim(p);
			if (sk != APPLICATION_OCTETSTREAM) {
				m_mime = sk;
			}
		}
	}// mime_type
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
	//
	bool blob_data::is_persisted(void) const {
		return m_persisted;
	}
	bool blob_data::is_storable(void) const {
		return (size() > 0) && (m_data.get() != nullptr) && (m_mime.length() > 0);
	}
	bool blob_data::has_url(void) const {
		string_t s = url();
		return (!s.empty());
	}
	
	bool blob_data::has_data(void) const {
		return (size() > 0) && (m_data.get() != nullptr);
	}
	size_t blob_data::size(void) const {
		return (m_size);
	}
	const blob_data::byte *blob_data::data(size_t & nLength) const {
		nLength = m_size;
		return (m_data.get());
	}
	blob_data::byte *blob_data::data(size_t & nLength) {
		nLength = m_size;
		return (m_data.get());
	}
	const string_t & blob_data::id(void) const {
		return (m_id);
	}
	void blob_data::id(const string_t &s) {
		m_id = s;
	}
	const string_t &blob_data::name(void) const {
		return (m_name);
	}
	void blob_data::name(const string_t &s) {
		m_name = s;
	}
	
	const string_t &blob_data::url(void) const {
		return (m_url);
	}
	void blob_data::url(const string_t &s) {
		m_url = s;
	}
	
	//////////////////////////
}// namespace info
