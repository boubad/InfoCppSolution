#pragma once
#ifndef ETUD_IMPORTER_H__
#define ETUD_IMPORTER_H__
////////////////////////////////
#include <base_importer.h>
/////////////////////////////////
namespace info {
	namespace domain {
		///////////////////////////
		class etud_importer : public base_importer
		{
		public:
			etud_importer(istream_t &in, const string_t &sdel = string_t{ U(";") });
			etud_importer(const string_t &filename, const string_t &sdel = string_t{ U(";") });
			~etud_importer();
		protected:
			virtual void register_item(const infomap &item) override;
			virtual bool create_item(infomap &oMap) const override;
			virtual void check_descs(void) override;
		};
		///////////////////////////
	}// namespace domain
}// namespace info
////////////////////////////
#endif // ETUD_IMPORTER_H__
