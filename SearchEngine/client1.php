<?php

	$host = "127.0.0.1";
	$port = "8080";

	$fp = fsockopen ($host, $port, $errno, $errstr, 30);

	if (!$fp) {

		echo "$errstr ($errno)\r\n";

	}else {

		fputs ($fp, "Hi");
		$rsps = "";

		while ($rsps <> "END") {
			$rsps = fgets ($fp, 1024);
			echo $rsps;
		}

		fclose ($fp);
	}

?>