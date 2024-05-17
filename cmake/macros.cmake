# this function should produce the same value as the macro MAKE_VERSION defined in the C code (file types.h)
function(make_version VERSION_VALUE)
  math(EXPR RESULT "0")
  foreach (ARG ${ARGN})
    math(EXPR RESULT "${RESULT} * 256 + ${ARG}")
  endforeach()
  set(${VERSION_VALUE} "${RESULT}" PARENT_SCOPE)
endfunction()

function(check_option EXEC TEST_OPTION EXEC_HINT)
  message(STATUS "Check if ${EXEC} supports ${TEST_OPTION}")
  execute_process(COMMAND ${EXEC} ${TEST_OPTION}
                  RESULT_VARIABLE CHECK_STATUS
                  OUTPUT_VARIABLE CHECK_OUTPUT
                  ERROR_VARIABLE CHECK_OUTPUT)
  if(NOT ${CHECK_STATUS} EQUAL 1)
    get_filename_component(EXEC_FILE ${EXEC} NAME)
    message(FATAL_ERROR "Error message: ${CHECK_OUTPUT}\
You might want to re-run ./build_oai -I
Or provide a path to ${EXEC_FILE} using
  ./build_oai ... --cmake-opt -D${EXEC_HINT}=/path/to/${EXEC_FILE}
or directly with
  cmake .. -D${EXEC_HINT}=/path/to/${EXEC_FILE}
")
  endif()
endfunction()
