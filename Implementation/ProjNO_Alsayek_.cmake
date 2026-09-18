set(ProjNO_Alsayek__NAME ProjNO_Alsayek_)				#Naziv prvog projekta u solution-u

file(GLOB ProjNO_Alsayek__SOURCES  ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
file(GLOB ProjNO_Alsayek__INCS  ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
file(GLOB ProjNO_Alsayek__INC_CNT  ${NATID_SDK_INC}/cnt/*.h)
file(GLOB ProjNO_Alsayek__INC_SC  ${NATID_SDK_INC}/sc/*.h)
file(GLOB ProjNO_Alsayek__INC_TD  ${NATID_SDK_INC}/td/*.h)
file(GLOB ProjNO_Alsayek__INC_THREAD  ${NATID_SDK_INC}/thread/*.h)
file(GLOB ProjNO_Alsayek__INC_GUI ${NATID_SDK_INC}/gui/*.h)

#Application icon
set(DIGIT_TWIN_PLIST  ${CMAKE_CURRENT_LIST_DIR}/res/appIcon/AppIcon.plist)
if(WIN32)
	set(DIGIT_TWIN_WINAPP_ICON ${CMAKE_CURRENT_LIST_DIR}/res/appIcon/winAppIcon.rc)
else()
	set(DIGIT_TWIN_WINAPP_ICON ${CMAKE_CURRENT_LIST_DIR}/res/appIcon/winAppIcon.cpp)
endif()

# add executable
add_executable(${ProjNO_Alsayek__NAME} ${ProjNO_Alsayek__INCS} ${ProjNO_Alsayek__SOURCES} ${ProjNO_Alsayek__INC_TD}  
	${ProjNO_Alsayek__INC_GUI} ${ProjNO_Alsayek__INC_CNT} ${ProjNO_Alsayek__INC_SC} ${ProjNO_Alsayek__INC_THREAD}
	${DIGIT_TWIN_WINAPP_ICON})

source_group("inc"            FILES ${ProjNO_Alsayek__INCS})
source_group("inc\\cnt"        FILES ${ProjNO_Alsayek__INC_CNT})
source_group("inc\\sc"        FILES ${ProjNO_Alsayek__INC_SC})
source_group("inc\\td"        FILES ${ProjNO_Alsayek__INC_TD})
source_group("inc\\thread"    FILES ${ProjNO_Alsayek__INC_THREAD})
source_group("inc\\gui"        FILES ${ProjNO_Alsayek__INC_GUI})
source_group("src"            FILES ${ProjNO_Alsayek__SOURCES})

target_link_libraries(${ProjNO_Alsayek__NAME} debug ${MU_LIB_DEBUG} debug ${NATGUI_LIB_DEBUG} debug ${SYMBCOMP_LIB_DEBUG}
										optimized ${MU_LIB_RELEASE} optimized ${NATGUI_LIB_RELEASE} optimized ${SYMBCOMP_LIB_RELEASE})


target_compile_definitions(${ProjNO_Alsayek__NAME} PUBLIC MU_SHOW_MEM_LEAKS)

setTargetPropertiesForGUIApp(${ProjNO_Alsayek__NAME} ${DIGIT_TWIN_PLIST})

setAppIcon(${ProjNO_Alsayek__NAME} ${CMAKE_CURRENT_LIST_DIR})

set(DevResPathLoc "${CMAKE_CURRENT_LIST_DIR}")

setIDEPropertiesForGUIExecutable(${ProjNO_Alsayek__NAME} ${CMAKE_CURRENT_LIST_DIR})

if(WIN32)
    set_target_properties(${ProjNO_Alsayek__NAME} PROPERTIES
        VS_DEBUGGER_COMMAND_ARGUMENTS "-devResPath=${CMAKE_CURRENT_LIST_DIR}"
    )
endif()

setPlatformDLLPath(${ProjNO_Alsayek__NAME})
