add_executable(Main main.cpp)
add_library(IpRange IpRange.cpp)
target_link_libraries(Main PRIVATE Boost::json gtest::gtest IpRange)