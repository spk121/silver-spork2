# Makefile for Silver Spork 2 HTTP Server
# Designed for Fedora Linux

CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c11
LDFLAGS = -lcivetweb -lpthread -ldl -lm

TARGET = server
SOURCES = server.c
OBJECTS = $(SOURCES:.c=.o)

.PHONY: all clean install-deps help

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)
	@echo ""
	@echo "Build successful!"
	@echo "Run './$(TARGET)' to start the server"
	@echo "Or './$(TARGET) <port>' to specify a custom port"

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)
	@echo "Cleaned build artifacts"

# Install CivetWeb development files on Fedora
install-deps:
	@echo "Installing CivetWeb and build dependencies on Fedora..."
	sudo dnf install -y civetweb-devel gcc make
	@echo ""
	@echo "Dependencies installed successfully!"
	@echo "You can now run 'make' to build the server."

help:
	@echo "Silver Spork 2 HTTP Server - Makefile Help"
	@echo "==========================================="
	@echo ""
	@echo "Available targets:"
	@echo "  make              - Build the server"
	@echo "  make clean        - Remove build artifacts"
	@echo "  make install-deps - Install CivetWeb and dependencies (Fedora)"
	@echo "  make help         - Show this help message"
	@echo ""
	@echo "Usage:"
	@echo "  1. Install dependencies: make install-deps"
	@echo "  2. Build the server:     make"
	@echo "  3. Run the server:       ./server [port]"
