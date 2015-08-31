#include <pebble.h>

#include "windows/splash.h"
#include "windows/message.h"
#include "windows/card_info.h"

enum AppMessageKeys {
	STATUS = 0,
	BALANCE = 1,
	SUGGESTION = 2
};

enum Status {
	NO_CARD = 0,
	INVALID_CARD = 1,
	VALID_CARD = 2,
	REQUEST_ERROR = 3,
	INVALID_FORMAT = 4
};

const char* StatusTitle[] = {
	"Sem Cartao",
	"Cartao Invalido",
	"Cartao Valido",
	"Erro na Consulta",
	"Formato Invalido"
};

const char* StatusDescription[] = {
	"Voce nao possui um cartao cadastrado.\nAcesse as configuracoes do Meu Saldo atraves do aplicativo Pebble em seu smartphone.",
	"O numero do cartao informado e invalido.\nPor favor, verifique o mesmo e tente novamente.",
	"O cartao informado e valido.\nSe estiver visualizando esta mensagem, entre em contato.",
	"Nao foi possivel efetuar a consulta do saldo do seu cartao.\nTente novamente.",
	"Nao foi possivel efetuar a consulta do saldo do seu cartao.\nEntre em contato."
};

static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
	APP_LOG(APP_LOG_LEVEL_INFO, "Message received!");

	// Store incoming information
	static int status;
	char *balance = "";
	char *suggestion = "";

	// Read first item
	Tuple *t = dict_read_first(iterator);

	// For all items
	while(t != NULL) {
		// Which key was received?
		switch(t->key) {
			case STATUS:
				status = t->value->int32;
				break;
			case BALANCE:
				balance = t->value->cstring;
				break;
			case SUGGESTION:
				suggestion = t->value->cstring;
				break;
			default:
				APP_LOG(APP_LOG_LEVEL_ERROR, "Key %d not recognized!", (int)t->key);
				break;
		}

		// Look for next item
		t = dict_read_next(iterator);
	}

	APP_LOG(APP_LOG_LEVEL_INFO, "Status: %s; Balance: %s; Suggestion: %s", StatusTitle[status], balance, suggestion);
	if (status == VALID_CARD) {
		hide_message_window();
		show_card_info_window(balance, suggestion);
	} else {
		hide_card_info_window();
		show_message_window(StatusTitle[status], StatusDescription[status]);
	}

	hide_splash_window();
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

static void init() {
	register_callbacks();
	create_splash_window();
	create_message_window();
	create_card_info_window();
	show_splash_window();
}

static void deinit() {
	destroy_splash_window();
	destroy_message_window();
	destroy_card_info_window();
}

int main(void) {
	init();
	app_event_loop();
	deinit();
}
