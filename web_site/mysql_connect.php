<?php

include 'set_variable.php';	

	$conn = new mysqli($db_host, $db_user, $db_pass, $db_name, $db_port);
	if ($conn->connect_error) {
    	 die('Connect Error (' . $conn->connect_errno . ') '
	            . $conn->connect_error);
	}


?>

