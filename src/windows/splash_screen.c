#include "splash_screen.h"

static Window *s_splash_screen_window;
static TextLayer *s_splash_screen_layer;

static void splash_screen_load(Window *window) {
	s_splash_screen_layer = text_layer_create(GRect(0, 65, 144, 40)); // GRect(x, y, width, high). Full Resolution is 144x168
	text_layer_set_background_color(s_splash_screen_layer, GColorBlack);
	text_layer_set_text_color(s_splash_screen_layer, GColorWhite);
	text_layer_set_font(s_splash_screen_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
	text_layer_set_text_alignment(s_splash_screen_layer, GTextAlignmentCenter);
	text_layer_set_text(s_splash_screen_layer, "Meu Saldo");
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_splash_screen_layer));
}

static void splash_screen_unload(Window *window) {
	text_layer_destroy(s_splash_screen_layer);
}

void open_splash_screen() {
	s_splash_screen_window = window_create();
	window_set_background_color(s_splash_screen_window, GColorBlack);
	#ifdef PBL_SDK_2
	window_set_fullscreen(s_splash_screen_window, true);
	#endif
	window_set_window_handlers(s_splash_screen_window, (WindowHandlers) {
		.load = splash_screen_load,
		.unload = splash_screen_unload
	});
	window_stack_push(s_splash_screen_window, true);
}

void close_splash_screen() {
	window_destroy(s_splash_screen_window);
}
