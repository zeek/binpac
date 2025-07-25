bison_target(PACParser pac_parse.yy ${BinPAC_BINARY_DIR}/src/pac_parse.cc
             DEFINES_FILE ${BinPAC_BINARY_DIR}/src/pac_parse.h COMPILE_FLAGS "--debug")
flex_target(PACScanner pac_scan.ll ${BinPAC_BINARY_DIR}/pac_scan.cc)
add_flex_bison_dependency(PACScanner PACParser)
if (MSVC)
    set_property(SOURCE pac_scan.cc APPEND_STRING PROPERTY COMPILE_FLAGS "/wd4018")
else ()
    set_property(SOURCE pac_scan.cc APPEND_STRING PROPERTY COMPILE_FLAGS "-Wno-sign-compare")
endif ()

include_directories(${PROJECT_SOURCE_DIR}/src ${PROJECT_BINARY_DIR}/src)

set(binpac_SRCS
    ${BISON_PACParser_INPUT}
    ${FLEX_PACScanner_INPUT}
    ${BISON_PACParser_OUTPUTS}
    ${FLEX_PACScanner_OUTPUTS}
    pac_action.cc
    pac_analyzer.cc
    pac_array.cc
    pac_attr.cc
    pac_btype.cc
    pac_case.cc
    pac_conn.cc
    pac_context.cc
    pac_cstr.cc
    pac_datadep.cc
    pac_dataptr.cc
    pac_dataunit.cc
    pac_decl.cc
    pac_embedded.cc
    pac_enum.cc
    pac_expr.cc
    pac_exttype.cc
    pac_field.cc
    pac_flow.cc
    pac_func.cc
    pac_id.cc
    pac_inputbuf.cc
    pac_let.cc
    pac_param.cc
    pac_paramtype.cc
    pac_primitive.cc
    pac_record.cc
    pac_redef.cc
    pac_regex.cc
    pac_state.cc
    pac_strtype.cc
    pac_type.cc
    pac_typedecl.cc
    pac_withinput.cc
    pac_output.cc
    pac_utils.cc
    pac_exception.cc
    pac_main.cc
    pac_action.h
    pac_analyzer.h
    pac_array.h
    pac_attr.h
    pac_btype.h
    pac_case.h
    pac_cclass.h
    pac_common.h
    pac_conn.h
    pac_context.h
    pac_cstr.h
    pac_ctype.h
    pac_datadep.h
    pac_dataptr.h
    pac_dataunit.h
    pac_dbg.h
    pac_decl-inl.h
    pac_decl.h
    pac_embedded.h
    pac_enum.h
    pac_exception.h
    pac_expr.h
    pac_exttype.h
    pac_field.h
    pac_flow.h
    pac_func.h
    pac_id.h
    pac_inputbuf.h
    pac_let.h
    pac_nullptr.h
    pac_number.h
    pac_output.h
    pac_param.h
    pac_paramtype.h
    pac_primitive.h
    pac_record.h
    pac_redef.h
    pac_regex.h
    pac_state.h
    pac_strtype.h
    pac_type.h
    pac_typedecl.h
    pac_utils.h
    pac_varfield.h
    pac_withinput.h)

include(${PROJECT_SOURCE_DIR}/cmake/RequireCXXStd.cmake)

add_executable(binpac ${binpac_SRCS})

if (MSVC)
    # If building separately from zeek, we need to add the libunistd subdirectory
    # so that linking doesn't fail.
    if ("${CMAKE_PROJECT_NAME}" STREQUAL "BinPAC")
        add_subdirectory(auxil/libunistd EXCLUDE_FROM_ALL)
    endif ()
    target_link_libraries(binpac PRIVATE libunistd)
endif ()

install(TARGETS binpac DESTINATION bin)

# This is set to assist superprojects that want to build BinPac from source and
# rely on it as a target
set(BinPAC_EXE binpac CACHE STRING "BinPAC executable" FORCE)
