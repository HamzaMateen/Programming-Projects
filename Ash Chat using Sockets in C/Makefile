CC=gcc
CFLAGS=-Ilib/libbcrypt -Iutil -Wall
LDFLAGS=-Llib/libbcrypt -l:bcrypt.a
UTIL_OBJS=util/util.o
BUILD_DIR=./build

define make_build_dir
    @if [ ! -d "$(BUILD_DIR)" ]; then \
        echo "Creating build directory"; \
        mkdir -p $(BUILD_DIR); \
    fi
endef

all: client server

client: client.c $(UTIL_OBJS) | build_dir
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/client client.c $(UTIL_OBJS) $(LDFLAGS)

server: server.c $(UTIL_OBJS) | build_dir
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/server server.c $(UTIL_OBJS) $(LDFLAGS)

$(UTIL_OBJS): util/util.c util/util.h
	$(CC) $(CFLAGS) -c -o $@ $<

build_dir:
	$(call make_build_dir)

clean:
	@rm -rf $(BUILD_DIR) util/*.o
