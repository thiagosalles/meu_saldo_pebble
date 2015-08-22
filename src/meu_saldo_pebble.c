#include <pebble.h>

#include "windows/splash.h"

enum AppMessageKeys {
	STATUS = 0,
	BALANCE = 1
};

static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
	APP_LOG(APP_LOG_LEVEL_INFO, "Message received!");

	// Store incoming information
	static int status;

	// Read first item
	Tuple *t = dict_read_first(iterator);

	// For all items
	while(t != NULL) {
		// Which key was received?
		switch(t->key) {
			case STATUS:
				status = t->value->int32;
				break;
			default:
				APP_LOG(APP_LOG_LEVEL_ERROR, "Key %d not recognized!", (int)t->key);
				break;
		}

		// Look for next item
		t = dict_read_next(iterator);
	}

	APP_LOG(APP_LOG_LEVEL_INFO, "Status: %d", status);
}

static void inbox_dropped_callback(AppMessageResult reason, void *context) {
	APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
	APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
	APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

static void register_callbacks() {
	// Register callbacks
	app_message_register_inbox_received(inbox_received_callback);
	app_message_register_inbox_dropped(inbox_dropped_callback);
	app_message_register_outbox_failed(outbox_failed_callback);
	app_message_register_outbox_sent(outbox_sent_callback);

	// Open AppMessage
	app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
}

int main(void) {
	register_callbacks();
	open_splash_window();
	app_event_loop();
	close_splash_window();
}
