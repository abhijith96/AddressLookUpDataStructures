
add_subdirectory(DSModelTreeImpl)
add_subdirectory(DSModelArrayImpl)
add_library(Models MacID.cpp IPAddress.cpp Subnet.cpp Subnet.h
        SubnetHost.cpp DSModel.cpp DSModelSingleton.cpp Host.cpp)
target_link_libraries(Models PUBLIC DSModelTreeImpl DSModelArrayImpl)
