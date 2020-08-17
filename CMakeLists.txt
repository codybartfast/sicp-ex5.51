
cmake_minimum_required (VERSION 3.8)

project (sicp-scheme)

set (SRC_CORE
	src/character.c
	src/convert.c
	src/error.c
	src/environment.c
	src/eval.c
	src/inport.c
	src/lexer.c
	src/list.c
	src/obj.c
	src/outport.c
	src/output.c
	src/parser.c
	src/primproc.c
	src/sicpstd.h
	src/strbldr.c
	)

set(BIN_DIR  ${CMAKE_SOURCE_DIR}/bin)
file(MAKE_DIRECTORY ${BIN_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})

add_executable (sicp ${SRC_CORE} src/sicp.c)
