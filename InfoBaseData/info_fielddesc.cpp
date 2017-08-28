#include "info_fielddesc.h"
#include "stringutils.h"
///////////////////////////////////////////////
namespace info {
		////////////////////////////////////
		static const string_t FD_DATATYPE{ U("datatype") };
		static const string_t FD_PROPNAME{ U("prop_name") };
		static const string_t FD_IMPNAME{ U("imp_name") };
		static const string_t FD_DISPNAME{ U("disp_name") };
		static const string_t FD_REQUIRED{ U("required") };
		static const string_t FD_MINVAL{ U("min_val") };
		static const string_t FD_MAXVAL{ U("max_val") };
		static const string_t FD_MAXLENGTH{ U("maxlength") };
		static const string_t FD_FILTER_OP{ U("filter_op") };
		static const string_t FD_FILTER_VAL{ U("filter_val") };
		////////////////////////////////////////////////////
		static const string_t STRING_TRUE(U("true"));
		static const string_t STRING_FALSE(U("false"));
		//////////////////////////////////////
		bool info_fielddesc::read_any_value(const string_t &sx, any &v) {
			return  stringutils::info_read_any_value(sx, v);
		}// read_any_value
		nullable_any info_fielddesc::get_field_value(const string_t &sx) const {
			string_t s = stringutils::trim(sx);
			if (s.empty()) {
				return nullable_any{};
			}
			any vx{};
			if (!read_any_value(s, vx)) {
				return nullable_any{};
			}
			if (!INFO_ANY_HAS_VALUE(vx)) {
				return nullable_any{};
			}
			info_datatype t = data_type();
			switch (t) {
			case info_datatype::text: {
				if (vx.type() == typeid(string_t)) {
					return optional<any>(vx);
				}
				else if (vx.type() == typeid(int)) {
					ostringstream_t os{};
					os << INFO_ANY_CAST<int>(vx);
					string_t sz = os.str();
					any v{ sz };
					return optional<any>(v);
				}
				else if (vx.type() == typeid(double)) {
					ostringstream_t os{};
					os << INFO_ANY_CAST<double>(vx);
					string_t sz = os.str();
					any v{ sz };
					return optional<any>(v);
				}
				else if (vx.type() == typeid(float)) {
					ostringstream_t os{};
					os << INFO_ANY_CAST<float>(vx);
					string_t sz = os.str();
					any v{ sz };
					return optional<any>(v);
				}
				else if (vx.type() == typeid(bool)) {
					bool b = INFO_ANY_CAST<bool>(vx);
					string_t sz{ (b) ? STRING_TRUE : STRING_FALSE };
					any v{ sz };
					return optional<any>(v);
				}
				else {
					return nullable_any{};
				}
			}
									  break;
			case info_datatype::real: {
				if (vx.type() == typeid(double)) {
					return optional<any>(vx);
				}
				else if (vx.type() == typeid(float)) {
					double sz = static_cast<double>(INFO_ANY_CAST<float>(vx));
					any v{ sz };
					return optional<any>(v);
				}
				else if (vx.type() == typeid(int)) {
					double sz = static_cast<double>(INFO_ANY_CAST<int>(vx));
					any v{ sz };
					return optional<any>(v);
				}
				else if (vx.type() == typeid(bool)) {
					bool b = INFO_ANY_CAST<bool>(vx);
					double sz{ (b) ? 1.0 : 0.0 };
					any v{ sz };
					return optional<any>(v);
				}
				else if (vx.type() == typeid(string_t)) {
					try {
						string_t sw = INFO_ANY_CAST<string_t>(vx);
						size_t cur{ 0 };
						double sz = std::stod(sw, &cur);
						any v{ sz };
						return optional<any>(v);
					}
					catch (std::exception & /* ex*/) {
						return nullable_any{};
					}
				}
			}
									  break;
			case info_datatype::integer: {
				if (vx.type() == typeid(int)) {
					return optional<any>(vx);
				}
				else if (vx.type() == typeid(double)) {
					int sz = static_cast<int>(INFO_ANY_CAST<double>(vx));
					any v{ sz };
					return optional<any>(v);
				}
				else if (vx.type() == typeid(float)) {
					int sz = static_cast<int>(INFO_ANY_CAST<float>(vx));
					any v{ sz };
					return optional<any>(v);
				}
				else if (vx.type() == typeid(bool)) {
					bool b = INFO_ANY_CAST<bool>(vx);
					int sz{ (b) ? 1 : 0 };
					any v{ sz };
					return optional<any>(v);
				}
				else if (vx.type() == typeid(string_t)) {
					try {
						string_t sw = INFO_ANY_CAST<string_t>(vx);
						size_t cur{ 0 };
						int sz = std::stoi(sw, &cur);
						any v{ sz };
						return optional<any>(v);
					}
					catch (std::exception & /* ex*/) {
						return nullable_any{};
					}
				}
			}
										 break;
			case info_datatype::boolean: {
				if (vx.type() == typeid(bool)) {
					return optional<any>(vx);
				}
				else if (vx.type() == typeid(int)) {
					bool sz = (INFO_ANY_CAST<int>(vx) != 0) ? true : false;
					any v{ sz };
					return optional<any>(v);
				}
				else if (vx.type() == typeid(double)) {
					bool sz = (INFO_ANY_CAST<double>(vx) != 0) ? true : false;
					any v{ sz };
					return optional<any>(v);
				}
				else if (vx.type() == typeid(float)) {
					bool sz = (INFO_ANY_CAST<float>(vx) != 0) ? true : false;
					any v{ sz };
					return optional<any>(v);
				}
				else {
					return nullable_any{};
				}
			}
										 break;
			case info_datatype::result: {
				if (vx.type() == typeid(string_t)) {
					string_t sl = stringutils::trim(INFO_ANY_CAST<string_t>(vx));
					if (sl == U("val")) {
						int sz = static_cast<int>(info_resulttype::val);
						any v{ sz };
						return optional<any>(v);
					}
					else if (sl == U("valc")) {
						int sz = static_cast<int>(info_resulttype::valc);
						any v{ sz };
						return optional<any>(v);
					}
					else if (sl == U("ajac")) {
						int sz = static_cast<int>(info_resulttype::ajac);
						any v{ sz };
						return optional<any>(v);
					}
					else if (sl == U("nar")) {
						int sz = static_cast<int>(info_resulttype::nar);
						any v{ sz };
						return optional<any>(v);
					}
					else if (sl == U("misc")) {
						int sz = static_cast<int>(info_resulttype::misc);
						any v{ sz };
						return optional<any>(v);
					}
					else {
						return nullable_any{};
					}
				}
			}
										break;
			default:
				break;
			}// t
			return nullable_any{};
		}// get_field_value
		bool info_fielddesc::apply_string_value(const string_t &sx, infomap &oMap) const {
			string_t pname = property_name();
			if (pname.empty()) {
				return (false);
			}
			nullable_any vv = get_field_value(sx);
			if (!INFO_OPTIONAL_HAS_VALUE(vv)) {
				return (false);
			}
			any v = *vv;
			if (!INFO_ANY_HAS_VALUE(v)) {
				return (false);
			}
			info_datatype t = data_type();
			switch (t) {
			case info_datatype::text: {
				if (v.type() == typeid(string_t)) {
					oMap[pname] = v;
					return (true);
				}
			}
									  break;
			case info_datatype::integer:
			case info_datatype::result:
			case info_datatype::enumtype:
			case info_datatype::status:
			case info_datatype::datatype:
			case info_datatype::kind:
			{
				if (v.type() == typeid(int)) {
					oMap[pname] = v;
					return (true);
				}
			}
			break;
			case info_datatype::real: {
				if (v.type() == typeid(double)) {
					oMap[pname] = v;
					return (true);
				}
			}
									  break;
			case info_datatype::boolean: {
				if (v.type() == typeid(bool)) {
					oMap[pname] = v;
					return (true);
				}
			}
										 break;
			default:
				if (v.type() == typeid(string_t)) {
					oMap[pname] = v;
					return (true);
				}
				break;
			}// t
			return (false);
		}//apply_string_value
		 /////////////////////////////////////
		info_fielddesc::info_fielddesc()
		{
		}
		info_fielddesc::info_fielddesc(const string_t &pname, info_datatype t /*= info_datatype::text*/) {
			property_name(pname);
			data_type(t);
		}
		info_fielddesc::info_fielddesc(const string_t &pname, const string_t &impname, info_datatype t /*= info_datatype::text*/) {
			property_name(pname);
			import_name(impname);
			data_type(t);
		}
		info_fielddesc::info_fielddesc(const string_t &pname, const string_t &dispname, const string_t &impname, info_datatype t /*= info_datatype::text*/) {
			property_name(pname);
			import_name(impname);
			display_name(dispname);
			data_type(t);
		}
		info_fielddesc::info_fielddesc(const info_fielddesc &other) : infobaseobject(other) {

		}
		info_fielddesc & info_fielddesc::operator=(const info_fielddesc &other) {
			if (this != &other) {
				infobaseobject::operator=(other);
			}
			return (*this);
		}
		info_fielddesc::~info_fielddesc()
		{
		}
		bool info_fielddesc::operator<(const info_fielddesc &other) const {
			string_t s1 = property_name();
			string_t s2 = other.property_name();
			return (s1 < s2);
		}
		bool info_fielddesc::operator==(const info_fielddesc &other) const {
			string_t s1 = property_name();
			string_t s2 = other.property_name();
			return (s1 == s2);
		}
		info_datatype info_fielddesc::data_type(void) const {
			info_datatype r{ info_datatype::unknown };
			int vi{ 0 };
			if (get_int_property(FD_DATATYPE, vi)) {
				r = static_cast<info_datatype>(vi);
			}
			return (r);
		}
		void info_fielddesc::data_type(info_datatype t) {
			if (t == info_datatype::unknown) {
				erase_property(FD_DATATYPE);
			}
			else {
				set_int_property(FD_DATATYPE, static_cast<int>(t));
			}
		}
		string_t info_fielddesc::property_name(void) const {
			string_t s{};
			get_string_property(FD_PROPNAME, s);
			return (s);
		}
		void info_fielddesc::property_name(const string_t &s) {
			set_string_property(FD_PROPNAME, s);
		}
		string_t info_fielddesc::import_name(void) const {
			string_t s{};
			get_string_property(FD_IMPNAME, s);
			if (s.empty()) {
				s = property_name();
			}
			return (s);
		}
		void info_fielddesc::import_name(const string_t &s) {
			set_string_property(FD_IMPNAME, stringutils::toupper(s));
		}
		string_t info_fielddesc::display_name(void) const {
			string_t s{};
			get_string_property(FD_DISPNAME, s);
			if (s.empty()) {
				s = import_name();
			}
			return (s);
		}
		void info_fielddesc::display_name(const string_t &s) {
			set_string_property(FD_DISPNAME, s);
		}
		bool info_fielddesc::required(void) const {
			bool bRet{ false };
			get_bool_property(FD_REQUIRED, bRet);
			return (bRet);
		}
		void info_fielddesc::required(bool b) {
			if (!b) {
				erase_property(FD_REQUIRED);
			}
			else {
				set_bool_property(FD_REQUIRED, b);
			}
		}
		any info_fielddesc::min_value(void) const {
			any v{};
			get_any_property(FD_MINVAL, v);
			return (v);
		}
		void info_fielddesc::min_value(const any &v) {
			if (!INFO_ANY_HAS_VALUE(v)) {
				erase_property(FD_MINVAL);
			}
			else  if ((v.type() == typeid(int)) || (v.type() == typeid(double)) ||
				(v.type() == typeid(float))) {
				set_any_property(FD_MINVAL, v);
			}
			else {
				erase_property(FD_MINVAL);
			}
		}
		any info_fielddesc::max_value(void) const {
			any v{};
			get_any_property(FD_MAXVAL, v);
			return (v);
		}
		void info_fielddesc::max_value(const any &v) {
			if (!INFO_ANY_HAS_VALUE(v)) {
				erase_property(FD_MAXVAL);
			}
			else  if ((v.type() == typeid(int)) || (v.type() == typeid(double)) ||
				(v.type() == typeid(float))) {
				set_any_property(FD_MAXVAL, v);
			}
			else {
				erase_property(FD_MAXVAL);
			}
		}
		size_t info_fielddesc::maxlength(void) const {
			int nRet{ 0 };
			get_int_property(FD_MAXLENGTH, nRet);
			return (nRet);
		}
		void info_fielddesc::maxlength(size_t n) {
			if (n > 0) {
				set_int_property(FD_MAXLENGTH, static_cast<int>(n));
			}
			else {
				erase_property(FD_MAXLENGTH);
			}
		}
		bool info_fielddesc::validate(const any &v) const {
			bool bReq = required();
			if (!INFO_ANY_HAS_VALUE(v)) {
				return ((bReq) ? false : true);
			}
			info_datatype t = data_type();
			switch (t) {
			case info_datatype::text: {
				if (v.type() != typeid(string_t)) {
					return (false);
				}
				string_t sx = stringutils::trim(INFO_ANY_CAST<string_t>(v));
				if (sx.empty() && bReq) {
					return (false);
				}
				size_t nt = maxlength();
				if ((nt > 0) && (sx.length() > nt)) {
					return (false);
				}
			}
									  break;
			case info_datatype::real: {
				double dVal{ 0 };
				if (!stringutils::get_num_value(v, dVal)) {
					return (false);
				}
				bool bMin{ false }, bMax{ false };
				double dMin{ 0 }, dMax{ 0 };
				any vmin = min_value();
				bMin = stringutils::get_num_value(vmin, dMin);
				any vmax = max_value();
				bMax = stringutils::get_num_value(vmax, dMax);
				if ((!bMin) && (!bMax)) {
					return true;
				}
				else if (bMin && bMax) {
					if (dMin >= dMax) {
						return (true);
					}
				}
				if (bMin && (dVal < dMin)) {
					return (false);
				}
				if (bMax && (dVal > dMax)) {
					return (false);
				}
			}
									  break;
			case info_datatype::integer: {
				int dVal{ 0 };
				if (!stringutils::get_num_value(v, dVal)) {
					return (false);
				}
				bool bMin{ false }, bMax{ false };
				int dMin{ 0 }, dMax{ 0 };
				any vmin = min_value();
				bMin = stringutils::get_num_value(vmin, dMin);
				any vmax = max_value();
				bMax = stringutils::get_num_value(vmax, dMax);
				if ((!bMin) && (!bMax)) {
					return true;
				}
				else if (bMin && bMax) {
					if (dMin >= dMax) {
						return (true);
					}
				}
				if (bMin && (dVal < dMin)) {
					return (false);
				}
				if (bMax && (dVal > dMax)) {
					return (false);
				}
			}
										 break;
			default:
				break;
			}// t
			return (true);
		}// validate
		nullable_any info_fielddesc::get_value(const string_t &sval) {
			any v{};
			if (!read_any_value(sval, v)) {
				return nullable_any{};
			}
			if (!validate(v)) {
				return nullable_any{};
			}
			return (optional<any>(v));
		}// get_value
		info_filteroptype info_fielddesc::filteroptype(void) const {
			info_filteroptype t{ info_filteroptype::ignore };
			int vi{ 0 };
			if (get_int_property(FD_FILTER_OP, vi)) {
				t = static_cast<info_filteroptype>(vi);
			}
			return (t);
		}
		void info_fielddesc::filteroptype(info_filteroptype t) {
			if (t == info_filteroptype::ignore) {
				erase_property(FD_FILTER_OP);
			}
			else {
				info_datatype d = data_type();
				switch (d) {
				case info_datatype::text: {
					if ((t != info_filteroptype::exists) && (t != info_filteroptype::notexists) &&
						(t != info_filteroptype::equals) && (t != info_filteroptype::notequals) &&
						(t != info_filteroptype::like)) {
						erase_property(FD_FILTER_OP);
						return;
					}
				}
										  break;
				case info_datatype::enumtype: {
					if ((t != info_filteroptype::exists) && (t != info_filteroptype::notexists) &&
						(t != info_filteroptype::equals) && (t != info_filteroptype::notequals)) {
						erase_property(FD_FILTER_OP);
						return;
					}
				}
											  break;
				case info_datatype::status: {
					if ((t != info_filteroptype::exists) && (t != info_filteroptype::notexists) &&
						(t != info_filteroptype::equals) && (t != info_filteroptype::notequals)) {
						erase_property(FD_FILTER_OP);
						return;
					}
				}
											break;
				case info_datatype::datatype: {
					if ((t != info_filteroptype::exists) && (t != info_filteroptype::notexists) &&
						(t != info_filteroptype::equals) && (t != info_filteroptype::notequals)) {
						erase_property(FD_FILTER_OP);
						return;
					}
				}
											  break;
				case info_datatype::kind: {
					if ((t != info_filteroptype::exists) && (t != info_filteroptype::notexists) &&
						(t != info_filteroptype::equals) && (t != info_filteroptype::notequals)) {
						erase_property(FD_FILTER_OP);
						return;
					}
				}
										  break;
				case info_datatype::result: {
					if ((t != info_filteroptype::exists) && (t != info_filteroptype::notexists) &&
						(t != info_filteroptype::equals) && (t != info_filteroptype::notequals)) {
						erase_property(FD_FILTER_OP);
						return;
					}
				}
											break;
				case info_datatype::boolean: {
					if ((t != info_filteroptype::exists) && (t != info_filteroptype::notexists) &&
						(t != info_filteroptype::equals) && (t != info_filteroptype::notequals)) {
						erase_property(FD_FILTER_OP);
						return;
					}
				}
											 break;
				case info_datatype::integer: {
					if (t == info_filteroptype::like) {
						erase_property(FD_FILTER_OP);
						return;
					}
				}
											 break;
				case info_datatype::real: {
					if (t == info_filteroptype::like) {
						erase_property(FD_FILTER_OP);
						return;
					}
				}
										  break;
				default:
					erase_property(FD_FILTER_OP);
					return;
				}// d
				set_int_property(FD_FILTER_OP, static_cast<int>(t));
			}
		}
		nullable_any info_fielddesc::filter_val(void) const {
			any v{};
			if (get_any_property(FD_FILTER_VAL, v)) {
				return optional<any>(v);
			}
			return optional<any>{};
		}
		void info_fielddesc::filter_val(const any &val) {
			if (!INFO_ANY_HAS_VALUE(val)) {
				erase_property(FD_FILTER_VAL);
			}
			else {
				set_any_property(FD_FILTER_VAL, val);
			}
		}
		nullable_any info_fielddesc::filter(void) const {
			string_t field = property_name();
			if (field.empty()) {
				return (nullable_any{});
			}
			nullable_any vv = filter_val();
			if (!INFO_OPTIONAL_HAS_VALUE(vv)) {
				return (nullable_any{});
			}
			info_filteroptype t = filteroptype();
			string_t sop{};
			switch (t) {
			case info_filteroptype::equals:
				sop = U("$eq");
				break;
			case info_filteroptype::notequals:
				sop = U("$ne");
				break;
			case info_filteroptype::less:
				sop = U("$lt");
				break;
			case info_filteroptype::lessequals:
				sop = U("$lte");
				break;
			case info_filteroptype::greater:
				sop = U("$gt");
				break;
			case info_filteroptype::greaterequals:
				sop = U("$gte");
				break;
			case info_filteroptype::like: {
				if (!INFO_OPTIONAL_HAS_VALUE(vv)) {
					return (nullable_any{});
				}
				any v = *vv;
				if (v.type() != typeid(string_t)) {
					return (nullable_any{});
				}
				string_t s = stringutils::trim(INFO_ANY_CAST<string_t>(vv));
				if (s.empty()) {
					return (nullable_any{});
				}
				string_t ss = U("(?i)") + s;
				sop = U("$regex");
				infomap vo{};
				vo[sop] = any{ ss };
				infomap vr{};
				vr[field] = any{ vo };
				any vz{ vr };
				return (optional<any>{vz});
			}
			case info_filteroptype::exists: {
				sop = U("$exists");
				infomap vo{};
				bool n{ true };
				vo[sop] = any(n);
				infomap vr{};
				vr[field] = any{ vo };
				any vz{ vr };
				return (optional<any>{vz});
			}
			case info_filteroptype::notexists: {
				sop = U("$exists");
				infomap vo{};
				bool n{ false };
				vo[sop] = any(n);
				infomap vr{};
				vr[field] = any{ vo };
				any vz{ vr };
				return (optional<any>{vz});
			}
			default:
				break;
			}// t
			if (!INFO_OPTIONAL_HAS_VALUE(vv)) {
				return (nullable_any{});
			}
			if (sop.empty()) {
				return (nullable_any{});
			}
			{
				any x = *vv;
				if (!INFO_ANY_HAS_VALUE(x)) {
					return (nullable_any{});
				}
				infomap vo{};
				vo[sop] = x;
				infomap vr{};
				vr[field] = any{ vo };
				any vz{ vr };
				return (optional<any>{vz});
			}
		}
}// namespace info
