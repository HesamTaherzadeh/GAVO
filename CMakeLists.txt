cmake_minimum_required(VERSION 3.0.0)
project(vo_gps VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(vo_gps main.cpp src/featurematching.cpp src/camera.cpp src/pose_estimation.cpp src/calibration.cpp src/videostream.cpp)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
find_package (Eigen3 3.3 REQUIRED)
target_link_libraries (vo_gps Eigen3::Eigen)
find_package( OpenCV REQUIRED )

include_directories( 
    ${OpenCV_INCLUDE_DIRS} 
    "/usr/include/eigen3/"
)

target_link_libraries( vo_gps ${OpenCV_LIBS} )

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
