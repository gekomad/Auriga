<?php


include 'mysql_connect.php';
include 'updateStatistics.php';



$sql = "SELECT uuid_perft,uuid_task FROM perft_tasks";
$result = $conn->query($sql);

if ($result->num_rows > 0) {
    while($row = $result->fetch_assoc()) {
		$uuid_perft=$row["uuid_perft"];
		$uuid_task=$row["uuid_task"];
		echo "$uuid_perft  $uuid_task\n";
		//update statistics in perft_tasks for uuid_task
		$sql2 = getUpdateTaskSQL($uuid_perft,$uuid_task);
		$conn->query($sql2);

		//update statistics in perft for uuid_perft
		$sql2 = getUpdatePerftSQL($uuid_perft);
		$conn->query($sql2);
		
	}
}


$conn->close();

?>
