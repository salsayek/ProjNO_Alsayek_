set(NOproject_NAME NOproject)				#Naziv prvog projekta u solution-u

file(GLOB NOproject_SOURCES  ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
file(GLOB NOproject_INCS  ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
file(GLOB NOproject_INC_CNT  ${NATID_SDK_INC}/cnt/*.h)
file(GLOB NOproject_INC_SC  ${NATID_SDK_INC}/sc/*.h)
file(GLOB NOproject_INC_TD  ${NATID_SDK_INC}/td/*.h)
file(GLOB NOproject_INC_THREAD  ${NATID_SDK_INC}/thread/*.h)
file(GLOB NOproject_INC_GUI ${NATID_SDK_INC}/gui/*.h)

#Application icon
set(DIGIT_TWIN_PLIST  ${CMAKE_CURRENT_LIST_DIR}/res/appIcon/AppIcon.plist)
if(WIN32)
	set(DIGIT_TWIN_WINAPP_ICON ${CMAKE_CURRENT_LIST_DIR}/res/appIcon/winAppIcon.rc)
else()
	set(DIGIT_TWIN_WINAPP_ICON ${CMAKE_CURRENT_LIST_DIR}/res/appIcon/winAppIcon.cpp)
endif()

# add executable
add_executable(${NOproject_NAME} ${NOproject_INCS} ${NOproject_SOURCES} ${NOproject_INC_TD}  
	${NOproject_INC_GUI} ${NOproject_INC_CNT} ${NOproject_INC_SC} ${NOproject_INC_THREAD}
	${DIGIT_TWIN_WINAPP_ICON})

source_group("inc"            FILES ${NOproject_INCS})
source_group("inc\\cnt"        FILES ${NOproject_INC_CNT})
source_group("inc\\sc"        FILES ${NOproject_INC_SC})
source_group("inc\\td"        FILES ${NOproject_INC_TD})
source_group("inc\\thread"    FILES ${NOproject_INC_THREAD})
source_group("inc\\gui"        FILES ${NOproject_INC_GUI})
source_group("src"            FILES ${NOproject_SOURCES})

target_link_libraries(${NOproject_NAME} debug ${MU_LIB_DEBUG} debug ${NATGUI_LIB_DEBUG} debug ${SYMBCOMP_LIB_DEBUG}
										optimized ${MU_LIB_RELEASE} optimized ${NATGUI_LIB_RELEASE} optimized ${SYMBCOMP_LIB_RELEASE})


target_compile_definitions(${NOproject_NAME} PUBLIC MU_SHOW_MEM_LEAKS)

setTargetPropertiesForGUIApp(${NOproject_NAME} ${DIGIT_TWIN_PLIST})

setAppIcon(${NOproject_NAME} ${CMAKE_CURRENT_LIST_DIR})

set(DevResPathLoc "${CMAKE_CURRENT_LIST_DIR}")

setIDEPropertiesForGUIExecutable(${NOproject_NAME} ${CMAKE_CURRENT_LIST_DIR})

if(WIN32)
    set_target_properties(${NOproject_NAME} PROPERTIES
        VS_DEBUGGER_COMMAND_ARGUMENTS "-devResPath=${CMAKE_CURRENT_LIST_DIR}"
    )
endif()

setPlatformDLLPath(${NOproject_NAME})
