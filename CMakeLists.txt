cmake_minimum_required(VERSION 3.13)
project(BM_Network_Messenger)

set(CMAKE_CXX_STANDARD 17)

include_directories(data_link_layer/include)
include_directories(include)
add_executable(
        BM_Network_Messenger
        main.cpp
        data_link_layer/src/BitIO.cpp
        data_link_layer/src/Hamming3ErrorController.cpp
        data_link_layer/src/Frame.cpp
        physical_layer/rs232.h
        physical_layer/rs232.c
        physical_layer/PhysicalLayer.cpp
        physical_layer/PhysicalLayer.h
        physical_layer/IPhysical.h
)

#find_package(Threads REQUIRED)
#SET(CMAKE_CXX_FLAGS -pthread)
#target_link_libraries(BM_Network_Messeng)