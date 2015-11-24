<?php 

header('Content-Transfer-Encoding: binary');
header('Content-Type: application/octet-stream');

header('Expires: 0');
header('Cache-Control: must-revalidate, post-check=0, pre-check=0');
header('Pragma: public');
include 'set_variable.php';	
$uuid_perft=$_GET['uuid_perft'];
$uuid_task=$_GET['uuid_task'];
if($uuid_perft=="?" || $uuid_task=="?"){
	include 'mysql_connect.php';
	//get random task
	$sql="SELECT pt.uuid_perft, pt.uuid_task
	FROM perft_tasks pt
	left join tasks t
	on pt.uuid_task=t.uuid_task
	where  ifnull(pt.perc_completed,-1)!=100"; 
	if($uuid_perft!="?")$sql.=" and pt.uuid_perft='".$uuid_perft."' ";
	$sql.=" ORDER BY ifnull(pt.creation_date ,0) asc,ifnull(pt.perc_completed,-1) asc,ifnull(heartbeat,-1) asc, RAND() LIMIT 0,1";

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
//error_log($filename);
$data = file_get_contents($filename);

header("Content-Length: ".filesize($filename));
header("uuid_perft: ".$uuid_perft);
header("uuid_task: ".$uuid_task);
echo $data;

