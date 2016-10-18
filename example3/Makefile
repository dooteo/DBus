CC=gcc
CFLAGS=`pkg-config --libs --cflags dbus-1`
CFLAGS2=`pkg-config --libs --cflags dbus-1 glib-2.0 dbus-glib-1`

.PHONY: all clean

all: dbus_reserve_name dbus_listen dbus_sent_to_listen

dbus_reserve_name: dbus_reserve_name.c
	$(CC) -o $@ $< $(CFLAGS)

dbus_listen: dbus_listen.c
	$(CC) -o $@ $< $(CFLAGS2)

dbus_sent_to_listen: dbus_sent_to_listen.c
	$(CC) -o $@ $< $(CFLAGS)

clean:
	rm -f dbus_reserve_name dbus_listen dbus_sent_to_listen

help:
	echo "These codes fetched from http://linoxide.com/how-tos/d-bus-ipc-mechanism-linux/"
