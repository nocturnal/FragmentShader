INCLUDE_DIRECTORIES(.)

ADD_EXECUTABLE(zoomapp zoomapp.cpp)
ADD_EXECUTABLE(dotviewer wxbitmapdc.cpp wxcanvas.cpp graphviz.cpp dotviewer.cpp xdot.cpp)
ADD_EXECUTABLE(igFragmentShader wxbitmapdc.cpp wxcanvas.cpp wxpopupmenu.cpp graphviz.cpp luavalid.cpp expr.c luabind.cpp graph.cpp propedit.cpp shader.cpp xdot.cpp)

SET(wxWidgets_USE_LIBS)
FIND_PACKAGE(wxWidgets)
IF(wxWidgets_FOUND)
	INCLUDE(${wxWidgets_USE_FILE})
	TARGET_LINK_LIBRARIES(zoomapp ${wxWidgets_LIBRARIES})
	TARGET_LINK_LIBRARIES(dotviewer ${wxWidgets_LIBRARIES})
	TARGET_LINK_LIBRARIES(igFragmentShader ${wxWidgets_LIBRARIES})
ENDIF(wxWidgets_FOUND)

TARGET_LINK_LIBRARIES(igFragmentShader lua libwx_mswd_propgrid-2.8.dll)
#TARGET_LINK_LIBRARIES(igFragmentShader lua libwx_gtk2u_propgrid-2.8.so)

ADD_EXECUTABLE(igfsc expr.c shaderc.c)
TARGET_LINK_LIBRARIES(igfsc lua m)

#ADD_CUSTOM_TARGET(
#	gperf.h
#	ALL
#	gperf -c -C -l -L ANSI-C -t -7 -m 100 gperf.in > gperf.h
#	DEPENDS gperf.in
#	WORKING_DIRECTORY ${shader_SOURCE_DIR}/src
#	VERBATIM
#)	
#SET_SOURCE_FILES_PROPERTIES(luabind.cpp PROPERTIES OBJECT_DEPENDS gperf.h)

#ADD_CUSTOM_COMMAND(
#	OUTPUT gperf.h
#	COMMAND gperf -c -C -l -L ANSI-C -t -7 -m 100 gperf.in > gperf.h
#	DEPENDS gperf.in
#	WORKING_DIRECTORY ${shader_SOURCE_DIR}/src
#	VERBATIM
#)
#SET_SOURCE_FILES_PROPERTIES(luabind.cpp PROPERTIES OBJECT_DEPENDS gperf.h)
