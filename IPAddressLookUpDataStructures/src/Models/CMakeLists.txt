
add_subdirectory(DSModelTreeImpl)
add_subdirectory(DSModelVEBTreeImpl)
add_subdirectory(DSModelArrayImpl)
add_library(Models MacID.cpp IPAddress.cpp Subnet.cpp
        SubnetHosts.cpp DSModelSingleton.cpp Host.cpp IpRange.cpp)
target_link_libraries(Models PUBLIC DSModelTreeImpl DSModelArrayImpl DSModelVebTreeImpl DSModelHashedVebTreeImpl)
