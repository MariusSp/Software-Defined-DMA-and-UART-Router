cmake_minimum_required(VERSION 3.17)
project(21ss_sw_defined_dma)

add_subdirectory(rodos)

add_rodos_executable(router
        src/connection.cpp
        src/router.cpp)

add_rodos_executable(boardYellow
        src/clients/boardYellow.cpp
        src/clients/modules/client.cpp
        src/clients/modules/clientConnection.cpp)

add_rodos_executable(boardBlue
        src/clients/boardBlue.cpp
        src/clients/modules/client.cpp
        src/clients/modules/clientConnection.cpp)
