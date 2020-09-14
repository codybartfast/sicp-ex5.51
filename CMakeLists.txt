
cmake_minimum_required (VERSION 3.8)

project (sicp-scheme)

set(C_FLAGS "-lm -O3 -std=c17 -pedantic -pedantic-errors -Wall -Wextra -Wdouble-promotion -Wmissing-prototypes -Wstrict-prototypes -Wold-style-definition")

set (SRC_CORE
	src/stb_image.h
	src/stb_image_write.h

	src/bitmap.c
	src/character.c
	src/convert.c
	src/custom.c
	src/error.c
	src/environment.c
	src/eval.c
	src/inport.c
	src/lexer.c
	src/list.c
	src/mceval.c
	src/obj.c
	src/outport.c
	src/output.c
	src/parser.c
	src/pict.c
	src/primproc.c
	src/storage.c
	src/strbldr.c
	)

set(BIN_DIR ${CMAKE_SOURCE_DIR}/bin)
file(MAKE_DIRECTORY ${BIN_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})

add_executable (sicp ${SRC_CORE} src/sicp.c)

if(MSVC)
	target_link_libraries(sicp)
else()
	target_link_libraries(sicp m)
endif()
