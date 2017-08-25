#pragma once
#ifndef ETUDIANT_H__
#define ETUDIANT_H__
//////////////////////////
#include "info_baseitem.h"
#include "etud_strings.h"
////////////////////////////
namespace info {
	namespace domain {
		////////////////////////////
		class info_etudiant : public info_baseitem ,public etud_strings{
		public:
			using note_t = optional<double>;
		private:
			void check_type(void);
			void check_fullname(void);
		protected:
			virtual void post_init(void) override;
			virtual void unique_properties(std::vector<std::vector<string_t>> &vvec) const override;
		public:
			info_etudiant();
			info_etudiant(const any &val);
			info_etudiant(const info_etudiant &other);
			info_etudiant & operator=(const info_etudiant &other);
			virtual ~info_etudiant();
			bool operator==(const info_etudiant &other) const;
		public:
			nullable_int apb(void) const;
			void apb(int n);
			nullable_string fullname(void) const;
			nullable_int annee(void) const;
			void annee(int n);
			nullable_string groupe(void) const;
			void groupe(const string_t &s);
			nullable_string dossier(void) const;
			void dossier(const string_t &s);
			nullable_string username(void) const;
			void username(const string_t &s);
			nullable_string firstname(void) const;
			void firstname(const string_t &s);
			nullable_string lastname(void) const;
			void lastname(const string_t &s);
			nullable_string sexe(void) const;
			void sexe(const string_t &s);
			nullable_int birthdate(void) const;
			void birthdate(int n);
			nullable_string departement(void) const;
			void departement(const string_t &s);
			nullable_string ville(void) const;
			void ville(const string_t &s);
			nullable_string etablissement(void) const;
			void etablissement(const string_t &s);
			nullable_string seriebac(void) const;
			void seriebac(const string_t &s);
			nullable_string optionbac(void) const;
			void optionbac(const string_t &s);
			nullable_string mentionbac(void) const;
			void mentionbac(const string_t &s);
			nullable_string etudessuperieures(void) const;
			void etudessuperieures(const string_t &s);
			nullable_string redoublant(void) const;
			void redoublant(const string_t &s);
			nullable_string avatar(void) const;
			void avatar(const string_t &s);
			nullable_string email(void) const;
			void email(const string_t &s);
			nullable_string  phone(void) const;
			void phone(const string_t &s);
			optional<std::vector<string_t>> datasets_sigles(void) const;
			void datasets_sigles(const std::vector<string_t> &vec);
		public:
			virtual bool is_storable(void) const override;
		public:
			note_t  ue11(void) const;
			void ue11(double n);
			note_t  ue12(void) const;
			void ue12(double n);
			note_t  ue13(void) const;
			void ue13(double n);
			note_t  moys1(void) const;
			void moys1(double n);
			optional<info_resulttype> result_s1(void) const;
			void result_s1(info_resulttype t);
			nullable_int rang_s1(void) const;
			void rang_s1(int n);
			nullable_string rem_s1(void) const;
			void rem_s1(const string_t &s);
			//
			note_t  ue21(void) const;
			void ue21(double n);
			note_t  ue22(void) const;
			void ue22(double n);
			note_t  ue23(void) const;
			void ue23(double n);
			note_t  moys2(void) const;
			void moys2(double n);
			optional<info_resulttype> result_s2(void) const;
			void result_s2(info_resulttype t);
			nullable_int rang_s2(void) const;
			void rang_s2(int n);
			note_t comp_s1s2(void) const;
			void comp_s1s2(double n);
			nullable_string rem_s2(void) const;
			void rem_s2(const string_t &s);
			//
			note_t  ue31(void) const;
			void ue31(double n);
			note_t  ue32(void) const;
			void ue32(double n);
			note_t  ue33(void) const;
			void ue33(double n);
			note_t  moys3(void) const;
			void moys3(double n);
			optional<info_resulttype> result_s3(void) const;
			void result_s3(info_resulttype t);
			nullable_int rang_s3(void) const;
			void rang_s3(int n);
			note_t comp_s2s3(void) const;
			void comp_s2s3(double n);
			nullable_string rem_s3(void) const;
			void rem_s3(const string_t &s);
			//
			note_t  ue41(void) const;
			void ue41(double n);
			note_t  ue42(void) const;
			void ue42(double n);
			note_t  ue43(void) const;
			void ue43(double n);
			note_t  moys4(void) const;
			void moys4(double n);
			optional<info_resulttype> result_s4(void) const;
			void result_s4(info_resulttype t);
			nullable_int rang_s4(void) const;
			void rang_s4(int n);
			note_t comp_s3s4(void) const;
			void comp_s3s4(double n);
			nullable_string rem_s4(void) const;
			void rem_s4(const string_t &s);
			//
		}; // class info_etudiant
		   ///////////////////////////
		using etudiant_ptr = std::shared_ptr<info_etudiant>;
		using etudiants_vector = std::vector<etudiant_ptr>;
		//////////////////////////////
	}// namespace domain
} // info
  ////////////////////////////
#endif // ETUDIANT_H__
