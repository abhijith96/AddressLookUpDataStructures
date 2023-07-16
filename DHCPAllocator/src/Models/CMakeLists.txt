
add_subdirectory(DSModelTreeImpl)
add_subdirectory(DSModelVEBTreeImpl)
add_library(Models MacID.cpp IPAddress.cpp Subnet.cpp
        SubnetHost.cpp DSModel.cpp DSModelSingleton.cpp IpRange.cpp)
target_link_libraries(Models PUBLIC DSModelTreeImpl)