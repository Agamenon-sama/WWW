.PHONY: all clean

APP := server
CXX := g++
CXX_FLAGS := -Wall -g
LINK_FLAGS := -pthread

# SRCS := $(wildcard ../src/*.cpp)
# OBJS := $(subst .cpp,.o, $(SRCS))
# OBJS := $(subst ../src/,, $(OBJS))

all: $(APP)

$(APP): Server.o HTTPHandler.o
	@echo "Linking the app"
	$(CXX) -o $(APP) ../src/main.cpp Server.o HTTPHandler.o $(LINK_FLAGS)
	@mv $(APP) ..

# main.o: ../src/Server.h
# 	@echo "compiling main object"
# 	$(CXX) ../src/main.cpp $(CXX_FLAGS) -c $(LINK_FLAGS)


Server.o: ../src/Server.cpp ../src/Server.h
	@echo "compiling server object"
	$(CXX) ../src/Server.cpp $(CXX_FLAGS) -c $(LINK_FLAGS)

HTTPHandler.o: ../src/HTTPHandler.cpp ../src/HTTPHandler.h
	@echo "compiling HTTPHandler object"
	$(CXX) ../src/HTTPHandler.cpp $(CXX_FLAGS) -c $(LINK_FLAGS)

clean:
	rm *.o ../$(APP)