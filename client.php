<?php

	$host = "127.0.0.1";
	$port = "8080";

	/*if (isset($_POST['send-rs'])){
		$rs = $_POST['research'];*/
		$rs = "test";

		$socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);
		if ($socket === false) {
		    echo "socket_create() a échoué : raison :  " . socket_strerror(socket_last_error()) . "\n";
		} else {
		    echo "OK.\n";
		}

		echo "Essai de connexion à '$host' sur le port '$port'...";
		$result = socket_connect($socket, $host, $port);
		if ($socket === false) {
		    echo "socket_connect() a échoué : raison : ($result) " . socket_strerror(socket_last_error($socket)) . "\n";
		} else {
		    echo "OK.\n";
		}


		echo "Envoi de la requête HTTP HEAD...";
		socket_write($socket, $rs, strlen($rs));
		echo "OK.\n";

		echo "Lire la réponse : \n\n";
		while ($out = socket_read($socket, 512, PHP_NORMAL_READ)) {
		    echo $out;
		}

		echo "Fermeture du socket...";
		socket_close($socket);
		echo "OK.\n\n";

	//}

?>