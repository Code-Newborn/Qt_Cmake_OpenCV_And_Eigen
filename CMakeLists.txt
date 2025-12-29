cmake_minimum_required(VERSION 3.16)

project(Qt_Cmake_OpenCV_And_Eigen
    VERSION 1.0.0
    LANGUAGES CXX
)

# C++ 标准（如无硬性要求建议至少 17；这里尊重你原来的 11）
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(MSVC)
    add_compile_options(
        "/source-charset:utf-8"
        "/Zc:__cplusplus"
    )
endif()

# ================= Qt & OpenCV =================
# Qt：通过 -DCMAKE_PREFIX_PATH 指向 Qt 安装前缀即可
find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets SerialPort PrintSupport)

# OpenCV：通过 -DmingwOpenCvRoot=... 传入
set(mingwOpenCvRoot "" CACHE PATH "Path to OpenCV installation root")
set(mingwOpenCV_INCLUDE_DIR "${mingwOpenCvRoot}/include")
set(mingwOpenCV_LIB_DIR     "${mingwOpenCvRoot}/x64/mingw/lib")
set(mingwOpenCV_BIN_DIR     "${mingwOpenCvRoot}/x64/mingw/bin")

# 你的写法是把 lib 目录下所有库都链进来（能用，但不够精准）
# 这里保持兼容（建议后续改为按需链接 opencv_core/opencv_imgproc/...）
file(GLOB mingwOpenCV_LIBS "${mingwOpenCV_LIB_DIR}/*.a" "${mingwOpenCV_LIB_DIR}/*.lib")

# ================= 输出目录（关键：走 build 目录） =================
# 统一让可执行文件输出到 <build>/bin，便于 tasks/launch 固化路径
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# ================= 头文件路径 =================
set(PROJECT_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/Eigen
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# ================= 源文件 =================
file(GLOB SRC_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${PROJECT_INCLUDE_DIRS}
    ${mingwOpenCV_INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    Qt5::SerialPort
    Qt5::PrintSupport
    ${mingwOpenCV_LIBS}
)

# ================= install（发布目录：exe + DLL） =================
# 1) 安装 exe 到 dist 根目录
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION .
)

# 2) 复制 OpenCV DLL（如果你用的是 DLL 版 OpenCV）
#    如果你是纯静态 OpenCV（只 .a 没有 dll），这一段不会拷贝到任何东西（无害）
if(WIN32)
    file(GLOB OPENCV_RUNTIME_DLLS "${mingwOpenCV_BIN_DIR}/*.dll")
    if(OPENCV_RUNTIME_DLLS)
        install(FILES ${OPENCV_RUNTIME_DLLS} DESTINATION .)
    endif()
endif()

# 3) 复制 Qt DLL：用 windeployqt 部署到 dist（CMake 3.16 推荐做法）
set(qtBinDir "" CACHE PATH "Path to Qt bin directory that contains windeployqt.exe")

if(WIN32)
    if(qtBinDir)
        set(WINDEPLOYQT_EXE "${qtBinDir}/windeployqt.exe")
    else()
        set(WINDEPLOYQT_EXE "windeployqt.exe")
    endif()

    # 注意：不要依赖 dist 里已经存在 exe；直接对 build 产物运行
    # CMAKE_RUNTIME_OUTPUT_DIRECTORY 在本工程中是 ${CMAKE_BINARY_DIR}/bin
    install(CODE
        "execute_process(
            COMMAND \"${WINDEPLOYQT_EXE}\"
                    \"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}.exe\"
                    --no-translations
            WORKING_DIRECTORY \"\${CMAKE_INSTALL_PREFIX}\"
        )"
    )
endif()

# ================= CPack（ZIP） =================
set(CPACK_GENERATOR "ZIP")
set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-windows")

include(CPack)
