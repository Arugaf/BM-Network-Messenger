cmake_minimum_required(VERSION 3.16)
project(BM_Network_Messenger)

set(CMAKE_CXX_STANDARD 17)

include_directories(data_link_layer/include)
add_executable(BM_Network_Messenger main.cpp data_link_layer/src/BitIO.cpp data_link_layer/src/HammingErrorController3.cpp)