find_path(DISCORDE_INCLUDE_DIR
        NAMES discorde_cpp.h
        PATHS ${DISCORDE_ROOT_DIR}
        PATH_SUFFIXES include include/discorde
)

find_library(DISCORDE_CPP_LIBRARY
        NAMES discorde_cpp
        PATHS ${DISCORDE_ROOT_DIR}
        PATH_SUFFIXES lib lib64
)

find_library(DISCORDE_C_LIBRARY
        NAMES discorde_c
        PATHS ${DISCORDE_ROOT_DIR}
        PATH_SUFFIXES lib lib64
)

mark_as_advanced(DISCORDE_FOUND DISCORDE_INCLUDE_DIR DISCORDE_C_LIBRARY DISCORDE_CPP_LIBRARY)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(DISCORDE
        REQUIRED_VARS DISCORDE_INCLUDE_DIR DISCORDE_C_LIBRARY DISCORDE_CPP_LIBRARY
)

if(DISCORDE_FOUND)
    set(DISCORDE_INCLUDE_DIRS ${DISCORDE_INCLUDE_DIR})
    set(DISCORDE_LIBRARIES ${DISCORDE_CPP_LIBRARY} ${DISCORDE_C_LIBRARY})
endif()

if(DISCORDE_FOUND AND NOT TARGET Discorde::discorde)
    add_library(Discorde::discorde INTERFACE IMPORTED)
    set_target_properties(Discorde::discorde PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES "${DISCORDE_INCLUDE_DIRS}"
            INTERFACE_LINK_LIBRARIES "${DISCORDE_LIBRARIES}"
    )
endif()
