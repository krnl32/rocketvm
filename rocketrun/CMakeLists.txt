file(GLOB_RECURSE ROCKETRUN_SOURCES
	"${CMAKE_CURRENT_SOURCE_DIR}/main.c"
)

add_executable(${ROCKETRUN_TARGET_NAME} ${ROCKETRUN_SOURCES})

set_compile_options_for_target(${ROCKETRUN_TARGET_NAME})

target_include_directories(${ROCKETRUN_TARGET_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../")
 target_link_libraries(${ROCKETRUN_TARGET_NAME} PRIVATE ${ROCKETVM_TARGET_NAME})

function(CopyLibToTarget LIB_NAME TARGET_NAME)
	add_custom_command(TARGET ${TARGET_NAME}
		POST_BUILD
		COMMAND
		${CMAKE_COMMAND} -E copy_if_different
		$<TARGET_FILE:${LIB_NAME}>
		$<TARGET_FILE_DIR:${TARGET_NAME}>
	)
endfunction()
