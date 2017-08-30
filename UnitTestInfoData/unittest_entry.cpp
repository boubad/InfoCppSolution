#include "stdafx.h"
#include "CppUnitTest.h"
////////////////////////
#include "info_env.h"
//
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTestInfoData
{		
	using namespace info::test;
	//////////////////////////////////////////
	TEST_MODULE_INITIALIZE(ModuleInitialize)
	{
		//Logger::WriteMessage("In Module Initialize");
		InfoEnv::SetUp();
	}

	TEST_MODULE_CLEANUP(ModuleCleanup)
	{
		//Logger::WriteMessage("In Module Cleanup");
		InfoEnv::TearDown();
	}
}