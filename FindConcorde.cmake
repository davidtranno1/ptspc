find_path(CONCORDE_INCLUDE_DIR
        NAMES concorde.h
        PATHS ${CONCORDE_ROOT_DIR}
        PATH_SUFFIXES include
)

find_library(CONCORDE_LIBRARY
        NAMES concorde
        PATHS ${CONCORDE_ROOT_DIR}
        PATH_SUFFIXES lib lib64
)

mark_as_advanced(CONCORDE_FOUND CONCORDE_INCLUDE_DIR CONCORDE_LIBRARY)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(CONCORDE
        REQUIRED_VARS CONCORDE_INCLUDE_DIR CONCORDE_LIBRARY
)

if(CONCORDE_FOUND)
    set(CONCORDE_INCLUDE_DIRS ${CONCORDE_INCLUDE_DIR})
    set(CONCORDE_LIBRARIES ${CONCORDE_LIBRARY})
endif()

if(CONCORDE_FOUND AND NOT TARGET Concorde::concorde)
    add_library(Concorde::concorde INTERFACE IMPORTED)
    set_target_properties(Concorde::concorde PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES "${CONCORDE_INCLUDE_DIRS}"
            INTERFACE_LINK_LIBRARIES "${CONCORDE_LIBRARIES}"
    )
endif()
