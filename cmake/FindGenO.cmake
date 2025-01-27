set(GENO_REPO   "https://github.com/commander15/GenO")
set(GENO_BRANCH "main")

set(GENO_DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/download)
set(GENO_ZIP          ${GENO_DOWNLOAD_DIR}/GenO.zip)
set(GENO_SOURCE_DIR   ${GENO_DOWNLOAD_DIR}/GenO-${GENO_BRANCH})

if (NOT EXISTS ${GENO_ZIP})
    message("Downloading GenO...")

    file(DOWNLOAD
        "${GENO_REPO}/archive/refs/heads/${GENO_BRANCH}.zip"
        ${GENO_ZIP}
        SHOW_PROGRESS
    )
endif()

if (NOT EXISTS ${GENO_SOURCE_DIR})
    message("Extracting GenO...")

    file(ARCHIVE_EXTRACT INPUT ${GENO_ZIP} DESTINATION ${GENO_DOWNLOAD_DIR})
endif()

add_subdirectory(${GENO_SOURCE_DIR})

add_library(GenO::GenO ALIAS GenO)
