cmake_minimum_required(VERSION 3.0)
project(QRTest)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}  -Wall  -O3 -march=native -pthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall   -O3 -march=native -pthread ")

# Check C++11 or C++0x support
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
   # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fno-elide-constructors") -fno-elide-constructors //忽略编译器优化
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 ")
   add_definitions(-DCOMPILEDWITHC11)
   message(STATUS "Using flag -std=c++11.")
elseif(COMPILER_SUPPORTS_CXX0X)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
   add_definitions(-DCOMPILEDWITHC0X)
   message(STATUS "Using flag -std=c++0x.")
else()
   message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()
include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/inlude)

find_package(OpenCV 3.0 QUIET)
if(NOT OpenCV_FOUND)
   find_package(OpenCV 2.4.3 QUIET)
   if(NOT OpenCV_FOUND)
      message(FATAL_ERROR "OpenCV > 2.4.3 not found.")
   endif()
endif()

include_directories(${OpenCV_INCLUDE_DIRS})

# 生成 二维码
add_library(qrCode STATIC IMPORTED )
set_target_properties( qrCode PROPERTIES IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/lib/libqrcodegencpp.a )

# svg 转 png
add_library(lunaSvg STATIC IMPORTED )
set_target_properties( lunaSvg PROPERTIES IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/lib/liblunasvg.a)

# 解码
add_library(zxing STATIC IMPORTED )
set_target_properties(zxing PROPERTIES IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/lib/libzxing.a)

# 访问
add_library(hv STATIC IMPORTED )
set_target_properties(hv PROPERTIES IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/lib/libhv.a)

add_executable(${PROJECT_NAME} main.cpp MatSource.cpp MatSource.h stb_image_write.h)
target_link_libraries(${PROJECT_NAME} qrCode  lunaSvg zxing hv ${OpenCV_LIBRARIES})