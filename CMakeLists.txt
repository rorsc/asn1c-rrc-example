cmake_minimum_required (VERSION 3.20)
project (asn1c-rrc-example LANGUAGES C)

include(cmake/macros.cmake)

# Check if asn1c is installed, and check if it supports all options we need
# a user can provide ASN1C_EXEC to override an asn1c to use (e.g., parallel
# installation)
find_program(ASN1C_EXEC_PATH asn1c HINTS /opt/asn1c/bin)
set(ASN1C_EXEC ${ASN1C_EXEC_PATH} CACHE FILEPATH "path to asn1c executable")
if(NOT ASN1C_EXEC)
  message(FATAL_ERROR "No asn1c found! please provide the path through -DASN1C_EXEC=/path/to/asn1c")
endif()
check_option(${ASN1C_EXEC} -gen-APER ASN1C_EXEC)
check_option(${ASN1C_EXEC} -no-gen-UPER ASN1C_EXEC)
check_option(${ASN1C_EXEC} -no-gen-JER ASN1C_EXEC)
check_option(${ASN1C_EXEC} -no-gen-BER ASN1C_EXEC)
check_option(${ASN1C_EXEC} -no-gen-OER ASN1C_EXEC)

set(ADDRESS_SANITIZER OFF CACHE STRING "compile with address sanitizer")
set_property(CACHE ADDRESS_SANITIZER PROPERTY TYPE BOOL)
if (ADDRESS_SANITIZER)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address -fno-omit-frame-pointer -fno-common")
endif ()

add_subdirectory(asn1c)

add_executable(rrc-reconfig rrc-reconfig.c)
target_link_libraries(rrc-reconfig PRIVATE asn1_nr_rrc)
