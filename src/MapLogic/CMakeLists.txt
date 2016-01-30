set(maplogic_src
	controller_field.cpp
	controller_field.h
	door.cpp
	door.h
	field.cpp
	field.h
	field_impl.h
	generic_map.h
	map.cpp
	map.h
	skull.cpp
	skull.h
	spikes.cpp
	spikes.h
	stairs.cpp
	stairs.h
	switch.cpp
	switch.h
	root_trap.cpp
	root_trap.h
)

include_directories(
	${SDL2_INCLUDE_DIR}
)

add_library(MapLogic ${maplogic_src})
