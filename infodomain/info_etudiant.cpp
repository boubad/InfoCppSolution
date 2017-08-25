#include "info_etudiant.h"
#include <stringutils.h>
////////////////////////
namespace info {
	namespace domain {
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
		bool info_etudiant::operator==(const info_etudiant &other) const {
			nullable_string s1 = dossier();
			nullable_string s2 = other.dossier();
			if (INFO_OPTIONAL_HAS_VALUE(s1) && INFO_OPTIONAL_HAS_VALUE(s2))
			{
				return (*s1 == *s2);
			}
			//
			s1 = username();
			s2 = other.username();
			if (INFO_OPTIONAL_HAS_VALUE(s1) && INFO_OPTIONAL_HAS_VALUE(s2)) {
				return (*s1 == *s2);
			}
			//
			s1 = email();
			s2 = other.email();
			if (INFO_OPTIONAL_HAS_VALUE(s1) && INFO_OPTIONAL_HAS_VALUE(s2)) {
				return (*s1 == *s2);
			}
			//
			s1 = lastname();
			s2 = other.lastname();
			nullable_string p1 = firstname();
			nullable_string p2 = other.firstname();
			nullable_int sa1 = birthdate();
			nullable_int sa2 = other.birthdate();
			if (INFO_OPTIONAL_HAS_VALUE(s1) && INFO_OPTIONAL_HAS_VALUE(s2) &&
				INFO_OPTIONAL_HAS_VALUE(p1) && INFO_OPTIONAL_HAS_VALUE(p2) &&
				INFO_OPTIONAL_HAS_VALUE(sa1) && INFO_OPTIONAL_HAS_VALUE(sa2)) {
				return (*s1 == *s2) && (*p1 == *p2) && (*sa1 == *sa2);
			}
			return (false);
		}

		void info_etudiant::check_fullname(void) {
			string_t s1{}, s2{};
			get_string_property(KEY_LASTNAME, s1);
			get_string_property(KEY_FIRSTNAME, s2);
			string_t ss = stringutils::trim(s1 + U(" ") + s2);
			set_string_property(KEY_FULLNAME, ss);
		}// check_fullname
		bool info_etudiant::is_storable(void) const {
			string_t s1{}, s2{}, s3{};
			get_string_property(KEY_DOSSIER, s1);
			get_string_property(KEY_USERNAME, s2);
			get_string_property(KEY_EMAIL, s3);
			return (!s1.empty()) || (!s2.empty()) || (!s3.empty());
		}
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
			set_string_property(etud_strings::KEY_REMS1, s);
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
			set_string_property(etud_strings::KEY_REMS2, s);
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
			set_string_property(etud_strings::KEY_REMS3, s);
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
			set_string_property(etud_strings::KEY_REMS4, s);
		}
		////////////////////////////////////////
	}// namespace domain
	 //////////////////////////////////////////
}// namespace info

