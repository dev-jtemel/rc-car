
add_library(rc_io
  io/journal_impl.cpp
)

add_executable(rc-car
  main.cpp
)

target_link_libraries(rc-car PUBLIC
  rc_io
)
