
add_subdirectory(DSModelTreeImpl)
add_library(Models MacID.cpp IPAddress.cpp Subnet.cpp
        SubnetHost.cpp DSModel.cpp DSModelSingleton.cpp)
target_link_libraries(Models PUBLIC DSModelTreeImpl)