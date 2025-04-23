cmake_minimum_required(VERSION 3.31)
project(libftpp)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

add_library(libftpp STATIC 
	include/data_structures.hpp
	include/data_structures/Pool.hpp 
	include/data_structures/PoolBuffer.hpp 
	include/data_structures/DataBuffer.hpp

	source/data_structures/DataBuffer.cpp

	include/design_patterns.hpp
		include/design_patterns/Memento.hpp
		include/design_patterns/Observer.hpp

		source/design_patterns/Memento.cpp
		include/design_patterns/Singleton.hpp)

enable_testing()

add_subdirectory(test)
