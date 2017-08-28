#pragma once
#ifndef DOMAINSTRINGS_H__
#define DOMAINSTRINGS_H__
///////////////////////////
#include <stringutils.h>
//////////////////////////////
namespace info {
	namespace domain {
		////////////////////////////
		class domainstrings 
		{
		public:
			/////////////////////////////
			static const string_t KEY_STATUS;
			static const string_t KEY_DESC;
			//
			static const string_t KEY_ANNEE;
			static const string_t KEY_SIGLE;
			static const string_t KEY_NAME;
			static const string_t KEY_DATASETSIGLE;
			static const string_t KEY_DATAKIND;
			static const string_t KEY_DATATYPE;
			static const string_t KEY_MODALITES;
			static const string_t KEY_VALUE;
			static const string_t KEY_VARSIGLE;
			static const string_t KEY_INDSIGLE;
			//
			static const string_t TYPE_DATASET;
			static const string_t TYPE_INDIV;
			static const string_t TYPE_VARIABLE;
			static const string_t TYPE_VALUE;
			static const string_t TYPE_ETUD;
			static const string_t TYPE_UE;
			static const string_t TYPE_MATIERE;
			static const string_t TYPE_NOTE;
			static const string_t TYPE_ANNEE;
			static const string_t TYPE_MODULE;
			static const string_t TYPE_SEMESTRE;
			//
			static const string_t KEY_DOSSIER;
			static const string_t KEY_FIRSTNAME;
			static const string_t KEY_LASTNAME;
			static const string_t KEY_FULLNAME;
			static const string_t KEY_USERNAME;
			//
			static const string_t KEY_SEXE;
			static const string_t KEY_DEPARTEMENT;
			static const string_t KEY_VILLE;
			static const string_t KEY_ETABLISSEMENT;
			static const string_t KEY_SERIEBAC;
			static const string_t KEY_OPTIONBAC;
			static const string_t KEY_MENTIONBAC;
			static const string_t KEY_ETUDESSUPERIEURES;
			static const string_t KEY_REDOUBLANT;
			static const string_t KEY_GROUPE;
			static const string_t KEY_BIRTHDATE;
			static const string_t KEY_AVATAR;
			static const string_t KEY_PHONE;
			static const string_t KEY_EMAIL;
			static const string_t KEY_APB;
			static const string_t KEY_DATASETS;
			//
			static const string_t KEY_UNITE;
			static const string_t KEY_MATIERE;
			static const string_t KEY_MODULE;
			static const string_t KEY_SEMESTRE;
			static const string_t KEY_NOTE;
			//
			static const string_t KEY_STARTDATE;
			static const string_t KEY_ENDDATE;
		};
		/////////////////////////
	}// namespace domain
}// namespace info
//////////////////////////////
#endif // DOMAINSTRINGS_H__
