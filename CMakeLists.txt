cmake_minimum_required(VERSION 3.13)
project(ur-sfml)
set(CMAKE_CXX_STANDARD 20)
set(EXECUTABLE_NAME ur)
set(CMAKE_CXX_FLAGS_DEBUG -DDEBUG)
# detect windows
if (WIN32)
  set(SFML_DIR /home/senders/Downloads/SFML-2.5.1/lib/cmake/SFML)
endif()

# setup SFML
set(SFML_LIBRARIES sfml-system sfml-window sfml-graphics)
find_package(SFML 2.5 REQUIRED COMPONENTS system window graphics)

add_executable(${EXECUTABLE_NAME} 
  src/ur.cpp 
  src/helper.cpp 
  src/timedLatch.cpp 
  src/random.cpp)
target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/res DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
