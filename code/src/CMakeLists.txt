# IO

add_library(rc_io
  io/journal_impl.cpp
)

###############

# Network

add_library(rc_network
  network/network_controller.cpp
)

target_link_libraries(rc_network PUBLIC
  rc_io
)

###############

# rc-car

add_executable(rc-car
  main.cpp
)

target_link_libraries(rc-car PUBLIC
  rc_io
  rc_network
)

###############
