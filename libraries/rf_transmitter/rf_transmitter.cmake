add_library(rf_transmitter INTERFACE)

target_sources(rf_transmitter INTERFACE
  ${CMAKE_CURRENT_LIST_DIR}/rf_transmitter.c
)

target_include_directories(rf_transmitter INTERFACE ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(rf_transmitter INTERFACE)