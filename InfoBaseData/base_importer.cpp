#include "base_importer.h"
#include "stringutils.h"
#include <algorithm>
////////////////////////////////
namespace info {
		////////////////////////////////////
		base_importer::base_importer(istream_t &in, const string_t &sdel) :m_in(in), m_delim(sdel) {
		}
		base_importer::~base_importer()
		{
		}
		const string_t & base_importer::delimiter(void) const {
			return (m_delim);
		}
		void base_importer::delimiter(const string_t &s) {
			m_delim = s;
		}
		const std::map<string_t, string_t> & base_importer::headers_map(void) const {
			return (m_dict);
		}
		std::map<string_t, string_t> & base_importer::headers_map(void) {
			return (m_dict);
		}
		string_t base_importer::transform_header(const string_t &s) {
			string_t sRet{ s };
			const std::map<string_t, string_t> &dict = headers_map();
			auto it = dict.find(s);
			if (it != dict.end()) {
				sRet = (*it).second;
			}
			return (sRet);
		}// transform_header
		const std::map<size_t, string_t> & base_importer::headers(void) const {
			return (m_headers);
		}
		std::map<size_t, string_t> & base_importer::headers(void) {
			return (m_headers);
		}

		const info_fielddesc *base_importer::find_desc(const string_t &fieldname) const {
			const info_fielddesc *pRet{ nullptr };
			const std::set<info_fielddesc> &vv = m_descs;
			auto it = std::find_if(vv.begin(), vv.end(), [fieldname](const info_fielddesc &p)->bool {
				return (p.import_name() == fieldname);
			});
			if (it != vv.end()) {
				pRet = &(*it);
			}
			return (pRet);
		}
		void base_importer::register_item(const infomap &  item) {
			if (!item.empty()) {
				infomap m{ item };
				any va{ m };
				m_data.push_back(va);
			}
		}
		bool base_importer::first(void) {
			optional<std::map<size_t, string_t>> vl = first_line_headers();
			if (!INFO_OPTIONAL_HAS_VALUE(vl)) {
				return (false);
			}
			m_headers = *vl;
			m_data.clear();
			return (true);
		}// first
		std::optional<string_t> base_importer::next_line(void) {
			istream_t &in = m_in;
			bool bDone{ false };
			while ((!in.eof()) && (!bDone)) {
				string_t s;
				std::getline(in, s);
				string_t ss = stringutils::trim(s);
				if (ss.empty()) {
					return  optional<string_t>{};
				}
				if (ss[0] == U('#')) {
					continue;
				}
				return (optional<string_t>(s));
			}// while
			return  optional<string_t>{};
		}
		optional<std::map<size_t, string_t>> base_importer::first_line_headers(void) {
			optional<string_t> vl = next_line();
			if (!INFO_OPTIONAL_HAS_VALUE(vl)) {
				return optional<std::map<size_t, string_t>>{};
			}
			std::map<size_t, string_t> hh{};
			std::vector<string_t> vv{};
			string_t line = *vl;
			stringutils::split_vector(line, vv, delimiter());
			size_t n = vv.size();
			for (size_t i = 0; i < n; ++i) {
				string_t s = vv[i];
				string_t ss = stringutils::trim(s);
				if (ss.empty()) {
					continue;
				}// ss
				string_t sval = transform_header(ss);
				hh[i] = stringutils::toupper(sval);
			}// i
			return (optional<std::map<size_t, string_t>>(hh));
		}
		optional<std::map<string_t, string_t>> base_importer::next_line_map(void) {
			optional<string_t> vl = next_line();
			if (!INFO_OPTIONAL_HAS_VALUE(vl)) {
				return optional<std::map<string_t, string_t>>{};
			}
			std::map<string_t, string_t> oRet{};
			std::vector<string_t> vv{};
			string_t line = *vl;
			stringutils::split_vector(line, vv, delimiter());
			size_t n = vv.size();
			const std::map<size_t, string_t> &hh = headers();
			for (size_t i = 0; i < n; ++i) {
				auto jt = hh.find(i);
				if (jt == hh.end()) {
					continue;
				}
				string_t s = vv[i];
				string_t ss = stringutils::trim(s);
				if (ss.empty()) {
					continue;
				}// ss
				string_t key = (*jt).second;
				oRet[key] = s;
			}// i
			return (optional<std::map<string_t, string_t >>(oRet));
		}// next_line_map
		void base_importer::process_line_items(const std::map<string_t, string_t> &lineData, infomap &item) const {
			for (auto it = lineData.begin(); it != lineData.end(); ++it) {
				const info_fielddesc *pDesc = find_desc((*it).first);
				if (pDesc != nullptr) {
					pDesc->apply_string_value((*it).second, item);
				}
			}// it
		}//process_line_items
		bool base_importer::next_item(infomap &oMap) {
			oMap.clear();
			optional<std::map<string_t, string_t>> vm = next_line_map();
			if (!INFO_OPTIONAL_HAS_VALUE(vm)) {
				return (false);
			}
			if (!create_item(oMap)) {
				return (false);
			}
			process_line_items(*vm, oMap);
			return (!oMap.empty());
		}
		void base_importer::set_dict(const std::map<string_t, string_t> &oDict) {
			m_dict = oDict;
		}
		const std::vector<any> & base_importer::get_items(void) {
			return (m_data);
		}
		std::future<bool> base_importer::import_async(void) {
			return std::async([this]()->bool {
				return (this->import());
			});
		}
		bool base_importer::import(void) {
			check_descs();
			if (!first()) {
				return (false);
			}
			bool done{ false };
            int nCount = 0;
			while (!done) {
                ++nCount;
				infomap oMap{};
				if (!next_item(oMap)) {
					done = true;
					break;
				}
				any a{oMap};
                string_t sx = stringutils::any_to_stringt(a);
				this->register_item(oMap);
			}// done
			return (true);
		}// import
		 ///////////////////////////
}// namespace info
