include config.mk

EXECUTABLE=opxflow
BIN_DIR=bin
SOURCES = src/main.c \
	  src/log/log.c \
	  src/server/server.c \
	  src/config/config.c \
	  src/backend/backend.c \
	  src/packets/packets.c \
	  src/packets/encoder/packets_encode_connack.c \
	  src/packets/encoder/packets_encode_pingresp.c \
	  src/packets/encoder/packets_encode_publish.c \
	  src/packets/encoder/packets_encode_suback.c \
	  src/packets/decoder/packets_decoder.c \
	  src/packets/decoder/packets_decode_connect.c \
	  src/packets/decoder/packets_decode_pingreq.c \
	  src/packets/decoder/packets_decode_publish.c \
	  src/packets/decoder/packets_decode_subscribe.c \

OBJECTS = $(patsubst %.c, %.o, $(SOURCES))

CFLAGS  = -I src \
		  -Wall -Wno-unused-but-set-variable -Wno-unused-variable \
		  -D VERSION="\"${VERSION}\""

LDFLAGS =
LIB_PATH =
LIBS =

all: $(EXECUTABLE)

$(EXECUTABLE):  $(OBJECTS)
	@ mkdir -p $(BIN_DIR)
	@ $(CC) $(CFLAGS) -o $(BIN_DIR)/$@ $(OBJECTS) $(LIB_PATH) $(LIBS)
	@ echo "LD " $@

%.o : %.c
	@ $(CC) $(CFLAGS) -c $< -o $@
	@ echo "CC " $<

.PHONY: clean
clean:
	@ rm -rf $(OBJECTS) $(EXECUTABLE) $(BIN_DIR)
	@ echo "RM " $(OBJECTS) $(EXECUTABLE) $(BIN_DIR)