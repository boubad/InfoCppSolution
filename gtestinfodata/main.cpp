#include <gtest/gtest.h>
////////////////////////
//#include "InfoEnv.h"
////////////////////////
int main(int argc, char **argv) {
	int nRet{ 1 };
	try {
//		::testing::AddGlobalTestEnvironment(new UnitTestDataLib::InfoEnv{});
		::testing::InitGoogleTest(&argc, argv);
		nRet = RUN_ALL_TESTS();
	}
	catch (std::exception &ex) {
//		info::string_t s = utility::conversions::to_string_t(ex.what());
		std::cerr << std::endl << ex.what() << std::endl;
	}
#ifdef _WIN32
#ifndef NDEBUG
	std::cout << std::endl << "Veuillez saisir un nombre pour quitter...: ";
	int xx;
	std::cin >> xx;
#endif
#endif // _WIN32
	return nRet;
}// main
