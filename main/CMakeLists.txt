set(SOURCES
    main.cpp
    models/tick_atomic.hpp
    models/tree_atomic.hpp
    models/top_model.hpp
)

add_executable(tree_sim ${SOURCES})

target_include_directories(tree_sim PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/models
)

