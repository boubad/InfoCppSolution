#pragma once
#ifndef INFOUTILS_H__
#define INFOUTILS_H__
//////////////////////////
#include "InfoDefs.h"
////////////////////////////
namespace info {
	/////////////////////////////
	class serverport {
	private:
		int value;
	public:
		serverport(int nPort = 0) :value(nPort) {}
		serverport(const serverport &other) :value(other.value) {}
		serverport & operator=(const serverport &other) {
			if (this != &other) {
				this->value = other.value;
			}
			return (*this);
		}
		~serverport() {}
		operator int(void) const { return value; }
		string_t to_string(void) const {
			stringstream_t os{};
			os << value;
			return (os.str());
		}
		string_t toString(void) const {
			stringstream_t os{};
			os << value;
			return (os.str());
		}
	};// struct serverport
	inline ostream_t & operator<<(ostream_t &os, const serverport &d) {
		os << ((int)d);
		return os;
	}
	////////////////////////////
	class hostname {
	private:
		string_t value;
	public:
		hostname(const string_t &v = string_t{}) :value(v) {}
		hostname(const hostname &other) :value(other.value) {}
		hostname & operator=(const hostname &other) {
			if (this != &other) {
				this->value = other.value;
			}
			return (*this);
		}
		~hostname() {}
		operator const string_t &  (void) const { return value; }
		bool empty(void) const { return value.empty(); }
		string_t to_string(void) const { return value; }
		string_t toString(void) const { return value; }
	};// struct hostname
	inline ostream_t & operator<<(ostream_t &os, const hostname &d) {
		os << ((const string_t &)d);
		return os;
	}
	//////////////////////////
	class filename {
	private:
		string_t value;
	public:
		filename(const string_t &v = string_t{}) :value(v) {}
		filename(const filename &other) :value(other.value) {}
		filename & operator=(const filename &other) {
			if (this != &other) {
				this->value = other.value;
			}
			return (*this);
		}
		~filename() {}
		operator const string_t &  (void) const { return value; }
		bool empty(void) const { return value.empty(); }
		string_t to_string(void) const { return value; }
		string_t toString(void) const { return value; }
	};// struct filename
	inline ostream_t & operator<<(ostream_t &os, const filename &d) {
		os << ((const string_t &)d);
		return os;
	}
	//
	class databasename {
	private:
		string_t value;
	public:
		databasename(const string_t &v = string_t{}) :value(v) {}
		databasename(const databasename &other) :value(other.value) {}
		databasename & operator=(const databasename &other) {
			if (this != &other) {
				this->value = other.value;
			}
			return (*this);
		}
		~databasename() {}
		operator const string_t & (void) const { return value; }
		bool empty(void) const { return value.empty(); }
		string_t to_string(void) const { return value; }
		string_t toString(void) const { return value; }
	};// struct databasename
	inline ostream_t & operator<<(ostream_t &os, const databasename &d) {
		os << ((const string_t &)d);
		return os;
	}
	/////////////////////////////
	class collectionname {
	private:
		string_t value;
	public:
		collectionname(const string_t &v = string_t{}) :value(v) {}
		collectionname(const collectionname &other) :value(other.value) {}
		collectionname & operator=(const collectionname &other) {
			if (this != &other) {
				this->value = other.value;
			}
			return (*this);
		}
		~collectionname() {}
		operator const string_t & (void) const { return value; }
		bool empty(void) const { return value.empty(); }
		string_t to_string(void) const { return value; }
		string_t toString(void) const { return value; }
	};// struct collectionname
	inline ostream_t & operator<<(ostream_t &os, const collectionname &d) {
		os << ((const string_t &)d);
		return os;
	}
	//////////////////////////////
	class filedelimiter {
	private:
		string_t value;
	public:
		filedelimiter(const string_t &v = U(";")) :value(v) {}
		filedelimiter(const filedelimiter &other) :value(other.value) {}
		filedelimiter & operator=(const filedelimiter &other) {
			if (this != &other) {
				this->value = other.value;
			}
			return (*this);
		}
		~filedelimiter() {}
		operator const string_t &  (void) const { return value; }
		bool empty(void) const { return value.empty(); }
		string_t to_string(void) const { return value; }
		string_t toString(void) const { return value; }
	};// struct filedelimiter
	inline ostream_t & operator<<(ostream_t &os, const filedelimiter &d) {
		os << ((const string_t &)d);
		return os;
	}
	////////////////////////////
	class serverurl {
	private:
		string_t value;
	public:
		serverurl(const string_t &v = string_t{}) :value(v) {}
		serverurl(const serverurl &other) :value(other.value) {}
		serverurl & operator=(const serverurl &other) {
			if (this != &other) {
				this->value = other.value;
			}
			return (*this);
		}
		~serverurl() {}
		operator const string_t &  (void) const { return value; }
		bool empty(void) const { return value.empty(); }
		string_t to_string(void) const { return value; }
		string_t toString(void) const { return value; }
	};// struct serverurl
	inline ostream_t & operator<<(ostream_t &os, const serverurl &d) {
		os << ((const string_t &)d);
		return os;
	}
	/////////////////////////////
	class dataserviceuri {
	private:
		string_t value;
	public:
		dataserviceuri(const string_t &v = string_t{}) :value(v) {}
		dataserviceuri(const dataserviceuri &other) :value(other.value) {}
		dataserviceuri & operator=(const dataserviceuri &other) {
			if (this != &other) {
				this->value = other.value;
			}
			return (*this);
		}
		~dataserviceuri() {}
		operator const string_t &  (void) const { return value; }
		bool empty(void) const { return value.empty(); }
		string_t to_string(void) const { return value; }
		string_t toString(void) const { return value; }
	};// struct dataserviceuri
	inline ostream_t & operator<<(ostream_t &os, const dataserviceuri &d) {
		os << ((const string_t &)d);
		return os;
	}
	/////////////////////////////
	class username {
	private:
		string_t value;
	public:
		username(const string_t &v = string_t{}) :value(v) {}
		username(const username &other) :value(other.value) {}
		username & operator=(const username &other) {
			if (this != &other) {
				this->value = other.value;
			}
			return (*this);
		}
		~username() {}
		operator const string_t &  (void) const { return value; }
		bool empty(void) const { return value.empty(); }
		string_t to_string(void) const { return value; }
		string_t toString(void) const { return value; }
	};// struct username
	inline ostream_t & operator<<(ostream_t &os, const username &d) {
		os << ((const string_t &)d);
		return os;
	}
	/////////////////////////////
	class password {
	private:
		string_t value;
	public:
		password(const string_t &v = string_t{}) :value(v) {}
		password(const password &other) :value(other.value) {}
		password & operator=(const password &other) {
			if (this != &other) {
				this->value = other.value;
			}
			return (*this);
		}
		~password() {}
		operator const string_t &  (void) const { return value; }
		bool empty(void) const { return value.empty(); }
		string_t to_string(void) const { return value; }
		string_t toString(void) const { return value; }
	};// struct password
	inline ostream_t & operator<<(ostream_t &os, const password &d) {
		os << ((const string_t &)d);
		return os;
	}
	/////////////////////////////
}// namespace info
 /////////////////////////////
#endif // INFOUTILS_H__
