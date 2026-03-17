file(GLOB_RECURSE ROCKETVM_SOURCES
	"${CMAKE_CURRENT_SOURCE_DIR}/main.c"

	"${CMAKE_CURRENT_SOURCE_DIR}/core/logger.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/core/logger.c"
)

add_executable(${ROCKETVM_TARGET_NAME} ${ROCKETVM_SOURCES})

set_compile_options_for_target(${ROCKETVM_TARGET_NAME})

target_include_directories(${ROCKETVM_TARGET_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../")
# target_link_libraries(${ROCKETVM_TARGET_NAME} PRIVATE)

function(CopyLibToTarget LIB_NAME TARGET_NAME)
	add_custom_command(TARGET ${TARGET_NAME}
		POST_BUILD
		COMMAND
		${CMAKE_COMMAND} -E copy_if_different
		$<TARGET_FILE:${LIB_NAME}>
		$<TARGET_FILE_DIR:${TARGET_NAME}>
	)
endfunction()
