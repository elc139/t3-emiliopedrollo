cmake_minimum_required(VERSION 3.15)
project(Trabalho3 CXX)

set(CMAKE_CXX_STANDARD 17)

add_executable(ThreadDemoABC ThreadDemoABC.cpp)
add_executable(OpenMPDemoABC OpenMPDemoABC.cpp)

if (WIN32)
    set_target_properties(ThreadDemoABC PROPERTIES LINK_FLAGS " --static")
    add_custom_command(TARGET OpenMPDemoABC POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${PROJECT_SOURCE_DIR}/libs"
            $<TARGET_FILE_DIR:OpenMPDemoABC>)
endif()

find_package(Threads REQUIRED)
find_package(OpenMP REQUIRED)

target_link_libraries(ThreadDemoABC ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(OpenMPDemoABC PUBLIC OpenMP::OpenMP_CXX)