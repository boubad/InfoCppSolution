#include "info_etudiant.h"
/////////////////////////////
#include <stringutils.h>
#include <couchdb_manager.h>
#include <algorithm>
////////////////////////
namespace info {
	namespace domain {
		//////////////////////////////
		using namespace info::couchdb;
		///////////////////////////////
		void info_etudiant::check_type(void) {
			type(TYPE_ETUD);
			check_fullname();
		}
		void info_etudiant::post_init(void) {
			check_type();
		}
		void info_etudiant::unique_properties(std::vector<std::vector<string_t>> &vvec) const {
			vvec.clear();
			{
				nullable_string s = dossier();
				if (INFO_OPTIONAL_HAS_VALUE(s)) {
					string_t ss = *s;
					if (!ss.empty()) {
						std::vector<string_t> vec{ KEY_TYPE,KEY_DOSSIER };
						vvec.push_back(vec);
					}
				}
			}// dossier
			{
				nullable_string s = username();
				if (INFO_OPTIONAL_HAS_VALUE(s)) {
					string_t ss = *s;
					if (!ss.empty()) {
						std::vector<string_t> vec{ KEY_TYPE,KEY_USERNAME };
						vvec.push_back(vec);
					}
				}
			}// username
			{
				nullable_string s = email();
				if (INFO_OPTIONAL_HAS_VALUE(s)) {
					string_t ss = *s;
					if (!ss.empty()) {
						std::vector<string_t> vec{ KEY_TYPE,KEY_EMAIL };
						vvec.push_back(vec);
					}
				}
			}// email
			{
				nullable_string s1 = lastname();
				nullable_string s2 = firstname();
				nullable_int n = birthdate();
				if (INFO_OPTIONAL_HAS_VALUE(s1) && INFO_OPTIONAL_HAS_VALUE(s2) &&
					INFO_OPTIONAL_HAS_VALUE(n)) {
					std::vector<string_t> vec{ KEY_TYPE,KEY_LASTNAME,KEY_FIRSTNAME };
					vvec.push_back(vec);
				}
			}// names
		}
		info_etudiant::info_etudiant() {
			check_type();
		}
		info_etudiant::info_etudiant(const any &oMap) : info_baseitem(oMap) {
			check_type();
		}
		info_etudiant::info_etudiant(const info_etudiant &other) : info_baseitem(other) {

		}
		info_etudiant & info_etudiant::operator=(const info_etudiant &other) {
			if (this != &other) {
				info_baseitem::operator=(other);
			}
			return (*this);
		}
		info_etudiant::~info_etudiant() {

		}
		bool info_etudiant::operator<(const info_etudiant &other) const {
			string_t s1{}, s2{};
			get_string_property(KEY_LASTNAME, s1);
			other.get_string_property(KEY_LASTNAME, s2);
			if (s1 < s2) {
				return (true);
			}
			else if (s1 > s2) {
				return (false);
			}
			s1.clear();
			s2.clear();
			get_string_property(KEY_FIRSTNAME, s1);
			other.get_string_property(KEY_FIRSTNAME, s2);
			if (s1 < s2) {
				return (true);
			}
			else if (s1 > s2) {
				return (false);
			}
			int n1{ 0 }, n2{ 0 };
			get_int_property(KEY_BIRTHDATE, n1);
			other.get_int_property(KEY_BIRTHDATE, n2);
			if (n2 > n1) {
				return (true);
			}
			else if (n2 < n1) {
				return (false);
			}
			return (false);
		}// operator<
		bool info_etudiant::operator==(const info_etudiant &other) const {
			string_t s1{}, s2{};
			get_string_property(KEY_LASTNAME, s1);
			other.get_string_property(KEY_LASTNAME, s2);
			if (s1 != s2) {
				return (false);
			}
			s1.clear();
			s2.clear();
			get_string_property(KEY_FIRSTNAME, s1);
			other.get_string_property(KEY_FIRSTNAME, s2);
			if (s1 != s2) {
				return (false);
			}
			int n1{ 0 }, n2{ 0 };
			get_int_property(KEY_BIRTHDATE, n1);
			other.get_int_property(KEY_BIRTHDATE, n2);
			return (n1 == n2);
		}
		string_t info_etudiant::avatar_url(void) const {
			string_t sRet{};
			nullable_string s0 = this->avatar();
			if (!INFO_OPTIONAL_HAS_VALUE(s0)) {
				return (sRet);
			}
			string_t s = *s0;
			std::vector<attachment_info> vv{};
			attachments(vv);
			auto it = std::find_if(vv.begin(), vv.end(), [s](const attachment_info &p)->bool {
				return (p.name() == s);
			});
			if (it != vv.end()) {
				const attachment_info &att = *it;
				sRet = att.url();
			}
			return (sRet);
		}// avatar_url
		bool info_etudiant::has_avatar_url(void) const {
			string_t s = avatar_url();
			return (!s.empty());
		}
		void info_etudiant::check_fullname(void) {
			string_t s1{}, s2{};
			get_string_property(KEY_LASTNAME, s1);
			get_string_property(KEY_FIRSTNAME, s2);
			string_t ss = stringutils::trim(s1 + U(" ") + s2);
			set_string_property(KEY_FULLNAME, ss);
		}// check_fullname
		nullable_int info_etudiant::annee(void) const {
			int n{ 0 };
			if (get_int_property(KEY_ANNEE, n)) {
				return (nullable_int{ n });
			}
			return nullable_int{};
		}
		void info_etudiant::annee(int n) {
			if (n < 1) {
				erase_property(KEY_ANNEE);
			}
			else {
				set_int_property(KEY_ANNEE, n);
			}
		}
		nullable_int info_etudiant::apb(void) const {
			int n{ 0 };
			if (get_int_property(KEY_APB, n)) {
				return nullable_int{ n };
			}
			else {
				return nullable_int{};
			}
		}
		void info_etudiant::apb(int n) {
			if (n < 1) {
				erase_property(KEY_APB);
			}
			else {
				set_int_property(KEY_APB, n);
			}
		}

		nullable_int info_etudiant::birthdate(void) const {
			int n{ 0 };
			if (get_int_property(KEY_BIRTHDATE, n)) {
				return nullable_int{ n };
			}
			return nullable_int{};
		}
		void info_etudiant::birthdate(int n) {
			if (n < 1) {
				erase_property(KEY_BIRTHDATE);
			}
			else {
				set_int_property(KEY_BIRTHDATE, n);
			}
		}
		nullable_string info_etudiant::groupe(void) const {
			string_t s{};
			if (get_string_property(KEY_GROUPE, s)) {
				return nullable_string{ s };
			}
			return nullable_string{};
		}
		void info_etudiant::groupe(const string_t &s) {
			string_t ss = stringutils::toupper(stringutils::trim(s));
			if (ss.empty()) {
				erase_property(KEY_GROUPE);
			}
			else {
				set_string_property(KEY_GROUPE, ss);
			}
		}
		nullable_string info_etudiant::username(void) const {
			string_t s{};
			if (get_string_property(KEY_USERNAME, s)) {
				return nullable_string{ s };
			}
			return nullable_string{};
		}
		void info_etudiant::username(const string_t &s) {
			string_t ss = stringutils::tolower(stringutils::trim(s));
			if (ss.empty()) {
				erase_property(KEY_USERNAME);
			}
			else {
				set_string_property(KEY_USERNAME, ss);
			}
		}
		nullable_string info_etudiant::dossier(void) const {
			string_t s{};
			if (get_string_property(KEY_DOSSIER, s)) {
				return nullable_string{ s };
			}
			return nullable_string{};
		}
		void info_etudiant::dossier(const string_t &s) {
			string_t ss = stringutils::toupper(stringutils::trim(s));
			if (ss.empty()) {
				erase_property(KEY_DOSSIER);
			}
			else {
				set_string_property(KEY_DOSSIER, ss);
			}
		}
		nullable_string info_etudiant::firstname(void) const {
			string_t s{};
			if (get_string_property(KEY_FIRSTNAME, s)) {
				return nullable_string{ s };
			}
			return nullable_string{};
		}
		void info_etudiant::firstname(const string_t &s) {
			string_t s1 = stringutils::trim(s);
			if (!s1.empty()) {
				if (s1.length() > 1) {
					s1 = stringutils::toupper(s1.substr(0, 1)) + stringutils::tolower(s1.substr(1));
				}
				else {
					s1 = stringutils::toupper(s1);
				}
			}// not empty
			if (s1.empty()) {
				erase_property(KEY_FIRSTNAME);
			}
			else {
				set_string_property(KEY_FIRSTNAME, s1);
			}
			check_fullname();
		}
		nullable_string info_etudiant::lastname(void) const {
			string_t s{};
			if (get_string_property(KEY_LASTNAME, s)) {
				return nullable_string{ s };
			}
			return nullable_string{};
		}
		void info_etudiant::lastname(const string_t &s) {
			string_t ss = stringutils::toupper(stringutils::trim(s));
			if (ss.empty()) {
				erase_property(KEY_LASTNAME);
			}
			else {
				set_string_property(KEY_LASTNAME, ss);
			}
			check_fullname();
		}
		nullable_string info_etudiant::fullname(void) const {
			string_t s{};
			if (get_string_property(KEY_FULLNAME, s)) {
				return nullable_string{ s };
			}
			return nullable_string{};
		}
		nullable_string info_etudiant::sexe(void) const {
			string_t s{};
			if (get_string_property(KEY_SEXE, s)) {
				return nullable_string{ s };
			}
			return nullable_string{};
		}
		void info_etudiant::sexe(const string_t &s) {
			string_t ss = stringutils::toupper(stringutils::trim(s));
			if (ss.empty()) {
				erase_property(KEY_SEXE);
			}
			else {
				set_string_property(KEY_SEXE, ss);
			}
		}
		nullable_string info_etudiant::departement(void) const {
			string_t s{};
			if (get_string_property(KEY_DEPARTEMENT, s)) {
				return nullable_string{ s };
			}
			return nullable_string{};
		}
		void info_etudiant::departement(const string_t &s) {
			string_t ss = stringutils::toupper(stringutils::trim(s));
			if (ss.empty()) {
				erase_property(KEY_DEPARTEMENT);
			}
			else {
				set_string_property(KEY_DEPARTEMENT, ss);
			}
		}
		nullable_string info_etudiant::ville(void) const {
			string_t s{};
			if (get_string_property(KEY_VILLE, s)) {
				return nullable_string{ s };
			}
			return nullable_string{};
		}
		void info_etudiant::ville(const string_t &s) {
			string_t ss = stringutils::toupper(stringutils::trim(s));
			if (ss.empty()) {
				erase_property(KEY_VILLE);
			}
			else {
				set_string_property(KEY_VILLE, ss);
			}
		}
		nullable_string info_etudiant::etablissement(void) const {
			string_t s{};
			if (get_string_property(KEY_ETABLISSEMENT, s)) {
				return nullable_string{ s };
			}
			return nullable_string{};
		}
		void info_etudiant::etablissement(const string_t &s) {
			string_t ss = stringutils::toupper(stringutils::trim(s));
			if (ss.empty()) {
				erase_property(KEY_ETABLISSEMENT);
			}
			else {
				set_string_property(KEY_ETABLISSEMENT, ss);
			}
		}
		nullable_string info_etudiant::seriebac(void) const {
			string_t s{};
			if (get_string_property(KEY_SERIEBAC, s)) {
				return nullable_string{ s };
			}
			return nullable_string{};
		}
		void info_etudiant::seriebac(const string_t &s) {
			string_t ss = stringutils::toupper(stringutils::trim(s));
			if (ss.empty()) {
				erase_property(KEY_SERIEBAC);
			}
			else {
				set_string_property(KEY_SERIEBAC, ss);
			}
		}
		nullable_string info_etudiant::optionbac(void) const {
			string_t s{};
			if (get_string_property(KEY_OPTIONBAC, s)) {
				return nullable_string{ s };
			}
			return nullable_string{};
		}
		void info_etudiant::optionbac(const string_t &s) {
			string_t ss = stringutils::toupper(stringutils::trim(s));
			if (ss.empty()) {
				erase_property(KEY_OPTIONBAC);
			}
			else {
				set_string_property(KEY_OPTIONBAC, ss);
			}
		}
		nullable_string info_etudiant::mentionbac(void) const {
			string_t s{};
			if (get_string_property(KEY_MENTIONBAC, s)) {
				return nullable_string{ s };
			}
			return nullable_string{};
		}
		void info_etudiant::mentionbac(const string_t &s) {
			string_t ss = stringutils::toupper(stringutils::trim(s));
			if (ss.empty()) {
				erase_property(KEY_MENTIONBAC);
			}
			else {
				set_string_property(KEY_MENTIONBAC, ss);
			}
		}
		nullable_string info_etudiant::etudessuperieures(void) const {
			string_t s{};
			if (get_string_property(KEY_ETUDESSUPERIEURES, s)) {
				return nullable_string{ s };
			}
			return nullable_string{};
		}
		void info_etudiant::etudessuperieures(const string_t &s) {
			string_t ss = stringutils::toupper(stringutils::trim(s));
			if (ss.empty()) {
				erase_property(KEY_ETUDESSUPERIEURES);
			}
			else {
				set_string_property(KEY_ETUDESSUPERIEURES, ss);
			}
		}
		nullable_string info_etudiant::redoublant(void) const {
			string_t s{};
			if (get_string_property(KEY_REDOUBLANT, s)) {
				return nullable_string{ s };
			}
			return nullable_string{};
		}
		void info_etudiant::redoublant(const string_t &s) {
			string_t ss = stringutils::toupper(stringutils::trim(s));
			if (ss.empty()) {
				erase_property(KEY_REDOUBLANT);
			}
			else {
				set_string_property(KEY_REDOUBLANT, ss);
			}
		}
		nullable_string info_etudiant::avatar(void) const {
			string_t s{};
			if (get_string_property(KEY_AVATAR, s)) {
				return nullable_string{ s };
			}
			return nullable_string{};
		}
		void info_etudiant::avatar(const string_t &s) {
			string_t ss = stringutils::trim(s);
			if (ss.empty()) {
				erase_property(KEY_AVATAR);
			}
			else {
				set_string_property(KEY_AVATAR, ss);
			}
		}
		nullable_string info_etudiant::email(void) const {
			string_t s{};
			if (get_string_property(KEY_EMAIL, s)) {
				return nullable_string{ s };
			}
			return nullable_string{};
		}
		void info_etudiant::email(const string_t &s) {
			string_t ss = stringutils::trim(s);
			if (ss.empty()) {
				erase_property(KEY_EMAIL);
			}
			else {
				set_string_property(KEY_EMAIL, ss);
			}
		}
		nullable_string  info_etudiant::phone(void) const {
			string_t s{};
			if (get_string_property(KEY_PHONE, s)) {
				return nullable_string{ s };
			}
			return nullable_string{};
		}
		void info_etudiant::phone(const string_t &s) {
			string_t ss = stringutils::trim(s);
			if (ss.empty()) {
				erase_property(KEY_PHONE);
			}
			else {
				set_string_property(KEY_PHONE, ss);
			}
		}
		optional<std::vector<string_t>> info_etudiant::datasets_sigles(void) const {
			std::vector<string_t> vRet{};
			if (this->get_stringvector_property(KEY_DATASETS, vRet)) {
				return optional<std::vector<string_t>>{vRet};
			}
			else {
				return optional<std::vector<string_t>>{vRet};
			}
		}
		void info_etudiant::datasets_sigles(const std::vector<string_t> &vec) {
			set_stringvector_property(KEY_DATASETS, vec);
		}
		////////////////////////////////////
		info_etudiant::note_t  info_etudiant::ue11(void) const {
			double d{ 0 };
			if (get_double_property(KEY_UE11, d)) {
				return note_t{ d };
			}
			else {
				return note_t{};
			}
		}
		void info_etudiant::ue11(double n) {
			if ((n < 0.0) || (n > 20.0)) {
				erase_property(KEY_UE11);
			}
			else {
				set_double_property(KEY_UE11, n);
			}
		}
		info_etudiant::note_t  info_etudiant::ue12(void) const {
			double d{ 0 };
			if (get_double_property(KEY_UE12, d)) {
				return note_t{ d };
			}
			else {
				return note_t{};
			}
		}
		void info_etudiant::ue12(double n) {
			if ((n < 0.0) || (n > 20.0)) {
				erase_property(KEY_UE12);
			}
			else {
				set_double_property(KEY_UE12, n);
			}
		}
		info_etudiant::note_t  info_etudiant::ue13(void) const {
			double d{ 0 };
			if (get_double_property(KEY_UE13, d)) {
				return note_t{ d };
			}
			else {
				return note_t{};
			}
		}
		void info_etudiant::ue13(double n) {
			if ((n < 0.0) || (n > 20.0)) {
				erase_property(KEY_UE13);
			}
			else {
				set_double_property(KEY_UE13, n);
			}
		}
		info_etudiant::note_t  info_etudiant::moys1(void) const {
			double d{ 0 };
			if (get_double_property(KEY_MOYS1, d)) {
				return note_t{ d };
			}
			else {
				return note_t{};
			}
		}
		void info_etudiant::moys1(double n) {
			if ((n < 0.0) || (n > 20.0)) {
				erase_property(KEY_MOYS1);
			}
			else {
				set_double_property(KEY_MOYS1, n);
			}
		}
		optional<info_resulttype> info_etudiant::result_s1(void) const {
			int n{ 0 };
			if (get_int_property(KEY_RES1, n)) {
				return optional<info_resulttype>(static_cast<info_resulttype>(n));
			}
			else {
				return optional<info_resulttype>();
			}
		}
		void info_etudiant::result_s1(info_resulttype t) {
			if (t == info_resulttype::unkown) {
				erase_property(KEY_RES1);
			}
			else {
				set_int_property(KEY_RES1, static_cast<int>(t));
			}
		}
		nullable_int info_etudiant::rang_s1(void) const {
			int n{ 0 };
			if (get_int_property(KEY_RGS1, n)) {
				return nullable_int{ n };
			}
			else {
				return nullable_int{};
			}
		}
		void info_etudiant::rang_s1(int n) {
			if (n < 1) {
				erase_property(KEY_RGS1);
			}
			else {
				set_int_property(KEY_RGS1, n);
			}
		}
		nullable_string info_etudiant::rem_s1(void) const {
			string_t s{};
			if (get_string_property(KEY_REMS1, s)) {
				return nullable_string{ s };
			}
			else {
				return nullable_string{};
			}
		}
		void info_etudiant::rem_s1(const string_t &s) {
			set_string_property(KEY_REMS1, s);
		}
		//
		info_etudiant::note_t  info_etudiant::ue21(void) const {
			double d{ 0 };
			if (get_double_property(KEY_UE21, d)) {
				return note_t{ d };
			}
			else {
				return note_t{};
			}
		}
		void info_etudiant::ue21(double n) {
			if ((n < 0.0) || (n > 20.0)) {
				erase_property(KEY_UE21);
			}
			else {
				set_double_property(KEY_UE21, n);
			}
		}
		info_etudiant::note_t  info_etudiant::ue22(void) const {
			double d{ 0 };
			if (get_double_property(KEY_UE22, d)) {
				return note_t{ d };
			}
			else {
				return note_t{};
			}
		}
		void info_etudiant::ue22(double n) {
			if ((n < 0.0) || (n > 20.0)) {
				erase_property(KEY_UE22);
			}
			else {
				set_double_property(KEY_UE22, n);
			}
		}
		info_etudiant::note_t  info_etudiant::ue23(void) const {
			double d{ 0 };
			if (get_double_property(KEY_UE23, d)) {
				return note_t{ d };
			}
			else {
				return note_t{};
			}
		}
		void info_etudiant::ue23(double n) {
			if ((n < 0.0) || (n > 20.0)) {
				erase_property(KEY_UE23);
			}
			else {
				set_double_property(KEY_UE23, n);
			}
		}
		info_etudiant::note_t  info_etudiant::moys2(void) const {
			double d{ 0 };
			if (get_double_property(KEY_MOYS2, d)) {
				return note_t{ d };
			}
			else {
				return note_t{};
			}
		}
		void info_etudiant::moys2(double n) {
			if ((n < 0.0) || (n > 20.0)) {
				erase_property(KEY_MOYS2);
			}
			else {
				set_double_property(KEY_MOYS2, n);
			}
		}
		optional<info_resulttype> info_etudiant::result_s2(void) const {
			int n{ 0 };
			if (get_int_property(KEY_RES2, n)) {
				return optional<info_resulttype>(static_cast<info_resulttype>(n));
			}
			else {
				return optional<info_resulttype>();
			}
		}
		void info_etudiant::result_s2(info_resulttype t) {
			if (t == info_resulttype::unkown) {
				erase_property(KEY_RES2);
			}
			else {
				set_int_property(KEY_RES2, static_cast<int>(t));
			}
		}
		nullable_int info_etudiant::rang_s2(void) const {
			int n{ 0 };
			if (get_int_property(KEY_RGS2, n)) {
				return nullable_int{ n };
			}
			else {
				return nullable_int{};
			}
		}
		void info_etudiant::rang_s2(int n) {
			if (n < 1) {
				erase_property(KEY_RGS2);
			}
			else {
				set_int_property(KEY_RGS2, n);
			}
		}
		info_etudiant::note_t info_etudiant::comp_s1s2(void) const {
			double d{ 0 };
			if (get_double_property(KEY_COMPS1S2, d)) {
				return note_t{ d };
			}
			else {
				return note_t{};
			}
		}
		void info_etudiant::comp_s1s2(double n) {
			if ((n < 0.0) || (n > 20.0)) {
				erase_property(KEY_COMPS1S2);
			}
			else {
				set_double_property(KEY_COMPS1S2, n);
			}
		}
		nullable_string info_etudiant::rem_s2(void) const {
			string_t s{};
			if (get_string_property(KEY_REMS2, s)) {
				return nullable_string{ s };
			}
			else {
				return nullable_string{};
			}
		}
		void info_etudiant::rem_s2(const string_t &s) {
			set_string_property(KEY_REMS2, s);
		}
		//
		info_etudiant::note_t  info_etudiant::ue31(void) const {
			double d{ 0 };
			if (get_double_property(KEY_UE31, d)) {
				return note_t{ d };
			}
			else {
				return note_t{};
			}
		}
		void info_etudiant::ue31(double n) {
			if ((n < 0.0) || (n > 20.0)) {
				erase_property(KEY_UE31);
			}
			else {
				set_double_property(KEY_UE31, n);
			}
		}
		info_etudiant::note_t  info_etudiant::ue32(void) const {
			double d{ 0 };
			if (get_double_property(KEY_UE32, d)) {
				return note_t{ d };
			}
			else {
				return note_t{};
			}
		}
		void info_etudiant::ue32(double n) {
			if ((n < 0.0) || (n > 20.0)) {
				erase_property(KEY_UE32);
			}
			else {
				set_double_property(KEY_UE32, n);
			}
		}
		info_etudiant::note_t  info_etudiant::ue33(void) const {
			double d{ 0 };
			if (get_double_property(KEY_UE33, d)) {
				return note_t{ d };
			}
			else {
				return note_t{};
			}
		}
		void info_etudiant::ue33(double n) {
			if ((n < 0.0) || (n > 20.0)) {
				erase_property(KEY_UE33);
			}
			else {
				set_double_property(KEY_UE33, n);
			}
		}
		info_etudiant::note_t  info_etudiant::moys3(void) const {
			double d{ 0 };
			if (get_double_property(KEY_MOYS3, d)) {
				return note_t{ d };
			}
			else {
				return note_t{};
			}
		}
		void info_etudiant::moys3(double n) {
			if ((n < 0.0) || (n > 20.0)) {
				erase_property(KEY_MOYS3);
			}
			else {
				set_double_property(KEY_MOYS3, n);
			}
		}
		optional<info_resulttype> info_etudiant::result_s3(void) const {
			int n{ 0 };
			if (get_int_property(KEY_RES3, n)) {
				return optional<info_resulttype>(static_cast<info_resulttype>(n));
			}
			else {
				return optional<info_resulttype>();
			}
		}
		void info_etudiant::result_s3(info_resulttype t) {
			if (t == info_resulttype::unkown) {
				erase_property(KEY_RES3);
			}
			else {
				set_int_property(KEY_RES3, static_cast<int>(t));
			}
		}
		nullable_int info_etudiant::rang_s3(void) const {
			int n{ 0 };
			if (get_int_property(KEY_RGS3, n)) {
				return nullable_int{ n };
			}
			else {
				return nullable_int{};
			}
		}
		void info_etudiant::rang_s3(int n) {
			if (n < 1) {
				erase_property(KEY_RGS3);
			}
			else {
				set_int_property(KEY_RGS3, n);
			}
		}
		info_etudiant::note_t info_etudiant::comp_s2s3(void) const {
			double d{ 0 };
			if (get_double_property(KEY_COMPS2S3, d)) {
				return note_t{ d };
			}
			else {
				return note_t{};
			}
		}
		void info_etudiant::comp_s2s3(double n) {
			if ((n < 0.0) || (n > 20.0)) {
				erase_property(KEY_COMPS2S3);
			}
			else {
				set_double_property(KEY_COMPS2S3, n);
			}
		}
		nullable_string info_etudiant::rem_s3(void) const {
			string_t s{};
			if (get_string_property(KEY_REMS3, s)) {
				return nullable_string{ s };
			}
			else {
				return nullable_string{};
			}
		}
		void info_etudiant::rem_s3(const string_t &s) {
			set_string_property(KEY_REMS3, s);
		}
		//
		info_etudiant::note_t  info_etudiant::ue41(void) const {
			double d{ 0 };
			if (get_double_property(KEY_UE41, d)) {
				return note_t{ d };
			}
			else {
				return note_t{};
			}
		}
		void info_etudiant::ue41(double n) {
			if ((n < 0.0) || (n > 20.0)) {
				erase_property(KEY_UE41);
			}
			else {
				set_double_property(KEY_UE41, n);
			}
		}
		info_etudiant::note_t  info_etudiant::ue42(void) const {
			double d{ 0 };
			if (get_double_property(KEY_UE42, d)) {
				return note_t{ d };
			}
			else {
				return note_t{};
			}
		}
		void info_etudiant::ue42(double n) {
			if ((n < 0.0) || (n > 20.0)) {
				erase_property(KEY_UE42);
			}
			else {
				set_double_property(KEY_UE42, n);
			}
		}
		info_etudiant::note_t  info_etudiant::ue43(void) const {
			double d{ 0 };
			if (get_double_property(KEY_UE43, d)) {
				return note_t{ d };
			}
			else {
				return note_t{};
			}
		}
		void info_etudiant::ue43(double n) {
			if ((n < 0.0) || (n > 20.0)) {
				erase_property(KEY_UE43);
			}
			else {
				set_double_property(KEY_UE43, n);
			}
		}
		info_etudiant::note_t  info_etudiant::moys4(void) const {
			double d{ 0 };
			if (get_double_property(KEY_MOYS4, d)) {
				return note_t{ d };
			}
			else {
				return note_t{};
			}
		}
		void info_etudiant::moys4(double n) {
			if ((n < 0.0) || (n > 20.0)) {
				erase_property(KEY_MOYS4);
			}
			else {
				set_double_property(KEY_MOYS4, n);
			}
		}
		optional<info_resulttype> info_etudiant::result_s4(void) const {
			int n{ 0 };
			if (get_int_property(KEY_RES4, n)) {
				return optional<info_resulttype>(static_cast<info_resulttype>(n));
			}
			else {
				return optional<info_resulttype>();
			}
		}
		void info_etudiant::result_s4(info_resulttype t) {
			if (t == info_resulttype::unkown) {
				erase_property(KEY_RES4);
			}
			else {
				set_int_property(KEY_RES4, static_cast<int>(t));
			}
		}
		nullable_int info_etudiant::rang_s4(void) const {
			int n{ 0 };
			if (get_int_property(KEY_RGS4, n)) {
				return nullable_int{ n };
			}
			else {
				return nullable_int{};
			}
		}
		void info_etudiant::rang_s4(int n) {
			if (n < 1) {
				erase_property(KEY_RGS4);
			}
			else {
				set_int_property(KEY_RGS4, n);
			}
		}
		info_etudiant::note_t info_etudiant::comp_s3s4(void) const {
			double d{ 0 };
			if (get_double_property(KEY_COMPS3S4, d)) {
				return note_t{ d };
			}
			else {
				return note_t{};
			}
		}
		void info_etudiant::comp_s3s4(double n) {
			if ((n < 0.0) || (n > 20.0)) {
				erase_property(KEY_COMPS3S4);
			}
			else {
				set_double_property(KEY_COMPS3S4, n);
			}
		}
		nullable_string info_etudiant::rem_s4(void) const {
			string_t s{};
			if (get_string_property(KEY_REMS4, s)) {
				return nullable_string{ s };
			}
			else {
				return nullable_string{};
			}
		}
		void info_etudiant::rem_s4(const string_t &s) {
			set_string_property(KEY_REMS4, s);
		}
		///////////////////////////////////////////
		std::future<bool> info_etudiant::set_avatar_blob(couchdb_manager &oMan, const blob_data &blob) {
			couchdb_manager *pMan = &oMan;
			std::shared_ptr<blob_data> pv = std::make_shared <blob_data>(blob);
			return std::async([this, pMan, pv]()->bool {
				bool bRet{ false };
				update_response rsp = pMan->update_document_attachment_async(*this, *pv).get();
				if (rsp.ok()) {
					string_t sname = pv->name();
					this->avatar(sname);
					bRet = this->save(*pMan).get();
				}
				return (bRet);
			});
		}//set_avatar_blob
		std::future<int> info_etudiant::get_count_async(couchdb_manager &oMan, const query_filter &filter ) {
			couchdb_manager *pMan = &oMan;
			std::shared_ptr<query_filter> pf = std::make_shared < query_filter>(filter);
			return std::async([pMan, pf]()->int {
				query_filter f{ *pf };
				f.add_equals(KEY_TYPE, any{ TYPE_ETUD });
				int nRet = pMan->find_documents_count_async(f).get();
				return (nRet);
			});
		}//get_count_async
		std::future<std::vector<info_etudiant> > info_etudiant::get_async(info::couchdb::couchdb_manager &oMan,
			const info::couchdb::query_filter &filter) {
			couchdb_manager *pMan = &oMan;
			std::shared_ptr<query_filter> pf = std::make_shared < query_filter>(filter);
			return std::async([pMan, pf]()->std::vector<info_etudiant> {
				std::vector<info_etudiant> oRet{};
				query_filter f{ *pf };
				f.add_equals(KEY_TYPE, any{ TYPE_ETUD });
				std::vector<couchdb_doc> vv = pMan->find_documents_async(f).get();
				for (auto p : vv) {
					info_etudiant cur{ p.get() };
					oRet.push_back(cur);
				}// p
				return (oRet);
			});
		}//get_async
		std::future<std::vector<bool>> info_etudiant::maintains_async(const std::vector<info_etudiant> &ovec,
			info::couchdb::couchdb_manager &oMan) {
			couchdb_manager *pMan = &oMan;
			std::shared_ptr<std::vector<info_etudiant>> pv = std::make_shared<std::vector<info_etudiant>>(ovec);
			return std::async([pMan, pv]()->std::vector<bool> {
				std::vector<bool> oRet{};
				for (auto it = pv->begin(); it != pv->end(); ++it) {
					info_etudiant &cur = *it;
					bool b = cur.save(*pMan).get();
					oRet.push_back(b);
				}// it
				return (oRet);
			});
		}//maintains_async
		std::future<bool> info_etudiant::check_indexes_async(info::couchdb::couchdb_manager &oMan) {
			couchdb_manager *pMan = &oMan;
			return std::async([pMan]()->bool {
				static const std::vector<string_t> fields{KEY_LASTNAME,KEY_FIRSTNAME,KEY_FULLNAME,
				KEY_USERNAME,KEY_EMAIL,KEY_BIRTHDATE,KEY_APB,KEY_ANNEE,KEY_GROUPE,KEY_DOSSIER,
				KEY_SEXE,KEY_DEPARTEMENT,KEY_VILLE,KEY_ETABLISSEMENT,KEY_SERIEBAC,KEY_OPTIONBAC,
				KEY_MENTIONBAC,KEY_ETUDESSUPERIEURES,KEY_REDOUBLANT,KEY_AVATAR,KEY_PHONE,
                KEY_STATUS,KEY_DESC,
                KEY_UE11,KEY_UE12,KEY_UE13,KEY_MOYS1,KEY_RES1,KEY_RGS1,KEY_REMS1,
				KEY_UE21,KEY_UE22,KEY_UE23,KEY_MOYS2,KEY_RES2,KEY_RGS2,KEY_REMS2,KEY_COMPS1S2,
				KEY_UE31,KEY_UE32,KEY_UE33,KEY_MOYS3,KEY_RES3,KEY_RGS3,KEY_REMS3,KEY_COMPS2S3,
				KEY_UE41,KEY_UE42,KEY_UE43,KEY_MOYS4,KEY_RES4,KEY_RGS4,KEY_REMS4,KEY_COMPS3S4
				};
				bool bRet{ true };
				for (auto field : fields) {
					index_response rsp = pMan->create_index_async(field).get();
                    if (!rsp.ok()){
                        bRet = false;
                        break;
                    }
				}// fields
				return (bRet);
			});
		}//check_indexes_async
		///////////////////////////////////////
		const std::set<info_fielddesc> info_etudiant::get_descs(void) const {
			std::set<info_fielddesc> oSet{};
			oSet.insert(info_fielddesc{ KEY_APB,U("APB"),U("APB"),info_datatype::integer });
			oSet.insert(info_fielddesc{ KEY_ANNEE,U("Année universitaire"),U("ANNEE"),info_datatype::integer });
			oSet.insert(info_fielddesc{ KEY_GROUPE,U("Groupe"),U("GROUPE"),info_datatype::text });
			oSet.insert(info_fielddesc{ KEY_DOSSIER,U("Dossier"),U("DOSSIER"),info_datatype::text });
			oSet.insert(info_fielddesc{ KEY_USERNAME,U("Identifiant utilisateur"),U("USERNAME"),info_datatype::text });
			oSet.insert(info_fielddesc{ KEY_FIRSTNAME,U("Prénom(s)"),U("FIRSTNAME"),info_datatype::text });
			oSet.insert(info_fielddesc{ KEY_LASTNAME,U("Nom de famille"),U("LASTNAME"),info_datatype::text });
			oSet.insert(info_fielddesc{ KEY_SEXE,U("Sexe"),U("SEXE"),info_datatype::text });
			oSet.insert(info_fielddesc{ KEY_BIRTHDATE,U("Année naissance"),U("BIRTHYEAR"),info_datatype::integer });
			oSet.insert(info_fielddesc{ KEY_DEPARTEMENT,U("Département"),U("DEPARTEMENT"),info_datatype::text });
			oSet.insert(info_fielddesc{ KEY_VILLE,U("Ville"),U("VILLE"),info_datatype::text });
			oSet.insert(info_fielddesc{ KEY_ETABLISSEMENT,U("Etablissement"),U("LYCEE"),info_datatype::text });
			oSet.insert(info_fielddesc{ KEY_SERIEBAC,U("Série Bac"),U("SERIEBAC"),info_datatype::text });
			oSet.insert(info_fielddesc{ KEY_OPTIONBAC,U("Option Bac"),U("OPTIONBAC"),info_datatype::text });
			oSet.insert(info_fielddesc{ KEY_MENTIONBAC,U("Mention Bac"),U("MENTIONBAC"),info_datatype::text });
			oSet.insert(info_fielddesc{ KEY_ETUDESSUPERIEURES,U("Etudes supérieures"),U("ETUDESSUPERIEURES"),info_datatype::text });
			oSet.insert(info_fielddesc{ KEY_REDOUBLANT,U("Redoublant"),U("REDOUBLANT"),info_datatype::text });
			oSet.insert(info_fielddesc{ KEY_AVATAR,U("Avatar"),U("AVATAR"),info_datatype::text });
			oSet.insert(info_fielddesc{ KEY_EMAIL,U("Email"),U("EMAIL"),info_datatype::text });
			oSet.insert(info_fielddesc{ KEY_PHONE,U("Téléphone"),U("PHONE"),info_datatype::text });
			oSet.insert(info_fielddesc{ KEY_DATASETS,U("Datasets"),U("DATASETS"),info_datatype::vector });
			//
			oSet.insert(info_fielddesc{ KEY_UE11,U("Ue1-S1"),U("UE11"),info_datatype::real });
			oSet.insert(info_fielddesc{ KEY_UE12,U("Ue2-S1"),U("UE12"),info_datatype::real });
			oSet.insert(info_fielddesc{ KEY_UE13,U("Ue3-S1"),U("UE13"),info_datatype::real });
			oSet.insert(info_fielddesc{ KEY_MOYS1,U("Moy-S1"),U("MOYS1"),info_datatype::real });
			oSet.insert(info_fielddesc{ KEY_RES1,U("Résultat-S1"),U("RES1"),info_datatype::result });
			oSet.insert(info_fielddesc{ KEY_RGS1,U("Rang-S1"),U("RGS1"),info_datatype::integer });
			oSet.insert(info_fielddesc{ KEY_REMS1,U("Remarques-S1"),U("REMS1"),info_datatype::text });
			//
			oSet.insert(info_fielddesc{ KEY_UE21,U("Ue1-S2"),U("UE21"),info_datatype::real });
			oSet.insert(info_fielddesc{ KEY_UE22,U("Ue2-S2"),U("UE22"),info_datatype::real });
			oSet.insert(info_fielddesc{ KEY_UE23,U("Ue3-S2"),U("UE23"),info_datatype::real });
			oSet.insert(info_fielddesc{ KEY_MOYS2,U("Moy-S2"),U("MOYS2"),info_datatype::real });
			oSet.insert(info_fielddesc{ KEY_RES2,U("Résultat-S2"),U("RES2"),info_datatype::result });
			oSet.insert(info_fielddesc{ KEY_RGS2,U("Rang-S2"),U("RGS2"),info_datatype::integer });
			oSet.insert(info_fielddesc{ KEY_REMS2,U("Remarques-S2"),U("REMS2"),info_datatype::text });
			oSet.insert(info_fielddesc{ KEY_COMPS1S2,U("Comp-S1S2"),U("CPMPS1S2"),info_datatype::real });
			//
			oSet.insert(info_fielddesc{ KEY_UE31,U("Ue1-S3"),U("UE31"),info_datatype::real });
			oSet.insert(info_fielddesc{ KEY_UE32,U("Ue2-S3"),U("UE32"),info_datatype::real });
			oSet.insert(info_fielddesc{ KEY_UE33,U("Ue3-S3"),U("UE33"),info_datatype::real });
			oSet.insert(info_fielddesc{ KEY_MOYS3,U("Moy-S3"),U("MOYS3"),info_datatype::real });
			oSet.insert(info_fielddesc{ KEY_RES3,U("Résultat-S3"),U("RES3"),info_datatype::result });
			oSet.insert(info_fielddesc{ KEY_RGS3,U("Rang-S3"),U("RGS3"),info_datatype::integer });
			oSet.insert(info_fielddesc{ KEY_REMS3,U("Remarques-S3"),U("REMS3"),info_datatype::text });
			oSet.insert(info_fielddesc{ KEY_COMPS2S3,U("Comp-S2S3"),U("CPMPS2S3"),info_datatype::real });
			//
			oSet.insert(info_fielddesc{ KEY_UE41,U("Ue1-S4"),U("UE41"),info_datatype::real });
			oSet.insert(info_fielddesc{ KEY_UE42,U("Ue2-S4"),U("UE42"),info_datatype::real });
			oSet.insert(info_fielddesc{ KEY_UE43,U("Ue3-S4"),U("UE43"),info_datatype::real });
			oSet.insert(info_fielddesc{ KEY_MOYS4,U("Moy-S4"),U("MOYS4"),info_datatype::real });
			oSet.insert(info_fielddesc{ KEY_RES4,U("Résultat-S4"),U("RES4"),info_datatype::result });
			oSet.insert(info_fielddesc{ KEY_RGS4,U("Rang-S4"),U("RGS4"),info_datatype::integer });
			oSet.insert(info_fielddesc{ KEY_REMS4,U("Remarques-S4"),U("REMS4"),info_datatype::text });
			oSet.insert(info_fielddesc{ KEY_COMPS3S4,U("Comp-S3S4"),U("CPMPS3S4"),info_datatype::real });
			//
			return (oSet);
		}//get_descs
		////////////////////////////////////////
	}// namespace domain
	 //////////////////////////////////////////
}// namespace info

