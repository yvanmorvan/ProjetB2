<?php

	$host = "127.0.0.1";
	$port = "8080";

	if (isset($_POST['rqst'])){
		echo $_POST['rqst'];
	}else{

		if (isset($_POST['send-rs'])){
			$rs = $_POST['research'];
		}else{
			$rs = "no reasearch";
		}

		// Socket instanciation...
		$client = stream_socket_client("tcp://$host:$port", $errno, $errorMessage);
		if ($client === false) {
		    throw new UnexpectedValueException("Failed to instanciate the socket : $errorMessage");
		}


		// Sending the request...
		fwrite($client, $rs);

		// And closing.
		fclose($client);
	}



?>