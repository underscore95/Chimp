include(${CMAKE_SOURCE_DIR}/ChimpCommon.cmake)

add_executable(ChimpEditor "Source/Main.cpp" "Source/Logger.h" "Source/stdafx.h" "Source/EditorScene.h" "Source/EditorScene.cpp" "Source/scene_view/SceneViewScript.h" "Source/scene_view/SceneViewScript.cpp" "Source/scene_hierarchy/SceneHierarchyScript.h" "Source/scene_hierarchy/SceneHierarchyScript.cpp" "Source/scene_hierarchy/EntityNameComponent.h" "Source/inspector/InspectorScript.h" "Source/inspector/InspectorScript.cpp" )

setup_chimp_target(ChimpEditor)
setup_chimp_executable(ChimpEditor)

target_link_libraries(ChimpEditor PRIVATE ChimpGame)
target_include_directories(ChimpEditor PRIVATE ../ChimpGame/Source)