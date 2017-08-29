#include "attachment_info.h"
#include "stringutils.h"
///////////////////////////////
namespace info {
		////////////////////////////////
		static const string_t APPLICATION_OCTETSTREAM(U("application/octet-stream"));
		/////////////////////////////////
		attachment_info::attachment_info()
		{
		}
		attachment_info::attachment_info(const any &va):infobaseobject(va) {

		}
		attachment_info::attachment_info(const attachment_info &other):infobaseobject(other) {

		}
		attachment_info & attachment_info::operator=(const attachment_info &other) {
			if (this != &other) {
				infobaseobject::operator=(other);
			}
			return (*this);
		}
		attachment_info::~attachment_info()
		{
		}
		string_t attachment_info::content_type(void) const {
			string_t sRet{};
			string_t s{};
			if (get_string_property(U("content_type"), s)) {
				static string_t delim(U(","));
				std::vector<string_t> vec{};
				stringutils::split_vector(s, vec, delim);
				for (auto p : vec) {
					string_t sk = stringutils::trim(p);
					if (sk != APPLICATION_OCTETSTREAM) {
						sRet = sk;
						break;
					}
				}// p
			}
			if (sRet.empty()) {
				sRet = APPLICATION_OCTETSTREAM;
			}
			return (sRet);
		}
		string_t attachment_info::data(void) const {
			string_t sRet{};
			get_string_property(U("data"), sRet);
			return (sRet);
		}
		string_t attachment_info::digest(void) const {
			string_t sRet{};
			get_string_property(U("digest"), sRet);
			return (sRet);
		}
		int attachment_info::encoded_length(void) const {
			int nRet{ 0 };
			get_int_property(U("encoded_length"), nRet);
			return (nRet);
		}
		string_t attachment_info::encoding(void) const {
			string_t sRet{};
			get_string_property(U("encoding"), sRet);
			return (sRet);
		}
		int attachment_info::length(void) const {
			int nRet{ 0 };
			get_int_property(U("length"), nRet);
			return (nRet);
		}
		int attachment_info::revpos(void) const {
			int nRet{ 0 };
			get_int_property(U("revpos"), nRet);
			return (nRet);
		}
		bool attachment_info::stub(void) const {
			bool nRet{ false };
			get_bool_property(U("stub"), nRet);
			return (nRet);
		}
		////////////////////////////
}// namespace info
