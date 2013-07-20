
include_directories(${GLFW3_SOURCE_DIR}/src
                    ${GLFW3_BINARY_DIR}/src
                    ${GLFW3_INCLUDE_DIRS})

if (MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

set(COMMON3_HEADERS ${GLFW3_SOURCE_DIR}/include/GLFW/glfw3.h internal.h)
set(COMMON3_SOURCES clipboard.c context.c gamma.c init.c input.c joystick.c
                   monitor.c time.c window.c)

if (_GLFW3_COCOA)
    set(GLFW3_HEADERS ${COMMON3_HEADERS} cocoa_platform.h)
    set(GLFW3_SOURCES ${COMMON3_SOURCES} cocoa_clipboard.m cocoa_gamma.c
                     cocoa_init.m cocoa_joystick.m cocoa_monitor.m cocoa_time.c
                     cocoa_window.m)
elseif (_GLFW3_WIN32)
    set(GLFW3_HEADERS ${COMMON3_HEADERS} win32_platform.h)
    set(GLFW3_SOURCES ${COMMON3_SOURCES} win32_clipboard.c win32_gamma.c
                     win32_init.c win32_joystick.c win32_monitor.c win32_time.c
                     win32_window.c)
elseif (_GLFW3_X11)
    set(GLFW3_HEADERS ${COMMON3_HEADERS} x11_platform.h)
    set(GLFW3_SOURCES ${COMMON3_SOURCES} x11_clipboard.c x11_gamma.c x11_init.c
                     x11_joystick.c x11_monitor.c x11_time.c x11_window.c
                     x11_unicode.c)
endif()

if (_GLFW3_EGL)
    list(APPEND GLFW3_HEADERS ${COMMON3_HEADERS} egl_platform.h)
    list(APPEND GLFW3_SOURCES ${COMMON3_SOURCES} egl_context.c)
elseif (_GLFW3_NSGL)
    list(APPEND GLFW3_HEADERS ${COMMON3_HEADERS} nsgl_platform.h)
    list(APPEND GLFW3_SOURCES ${COMMON3_SOURCES} nsgl_context.m)
elseif (_GLFW3_WGL)
    list(APPEND GLFW3_HEADERS ${COMMON3_HEADERS} wgl_platform.h)
    list(APPEND GLFW3_SOURCES ${COMMON3_SOURCES} wgl_context.c)
elseif (_GLFW3_X11)
    list(APPEND GLFW3_HEADERS ${COMMON3_HEADERS} glx_platform.h)
    list(APPEND GLFW3_SOURCES ${COMMON3_SOURCES} glx_context.c)
endif()

if (APPLE)
    # For some reason, CMake doesn't know about .m
    set_source_files_properties(${GLFW3_SOURCES} PROPERTIES LANGUAGE C)
endif()

add_library(glfw3 ${GLFW3_SOURCES} ${GLFW3_HEADERS})
set_target_properties(glfw3 PROPERTIES OUTPUT_NAME "${GLFW3_LIB_NAME}")

if (BUILD_SHARED_LIBS)
    # Include version information in the output
    set_target_properties(glfw3 PROPERTIES VERSION ${GLFW3_VERSION})
    if (UNIX)
        set_target_properties(glfw3 PROPERTIES SOVERSION ${GLFW3_VERSION_MAJOR})
    endif()

    if (WIN32)
        # The GLFW DLL needs a special compile-time macro and import library name
        set_target_properties(glfw3 PROPERTIES PREFIX "" IMPORT_PREFIX "")

        if (MINGW)
            set_target_properties(glfw3 PROPERTIES IMPORT_SUFFIX "dll.a")
        else()
            set_target_properties(glfw3 PROPERTIES IMPORT_SUFFIX "dll.lib")
        endif()
    elseif (APPLE)
        # Append -fno-common to the compile flags to work around a bug in
        # Apple's GCC
        get_target_property(GLFW3_CFLAGS glfw3 COMPILE_FLAGS)
        if (NOT GLFW3_CFLAGS)
            set(GLFW3_CFLAGS "")
        endif()
        set_target_properties(glfw3 PROPERTIES
                              COMPILE_FLAGS "${GLFW3_CFLAGS} -fno-common")
    endif()

    target_link_libraries(glfw3 ${GLFW3_LIBRARIES})
    target_link_libraries(glfw3 LINK_INTERFACE_LIBRARIES)
endif()

if (GLFW3_INSTALL)
    install(TARGETS glfw3 EXPORT glfwTargets DESTINATION lib${LIB_SUFFIX})
endif()

