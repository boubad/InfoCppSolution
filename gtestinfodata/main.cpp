#include <gtest/gtest.h>
////////////////////////
#include "info_env.h"
////////////////////////
int main(int argc, char **argv) {
	int nRet{ 1 };
	try {
		::testing::AddGlobalTestEnvironment(new InfoEnv{});
		::testing::InitGoogleTest(&argc, argv);
		nRet = RUN_ALL_TESTS();
	}
	catch (std::exception &ex) {
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
