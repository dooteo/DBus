/* 
	https://leonardoce.wordpress.com/2015/03/17/dbus-tutorial-part-3/
*/

#include <dbus/dbus.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void check_and_abort(DBusError *error);
static DBusHandlerResult tutorial_messages(DBusConnection *connection, DBusMessage *message, void *user_data);
static void respond_to_introspect(DBusConnection *connection, DBusMessage *request);
static void respond_to_sum(DBusConnection *connection, DBusMessage *request);

int main() {
	DBusConnection *connection;
	DBusError error;
	DBusObjectPathVTable vtable;

	dbus_error_init(&error);
	connection = dbus_bus_get(DBUS_BUS_SESSION, &error);
	check_and_abort(&error);

	dbus_bus_request_name(connection, "org.wayoutwest.house.DBusTutorial", 0, &error);
	check_and_abort(&error);

	vtable.message_function = tutorial_messages;
	vtable.unregister_function = NULL;

	dbus_connection_try_register_object_path(connection,
		                "/org/wayoutwest/house/DBusTutorial",
		                &vtable,
		                NULL,
		                &error);
	check_and_abort(&error);

	printf("Use d-feet tool to analize this service.\nLook at Session bus for: org.wayoutwest.house\nContains 2 interfaces.\n");

	while(1) {
	   dbus_connection_read_write_dispatch(connection, 1000);
	}

	return 0;
}

static void check_and_abort(DBusError *error) {

   if (dbus_error_is_set(error)) {
       puts(error->message);
       abort();
   }
}

static DBusHandlerResult tutorial_messages(DBusConnection *connection, DBusMessage *message, void *user_data) {

   const char *interface_name = dbus_message_get_interface(message);
   const char *member_name = dbus_message_get_member(message);
    
   if (0==strcmp("org.freedesktop.DBus.Introspectable", interface_name) &&
       0==strcmp("Introspect", member_name)) {

       respond_to_introspect(connection, message);
       return DBUS_HANDLER_RESULT_HANDLED;
   } else if (0==strcmp("it.interfree.leonardoce.DBusTutorial", interface_name) &&
          0==strcmp("Sum", member_name)) {
        
       respond_to_sum(connection, message);
       return DBUS_HANDLER_RESULT_HANDLED;
   } else {
       return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
   }
}

static void respond_to_introspect(DBusConnection *connection, DBusMessage *request) {
   DBusMessage *reply;

   const char *introspection_data =
       " <!DOCTYPE node PUBLIC \"-//freedesktop//DTD D-BUS Object Introspection 1.0//EN\" "
       "\"http://www.freedesktop.org/standards/dbus/1.0/introspect.dtd\">"
       " <!-- dbus-sharp 0.8.1 -->"
       " <node>"
       "   <interface name=\"org.freedesktop.DBus.Introspectable\">"
       "     <method name=\"Introspect\">"
       "       <arg name=\"data\" direction=\"out\" type=\"s\" />"
       "     </method>"
       "   </interface>"
       "   <interface name=\"it.interfree.leonardoce.DBusTutorial\">"
       "     <method name=\"Sum\">"
       "       <arg name=\"a\" direction=\"in\" type=\"i\" />"
       "       <arg name=\"b\" direction=\"in\" type=\"i\" />"
       "       <arg name=\"ret\" direction=\"out\" type=\"i\" />"
       "     </method>"
       "   </interface>"
       " </node>";
    
   reply = dbus_message_new_method_return(request);
   dbus_message_append_args(reply,
                DBUS_TYPE_STRING, &introspection_data,
                DBUS_TYPE_INVALID);
   dbus_connection_send(connection, reply, NULL);
   dbus_message_unref(reply);
}

static void respond_to_sum(DBusConnection *connection, DBusMessage *request) {
   DBusMessage *reply;
   DBusError error;
   int a=0, b=0, ret=0;

   dbus_error_init(&error);

   dbus_message_get_args(request, &error,
                 DBUS_TYPE_INT32, &a,
                 DBUS_TYPE_INT32, &b,
                 DBUS_TYPE_INVALID);
   if (dbus_error_is_set(&error)) {
       reply = dbus_message_new_error(request, "wrong_arguments", "Illegal arguments to Sum");
       dbus_connection_send(connection, reply, NULL);
       dbus_message_unref(reply);
       return;
   }

   ret = a+b;
    
   reply = dbus_message_new_method_return(request);
   dbus_message_append_args(reply,
                DBUS_TYPE_INT32, &ret,
                DBUS_TYPE_INVALID);
   dbus_connection_send(connection, reply, NULL);
   dbus_message_unref(reply);
}


