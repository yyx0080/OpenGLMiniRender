cmake_minimum_required(VERSION 3.16)
project(OpenGLMiniRender)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

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
    # glm 是头文件库
)

# Include dirs（保证包含头文件）
# glad/glfw 使用 FetchContent 后，CMake 会提供变量，但包含路径写成下面保险
target_include_directories(OpenGLMiniRender PRIVATE
    ${glad_SOURCE_DIR}/include
    ${glfw_SOURCE_DIR}/include
    external/glm        # glm 是 header-only，直接指向源码根目录即可
    external            # 指向存放stb_image.h 的目录
)

# 如果你在 Windows 上需要链接系统 OpenGL 库
if (WIN32)
    target_link_libraries(OpenGLMiniRender PRIVATE opengl32)
endif()
