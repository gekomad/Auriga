<?php

$uuid_perft=$_POST["uuid_perft"];
$uuid_task=$_POST["uuid_task"];
$heartbeat=$_POST["heartbeat"];
$tot=$_POST["tot"];
$engine=$_POST["engine"];
$author=$_POST["author"];
$fen=$_POST["fen"];
$minutes=$_POST["minutes"];
$depth=$_POST["depth"];

$ip = $_SERVER['REMOTE_ADDR'];
if (array_key_exists('HTTP_X_FORWARDED_FOR', $_SERVER)) {
    $ip = array_pop(explode(',', $_SERVER['HTTP_X_FORWARDED_FOR']));
}

preg_match('/^(\d{1,3}\.\d{1,3}\.\d{1,3})\.\d{1,3}\z/', $ip, $re);
$miniip = $re[1];

include 'mysql_connect.php';

$country="";
$sql = "select country_iso_code from ip_country_timezone where network ='$miniip'";
$result = $conn->query($sql);
if ($result->num_rows > 0) {
	$row = $result->fetch_assoc();
	$country=$row['country_iso_code'];
}

include 'updateStatistics.php';

if($heartbeat & 1 ){
	$sql = "delete from tasks where heartbeat & 1 and fen= '$fen' and depth=$depth and engine ='$engine' and author ='$author' ";
	$conn->query($sql);
}

//insert row in tasks
$sql = "INSERT INTO tasks (uuid_perft, uuid_task,heartbeat,tot,engine,author,fen,minutes,depth,country)VALUES ('$uuid_perft', '$uuid_task','$heartbeat','$tot','$engine','$author','$fen',$minutes,$depth,'$country')";
$conn->query($sql);

//update statistics in perft_tasks for uuid_task
$sql = getUpdateTaskSQL($uuid_task);
$conn->query($sql);

//update statistics in perft for uuid_perft
$sql = getUpdatePerftSQL($uuid_perft);
$conn->query($sql);

$conn->close();

?>
