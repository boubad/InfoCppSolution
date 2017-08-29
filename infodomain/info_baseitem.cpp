#include "info_baseitem.h"
#include <stringutils.h>
//////////////////////////////
namespace info {
	namespace domain {
		////////////////////////
		info_baseitem::info_baseitem()
		{
		}
		info_baseitem::info_baseitem(const any &v): info_document(v) {

		}
		info_baseitem::info_baseitem(const info_baseitem &other): info_document(other) {

		}
		info_baseitem & info_baseitem::operator=(const info_baseitem &other) {
			if (this != &other) {
				info_document::operator=(other);
			}
			return (*this);
		}
		info_baseitem::~info_baseitem()
		{
		}
		void info_baseitem::get_descs(std::set<info_fielddesc> &oSet) const {
			info_document::get_descs(oSet);
			oSet.insert(info_fielddesc{ KEY_STATUS,U("Status"),U("STATUS"),info_datatype::status });
			oSet.insert(info_fielddesc{ KEY_DESC,U("Observations"),U("OBSERVATIONS"),info_datatype::text });
		}
		info_status info_baseitem::status(void) const {
			int n{ 0 };
			if (get_int_property(KEY_STATUS, n)) {
				if (n >= 0) {
					return static_cast<info_status>(n);
				}
			}
			return info_status::unknown;
		}
		void info_baseitem::status(info_status s) {
			if (s == info_status::unknown) {
				erase_property(KEY_STATUS);
			}
			else {
				set_int_property(KEY_STATUS, static_cast<int>(s));
			}
		}
		nullable_string info_baseitem::observations(void) const {
			string_t s{};
			if (get_string_property(KEY_DESC, s)) {
				return nullable_string{ s };
			}
			return nullable_string{};
		}
		void info_baseitem::observations(const string_t &s) {
			string_t ss = stringutils::trim(s);
			if (ss.empty()) {
				erase_property(KEY_DESC);
			}
			else {
				set_string_property(KEY_DESC, ss);
			}
		}
	}// namespace domain
}// namespace info
