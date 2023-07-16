
add_subdirectory(DSModelTreeImpl)
add_subdirectory(DSModelVEBTreeImpl)
add_subdirectory(DSModelArrayImpl)
add_library(Models MacID.cpp IPAddress.cpp Subnet.cpp
        SubnetHost.cpp DSModel.cpp DSModelSingleton.cpp Host.cpp)
target_link_libraries(Models PUBLIC DSModelTreeImpl DSModelArrayImpl DSModelVebTreeImpl)
