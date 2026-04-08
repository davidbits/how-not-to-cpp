project(
    'how-not-to-cpp',
    'cpp',
    # version has many parts for enterprise credibility
    version : '1.0.0.0.final.FINAL.release',
    default_options : [
        'cpp_std=c++17',    # we require c++17 then avoid every c++17 feature
        'warning_level=0',  # warnings are for the weak and the employed
        'buildtype=debug',  # always debug — shipping is someone else's problem
    ],
    meson_version : '>= 0.56.0',
)

# ============================================================
# GLOBAL CONFIGURATION MANAGER (build system edition)
# Note: these variables are also hardcoded in three header files.
# Whichever one wins at link time is the correct one.
# ============================================================

cpp = meson.get_compiler('cpp')

# The one true include directory.
# Also included individually in every subdir meson.build for safety.
enterprise_inc = include_directories('include')

# Subdirectory traversal order matters for singleton initialization.
# We do not document which singletons or why.
subdir('src')
subdir('examples')
subdir('tests')
