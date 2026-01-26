cmake_minimum_required(VERSION 3.16)
project(SMLUI LANGUAGES CXX)

find_package(Vulkan REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GLFW REQUIRED glfw3)

add_library(SMLUI STATIC
    src/sml_ui.cpp
    imgui/imgui.cpp
    imgui/imgui_demo.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
    imgui/backends/imgui_impl_glfw.cpp
    imgui/backends/imgui_impl_vulkan.cpp
)

target_include_directories(SMLUI PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends
    ${GLFW_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/../SMLParser/include
)

target_compile_features(SMLUI PUBLIC cxx_std_11)

target_link_libraries(SMLUI PUBLIC
    Vulkan::Vulkan
    ${GLFW_LIBRARIES}
    SMLParser
)

if(GLFW_LIBRARY_DIRS)
    target_link_directories(SMLUI PUBLIC ${GLFW_LIBRARY_DIRS})
endif()
