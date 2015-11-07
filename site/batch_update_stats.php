<?php


include 'mysql_connect.php';
include 'updateStatistics.php';



$sql = "SELECT distinct uuid_task FROM perft_tasks";
$result = $conn->query($sql);

if ($result->num_rows > 0) {
    while($row = $result->fetch_assoc()) {
		$uuid_task=$row["uuid_task"];
		echo "uuid_task:  $uuid_task\n";
		//update statistics in perft_tasks for uuid_task
		$sql2 = getUpdateTaskSQL($uuid_task);
		$conn->query($sql2);		
	}
}

$sql = "SELECT distinct uuid_perft FROM perft_tasks";
$result = $conn->query($sql);

if ($result->num_rows > 0) {
    while($row = $result->fetch_assoc()) {
		$uuid_perft=$row["uuid_perft"];
		echo "uuid_perft: $uuid_perft\n";	

		//update statistics in perft for uuid_perft
		$sql = getUpdatePerftSQL($uuid_perft);
		$conn->query($sql);		
	}
}


$conn->close();

?>
