file(GLOB_RECURSE ROCKETVM_SOURCES
	"${CMAKE_CURRENT_SOURCE_DIR}/main.c"

	"${CMAKE_CURRENT_SOURCE_DIR}/memory.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/memory.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/memory.c"

	"${CMAKE_CURRENT_SOURCE_DIR}/utility/logger.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/utility/logger.c"

	"${CMAKE_CURRENT_SOURCE_DIR}/cpu/cpu.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/cpu/cpu.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/cpu/cpu_encoder.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/cpu/cpu_data.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/cpu/cpu_data.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/cpu/cpu_mem.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/cpu/cpu_mem.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/cpu/cpu_alu.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/cpu/cpu_alu.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/cpu/cpu_ctrlf.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/cpu/cpu_ctrlf.c"
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
