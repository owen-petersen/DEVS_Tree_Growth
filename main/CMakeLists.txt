add_executable(main main.cpp)

target_include_directories(main PRIVATE "." "models" $ENV{CADMIUM})

target_compile_options(main PUBLIC -std=gnu++2b)