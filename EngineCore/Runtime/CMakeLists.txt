project(Runtime LANGUAGES CXX)

# Executable target
add_executable(Runtime
    src/main.cpp
)

# Link with Jelly library
target_link_libraries(Runtime PRIVATE Jelly)

# Output directories
set_target_properties(Runtime PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIR}"
    LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_DIR}"
    ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_DIR}"
)
