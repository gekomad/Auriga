<?php include("setTimezone.php");?>
<html><head>
<meta charset="UTF-8">
<meta name="robots" content="noindex">
<title>Perft</title>
<link rel="stylesheet"  href="/css/css1.css" type="text/css"/>
<link rel="stylesheet" href="/css/layout.css" type="text/css" />

</head>
<body onload="checkCookie()">
<?php 
echo "<input type='hidden' value=\"".$time_zone."\">";
include_once("analyticstracking.php");?>
<?php include 'menu.php';?>

<?php
$uuid_perft=$_GET['uuid_perft'];
$from =$_GET["from"];
if($from == "")$from=0;
if($uuid_perft == ""){
	header("Location: 404.html");//TODO fare pagina
}

include 'mysql_connect.php';
$conn->query("SET time_zone = '{$time_zone}'");
$sql = "SELECT fen, depth,tasks, creation_date,tot,ifnull(perc_completed,0)perc_completed FROM perft where uuid_perft='".$uuid_perft."'";

$result = $conn->query($sql);
if ($result->num_rows == 0 ) {
	$conn->close();
	header("Location: 404.html");//TODO fare pagina
}
$row = $result->fetch_assoc();
$fen =$row["fen"];
$deph =$row["depth"];
$tasks =$row["tasks"];
$creation_date =$row["creation_date"];
$tot =$row["tot"];


$perc_completed =$row["perc_completed"];


?>
<section class="container">
          <hgroup><?php echo "<h1>Perft id ".$uuid_perft."</h1> ";?></hgroup>
        <div class="row">        
            <section>             
<?php   

echo "<div align='center'><img src='http://webchess.freehostia.com/diag/chessdiag.php?fen=".$fen."&amp&size=large&amp&coord=yes&amp&cap=no&amp&stm=yes&amp&fb=no&amp&theme=classic&amp&color1=lavander&amp&color2=635147&amp&color3=000000'  height='300' width='300'></div>";
echo "<br><br>Fen: $fen<br>";
echo "Depth: $deph<br>";
echo "Tasks: $tasks<br>";
echo "Last update: $creation_date<br>";
echo "Tot: $tot<br>";
echo "Completed: $perc_completed%<br>";
$sql="SELECT engine FROM perft_engines where uuid_perft='".$uuid_perft."'";
$result = $conn->query($sql);
if ($result->num_rows > 0) {
	echo "Engines: ";
	while($row = $result->fetch_assoc()) {		
		echo $row["engine"];
		if((++$count) != $result->num_rows)echo ", ";
	}
}
echo "<br><hr><br>";

$sql="select uuid_task,engine n_engine,perc_completed, creation_date,minutes,if(minutes=0,' < 1 min', if(minutes>=60*24,concat(round(minutes/60/24),' days'),if(minutes>=60,concat(round(minutes/60,' hours'),' hours'),concat(minutes,' min'))))time from perft_tasks where uuid_perft ='".$uuid_perft."' order by creation_date desc,perc_completed asc limit ".$from.",500";

$result = $conn->query($sql);
if ($result->num_rows > 0) {

	include("_command_area.php");
	echo "<button onclick='writeCommandRandomTask(\"$uuid_perft\")'>Generate command for random task</button><br><br>";


	echo "<table>";
		echo "<tr>";
		echo "<td><b>#</b></td>" ;
		echo "<td><b>Task ID</b></td>" ;
		echo "<td><b>Completed</b></td>" ;
		echo "<td><b>Last update</b></td>" ;
		echo "<td><b># Engine</b></td>" ;
		echo "<td><b>Time</b></td>" ;
	 	echo "</tr>";
		$count=$from;
    while($row = $result->fetch_assoc()) {		
		echo "<tr>";
		$count++;
		echo "<td>$count</td>";
		echo '<td><font size="1"><a href="task.php?uuid_task='.$row["uuid_task"].'&uuid_perft='.$uuid_perft.'">'.$row["uuid_task"].'</a></font></td>';
		$completed=$row["perc_completed"];
		if($completed=="")$completed="0";
		echo "<td>".$completed."%</td>";
		$creation_date=$row["creation_date"];
		echo "<td>$creation_date</td>";
		echo "<td>".$row["n_engine"]."</td>";
		$time=$row["time"];		
		echo "<td>".$time."</td>";
		echo "</tr>";
    }
	echo "</table>";
	if($count>500){
		echo "<div class='pagination'><a class='page' href='perft.php?uuid_perft=".$uuid_perft."'>first page</a>";
	}
	if($count<$tasks){
		echo"<a class='page'href='perft.php?uuid_perft=".$uuid_perft."&from=".($count)."'>next page</a>";
	}
		echo"</div>";
} else {
    echo "0 results";
}
$conn->close();


?> 

</section>

<?php include 'footer.html'; ?>    

</body>
</html>
