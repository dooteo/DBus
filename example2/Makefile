CC=gcc
CFLAGS=`pkg-config --libs --cflags dbus-1`

.PHONY: all clean 
TARGET_PROG= to_analice_with_dfeet

all: $(TARGET_PROG)

$(TARGET_PROG): $(TARGET_PROG).c
	$(CC) -o $@ $< $(CFLAGS)

clean:
	rm -f $(TARGET_PROG)
