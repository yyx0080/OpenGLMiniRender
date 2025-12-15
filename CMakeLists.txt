cmake_minimum_required(VERSION 3.16)
project(OpenGLMiniRender)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# FetchContent 将下载的源码存放在项目根目录下的 external/ 文件夹中
set(GLAD_LOCAL_PATH "${CMAKE_SOURCE_DIR}/external/glad_source")
set(GLFW_LOCAL_PATH "${CMAKE_SOURCE_DIR}/external/glfw_source")


# -----------------------------
# 可覆盖的仓库 URL（默认指向 GitHub 官方）
# 在命令行可以用 -DGLAD_GIT_URL=... 来覆盖
# -----------------------------
set(GLAD_GIT_URL    "https://github.com/Dav1dde/glad.git" CACHE STRING "GLAD git repository URL")
set(GLFW_GIT_URL    "https://github.com/glfw/glfw.git" CACHE STRING "GLFW git repository URL")

# -----------------------------
# 例：常见可替换的镜像（你可以把下面某个复制到 -D...）
#  1) GitHub 官方（默认）
#  2) cnpmjs.org 代理:    https://github.com.cnpmjs.org/<owner>/<repo>.git
#  3) fastgit 代理:      https://hub.fastgit.org/<owner>/<repo>.git
#  4) gitclone 代理:     https://gitclone.com/github.com/<owner>/<repo>.git
#  5) gitee 镜像（若存在）: https://gitee.com/<mirror-owner>/<repo>.git
#
# 示例如何在命令行覆盖（PowerShell / cmd / VS 的CMake设置均可用）:
#   cmake -S . -B out/build -DGLAD_GIT_URL="https://hub.fastgit.org/Dav1dde/glad.git" ...
#
# 注意：镜像是否可用取决于同步质量与镜像是否存在 —— 若某镜像 404，请换另一个或回到官方。
# -----------------------------

# ---------- GLAD ----------
FetchContent_Declare(
    glad
    GIT_REPOSITORY ${GLAD_GIT_URL}
    GIT_TAG origin/master
)
# 若需要可以设置浅克隆
set(FETCHCONTENT_QUIET OFF)
FetchContent_MakeAvailable(glad)

# ---------- GLFW ----------
FetchContent_Declare(
    glfw
    GIT_REPOSITORY ${GLFW_GIT_URL}
    GIT_TAG 3.4
)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

# ---------- GLM ----------
# FetchContent_Declare(
#  glm
#  GIT_REPOSITORY ${GLM_GIT_URL}
#  GIT_TAG 0.9.9.8
# )
# FetchContent_MakeAvailable(glm)

# ---------- ASSIMP (手动配置) ----------
# 定义库的存放路径
set(ASSIMP_DIR ${CMAKE_SOURCE_DIR}/external/assimp_lib)
# 明确定义导入库的完整路径（静态链接库）
set(ASSIMP_LIBRARY_FILE "${ASSIMP_DIR}/lib/assimp-vc143-mtd.lib") # lib文件名称

if(EXISTS ${ASSIMP_LIBRARY_FILE})
    message(STATUS "Found Assimp library for linking: ${ASSIMP_LIBRARY_FILE}")
else()
    # 如果找不到，输出自定义的错误信息
    message(FATAL_ERROR "Assimp import library (.lib) not found at expected path: ${ASSIMP_LIBRARY_FILE}")
endif()

set(ASSIMP_LIBRARY ${ASSIMP_LIBRARY_FILE})
find_library(ASSIMP_LIBRARY assimp HINTS ${ASSIMP_DIR}/lib)

if(ASSIMP_LIBRARY)
    message(STATUS "Found Assimp library for linking: ${ASSIMP_LIBRARY}")
else()
    # 如果找不到，你需要将 .lib 文件放到 ${ASSIMP_DIR}/lib 目录下
    message(FATAL_ERROR "Assimp import library (.lib) not found in ${ASSIMP_DIR}/lib. Please download or generate it.")
endif()

# ============================================================
# Executable
# ============================================================

# 自动收集全部的cpp源文件
file(GLOB_RECURSE SOURCES
    src/*.cpp
)

add_executable(OpenGLMiniRender ${SOURCES})

# ============================================================
# Linking
# ============================================================
target_link_libraries(OpenGLMiniRender
    PRIVATE
    glfw
    glad
    ${ASSIMP_LIBRARY}  # 链接到 Assimp 的 .lib 文件
    # glm 是头文件库
)

# Include dirs（保证包含头文件）
# glad/glfw 使用 FetchContent 后，CMake 会提供变量，但包含路径写成下面保险
target_include_directories(OpenGLMiniRender PRIVATE
    ${glad_SOURCE_DIR}/include
    ${glfw_SOURCE_DIR}/include
    ${ASSIMP_DIR}/include
    external/glm        # glm 是 header-only，直接指向源码根目录即可
    external            # 指向存放stb_image.h 的目录
)

# 如果你在 Windows 上需要链接系统 OpenGL 库
if (WIN32)
    target_link_libraries(OpenGLMiniRender PRIVATE opengl32)
endif()
