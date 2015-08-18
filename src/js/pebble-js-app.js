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
			var responseJosn = JSON.parse(responseText);
			console.log("-----O QUE DEU?-----");
			console.log("responseText: " + responseText);
			console.log("statusCode: " + statusCode);
			sendStatus((statusCode == 200) ? 1 : 0);
		}, function() {
			sendStatus(3);
		});
	} else {
		sendStatus(4);
	}
};

var sendStatus = function(statusCode) {
	var dictionary = {
		'STATUS': statusCode
	};

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
	Pebble.openURL('https://htmlpreview.github.io?https://raw.githubusercontent.com/thiagosalles/meu_saldo_pebble/master/html/configuration/index.html');
});

Pebble.addEventListener('webviewclosed', function(e) {
	var configuration = JSON.parse(decodeURIComponent(e.response));
	if (configuration.clear) {
		localStorage.clear();
	} else if (configuration.card) {
		localStorage.setItem('card', configuration.card.trim());
	}
	console.log('Configuration window returned: ', JSON.stringify(configuration));
	getBalance();
});
