cmake_minimum_required(VERSION 3.13)
project(BM_Network_Messenger)

set(CMAKE_CXX_STANDARD 17)

include_directories(data_link_layer/include)
include_directories(physical_layer)
include_directories(include)
add_executable(
        BM_Network_Messenger
        main.cpp
        data_link_layer/src/BitIO.cpp
        data_link_layer/src/Hamming3ErrorController.cpp
        data_link_layer/src/Frame.cpp
        data_link_layer/src/DataLinkController.cpp
        physical_layer/rs232.c
        physical_layer/PhysicalController.cpp
)

#find_package(Threads REQUIRED)
#SET(CMAKE_CXX_FLAGS -pthread)
#target_link_libraries(BM_Network_Messeng)