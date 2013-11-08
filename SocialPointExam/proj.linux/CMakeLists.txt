cmake_minimum_required(VERSION 2.8) 

set(PLATFORM "linux" CACHE STRING "Platform to compile")
set(CMAKE_C_FLAGS "-Wall -DLINUX")
set(CMAKE_CXX_FLAGS "-Wall -DLINUX -std=c++11")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

project(sp-mobile-exam)

# cocos2dx
set(COCOS2DX_SOURCE_DIR "../lib/cocos2dx/cocos2dx")
include_directories("${COCOS2DX_SOURCE_DIR}")
include_directories("${COCOS2DX_SOURCE_DIR}/kazmath/include")
include_directories("${COCOS2DX_SOURCE_DIR}/include")
include_directories("${COCOS2DX_SOURCE_DIR}/platform")
include_directories("${COCOS2DX_SOURCE_DIR}/platform/linux")
include_directories("${COCOS2DX_SOURCE_DIR}/platform/third_party/linux")
include_directories("${COCOS2DX_SOURCE_DIR}/platform/third_party/linux/libwebp")
include_directories("/usr/include/freetype2")
file(GLOB COCOS2DX_BASE_SOURCES
	"${COCOS2DX_SOURCE_DIR}/*.cpp"
	"${COCOS2DX_SOURCE_DIR}/*.c"
	"${COCOS2DX_SOURCE_DIR}/platform/*.cpp"
)
file(GLOB_RECURSE COCOS2DX_SOURCES
	"${COCOS2DX_SOURCE_DIR}/actions/*.cpp"
	"${COCOS2DX_SOURCE_DIR}/base_nodes/*.cpp"
	"${COCOS2DX_SOURCE_DIR}/cocoa/*.cpp"
	"${COCOS2DX_SOURCE_DIR}/draw_nodes/*.cpp"
	"${COCOS2DX_SOURCE_DIR}/effects/*.cpp"
	"${COCOS2DX_SOURCE_DIR}/include/*.cpp"
	"${COCOS2DX_SOURCE_DIR}/kazmath/*.c"
	"${COCOS2DX_SOURCE_DIR}/keypad_dispatcher/*.cpp"
	"${COCOS2DX_SOURCE_DIR}/label_nodes/*.cpp"
	"${COCOS2DX_SOURCE_DIR}/layers_scenes_transitions_nodes/*.cpp"
	"${COCOS2DX_SOURCE_DIR}/menu_nodes/*.cpp"
	"${COCOS2DX_SOURCE_DIR}/misc_nodes/*.cpp"
	"${COCOS2DX_SOURCE_DIR}/particle_nodes/*.cpp"
	"${COCOS2DX_SOURCE_DIR}/script_support/*.cpp"
	"${COCOS2DX_SOURCE_DIR}/shaders/*.cpp"
	"${COCOS2DX_SOURCE_DIR}/sprite_nodes/*.cpp"
	"${COCOS2DX_SOURCE_DIR}/support/*.cpp"
	"${COCOS2DX_SOURCE_DIR}/text_input_node/*.cpp"
	"${COCOS2DX_SOURCE_DIR}/textures/*.cpp"
	"${COCOS2DX_SOURCE_DIR}/tilemap_parallax_nodes/*.cpp"
	"${COCOS2DX_SOURCE_DIR}/touch_dispatcher/*.cpp"
)
file(GLOB_RECURSE COCOS2DX_PLATFORM_SOURCES
	"${COCOS2DX_SOURCE_DIR}/platform/linux/*.cpp"
)
add_library(cocos2dx ${COCOS2DX_SOURCES} ${COCOS2DX_BASE_SOURCES} ${COCOS2DX_PLATFORM_SOURCES})

# cocos2dx-extensions
set(COCOS2DX_EXTENSIONS_SOURCE_DIR "../lib/cocos2dx/extensions")
include_directories("${COCOS2DX_EXTENSIONS_SOURCE_DIR}")
include_directories("${COCOS2DX_EXTENSIONS_SOURCE_DIR}/include")
file(GLOB_RECURSE COCOS2DX_EXTENSIONS_SOURCES
	"${COCOS2DX_EXTENSIONS_SOURCE_DIR}/CCBReader/*.cpp"
	"${COCOS2DX_EXTENSIONS_SOURCE_DIR}/GUI/CCControlExtension/*.cpp"
	"${COCOS2DX_EXTENSIONS_SOURCE_DIR}/GUI/CCScrollView/*.cpp"
)
add_library(cocos2dx-extensions ${COCOS2DX_EXTENSIONS_SOURCES})

# exam
set(EXAM_SOURCE_DIR "../src")
include_directories("${EXAM_SOURCE_DIR}")
file(GLOB EXAM_SOURCES
	"${EXAM_SOURCE_DIR}/service/*.cpp"
	"${EXAM_SOURCE_DIR}/gui/*.cpp"
	"${EXAM_SOURCE_DIR}/game/*.cpp"
	"${EXAM_SOURCE_DIR}/game/building/*.cpp"
	"${EXAM_SOURCE_DIR}/*.cpp"
)
file(GLOB EXAM_PLATFORM_SOURCES
    "${EXAM_SOURCE_DIR}/platform/linux/*.cpp"
)
add_executable(exam ${EXAM_SOURCES} ${EXAM_PLATFORM_SOURCES})
target_link_libraries(cocos2dx X11 GL GLEW glfw z png tiff jpeg fontconfig freetype)
target_link_libraries(cocos2dx "${CMAKE_BINARY_DIR}/${COCOS2DX_SOURCE_DIR}/platform/third_party/linux/libraries/lib64/libwebp.a")
target_link_libraries(cocos2dx-extensions cocos2dx)
target_link_libraries(exam cocos2dx cocos2dx-extensions)
