var Status = {
	"NO_CARD": 0,
	"INVALID_CARD": 1,
	"VALID_CARD": 2,
	"REQUEST_ERROR": 3,
	"INVALID_FORMAT": 4
};

var xhrRequest = function (url, type, data, onSuccess, onError) {
	var xhr = new XMLHttpRequest();
	xhr.onload = function () {
		onSuccess(this.responseText, this.status);
	};
	xhr.onerror = function () {
		onError();
	};
	xhr.open(type, url, true);
	if (type == "POST") {
		xhr.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
		xhr.setRequestHeader("Content-length", data.length);
		xhr.setRequestHeader("Connection", "close");
		xhr.send(data);
	} else {
		xhr.send();
	}
};

var getCard = function() {
	return localStorage.getItem('card') || "";
};

var getBalance = function() {
	var card = getCard();
	if (card) {
		xhrRequest("http://m.alelo.com.br/ios/ajax-se.do?cartao="+card+"&tipo=0", "GET", null, function(responseText, statusCode) {
			var responseJson = JSON.parse(responseText);
			if (typeof responseJson.Saldo == "object") {
				if (typeof responseJson.Saldo.valDisp != "undefined" && typeof responseJson.Saldo.valGastoSugerido != "undefined") {
					sendStatus(Status.VALID_CARD, responseJson.Saldo.valDisp.toString(), responseJson.Saldo.valGastoSugerido.toString());
				} else {
					sendStatus(Status.INVALID_FORMAT);
				}
			} else {
				sendStatus(Status.INVALID_CARD);
			}
		}, function() {
			sendStatus(Status.REQUEST_ERROR);
		});
	} else {
		sendStatus(Status.NO_CARD);
	}
};

var sendStatus = function(statusCode, balance, suggestion) {
	var dictionary = {
		'STATUS': statusCode
	};

	if (typeof balance != "undefined" && typeof suggestion != "undefined") {
		dictionary.BALANCE = balance.toString();
		dictionary.SUGGESTION = suggestion.toString();
	}

	// Send to Pebble
	Pebble.sendAppMessage(dictionary, function(e) {
		console.log('Result info sent to Pebble successfully!');
	}, function(e) {
		console.log('Error sending result info to Pebble!');
	});
};

Pebble.addEventListener("ready", function(e) {
	getBalance();
});

Pebble.addEventListener('showConfiguration', function(e) {
	var card = getCard();
	Pebble.openURL('https://htmlpreview.github.io?https://raw.githubusercontent.com/thiagosalles/meu_saldo_pebble/master/html/configuration/index.html?card='+card);
});

Pebble.addEventListener('webviewclosed', function(e) {
	var configuration = JSON.parse(decodeURIComponent(e.response));
	if (configuration.clear) {
		localStorage.clear();
	} else if (configuration.card) {
		localStorage.setItem('card', configuration.card.trim());
	}
	getBalance();
});
