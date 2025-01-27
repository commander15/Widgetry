set(JSONER_REPO   "https://github.com/commander15/Jsoner")
set(JSONER_BRANCH "main")

set(JSONER_DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/download)
set(JSONER_ZIP          ${JSONER_DOWNLOAD_DIR}/Jsoner.zip)
set(JSONER_SOURCE_DIR   ${JSONER_DOWNLOAD_DIR}/Jsoner-${JSONER_BRANCH})

if (NOT EXISTS ${JSONER_ZIP})
    message("Downloading Jsoner...")

    file(DOWNLOAD
        "${JSONER_REPO}/archive/refs/heads/${JSONER_BRANCH}.zip"
        ${JSONER_ZIP}
        SHOW_PROGRESS
    )
endif()

if (NOT EXISTS ${JSONER_SOURCE_DIR})
    message("Extracting Jsoner...")

    file(ARCHIVE_EXTRACT INPUT ${JSONER_ZIP} DESTINATION ${JSONER_DOWNLOAD_DIR})
endif()

add_subdirectory(${JSONER_SOURCE_DIR})

add_library(Jsoner::Jsoner ALIAS Jsoner)
