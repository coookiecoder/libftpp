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
	include/design_patterns/Singleton.hpp
	include/design_patterns/StateMachine.hpp

	source/design_patterns/Memento.cpp

		include/thread_safe_iostream.hpp
		source/thread_safe_iostream.cpp
		include/threading.hpp
		include/threading/thread_safe_queue.hpp)

enable_testing()

add_subdirectory(test)
