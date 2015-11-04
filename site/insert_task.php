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

include 'mysql_connect.php';
include 'updateTaskSQL.php';

//insert row in tasks
$sql = "INSERT INTO tasks (uuid_perft, uuid_task,heartbeat,tot,engine,author,fen,minutes,depth)VALUES ('$uuid_perft', '$uuid_task','$heartbeat','$tot','$engine','$author','$fen',$minutes,$depth)";
$conn->query($sql);

//update statistics in perft_tasks for uuid_task
$sql = getUpdateTaskSQL($uuid_perft,$uuid_task);
$conn->query($sql);

//update statistics in perft for uuid_perft
$sql = getUpdatePerftSQL($uuid_perft);
$conn->query($sql);

$conn->close();

?>
