cmake_minimum_required(VERSION 3.16) # 要求 CMake 的最小版本

project(Qt_Cmake_OpenCV_And_Eigen VERSION 0.1.0) # 定义工程名称和版本

# 指定 .h 头文件目录列表
set(Include_DIR
    ./Eigen
    ./include
)

# 添加 .h 头文件目录到项目
include_directories(${Include_DIR})
# include_directories(${CMAKE_CURRENT_SOURCE_DIR})   # 如需包含 CMakeLists 所在目录可打开

# 查找 OpenCV（使用你指定的路径）
find_package(OpenCV
    PATHS D:/OpenCV-4.5.0-withContribNonfree-MinGW730-Qt # OpenCVConfig.cmake 所在路径
    NO_DEFAULT_PATH
    REQUIRED
)

if (OpenCV_FOUND)
    include_directories(${OpenCV_INCLUDE_DIRS}) # 包含 OpenCV 头文件路径
    message(${OpenCV_LIBS})                     # 打印 OpenCV 库列表
else()
    message("OpenCV not found, so we won't build the project.")
endif()

# 指定 .cpp 源文件列表并存于变量 SRCS_LIST
file(GLOB SRCS_LIST
    ./*.cpp
    ./src/*.cpp
)

# 基于指定的源文件，创建可执行文件
add_executable(${PROJECT_NAME} ${SRCS_LIST})

# ✅ 关键：根据配置把 exe 输出到 ./Debug 或 ./Release
# MinGW Makefiles 这种单配置生成器会根据 CMAKE_BUILD_TYPE 产生 Debug / Release
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/$<CONFIG>"
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 链接 目标和其依赖项所要使用的库
# 需要将 D:/OpenCV-4.5.0-withContribNonfree-MinGW730-Qt/x64/mingw/bin 加入 PATH
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
