#include "card_info.h"

static Window *s_card_info_window;
static TextLayer *s_card_info_balance_title_layer;
static TextLayer *s_card_info_balance_text_layer;
static TextLayer *s_card_info_suggestion_title_layer;
static TextLayer *s_card_info_suggestion_text_layer;
static const char *s_card_info_balance;
static const char *s_card_info_suggestion;
static const char prefix[] = "R$ ";
static char *l_balance;
static char *l_suggestion;

static void card_info_window_load(Window *window) {

	s_card_info_balance_title_layer = text_layer_create(GRect(0, 10, 144, 35)); // GRect(x, y, width, high). Full Resolution is 144x168
	text_layer_set_font(s_card_info_balance_title_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
	text_layer_set_text_alignment(s_card_info_balance_title_layer, GTextAlignmentCenter);
	text_layer_set_text(s_card_info_balance_title_layer, "Saldo");
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_card_info_balance_title_layer));

	s_card_info_balance_text_layer = text_layer_create(GRect(0, 45, 144, 50)); // GRect(x, y, width, high). Full Resolution is 144x168
	text_layer_set_font(s_card_info_balance_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28));
	text_layer_set_text_alignment(s_card_info_balance_text_layer, GTextAlignmentCenter);
	l_balance = malloc(strlen(prefix) + strlen(s_card_info_balance) + 1);
	strcpy(l_balance, prefix);
	strcat(l_balance, s_card_info_balance);
	text_layer_set_text(s_card_info_balance_text_layer, l_balance);
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_card_info_balance_text_layer));

	s_card_info_suggestion_title_layer = text_layer_create(GRect(0, 80, 144, 35)); // GRect(x, y, width, high). Full Resolution is 144x168
	text_layer_set_font(s_card_info_suggestion_title_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
	text_layer_set_text_alignment(s_card_info_suggestion_title_layer, GTextAlignmentCenter);
	text_layer_set_text(s_card_info_suggestion_title_layer, "Sugestao");
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_card_info_suggestion_title_layer));

	s_card_info_suggestion_text_layer = text_layer_create(GRect(0, 115, 144, 40)); // GRect(x, y, width, high). Full Resolution is 144x168
	text_layer_set_font(s_card_info_suggestion_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28));
	text_layer_set_text_alignment(s_card_info_suggestion_text_layer, GTextAlignmentCenter);
	l_suggestion = malloc(strlen(prefix) + strlen(s_card_info_suggestion) + 1);
	strcpy(l_suggestion, prefix);
	strcat(l_suggestion, s_card_info_suggestion);
	text_layer_set_text(s_card_info_suggestion_text_layer, l_suggestion);
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_card_info_suggestion_text_layer));
}

static void card_info_window_unload(Window *window) {
	text_layer_destroy(s_card_info_balance_title_layer);
	text_layer_destroy(s_card_info_balance_text_layer);
	text_layer_destroy(s_card_info_suggestion_title_layer);
	text_layer_destroy(s_card_info_suggestion_text_layer);
	free(l_balance);
	free(l_suggestion);
}

void create_card_info_window() {
	s_card_info_window = window_create();
	window_set_background_color(s_card_info_window, GColorWhite);
	#ifdef PBL_SDK_2
	window_set_fullscreen(s_card_info_window, true);
	#endif
	window_set_window_handlers(s_card_info_window, (WindowHandlers) {
		.load = card_info_window_load,
		.unload = card_info_window_unload
	});
}

void destroy_card_info_window() {
	window_destroy(s_card_info_window);
}

void show_card_info_window(const char *balance, const char *suggestion) {
	s_card_info_balance = balance;
	s_card_info_suggestion = suggestion;
	hide_card_info_window();
	window_stack_push(s_card_info_window, true);
}

void hide_card_info_window() {
	if (window_stack_contains_window(s_card_info_window)) {
		window_stack_remove(s_card_info_window, false);
	}
}
