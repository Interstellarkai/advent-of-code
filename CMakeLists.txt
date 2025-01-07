cmake_minimum_required(VERSION 3.30)
project(2024)

set(CMAKE_CXX_STANDARD 20)

function(day name)
    # Add executable for the solution
    add_executable(2024-${name} ${name}/solution.cpp)

    # Set the working directory for runtime (if needed)
    set_target_properties(2024-${name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${name})

    # Copy input files into the respective build directory
    file(GLOB inputs "${name}/*.txt")
    foreach (input ${inputs})
        add_custom_command(
                TARGET 2024-${name} PRE_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy ${input} $<TARGET_FILE_DIR:2024-${name}>
        )
    endforeach ()
endfunction()

day(day-1)
day(day-2)
day(day-3)
day(day-4)
day(day-5)