#include "splash.h"

static Window *s_splash_window;
static TextLayer *s_splash_layer;

static void splash_window_load(Window *window) {
	s_splash_layer = text_layer_create(GRect(0, 65, 144, 40)); // GRect(x, y, width, high). Full Resolution is 144x168
	text_layer_set_background_color(s_splash_layer, GColorBlack);
	text_layer_set_text_color(s_splash_layer, GColorWhite);
	text_layer_set_font(s_splash_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
	text_layer_set_text_alignment(s_splash_layer, GTextAlignmentCenter);
	text_layer_set_text(s_splash_layer, "Meu Saldo");
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_splash_layer));
}

static void splash_window_unload(Window *window) {
	text_layer_destroy(s_splash_layer);
}

void open_splash_window() {
	s_splash_window = window_create();
	window_set_background_color(s_splash_window, GColorBlack);
	#ifdef PBL_SDK_2
	window_set_fullscreen(s_splash_window, true);
	#endif
	window_set_window_handlers(s_splash_window, (WindowHandlers) {
		.load = splash_window_load,
		.unload = splash_window_unload
	});
	window_stack_push(s_splash_window, true);
}

void close_splash_window() {
	window_destroy(s_splash_window);
}
