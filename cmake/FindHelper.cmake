function(find name)
    set(options)
    set(oneValueArgs REPOSITORY BRANCH)
    set(multiValueArgs)
    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    string(TOUPPER NAME ${name})

    if (NOT ARG_REPOSITORY)
        set(ARG_REPOSITORY "https://github.com/commander15/${name}")
    endif()

    if (NOT ARG_BRANCH)
        set(ARG_BRANCH "main")
    endif()

    set(DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/download)
    set(ZIP_FILE     ${DOWNLOAD_DIR}/${name}.zip)
    set(SOURCE_DIR   ${DOWNLOAD_DIR}/${name}-${ARG_BRANCH})

    if (NOT EXISTS ${SOURCE_DIR})
        message("Downloading ${name}...")

        file(DOWNLOAD
            "${ARG_REPOSITORY}/archive/refs/heads/${ARG_BRANCH}.zip"
            ${ZIP_FILE}
            SHOW_PROGRESS
        )

        message("Extracting ${name}...")

        file(ARCHIVE_EXTRACT INPUT ${ZIP_FILE} DESTINATION ${DOWNLOAD_DIR})
    endif()

    add_subdirectory(${SOURCE_DIR})

    set(ALIAS ${name}::${name})
    add_library(${ALIAS} ALIAS ${name})
endfunction()
