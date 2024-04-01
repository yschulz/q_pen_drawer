cmake_minimum_required(VERSION 3.8)
project(q_pen_drawer)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)

# set(CMAKE_CXX_STANDARD 11)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(ament_cmake REQUIRED)
find_package(Qt5Widgets REQUIRED)

set(q_pen_drawer_SRCS
  src/main.cpp
  src/mainwindow.cpp
)

set(q_pen_drawer_HDRS
  include/q_pen_drawer/mainwindow.h
)

set(q_pen_drawer_UIS
  resource/mainwindow.ui
)

qt5_wrap_cpp(q_pen_drawer_MOCS ${q_pen_drawer_HDRS})

qt5_wrap_ui(q_pen_drawer_UIS_H resource/mainwindow.ui)

add_executable(${PROJECT_NAME}
  ${q_pen_drawer_SRCS}
  ${q_pen_drawer_MOCS}
  ${q_pen_drawer_UIS}
)

target_link_libraries(${PROJECT_NAME} Qt5::Widgets)

target_include_directories(${PROJECT_NAME} PUBLIC
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
  "$<INSTALL_INTERFACE:include/${PROJECT_NAME}>"
)

install(
  TARGETS ${PROJECT_NAME}
  DESTINATION lib/${PROJECT_NAME}
  # EXPORT ${PROJECT_NAME}
  # ARCHIVE DESTINATION lib/${PROJECT_NAME}
  # LIBRARY DESTINATION lib/${PROJECT_NAME}
  # RUNTIME DESTINATION bin/${PROJECT_NAME}
)

ament_package()
