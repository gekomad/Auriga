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

	echo $db_host."a<br>";
	echo $db_user."b<br>";
	
	echo $db_name."d<br>";
	echo $db_port."e<br>";

	
	$sql = "INSERT INTO tasks (uuid_perft, uuid_task,heartbeat,tot,engine,author,fen,minutes,depth)VALUES ('$uuid_perft', '$uuid_task','$heartbeat','$tot','$engine','$author','$fen',$minutes,$depth)";
	$conn->query($sql);

$sql = "update perft_tasks pt ".
"join (select pt.uuid_perft, pt.uuid_task,  ".
"(select count(distinct engine)from tasks tt2 where tt2.uuid_task=t.uuid_task and tt2.uuid_perft='".$uuid_perft."' group by tt2.uuid_task,tt2.uuid_perft )n_engine,  ".
"ifnull(floor ((select count(distinct tt.fen) from tasks tt where tt.tot is not null and tt.uuid_task=t.uuid_task and tt.uuid_perft='".$uuid_perft."'".
"group by tt.uuid_perft,tt.uuid_task)/fens*100),0) perc_completed , ".
"max(t.creation_date)creation_date from perft_tasks pt  ".
"left join tasks t  ".
"on  t.uuid_task=pt.uuid_task  ".
"where pt.uuid_perft ='".$uuid_perft."'". 
"group by pt.uuid_perft,pt.uuid_task  ".
")t ".
"on t.uuid_perft=pt.uuid_perft and t.uuid_task=pt.uuid_task ".
"set pt.n_engine =t.n_engine, pt.perc_completed = t.perc_completed ,pt.creation_date= t.creation_date";
$conn->query($sql);
$conn->close();

?>
