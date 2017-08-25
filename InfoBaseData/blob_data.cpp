#include "blob_data.h"
#include "stringutils.h"
/////////////////////////////
namespace info {
///////////////////////////////
#ifdef _MSC_VER
static const string_t STRING_FILEDELIM(U("\\"));
#else
static const string_t STRING_FILEDELIM(U("/"));
#endif
///////////////////////////
blob_data::blob_data():m_size(0)
{
}
blob_data::blob_data(const string_t &filename):m_size(0) {
    try {
        std::string sfile = stringt_to_string(filename);
        std::ifstream file { sfile,std::ios::in | std::ios::binary | std::ios::ate};
        if (file.is_open()) {
            size_t n = static_cast<size_t>(file.tellg());
            if (n > 0) {
                std::shared_ptr<byte> xdata { new byte[n] };
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
                    string_t sext {}, sname {};
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
                    if ((sext == U("jpg")) || (sext == U("jpeg"))) {
                        mime_type(U("image/jpeg"));
                    }
                    else if (sext == U("png")) {
                        mime_type(U("image/png"));
                    }
                    else if (sext == U("json")) {
                        mime_type(U("application/json"));
                    }
                    else if (sext == U("csv")) {
                        mime_type(U("text/csv"));
                    }
                    else if (sext == U("txt")) {
                        mime_type(U("text/plain"));
                    }
                    else if (sext == U("html")) {
                        mime_type(U("text/html"));
                    }
                    else if (sext == U("pdf")) {
                        mime_type(U("application/pdf"));
                    }
                    else if (sext == U("doc")) {
                        mime_type(U("application/msword"));
                    } else if (sext == U("odt")) {
                        mime_type(U("application/vnd.oasis.opendocument.text"));
                    }
                    else if (sext == U("ods")) {
                        mime_type(U("application/vnd.oasis.opendocument.spreadsheet"));
                    }
                    else if (sext == U("odp")) {
                        mime_type(U("application/vnd.oasis.opendocument.presentation"));
                    }
                    else if (sext == U("ppt")) {
                        mime_type(U("application/vnd.ms-powerpoint"));
                    }
                    else if (sext == U("rtf")) {
                        mime_type(U("application/rtf"));
                    }
                    else if (sext == U("svg")) {
                        mime_type(U("image/svg+xml"));
                    }
                    else if (sext == U("xls")) {
                        mime_type(U("application/vnd.ms-excel"));
                    }
                    else if (sext == U("xlsx")) {
                        mime_type(U("application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"));
                    } else if (sext == U("xml")) {
                        mime_type(U("application/xml"));
                    } else if (sext == U("zip")) {
                        mime_type(U("application/zip"));
                    } else if (sext == U("rar")) {
                        mime_type(U("application/x-rar-compressed"));
                    }
                    else {
                        mime_type(U("application/octet-stream"));
                    }
                }// p
            }// n
        }// file
    } catch(std::exception & /*ex*/) {}
}
void blob_data::mime_type(const string_t &s) {
    string_t delim(U(","));
    stringutils::split_vector(s, m_mimes, delim, false);

}// mime_type
blob_data::blob_data(size_t n):m_size(0) {
    if (n > 0) {
        m_data.reset(new byte[n]);
    }
}
blob_data::blob_data(const string_t &sname, const string_t &smime, const std::vector<byte> &d,
                     const string_t &sid /*= string_t{}*/):m_size(0),m_id(sid),m_name(sname) {
    mime_type(smime);
    data(d);
}
blob_data::blob_data(const blob_data &other):m_size(other.m_size),m_id(other.m_id),m_rev(other.m_rev),m_name(other.m_name),
    m_data(other.m_data),m_mimes(other.m_mimes) {

}
blob_data & blob_data::operator=(const blob_data &other) {
    if (this != &other) {
        m_size = other.m_size;
        m_id = other.m_id;
        m_rev = other.m_rev;
        m_name = other.m_name;
        m_data = other.m_data;
        m_mimes = other.m_mimes;
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
