add_executable(HttpServer HttpServer.cpp)
target_link_libraries(HttpServer PUBLIC IpRouting IpAllocator)
target_link_libraries(HttpServer PUBLIC boost::boost)