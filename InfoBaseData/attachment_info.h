#pragma once
#ifndef ATTACHMENT_INFO_H__
#define ATTACHMENT_INFO_H__
////////////////////////////
#include "infobaseobject.h"
//////////////////////////
namespace info {
		////////////////////////////
		class attachment_info : public infobaseobject
		{
		public:
			attachment_info();
			attachment_info(const any &va);
			attachment_info(const attachment_info &other);
			attachment_info & operator=(const attachment_info &other);
			virtual ~attachment_info();
		public:
			string_t content_type(void) const;
			string_t data(void) const;
			string_t digest(void) const;
			int encoded_length(void) const;
			string_t encoding(void) const;
			int length(void) const;
			int revpos(void) const;
			bool stub(void) const;
		}; // class attachment_info
		////////////////////////////
}// namespace info
//////////////////////////
#endif // ATTACHMENT_INFO_H__
