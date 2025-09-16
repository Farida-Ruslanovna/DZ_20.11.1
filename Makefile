CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

OBJS = main.o user.o message.o chat.o

TARGET = chat_app

all: $(TARGET)

$(TARGET): $(OBJS)
    $(CXX) -o $@ $^

main.o: main.cpp
    $(CXX) $(CXXFLAGS) -c main.cpp

user.o: user.cpp user.h
    $(CXX) $(CXXFLAGS) -c user.cpp

message.o: message.cpp message.h
    $(CXX) $(CXXFLAGS) -c message.cpp

chat.o: chat.cpp chat.h
    $(CXX) $(CXXFLAGS) -c chat.cpp

clean:
    rm -f $(OBJS) $(TARGET)