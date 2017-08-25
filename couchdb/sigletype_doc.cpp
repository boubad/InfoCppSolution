#include "sigletype_doc.h"
#include "couchdb_manager.h"
#include <stringutils.h>
////////////////////////////
namespace info {
	namespace couchdb {
		///////////////////////
		const string_t type_doc::KEY_TYPE(U("type"));
		/////////////////////////////
		type_doc::type_doc() {

		}
		type_doc::type_doc(const any &va) :couchdb_doc(va) {

		}
		type_doc::type_doc(const type_doc &other) : couchdb_doc(other) {

		}
		type_doc & type_doc::operator=(const type_doc &other) {
			if (this != &other) {
				couchdb_doc::operator=(other);
			}
			return (*this);
		}
		type_doc::~type_doc() {

		}
		void type_doc::type(const string_t &s) {
			string_t ss = stringutils::tolower(stringutils::trim(s));
			set_string_property(KEY_TYPE, ss);
		}
		void type_doc::unique_properties(std::vector<std::vector<string_t>> &vvec) const {
			vvec.clear();
		}
		bool type_doc::is_storable(void) const {
			string_t s = type();
			return (!s.empty());
		}
		string_t type_doc::type(void) const {
			string_t s{};
			get_string_property(KEY_TYPE, s);
			return (s);
		}
		bool type_doc::load_by_id(couchdb_manager &oMan, couchdb_doc &doc) const {
			string_t docid = id();
			if (docid.empty()) {
				return false;
			}
			doc = oMan.get_document_by_id_async(docid).get();
			return (!doc.empty());
		}//load_by_id
		bool type_doc::load_unique(couchdb_manager &oMan, couchdb_doc &doc) const {
			std::vector<std::vector<string_t>> vvec{};
			this->unique_properties(vvec);
			for (auto vec : vvec) {
				query_filter filter{};
				for (auto key : vec) {
					any va{};
					if (!get_any_property(key, va)) {
						return (false);
					}
					filter.add_equals(key, va);
				}// key
				filter.set_limit(1);
				filter.set_skip(0);
				std::vector<couchdb_doc> vv = oMan.find_documents_async(filter).get();
				if (!vv.empty()) {
					doc = vv[0];
					return (true);
				}
			}// vec
			return (false);
		}//load_unique
		std::future<bool> type_doc::load(couchdb_manager &oMan) {
			couchdb_manager *pMan = &oMan;
			return std::async([this, pMan]()->bool {
				couchdb_doc doc{};
				string_t docid = this->id();
				if (!docid.empty()) {
					if (this->load_by_id(*pMan, doc)) {
						if (!doc.empty()) {
							this->set(doc.get());
							return (true);
						}
					}
				}
				if (this->load_unique(*pMan, doc)) {
					if (!doc.empty()) {
						this->set(doc.get());
						return (true);
					}
				}
				return (false);
			});
		}// load
		std::future<bool> type_doc::save(couchdb_manager &oMan) {
			couchdb_manager *pMan = &oMan;
			return std::async([this, pMan]()->bool {
				if (!this->is_storable()) {
					return (false);
				}
				couchdb_doc d{};
				this->load_unique(*pMan, d);
				string_t docid = this->id();
				string_t sid2 = d.id();
				if (!docid.empty()) {
					if (!sid2.empty()) {
						if (sid2 != docid) {
							return (false);
						}
					}
				}// id
				if (d.is_persisted()) {
					this->id(d.id());
					this->version(d.version());
				}
				update_response r = pMan->maintains_document_async(*this).get();
				if (r.ok()) {
					return this->load(*pMan).get();
				}
				return (false);
			});
		}// save
		std::future<bool> type_doc::remove(couchdb_manager &oMan) {
			couchdb_manager *pMan = &oMan;
			return std::async([this, pMan]()->bool {
				bool b = this->load(*pMan).get();
				if (!b) {
					return (false);
				}
				update_response r = pMan->delete_document_async(*this).get();
				return (r.ok());
			});
		}// remove
		/////////////////
	}// namespace couchdb
} // namespace info
