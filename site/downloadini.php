<?php 

header('Content-type: application/x-gzip');
include 'set_variable.php';	
$uuid_perft=$_GET['id'];
$uuid_task="";
if($uuid_perft==""){
	include 'mysql_connect.php';
	$sql="SELECT uuid_perft,uuid_task FROM perft_tasks where tot is null ORDER BY RAND() LIMIT 0,1";
	$result = $conn->query($sql);
	if ($result->num_rows > 0) {
		$row = $result->fetch_assoc();
		$uuid_perft=$row['uuid_perft'];
		$uuid_task=$row['uuid_task'];
	}	
	$conn->close();
}
if($uuid_perft==""){
	echo "X";
	exit(0);
}
$filename=$auriga_root."/data/".$uuid_perft."/".$uuid_perft.".ini.gz";
echo "XXX".$uuid_perft."XXX".$uuid_task."XXX".filesize($filename)."XXX".file_get_contents($filename);?>
