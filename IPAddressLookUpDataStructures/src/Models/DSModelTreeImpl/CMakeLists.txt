add_library(DSModelTreeImpl DSModelTreeImpl.cpp TreeMapValueObject.cpp TreeMapValueObjectForUnusedObjectInArray.cpp)
target_link_libraries(DSModelTreeImpl PUBLIC Models boost::boost)