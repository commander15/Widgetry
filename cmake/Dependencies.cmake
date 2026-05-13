include(FetchContent)
set(FETCHCONTENT_TRY_FIND_PACKAGE_MODE ALWAYS CACHE STRING "")

# DataGate

FetchContent_Declare(
    DataGate
    GIT_REPOSITORY "https://github.com/commander15/DataGate.git"
    GIT_TAG "v3.0.1"
    FIND_PACKAGE_ARGS 3.0.1
)

# We find/download them
FetchContent_MakeAvailable(DataGate)

if (NOT TARGET DataGate::DataGate)
    add_library(DataGate::DataGate ALIAS DataGate)
endif()
