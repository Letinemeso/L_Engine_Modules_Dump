cmake_minimum_required(VERSION 3.5)

project(L_Gamedev_Lib LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

FILE(GLOB_RECURSE Headers "include/*.h")
add_custom_target(_L_Engine_Modules_Dump_include SOURCES ${Headers})
FILE(GLOB_RECURSE Sources "source/*.cpp")

add_library(L_Engine_Modules_Dump STATIC ${Sources})

target_include_directories(L_Engine_Modules_Dump PUBLIC
                            "${PROJECT_SOURCE_DIR}"
                            "${PROJECT_SOURCE_DIR}/include"
                            "${PROJECT_SOURCE_DIR}/../L_Utility/include"
                            "${PROJECT_SOURCE_DIR}/../L_Variable/include"
                            "${PROJECT_SOURCE_DIR}/../LEti_Engine/include"
                            "${PROJECT_SOURCE_DIR}/../LEti_Engine/OpenGL/GLM"
                            "${PROJECT_SOURCE_DIR}/../L_Renderer/include"
                            "${PROJECT_SOURCE_DIR}/../L_Renderer/OpenGL/include"
                            "${PROJECT_SOURCE_DIR}/../L_Physics/include"
                            "${PROJECT_SOURCE_DIR}/../L_Sound/include"
)

