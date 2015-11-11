<?php 

header('Content-type: application/x-gzip');
include 'set_variable.php';	
$uuid_perft=$_GET['id'];
$uuid_task="";
if($uuid_perft==""){
	include 'mysql_connect.php';
	//get random task
	$sql="SELECT pt.uuid_perft, pt.uuid_task
	FROM perft_tasks pt
	left join tasks t
	on pt.uuid_task=t.uuid_task
	where  ifnull(pt.perc_completed,-1)!=100 
	ORDER BY ifnull(pt.creation_date ,0) asc,ifnull(pt.perc_completed,-1) asc,ifnull(heartbeat,-1) asc, RAND() LIMIT 0,1";

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
error_log($filename);
$data = file_get_contents($filename);
$base64 = base64_encode($data);
echo "XXX".$uuid_perft."XXX".$uuid_task."XXX".strlen($base64)."XXXSTART".$base64;?>
