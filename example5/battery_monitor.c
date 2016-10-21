#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dbus/dbus.h>

// #include <dbus-message.h>

static void abort_on_error (DBusError *error);
static double get_double_property (DBusConnection *connection, const char *bus_name, 
									const char *path, const char *iface, 
									const char *proname, DBusError *error);

static DBusMessage *create_property_get_message(const char *bus_name, const char *path, 
												const char *iface, const char *proname);

static double extract_double_from_variant(DBusMessage *reply, DBusError *error);

int main (int argc, char *argv[]) {

	DBusConnection *connection = NULL;
	DBusError error;
	double energy = 0;
	double fullEnergy = 0;

	dbus_error_init(&error);
	connection = dbus_bus_get(DBUS_BUS_SYSTEM, &error);
	abort_on_error(&error);

	energy = get_double_property(connection, "org.freedesktop.UPower",
						"/org/freedesktop/UPower/devices/baterry_BAT1",
						"org.freedesktop.UPower.Device", 
						"Energy", &error);
	abort_on_error(&error);

	fullEnergy = get_double_property(connection, "org.freedesktop.UPower",
						"/org/freedesktop/UPower/devices/baterry_BAT1",
						"org.freedesktop.UPower.Device", 
						"EnergyFull", &error);
	abort_on_error(&error);

	printf("%1f\n", ((energy * 100) / fullEnergy) );
	
	return 0;
}

static void abort_on_error(DBusError *error) {
	if (dbus_error_is_set(error)) {
		fprintf(stderr, "%s", error->message);
		abort();
	}
}
static double get_double_property (DBusConnection *connection, const char *bus_name, 
										const char *path, const char *iface, 
										const char *propname, DBusError *error) {

	DBusError myError;
	double result = 0.0;

	DBusMessage *msgQuery = NULL;
	DBusMessage *msgReply = NULL;

	dbus_error_init(&myError);
	msgQuery = create_property_get_message(bus_name, path, iface, propname);
	msgReply = dbus_connection_send_with_reply_and_block(connection, msgQuery, 1000, &myError);

	dbus_message_unref(msgQuery);

	if (dbus_error_is_set(&myError)) {
		dbus_move_error(&myError, error);
		return 0;
	}

	result = extract_double_from_variant(msgReply, &myError);
	if (dbus_error_is_set(&myError)) {
		dbus_move_error(&myError, error);
		return 0;
	}

	dbus_message_unref(msgReply);

	return (result);
}

static DBusMessage *create_property_get_message(const char *bus_name, const char *path, 
												const char *iface, const char *propname) {

	DBusMessage *queryMsg = NULL;
	printf("\nbus_name = %s\npath = %s\niface = %s\npropname = %s\n\n", bus_name, path, iface, propname);
	queryMsg = dbus_message_new_method_call (bus_name, path, "org.freedesktop.DBus.Properties", "Get");

	dbus_message_append_args(queryMsg, DBUS_TYPE_STRING, &iface, 
							DBUS_TYPE_STRING, &propname, DBUS_TYPE_INVALID);

	return (queryMsg);
}

static double extract_double_from_variant(DBusMessage *reply, DBusError *error) {
	
	DBusMessageIter iter;
	DBusMessageIter sub;
	double result = 0.0;

	dbus_message_iter_init(reply, &iter);

	if (DBUS_TYPE_VARIANT != dbus_message_iter_get_arg_type(&iter)) {
		dbus_set_error_const(error, 
						"variant_should_be_double", 
						"This variant reply message must have double content");
		return 0;
	}

	dbus_message_iter_recurse(&iter, &sub);
}
