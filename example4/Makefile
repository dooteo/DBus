CC=gcc
USERID=$(shell id -u)

.PHONY: all clean showhelp install uninstall help

all: clean start_service just-full-moon showhelp

start_service: start-service.c
	$(CC) -o $@ $< `pkg-config --libs --cflags dbus-1`

just-full-moon: just-full-moon.c
	$(CC) -o $@ $< `pkg-config --libs --cflags dbus-1`

clean:
	rm -f start_service just-full-moon

# Use '@' prefix to not to print the command being executed
showhelp:
	@echo "\n\nCopy just.full.moon.service file into"
	@echo "/usr/share/dbus-1/service/ directory as root"
	@echo "Once this test is used, don't forget to remove it from /usr/share/dbus-1/service/\n\n"

install:
ifeq ($(USERID), 0)
	cp just.full.moon.service /usr/share/dbus-1/services/
else
	@echo "You are not root. Nothing to do."
endif


uninstall:
ifeq ($(USERID), 0)
	rm /usr/share/dbus-1/services/just.full.moon.service
else
	@echo "You are not root. Nothing to do."
endif


help:
	@echo "\nPlease, modify 'just.full.moon.service' file\nto adjust to your path"
