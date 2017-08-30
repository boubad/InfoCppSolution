#pragma once
#ifndef INFO_ENV_H__
#define INFO_ENV_H__
///////////////////////////
#include <gtest/gtest.h>
////////////////////////////////
#include <http_manager.h>
#include <couchdb_storemanager.h>
#include <info_etudiant.h>
/////////////////////////////////
using namespace info;
using namespace info::http;
using namespace info::couchdb;
using namespace info::domain;
////////////////////////////////
class InfoEnv : public ::testing::Environment {
private:
    string_t m_url;
    string_t m_dbname;
    string_t m_username;
    string_t m_password;
public:
    InfoEnv():m_url(U("http://localhost:5984")),m_dbname(U("xxtest")){}
    virtual ~InfoEnv(){}
    virtual void SetUp(){
        http_manager client{m_url,m_username,m_password};
        couchdb_storemanager oMan{client,m_dbname};
        if (!oMan.exists_database(m_dbname)){
            bool b = oMan.create_database(m_dbname);
            assert(b);
        }
        bool bRet = info_etudiant::check_indexes_async(oMan).get();
        assert(bRet);
    }// SetUp
    virtual void TearDown(){
        
    }// TearDown
public:
    const string_t & server_url(void) const {
        return m_url;
    }
    const string_t & username(void) const {
        return m_username;
    }
    const string_t & password(void) const {
        return m_password;
    }
    const string_t & dbname(void) const {
        return m_dbname;
    }
}; //class InfoEnv
///////////////////////////////
#endif // INFO_ENV_H__
