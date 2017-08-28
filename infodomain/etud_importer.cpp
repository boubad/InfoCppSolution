#include "etud_importer.h"
#include "info_etudiant.h"
////////////////////////////
namespace info {
	namespace domain {
		////////////////////////////
		etud_importer::etud_importer(istream_t &in, const string_t &sdel /*= string_t{ U(";") }*/):
			base_importer(in,sdel)
		{
		}
		

		etud_importer::~etud_importer()
		{
		}
		bool etud_importer::create_item(infomap &oMap) const {
			info_etudiant r{};
			any a = r.get();
			oMap = INFO_ANY_CAST<infomap>(a);
			return (!oMap.empty());
		}
		void etud_importer::check_descs(void) {
			info_etudiant r{};
			this->m_descs = r.get_descs();
		}
		void etud_importer::register_item(const infomap & item) {
			if (!item.empty()) {
				info_etudiant r{ any{item} };
				if (r.is_storable()) {
					base_importer::register_item(item);
				}
			}// value
		}//register_item
		//////////////////////////
	}// namespace domain
}// namespace info
