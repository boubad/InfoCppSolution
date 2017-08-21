#include "stdafx.h"
#include "CppUnitTest.h"
////////////////////////
#include <stringutils.h>
#include <http_manager.h>
//
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTestInfoData
{		
	using namespace info;
	using namespace info::http;
	///////////////////////////
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			serverurl url{ U("http://localhost:5984") };
			http_manager oMan{ url };
			dataserviceuri suri{};
			info_response_ptr rsp = oMan.get(suri).get();
			info_response *p = rsp.get();
			Assert::IsNotNull(p);
			any va = p->jsonval;
			Assert::IsTrue(va.has_value());
			bool b = va.type() == typeid(infomap);
			Assert::IsTrue(b);
			string_t ss = stringutils::any_to_stringt(va);
			Logger::WriteMessage(ss.c_str());
		}

	};
}