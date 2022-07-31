 # CMakeLists.txt
    include(WiresharkPlugin)

    # Plugin name and version info (major minor micro extra)
    set_module_info(foo 0 0 4 0)

    set(DISSECTOR_SRC
    packet-foo.c
    )

    set(PLUGIN_FILES
    plugin.c
    ${DISSECTOR_SRC}
    )

    set_source_files_properties(
    ${PLUGIN_FILES}
    PROPERTIES
    COMPILE_FLAGS "${WERROR_COMMON_FLAGS}"
    )

    register_plugin_files(plugin.c
    plugin
    ${DISSECTOR_SRC}
    )

    add_plugin_library(foo epan)

    target_link_libraries(foo epan)

    install_plugin(foo epan)

    file(GLOB DISSECTOR_HEADERS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "*.h")
    CHECKAPI(
    NAME
    foo
    SWITCHES
    --group dissectors-prohibited
    --group dissectors-restricted
    SOURCES
    ${DISSECTOR_SRC}
    ${DISSECTOR_HEADERS}
    )

