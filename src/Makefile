CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

SRCS = main.cpp Carte.cpp CarteDigitala.cpp Audiobook.cpp MangaComics.cpp FilmCarte.cpp Biblioteca.cpp Utilizator.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = biblioteca

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

run: all
	./$(TARGET)
