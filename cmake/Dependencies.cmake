include(FetchContent)
set(FETCHCONTENT_TRY_FIND_PACKAGE_MODE ALWAYS CACHE STRING "")

# Jsoner

FetchContent_Declare(
    Jsoner
    GIT_REPOSITORY https://github.com/commander15/Jsoner.git
    GIT_TAG v1.0.0
    EXCLUDE_FROM_ALL
    FIND_PACKAGE_ARGS 1.0.0
)

# DataGate

FetchContent_Declare(
    DataGate
    GIT_REPOSITORY https://github.com/commander15/DataGate.git
    GIT_TAG v2.0.1
    EXCLUDE_FROM_ALL
    FIND_PACKAGE_ARGS 2.0.1
)

# We find/download them
FetchContent_MakeAvailable(Jsoner DataGate)

if (NOT TARGET Jsoner::Jsoner)
    add_library(Jsoner::Jsoner ALIAS Jsoner)
endif()

if (NOT TARGET DataGate::DataGate)
    add_library(DataGate::DataGate ALIAS DataGate)
endif()
