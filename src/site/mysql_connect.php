<?php

/*$db_host = $_ENV['OPENSHIFT_MYSQL_DB_HOST'];
	$db_user = $_ENV['OPENSHIFT_MYSQL_DB_USERNAME'];
	$db_pass = $_ENV['OPENSHIFT_MYSQL_DB_PASSWORD'];
	$db_name = $_ENV['OPENSHIFT_APP_NAME'];
	$db_port = $_ENV['OPENSHIFT_MYSQL_DB_PORT'];*/
	 
	$db_host = "127.0.0.1";
	$db_user = "root";
	$db_pass = "root";
	$db_name = "auriga";
	$db_port = 3306;	

	$conn = new mysqli($db_host, $db_user, $db_pass, $db_name, $db_port);
	if ($conn->connect_error) {
    	 die('Connect Error (' . $conn->connect_errno . ') '
	            . $conn->connect_error);
	}


?>

