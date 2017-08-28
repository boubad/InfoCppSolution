#pragma once
#ifndef BASE_IMPORTER_H__
#define BASE_IMPORTER_H__
///////////////////////////////
#include "info_fielddesc.h"
//////////////////////
#include <set>
#include <future>
///////////////////////
namespace info {
		class base_importer
		{
		private:
			base_importer(const  base_importer &other) = delete;
			base_importer & operator=(const  base_importer &other) = delete;
		protected:
			istream_t *m_pin;
			string_t m_delim;
			std::shared_ptr<istream_t> m_in;
			std::set<info_fielddesc> m_descs;
			//
			std::vector<any> m_data;
			std::map<string_t, string_t> m_dict;
			std::map<size_t, string_t> m_headers;
			//
		protected:
			istream_t & get_stream(void) const;
			const std::map<string_t, string_t> & headers_map(void) const;
			std::map<string_t, string_t> & headers_map(void);
			const std::map<size_t, string_t> &headers(void) const;
			std::map<size_t, string_t> &headers(void);
			void process_line_items(const std::map<string_t, string_t> &lineData, infomap &item) const;
			bool first(void);
			optional<string_t> next_line(void);
			optional<std::map<size_t, string_t>> first_line_headers(void);
			optional<std::map<string_t, string_t>> next_line_map(void);
			bool next_item(infomap &oMap);
			const info_fielddesc *find_desc(const string_t &fieldname) const;
			//
			virtual void register_item(const infomap &oMap);
			virtual bool create_item(infomap &oMap) const = 0;
			virtual void check_descs(void) = 0;
			virtual string_t transform_header(const string_t &s);
		protected:
			base_importer(istream_t &in, const string_t &sdel = string_t{ U(";") });
			base_importer(const string_t &filename, const string_t &sdel = string_t{ U(";") });
		public:
			virtual ~base_importer();
		public:
			const string_t & delimiter(void) const;
			void delimiter(const string_t &s);
			void set_dict(const std::map<string_t, string_t> &oDict);
			const std::vector<any> & get_items(void);
			std::future<bool> import_async(void);
			bool import(void);
		};// class base_importer
}// namespace info
 ///////////////////////////////
#endif // BASE_IMPORTER_H__
