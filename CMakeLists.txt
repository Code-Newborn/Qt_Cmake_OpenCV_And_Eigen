cmake_minimum_required(VERSION 3.16) # 要求 CMake 的最小版本

project(Qt_Cmake_OpenCV_And_Eigen LANGUAGES CXX) # 定义工程名称和版本

# C++ 标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 生成 compile_commands.json（VS Code IntelliSense 必需）
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# MSVC 编码与告警控制
if(MSVC)
    add_compile_options(
        "/source-charset:utf-8"
        "/Zc:__cplusplus"
    )
endif()

# ================= Qt & OpenCV =================
# Qt 的路径通过 tasks.json 的 -DCMAKE_PREFIX_PATH 传入，这里只需要 find_package
find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets SerialPort PrintSupport)

# OpenCV：根据你 launch.json 中的路径，设置 OpenCV_DIR，然后 find_package
# 如有变化，只改这一个路径即可
set(mingwOpenCvRoot "" CACHE PATH "Path to OpenCV installation root")
set(mingwOpenCV_INCLUDE_DIR "${mingwOpenCvRoot}/include")
set(mingwOpenCV_LIB_DIR     "${mingwOpenCvRoot}/x64/mingw/lib")
file(GLOB mingwOpenCV_LIBS "${mingwOpenCV_LIB_DIR}/*.a" "${mingwOpenCV_LIB_DIR}/*.lib")

# ================= 输出与包含路径 =================
# 让 VS 把 exe 放在 <工程根>/Debug 和 <工程根>/Release，配合你的 launch.json
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR})

# 头文件路径
set(PROJECT_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/Eigen
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# ================= 源文件 头文件 =================
file(GLOB SRC_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

file(GLOB HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
)

# 可执行文件
add_executable(${PROJECT_NAME}
    ${SRC_FILES}
)

# 包含目录
target_include_directories(${PROJECT_NAME} PRIVATE
    ${PROJECT_INCLUDE_DIRS}
    ${mingwOpenCV_INCLUDE_DIR}
)

# 链接库
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    Qt5::SerialPort
    Qt5::PrintSupport
    ${mingwOpenCV_LIBS}
)

# 输出目录（Debug / Release）
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG   ${CMAKE_SOURCE_DIR}/Debug
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/Release
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
