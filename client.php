<?php

echo "<h2>TCP/IP Connection</h2><br />";

/* Get the port for the WWW service. */
$service_port = 8080;

/* Get the IP address for the target host. */
$address = '127.0.0.1';

/* Create a TCP/IP socket. */
$socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);
if ($socket === false) {
    echo "socket_create() failed: reason: " . socket_strerror(socket_last_error());
} else {
    echo "OK.";
}

echo "Attempting to connect to '$address' on port '$service_port'...";
$result = socket_connect($socket, $address, $service_port);
if ($result === false) {
    echo "socket_connect() failed.<br />Reason: ($result) " . socket_strerror(socket_last_error($socket)) . "<br />";
} else {
    echo "OK.<br />";
}

if (isset($_POST['send-rs'])){
	$in = $_POST['research'];
}
$out = '';

echo "Sending request...";
socket_write($socket, $in, strlen($in));
echo "OK.<br />";

echo "Reading response:<br /><br />";
while ($out = socket_read($socket, 2048)){
    echo $out."<br /><br />";
    if ($out != ''){
    	break;
    }
}

echo "Closing socket...";
socket_close($socket);
echo "OK.<br /><br />";
?>