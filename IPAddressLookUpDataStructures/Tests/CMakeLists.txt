add_subdirectory(TreeImplTests)
add_subdirectory(TestVebTreeImplementation)
add_subdirectory(DsModelVebTreeTests)
add_subdirectory(DsModelHashedVebTreeTests)
add_subdirectory(BenchMarks)
add_subdirectory(ArrayImplTests)
add_executable(TestSingleton TestSingleton.cpp)
target_link_libraries(TestSingleton PUBLIC Models gtest::gtest)