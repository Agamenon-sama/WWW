.PHONY: all clean

APP := server
CXX := g++
CXX_FLAGS := -Wall -Os -O3
LINK_FLAGS := -pthread


all: $(APP)

$(APP): Server.o HTTPHandler.o
	@echo "Linking the app"
	$(CXX) -o $(APP) ../src/main.cpp Server.o HTTPHandler.o $(LINK_FLAGS)
	@mv $(APP) ..

Server.o: ../src/Server.cpp ../src/Server.h
	@echo "compiling server object"
	$(CXX) ../src/Server.cpp $(CXX_FLAGS) -c $(LINK_FLAGS)

HTTPHandler.o: ../src/HTTPHandler.cpp ../src/HTTPHandler.h
	@echo "compiling HTTPHandler object"
	$(CXX) ../src/HTTPHandler.cpp $(CXX_FLAGS) -c $(LINK_FLAGS)

clean:
	rm *.o ../$(APP)