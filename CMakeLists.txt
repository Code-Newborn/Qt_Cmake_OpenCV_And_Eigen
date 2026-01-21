cmake_minimum_required(VERSION 3.16)

project(
  Qt_Cmake_OpenCV_And_Eigen
  VERSION 1.0.0
  LANGUAGES CXX)

# 基础设置（保持原行为）
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

if(MSVC)
  add_compile_options("/source-charset:utf-8" "/Zc:__cplusplus")
endif()

# 依赖开关
option(PROJECT_USE_QT "Enable Qt" ON)
option(PROJECT_USE_OPENCV "Enable OpenCV" ON)
option(PROJECT_USE_EIGEN "Enable Eigen" ON)

# ---------- Qt (manual root) ----------
if(PROJECT_USE_QT)
  set(mingwQtRoot
      ""
      CACHE PATH "Path to Qt installation root")
  set(_qt_inc "${mingwQtRoot}/include")
  set(_qt_lib "${mingwQtRoot}/lib")
  file(GLOB _qt_libs "${_qt_lib}/*.a" "${_qt_lib}/*.lib"
  )# 直接 glob 全部 .lib 可能引发重复符号、链接顺序问题或引入不必要模块。

  add_library(project_qt INTERFACE)
  add_library(project::qt ALIAS project_qt)
  target_include_directories(project_qt INTERFACE "${_qt_inc}")
  target_link_libraries(project_qt INTERFACE ${_qt_libs})
endif()

# ---------- OpenCV (MinGW root) ----------
if(PROJECT_USE_OPENCV)
  set(mingwOpenCvRoot
      ""
      CACHE PATH "Path to OpenCV installation root") # INFO settings.json 中修改路径
  set(_opencv_inc "${mingwOpenCvRoot}/include")
  set(_opencv_lib "${mingwOpenCvRoot}/x64/mingw/lib")
  file(GLOB _opencv_libs "${_opencv_lib}/*.a" "${_opencv_lib}/*.lib")

  add_library(project_opencv INTERFACE)
  add_library(project::opencv ALIAS project_opencv)
  target_include_directories(project_opencv INTERFACE "${_opencv_inc}")
  target_link_libraries(project_opencv INTERFACE ${_opencv_libs})
endif()

# ---------- Eigen ----------
if(PROJECT_USE_EIGEN)
  set(mingwEigenRoot
      ""
      CACHE PATH "Path to Eigen root") # INFO settings.json 修改路径
  set(_eigen_inc "${mingwEigenRoot}")
  set(_eigen_lib "${mingwEigenRoot}/lib") # _eigen_libs 为空时，CMake 行为是安全的
  file(GLOB _eigen_libs "${_eigen_lib}/*.a" "${_eigen_lib}/*.lib")

  add_library(project_eigen INTERFACE)
  add_library(project::eigen ALIAS project_eigen)
  target_include_directories(project_eigen INTERFACE "${_eigen_inc}")
  target_link_libraries(project_eigen INTERFACE ${_eigen_libs})
endif()

# ---------- Sources ----------
file(GLOB SRC_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_executable(${PROJECT_NAME} ${SRC_FILES})

# ---------- Headers ----------
target_include_directories(${PROJECT_NAME}
                           PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")

# ---------- Link ----------
if(PROJECT_USE_QT)
  target_link_libraries(${PROJECT_NAME} PRIVATE project::qt)
endif()
if(PROJECT_USE_OPENCV)
  target_link_libraries(${PROJECT_NAME} PRIVATE project::opencv)
endif()
if(PROJECT_USE_EIGEN)
  target_link_libraries(${PROJECT_NAME} PRIVATE project::eigen)
endif()

# ================= Extension template =================
# option(PROJECT_USE_XXX "Enable XXX" ON)
# if(PROJECT_USE_XXX)
#     set(XXX_ROOT "" CACHE PATH "Path to XXX root")
#     set(_xxx_inc "${XXX_ROOT}/include")
#     set(_xxx_lib "${XXX_ROOT}/lib")
#     file(GLOB _xxx_libs "${_xxx_lib}/*.a" "${_xxx_lib}/*.lib")
#
#     add_library(project_xxx INTERFACE)
#     add_library(project::xxx ALIAS project_xxx)
#     target_include_directories(project_xxx INTERFACE "${_xxx_inc}")
#     target_link_libraries(project_xxx INTERFACE ${_xxx_libs})
# endif()
#
# if(PROJECT_USE_XXX)
#     target_link_libraries(${PROJECT_NAME} PRIVATE project::xxx)
# endif()