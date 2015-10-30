<html><head>
<title>Perft</title>

</head>
<body>

<?php

$uuid_perft=$_GET['uuid_perft'];
if($uuid_perft == ""){
	echo "uuid_perft missing<br><br>";
	return;
}

include 'mysql_connect.php';

$sql = "SELECT fen, depth,tasks,creation_date,tot FROM perft where uuid_perft='".$uuid_perft."'";
$result = $conn->query($sql);


$row = $result->fetch_assoc();
$fen =$row["fen"];
$deph =$row["depth"];
$tasks =$row["tasks"];
$creation_date =$row["creation_date"];

$sql = "select pt.uuid_perft, pt.uuid_task, sum(not isnull(t.tot)) tot,sum(not isnull(t.partial_moves))partial from perft_tasks pt ". 
"left join tasks t ".
"on  t.uuid_task=pt.uuid_task ".
"where pt.uuid_perft ='".$uuid_perft."' ".
"group by pt.uuid_perft,pt.uuid_task ".
"order by 1,2,3 ";


$result = $conn->query($sql);
echo "Perft uuid: ".$uuid_perft."<br><br>";
echo "<img src='http://webchess.freehostia.com/diag/chessdiag.php?fen=".$fen."&amp&size=large&amp&coord=yes&amp&cap=no&amp&stm=yes&amp&fb=no&amp&theme=classic&amp&color1=E3CEAA&amp&color2=635147&amp&color3=000000'  height='300' width='300'>";
echo "<br><br>fen: $fen<br><br>";
echo "depth: $deph<br>";
echo "tasks: $tasks<br>";
echo "creation_date: $creation_date<br>";
echo "<br>";

if ($result->num_rows > 0) {
    while($row = $result->fetch_assoc()) {
		echo '<a href="task.php?uuid_task='.$row["uuid_task"].'&uuid_perft='.$uuid_perft.'">'.$row["uuid_task"].'</a><br>';
    }
} else {
    echo "0 results";
}

$conn->close();
?> 

</body>
</html>
