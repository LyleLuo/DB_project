# Where to find user code.
USER_DIR = .

USER_HEADER_DIR = ../include

# Flags passed to the C++ compiler.
CXXFLAGS += -g -pthread -std=c++11

TARGET = ./bin/ycsb

DATA = ../data

BIN = ./bin

USER_HEADERS = ../include/*.h

all :  $(DATA) $(BIN) $(TARGET)

clean :
	rm -f *.o ../data/*

cleano :
	rm -f $(GTEST_LIBS) *.o

cleand : 
	rm -f ../data/*

data_page.o: $(USER_DIR)/data_page.cpp ../include/data_page.h ../include/pm_ehash_struct.h
	$(CXX) $(CXXFLAGS) -c $(USER_DIR)/data_page.cpp -I $(USER_HEADER_DIR)

pm_ehash.o: $(USER_DIR)/pm_ehash.cpp $(USER_DIR)/data_page.cpp ../include/pm_ehash.h ../include/data_page.h ../include/pm_ehash_struct.h
	$(CXX) $(CXXFLAGS) -c $(USER_DIR)/pm_ehash.cpp -I $(USER_HEADER_DIR)

$(TARGET) : pm_ehash.o ycsb.o data_page.o
	$(CXX) $(CXXFLAGS) -L -lpthread $^ -lpmem -o $@

ycsb.o : ycsb.cpp $(USER_DIR)/data_page.cpp $(USER_HEADER_DIR)/pm_ehash.h
	$(CXX) $(CXXFLAGS) -c ycsb.cpp -I $(USER_HEADER_DIR)

$(DATA):
	mkdir -p ../data

$(BIN):
	mkdir -p ./bin
