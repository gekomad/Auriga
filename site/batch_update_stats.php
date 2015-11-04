<?php


include 'mysql_connect.php';
include 'updateStatistics.php';

$uuid_perft="CG903DC1-CA9E-BFE7-A1D2-4D24C3G55472";
$uuid_task="013BF886-928G-AC19-G2G5-F6394D26FEB5";

//update statistics in perft_tasks for uuid_task
$sql = getUpdateTaskSQL($uuid_perft,$uuid_task);
$conn->query($sql);

//update statistics in perft for uuid_perft
$sql = getUpdatePerftSQL($uuid_perft);
$conn->query($sql);

$conn->close();

?>
