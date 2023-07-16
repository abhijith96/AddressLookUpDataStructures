add_executable(Test1 Test1.cpp)
add_executable(Test2 Test2.cpp)
target_link_libraries(Test1 DSModelTreeImpl GTest::GTest)
target_link_libraries(Test2 DSModelTreeImpl GTest::GTest)