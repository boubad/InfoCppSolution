add_executable(gtestinfodata main.cpp tests/test_couchdbmanager.cpp)
target_link_libraries(gtestinfodata ${TEST_LIBS} ${INFO_LIBS} ${WORK_ALL_LIBS})
