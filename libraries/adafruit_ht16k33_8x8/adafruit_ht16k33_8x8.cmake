add_library(adafruit_ht16k33_8x8 INTERFACE)

target_sources(adafruit_ht16k33_8x8 INTERFACE
  ${CMAKE_CURRENT_LIST_DIR}/ht16k33_8x8.c
)

target_include_directories(adafruit_ht16k33_8x8 INTERFACE ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(adafruit_ht16k33_8x8 INTERFACE pico_stdlib hardware_i2c)