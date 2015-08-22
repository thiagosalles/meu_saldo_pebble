#include "message.h"

static Window *s_message_window;
static TextLayer *s_message_layer;
static const char *s_message_text;

static void message_window_load(Window *window) {
	s_message_layer = text_layer_create(GRect(0, 65, 144, 40)); // GRect(x, y, width, high). Full Resolution is 144x168
	text_layer_set_background_color(s_message_layer, GColorBlack);
	text_layer_set_text_color(s_message_layer, GColorWhite);
	text_layer_set_font(s_message_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
	text_layer_set_text_alignment(s_message_layer, GTextAlignmentCenter);
	text_layer_set_text(s_message_layer, s_message_text);
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_message_layer));
}

static void message_window_unload(Window *window) {
	text_layer_destroy(s_message_layer);
}

void create_message_window() {
	s_message_window = window_create();
	window_set_background_color(s_message_window, GColorBlack);
	#ifdef PBL_SDK_2
	window_set_fullscreen(s_message_window, true);
	#endif
	window_set_window_handlers(s_message_window, (WindowHandlers) {
		.load = message_window_load,
		.unload = message_window_unload
	});
}

void destroy_message_window() {
	window_destroy(s_message_window);
}

void show_message_window(const char *message) {
	s_message_text = message;
	hide_message_window();
	window_stack_push(s_message_window, true);
}

void hide_message_window() {
	if (window_stack_contains_window(s_message_window)) {
		window_stack_remove(s_message_window, false);
	}
}
