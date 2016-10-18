CC=gcc
CFLAGS=`pkg-config --libs --cflags dbus-1`

.PHONY: all clean 
TARGET_PROG= dbus_get_machine_id

all: $(TARGET_PROG)

$(TARGET_PROG): dbus_get_machine_id.c
	$(CC) -o $@ $< $(CFLAGS)

clean:
	rm -f $(TARGET_PROG)
