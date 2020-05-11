cmake_minimum_required(VERSION 3.13)
project(BM_Network_Messenger)

set(CMAKE_CXX_STANDARD 17)

#
# Prefer -pthread compiler and linker flag when using libpthread. This must
# be set before call to find_package(Threads).
#
set(THREADS_HAVE_PTHREAD_ARG 1)
find_package(Threads REQUIRED)
#
find_package(PkgConfig)
pkg_check_modules(SERIAL libserial)

include_directories(data_link_layer/include)
add_executable(BM_Network_Messenger main.cpp Application.h Application.cpp data_link_layer/src/BitIO.cpp data_link_layer/src/Hamming3ErrorController.cpp
        data_link_layer/src/Frame.cpp data_link_layer/src/Controller.cpp data_link_layer/src/FrameTransformer.cpp Application.h physical_layer/PhysicalLayer.cpp physical_layer/PhysicalLayer.h)