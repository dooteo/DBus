/* 
	Description: this program send a msg to system and session bus
		and prints responses on std output.

	Developed to learn how to works with DBus (Desktop Bus).

	Developer: Iñaki Larrañaga Murgoitio <dooteo@zundan.com>, 2016.
	License: GPL v3 or newer.

	Based on https://leonardoce.wordpress.com/2015/03/13/dbus-tutorial-part-2/
	DBus API: https://dbus.freedesktop.org/doc/api/html/

	Note: install and use "d-feet" to check DBus object path, interfaces 
	      and methods beside other things...
*/

#include <dbus/dbus.h>
#include <stdio.h>
#include <stdlib.h>

static void check_and_abort(DBusError *error);

int main (int argc, char *argv[]) {

	DBusConnection *connection = NULL;
	DBusError error;
	DBusMessage *msgQuery = NULL;
	DBusMessage *msgReply = NULL;

	const char *interfaceName = NULL;	
	const char *versionValue = NULL;	

	dbus_error_init(&error);
	// Connect to Session bus (yes, it's an obvious comment :P)
	connection = dbus_bus_get(DBUS_BUS_SESSION, &error);
	check_and_abort(&error);

	// Spoted in System Bus
	interfaceName = "org.freedesktop.DBus.Peer";

	/* Create a new message to send it to the system bus

	   Note: Next data were gotten from d-feet at SESSION bus
	*/

	msgQuery = dbus_message_new_method_call(
						"org.freedesktop.FileManager1",
						"/org/freedesktop/FileManager1",
						interfaceName,
						"GetMachineId");
	/* 
	   Once message is created, msg is sent on the bus and wait 
	   for other app response for two sec (2000). function returns the reply:
	*/
	msgReply = dbus_connection_send_with_reply_and_block(connection, msgQuery, 2000, &error);

	// Free msgQuery
	dbus_message_unref(msgQuery);

	dbus_message_get_args(msgReply, &error, DBUS_TYPE_STRING, &versionValue, DBUS_TYPE_INVALID);
	printf("Machine ID is: %s\n", versionValue);

	dbus_message_unref(msgReply);

	return 0;
}

static void check_and_abort(DBusError *error) {

	if (! dbus_error_is_set(error)) {
		return;
	}

	puts(error->message);
	abort();
}
