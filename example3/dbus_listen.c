#include <stdio.h>
#include <dbus/dbus.h>
#include <dbus/dbus-glib.h>
#include <glib.h>

static DBusHandlerResult 
dbus_filter(DBusConnection *connection, DBusMessage *message, void *user_data) {

	if ( dbus_message_is_signal(message,"org.share.linux","Customize" ) ) {

		printf("Message customize received\n");
		return DBUS_HANDLER_RESULT_HANDLED;
	}

	if ( dbus_message_is_signal(message,"org.share.linux","Quit" ) ) {

		printf("Message quit received\n");
		GMainLoop *loop = (GMainLoop*) user_data;
		g_main_loop_quit(loop);
		return DBUS_HANDLER_RESULT_HANDLED;
	}

	return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
}

int main(int argc, char *argv[]) {

	DBusConnection *connection;
	DBusError error;

	/* glib main loop */
	GMainLoop *loop;

	loop = g_main_loop_new(NULL,FALSE);

	dbus_error_init(&error);
	connection = dbus_bus_get(DBUS_BUS_SESSION, &error);

	if ( dbus_error_is_set(&error) ) {

		printf("Error connecting to the daemon bus: %s",error.message);
		dbus_error_free(&error);
		return 1;
	}

	dbus_bus_add_match (connection, "type='signal',interface='org.share.linux'",NULL);
	dbus_connection_add_filter (connection, dbus_filter, loop, NULL);

	/* dbus-glib call */
	dbus_connection_setup_with_g_main(connection,NULL);

	/* run glib main loop */
	g_main_loop_run(loop);

	return 0;
}

