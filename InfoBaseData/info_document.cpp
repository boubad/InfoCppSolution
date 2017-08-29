#include "info_document.h"
#include "datastore_manager.h"
#include "query_filter.h"
#include "stringutils.h"
/////////////////////////////
namespace info {
	///////////////////////////////////
	const string_t info_document::KEY_TYPE(U("type"));
	static const string_t KEY_ATTACHMENTS(U("_attachments"));
	//////////////////////////////////
	info_document::info_document()
	{
	}
	info_document::info_document(const infomap &oMap) {
		infomap m{ oMap };
		this->set(any{ m });
	}
	info_document::info_document(const any &va):infobaseobject(va) {
		std::vector<blob_data> &vec = m_blobs;
		vec.clear();
		infomap oMap{};
		get_obj_property(KEY_ATTACHMENTS, oMap);
		string_t docid = this->id();
		for (auto it = oMap.begin(); it != oMap.end(); ++it) {
			string_t key = (*it).first;
			any va = (*it).second;
			blob_data info{docid,key, va };
			vec.push_back(info);
		}// it
	}
	info_document::info_document(const info_document &other):infobaseobject(other),m_blobs(other.m_blobs) {

	}
	info_document & info_document::operator=(const info_document &other) {
		if (this != &other) {
			infobaseobject::operator=(other);
			m_blobs = other.m_blobs;
		}
		return (*this);
	}

	info_document::~info_document()
	{
	}
	void info_document::post_init(void) {
		// do nothing  here
	}
	void info_document::unique_properties(std::vector<std::vector<string_t>> &vvec) const {
		vvec.clear();
	}
	void info_document::set(const any &oMap) {
		infobaseobject::set(oMap);
		std::vector<blob_data> &vec = m_blobs;
		vec.clear();
		infomap xMap{};
		get_obj_property(KEY_ATTACHMENTS, xMap);
		string_t docid = this->id();
		for (auto it = xMap.begin(); it != xMap.end(); ++it) {
			string_t key = (*it).first;
			any va = (*it).second;
			blob_data info{ docid,key, va };
			vec.push_back(info);
		}// it
	}
	const std::vector<blob_data> &info_document::blobs(void) const {
		return (m_blobs);
	}
	std::vector<blob_data> &info_document::blobs(void)  {
		return (m_blobs);
	}
	void info_document::check_blobs_urls(const string_t &baseurl) {
		std::vector<blob_data> &vec = m_blobs;
		if (vec.empty()) {
			return;
		}
		string_t sbase{ baseurl };
		size_t n = sbase.length();
		assert(n > 1);
		if (sbase[n - 1] != U('/')) {
			sbase += U('/');
		}
		string_t docid = this->id();
		for (auto it = vec.begin(); it != vec.end(); ++it) {
			blob_data &d = *it;
			string_t s = sbase + docid + U("/") + d.name();
			d.url(s);
		}// it
	}//check_blobs_urls
	void info_document::type(const string_t &s) {
		string_t ss = stringutils::trim(s);
		if (ss.empty()) {
			erase_property(KEY_TYPE);
		}
		else {
			set_string_property(KEY_TYPE, ss);
		}
	}
	string_t info_document::type(void) const {
		string_t s{};
		get_string_property(KEY_TYPE, s);
		return (s);
	}
	void info_document::set_deleted(bool b /*= true*/) {
		set_bool_property(U("_deleted"), b);
	}
	string_t  info_document::id(void) const {
		string_t s{};
		obj_id(s);
		return (s);
	}
	void info_document::id(const string_t &s) {
		string_t ss = stringutils::trim(s);
		if (ss.empty()) {
			erase_property(KEY_ID);
		}
		else {
			set_string_property(KEY_ID, ss);
		}
	}
	string_t info_document::version(void) const {
		string_t s{};
		obj_version(s);
		return (s);
	}
	void info_document::version(const string_t &s) {
		string_t ss = stringutils::trim(s);
		if (ss.empty()) {
			erase_property(KEY_VERSION);
		}
		else {
			set_string_property(KEY_VERSION, ss);
		}
	}
	bool info_document::is_persisted(void) const {
		string_t s1 = id();
		string_t s2 = version();
		return ((!s1.empty()) && (!s2.empty()));
	}
	
	bool info_document::load_by_id(datastore_manager &oMan, info_document &doc) const {
		string_t docid = id();
		if (docid.empty()) {
			return false;
		}
		infomap oMap{};
		oMap[KEY_ID] = any{ docid };
		infomap mx = oMan.find_document_async(oMap).get();
		bool bRet = (mx.find(KEY_ID) != mx.end()) && (mx.find(KEY_VERSION) != mx.end());
		if (bRet) {
			any vx{ mx };
			doc.set(vx);
			doc.check_blobs_urls(oMan.get_base_url());
		}
		return (bRet);
	}
	bool info_document::load_unique(datastore_manager &oMan, info_document &doc) const {
		std::vector<std::vector<string_t>> vvec{};
		this->unique_properties(vvec);
		for (auto vec : vvec) {
			bool bFound{ true };
			query_filter filter{};
			for (auto key : vec) {
				any va{};
				if (!get_any_property(key, va)) {
					bFound = false;
					break;
				}
				if (!INFO_ANY_HAS_VALUE(va)) {
					bFound = false;
					break;
				}
				filter.add_equals(key, va);
			}// key
			if (bFound && (!filter.empty())) {
				filter.set_limit(1);
				filter.set_skip(0);
				std::vector<infomap> vv = oMan.query_documents_async(filter).get();
				if (!vv.empty()) {
					infomap mx = vv[0];
					if ((mx.find(KEY_ID) != mx.end()) && (mx.find(KEY_VERSION) != mx.end())) {
						any vx{ mx };
						doc.set(vx);
						doc.check_blobs_urls(oMan.get_base_url());
						return (true);
					}
				}
			}// bFound
		}// vec
		return (false);
	}
	bool info_document::is_storable(void) const {
		string_t sid = id();
		if (!sid.empty()) {
			return (true);
		}
		std::vector<std::vector<string_t>> vvec{};
		unique_properties(vvec);
		for (auto vec : vvec) {
			bool bFound{ true };
			for (auto key : vec) {
				any va{};
				if (!get_any_property(key, va)) {
					bFound = false;
					break;
				}
				if (!INFO_ANY_HAS_VALUE(va)) {
					bFound = false;
					break;
				}
			}// key
			if (bFound) {
				return (true);
			}
		}// vec
		return (false);
	}
	std::future<bool> info_document::load(datastore_manager &oMan) {
		datastore_manager *pMan = &oMan;
		return std::async([this, pMan]()->bool {
			info_document doc{};
			string_t docid = this->id();
			if (!docid.empty()) {
				if (this->load_by_id(*pMan, doc)) {
					if (!doc.empty()) {
						this->set(doc.get());
						this->check_blobs_urls(pMan->get_base_url());
						return (true);
					}
				}
			}
			if (this->load_unique(*pMan, doc)) {
				if (!doc.empty()) {
					this->set(doc.get());
					this->check_blobs_urls(pMan->get_base_url());
					return (true);
				}
			}
			return (false);
		});
	}
	std::future<bool> info_document::save(datastore_manager &oMan) {
		datastore_manager *pMan = &oMan;
		return std::async([this, pMan]()->bool {
			if (!this->is_storable()) {
				return (false);
			}
			string_t docid = this->id();
			std::vector<std::vector<string_t>> vvec{};
			this->unique_properties(vvec);
			for (auto vec : vvec) {
				bool bFound{ true };
				query_filter filter{};
				for (auto key : vec) {
					any va{};
					if (!get_any_property(key, va)) {
						bFound = false;
						break;
					}
					if (!INFO_ANY_HAS_VALUE(va)) {
						bFound = false;
						break;
					}
					filter.add_equals(key, va);
				}// key
				if (bFound && (!filter.empty())) {
					filter.set_limit(1);
					filter.set_skip(0);
					std::vector<infomap> vv = pMan->query_documents_async(filter).get();
					if (!vv.empty()) {
						infomap mx = vv[0];
						if ((mx.find(KEY_ID) != mx.end()) && (mx.find(KEY_VERSION) != mx.end())) {
							any vx = mx[KEY_ID];
							if (vx.type() == typeid(string_t)) {
								string_t sx = INFO_ANY_CAST<string_t>(vx);
								if (!sx.empty()) {
									if ((!docid.empty()) && (sx != docid)) {
										return (false);
									}
									else if (docid.empty()) {
										docid = sx;
										this->id(sx);
									}
								}// sx
							}// type
						}// mx
					}
				}// bFound
			}// vec
			any vz{ this->get() };
			infomap mzz = INFO_ANY_CAST<infomap>(vz);
			infomap mz = pMan->maintains_document_async(mzz).get();
			if ((mz.find(KEY_ID) != mz.end()) && (mz.find(KEY_VERSION) != mz.end())) {
				any vk{ mz };
				this->set(vk);
				return (true);
			}
			return (false);
		});
	}
	std::future<bool> info_document::remove(datastore_manager &oMan) {
		datastore_manager *pMan = &oMan;
		return std::async([this, pMan]()->bool {
			bool b = this->load(*pMan).get();
			if (!b) {
				return (false);
			}
			any vz{ this->get() };
			infomap mzz = INFO_ANY_CAST<infomap>(vz);
			b = pMan->remove_document_async(mzz).get();
			return (b);
		});
	}
	void info_document::get_descs(std::set<info_fielddesc> &oSet) const {
		oSet.clear();
	}
	////////////////////////////
}// namespace info
