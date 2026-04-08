# src/meson.build
#
# Builds the core anti-pattern library and the main executable.
# Each source file is a standalone showcase of one terrible idea.
# They are linked together because integration makes everything worse.

how_not_to_cpp_sources = [
    'memory_leaks.cpp',
    'singleton_registry.cpp',
    'bool_parameter_hell.cpp',
    'inheritance_abuse.cpp',
    'dangling_reference_showcase.cpp',
    'macros_are_templates_now.cpp',
]

# The "enterprise library." Static. Eternal. Leaked.
how_not_to_cpp_lib = static_library(
    'how_not_to_cpp',
    how_not_to_cpp_sources,
    include_directories : enterprise_inc,
)

# Main executable. Links against the library.
# If it crashes, that is a feature of the anti-patterns, not the build system.
executable(
    'how-not-to-cpp',
    'main.cpp',
    include_directories : enterprise_inc,
    link_with           : how_not_to_cpp_lib,
)
