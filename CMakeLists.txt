cmake_minimum_required(VERSION 3.8)
project(q_pen_drawer)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# set(CMAKE_AUTOMOC ON)


find_package(ament_cmake REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package (Eigen3 3.3 REQUIRED NO_MODULE)

set(q_pen_drawer_SRCS
  src/main.cpp
  src/mainwindow.cpp
  src/canvasview.cpp
  src/bezierline.cpp
)

set(q_pen_drawer_HDRS
  include/q_pen_drawer/mainwindow.h
  include/q_pen_drawer/canvasview.h
  include/q_pen_drawer/bezierline.h
  include/q_pen_drawer/newbutton.hpp
)

set(q_pen_drawer_UIS
  resource/mainwindow.ui
)

list(APPEND RESOURCES resource/qt_resources.qrc)
qt5_add_resources(RCC_SOURCES ${RESOURCES})

qt5_wrap_cpp(q_pen_drawer_MOCS ${q_pen_drawer_HDRS})

qt5_wrap_ui(q_pen_drawer_UIS_H resource/mainwindow.ui)

add_executable(${PROJECT_NAME}
  ${q_pen_drawer_SRCS}
  ${q_pen_drawer_MOCS}
  ${q_pen_drawer_UIS}
  ${RCC_SOURCES}
)

target_include_directories(${PROJECT_NAME} PUBLIC
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
  "$<INSTALL_INTERFACE:include/${PROJECT_NAME}>"
  ${EIGEN3_INCLUDE_DIRS}
)

ament_target_dependencies(${PROJECT_NAME} PUBLIC
  Eigen3
)

target_link_libraries(${PROJECT_NAME} 
  Qt5::Widgets
)

install(
  TARGETS ${PROJECT_NAME}
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()
