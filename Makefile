# Pool
# POOL_OBJECT =
POOL_HEADER = include/pool.hpp

# Data Buffer
DATA_BUFFER_OBJECT = object/data_buffer.o
DATA_BUFFER_HEADER = include/data_buffer.hpp

# Memento
MEMENTO_OBJECT = object/memento.o
MEMENTO_HEADER = include/memento.hpp

# Observer
# OBSERVER_OBJECT =
OBSERVER_HEADER = include/observer.hpp

# Singleton
# SINGLETON_OBJECT =
SINGLETON_HEADER = include/singleton.hpp

# State Machine
# STATE_MACHINE_OBJECT =
STATE_MACHINE_HEADER = include/state_machine.hpp

# Thread-Safe IO Stream
# THREAD_SAFE_IOSTREAM_OBJECT =
THREAD_SAFE_IOSTREAM_HEADER = include/thread_safe_iostream.hpp

# Thread Safe Queue
# THREAD_SAFE_QUEUE_OBJECT =
THREAD_SAFE_QUEUE_HEADER = include/thread_safe_queue.hpp

# Thread Management
THREAD_OBJECT = object/thread.o
THREAD_HEADER = include/thread.hpp

# Worker Pool
WORKER_POOL_OBJECT = object/worker_pool.o
WORKER_POOL_HEADER = include/worker_pool.hpp

# Persistent Worker
PERSISTENT_WORKER_OBJECT = object/persistent_worker.o
PERSISTENT_WORKER_HEADER = include/persistent_worker.hpp

# Networking: Message
MESSAGE_OBJECT = object/message.o
MESSAGE_HEADER = include/message.hpp

# Networking: Client
CLIENT_OBJECT = object/client.o
CLIENT_HEADER = include/client.hpp

# Networking: Server
SERVER_OBJECT = object/server.o
SERVER_HEADER = include/server.hpp

# Mathematics: 2D Vector
IVECTOR2_OBJECT = object/ivector2.o
IVECTOR2_HEADER = include/ivector2.hpp

# Mathematics: 3D Vector
IVECTOR3_OBJECT = object/ivector3.o
IVECTOR3_HEADER = include/ivector3.hpp

# Random 2D Coordinate Generator
RANDOM_2D_COORDINATE_GENERATOR_OBJECT = object/random_2D_coordinate_generator.o
RANDOM_2D_COORDINATE_GENERATOR_HEADER = random_2D_coordinate_generator.hpp

# Perlin Noise 2D
PERLIN_NOISE_2D_OBJECT = object/perlin_noise_2D.o
PERLIN_NOISE_2D_HEADER = perlin_noise_2D.hpp

OBJECT = $(DATA_BUFFER_OBJECT) $(MEMENTO_OBJECT) $(THREAD_OBJECT) $(WORKER_POOL_OBJECT) $(PERSISTENT_WORKER_OBJECT) $(MESSAGE_OBJECT) $(CLIENT_OBJECT) $(SERVER_OBJECT) $(IVECTOR2_OBJECT) $(IVECTOR3_OBJECT) $(RANDOM_2D_COORDINATE_GENERATOR_OBJECT) $(PERLIN_NOISE_2D_OBJECT)

NAME = libftpp.a
NAME_HEADER = data_structures.hpp

TEST = test

CXXFLAGS = -g3 -Wall -Wextra

all: $(NAME) $(TEST)

$(NAME): $(OBJECT)
	@echo compiling eveything
	ar -rc $(NAME) $(objects)

clean:
	@echo removing object file : $(OBJECT)
	@rm -rf object

fclean: clean
	@echo removing static library file : $(NAME)
	@rm -rf $(NAME)

re: fclean all

object/%.o: source/%.cpp
	@echo compiling $<
	@mkdir -p $(dir $@)
	@c++ $(CXXFLAGS) -o $@ -c $<

$(TEST): $(NAME)
	@echo compiling with main.cpp
	@c++ $(CXXFLAGS) $(NAME) main.cpp -Iinclude -o test

clean_$(TEST): fclean
	rm -rf test

re_$(TEST): fclean $(TEST)