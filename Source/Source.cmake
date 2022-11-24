# Set the root
set(SOURCE_ROOT ${IRVINE_REPO_ROOT}/Source)

# Set the Programs root
set(PROGRAMS_ROOT ${SOURCE_ROOT}/Programs)
# Set the RUNTIME root
set(RUNTIME_ROOT ${SOURCE_ROOT}/Runtime)
# Set the Tests root
set(TESTS_ROOT ${SOURCE_ROOT}/Tests)
# Set the ThirdParty root
set(THIRD_PARTY_ROOT ${SOURCE_ROOT}/ThirdParty)

include(${SOURCE_ROOT}/Programs/Programs.cmake)
include(${SOURCE_ROOT}/Runtime/Runtime.cmake)
include(${SOURCE_ROOT}/Tests/Tests.cmake)
include(${SOURCE_ROOT}/ThirdParty/ThirdParty.cmake)
