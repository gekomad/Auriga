<?php
error_log ("inizio lettura",0);
$uuid_perft=$_POST["uuid_perft"];
$uuid_task=$_POST["uuid_task"];
$partial_moves=$_POST["partial_moves"];
$tot=$_POST["tot"];
$engine=$_POST["engine"];
$author=$_POST["author"];
$fen=$_POST["fen"];
$hours=$_POST["hours"];
$depth=$_POST["depth"];

	include 'mysql_connect.php';

	echo $db_host."a<br>";
	echo $db_user."b<br>";
	
	echo $db_name."d<br>";
	echo $db_port."e<br>";

	
	$query = "INSERT INTO tasks (uuid_perft, uuid_task,partial_moves,tot,engine,author,fen,hours,depth)VALUES ('$uuid_perft', '$uuid_task','$partial_moves','$tot','$engine','$author','$fen',$hours,$depth)";
    error_log ( $query,0);


	if ($conn->query($query) === TRUE) {

	     error_log ("New record created successfully",0);		
	} else {
	    error_log ( "Error: " . $query . "\n" . $conn->error);
	}
	

	$conn->close();
    echo "<br>----end mysql---------";


?>
