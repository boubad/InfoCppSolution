#include "infoconv.h"
////////////////////
namespace info {
	namespace http {
		//////////////////////////////
		void convert_any(const any &src, value &dest) {
			dest = value{};
			if (!INFO_ANY_HAS_VALUE(src)) {
				return;
			}
			if (src.type() == typeid(string_t)) {
				string_t s = INFO_ANY_CAST<string_t>(src);
				dest = value{ s };
			}
			else if (src.type() == typeid(int)) {
				int s = INFO_ANY_CAST<int>(src);
				dest = value{ s };
			}
			else if (src.type() == typeid(bool)) {
				bool s = INFO_ANY_CAST<bool>(src);
				dest = value{ s };
			}
			else if (src.type() == typeid(double)) {
				double s = INFO_ANY_CAST<double>(src);
				dest = value{ s };
			}
			else if (src.type() == typeid(float)) {
				float s = INFO_ANY_CAST<float>(src);
				dest = value{ s };
			}
			else if (src.type() == typeid(infovector)) {
				infovector vv = INFO_ANY_CAST<infovector>(src);
				std::vector<value> vz{};
				for (auto x : vv) {
					value vx{};
					convert_any(x, vx);
					vz.push_back(vx);
				}// x
				dest = value::array(vz);
			}
			else if (src.type() == typeid(infomap)) {
				infomap oMap = INFO_ANY_CAST<infomap>(src);
				dest = value::object();
				object &obj = dest.as_object();
				for (auto it = oMap.begin(); it != oMap.end(); ++it) {
					string_t key = (*it).first;
					any a = (*it).second;
					value vx{};
					convert_any(a, vx);
					obj[key] = vx;
				}// it
			}
		}// convert_any
		/////////////////////////////////////
		void convert_any(const value &src, any &dest) {
			dest = any{};
			if (src.is_string()) {
				string_t s = src.as_string();
				dest = any{ s };
			}
			else if (src.is_integer()) {
				int s = src.as_integer();
				dest = any{ s };
			}
			else if (src.is_boolean()) {
				bool s = src.as_bool();
				dest = any{ s };
			}
			else if (src.is_double()) {
				double s = src.as_double();
				dest = any{ s };
			}
			else if (src.is_number()) {
				const web::json::number &n = src.as_number();
				double s = n.to_double();
				dest = any{ s };
			}
			else if (src.is_array()) {
				const array &oAr = src.as_array();
				infovector vz{};
				for (auto it = oAr.begin(); it != oAr.end(); ++it) {
					const value &vx = *it;
					any a{};
					convert_any(vx, a);
					vz.push_back(a);
				}// it
				dest = any{ vz };
			}
			else if (src.is_object()) {
				const object &obj = src.as_object();
				infomap oMap{};
				for (auto it = obj.begin(); it != obj.end(); ++it) {
					string_t key = (*it).first;
					const value & vx = (*it).second;
					any a{};
					convert_any(vx, a);
					oMap[key] = a;
				}// it
				dest = any{ oMap };
			}
		}// conert_any
		//////////////////////////////////
	}// namespace http
}// namespace info
