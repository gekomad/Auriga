<?php include("setTimezone.php");?>
<html><head>
<meta charset="UTF-8">
<meta name="robots" content="noindex">
<title>Task</title>
<link rel="stylesheet"  href="css/css1.css" type="text/css"/>
<link rel="stylesheet" href="css/layout.css" type="text/css" />
</head>
<body onload="checkCookie()">
<?php include_once("analyticstracking.php");?>
<?php
echo "<input type='hidden' value=\"".$time_zone."\">";

include 'menu.php';?>
 
<?php

$uuid_task=$_GET['uuid_task'];
$uuid_perft=$_GET['uuid_perft'];
if($uuid_task == ""){
	header("Location: 404.html");//TODO fare pagina
}


include 'mysql_connect.php';
$conn->query("SET time_zone = '{$time_zone}'");
if($uuid_perft == ""){
	$sql="SELECT uuid_perft FROM perft_tasks WHERE uuid_task ='".$uuid_task."'";
	$result = $conn->query($sql);
	if ($result->num_rows > 0) {
		$row = $result->fetch_assoc();
		$uuid_perft=$row['uuid_perft'];
	}else{
		$conn->close();
		header("Location: 404.html");//TODO fare pagina
	}	
	if($uuid_perft == ""){
		$conn->close();
		header("Location: 404.html");//TODO fare pagina
	}
}

$sql ="select f.fen,f.depth,creation_date,heartbeat,tot,engine,author,if(minutes=0,' < 1 min', if(minutes>=60*24,concat(round(minutes/60/24),' days'),if(minutes>=60,concat(round(minutes/60,' hours'),' hours'),concat(minutes,' min'))))time,country".
" from task_fens f left join tasks pt ".
" on pt.uuid_task=f.uuid_task and pt.fen=f.fen ".
" where f.uuid_task ='".$uuid_task."' order by fen,ifnull(tot,0) asc,fen, creation_date desc";

$result = $conn->query($sql);

if ($result->num_rows == 0) {
	$conn->close();	
	header("Location: 404.html");//TODO fare pagina
}

?>
<section class="container">
          <hgroup>

<?php 
  echo "<h1><br>Task id ".$uuid_task."</h1> ";
	echo "<h2><br>Perft id <a href='perft.php?uuid_perft=$uuid_perft'>$uuid_perft</a></h2><br><br>";
	?>
          </hgroup><div class="row"><section>
			
              
<?php include("_command_area.php");?>
<?php echo "<button onclick='writeCommand(\"$uuid_perft\",\"$uuid_task\")'>Generate command</button>";?>
<?php 
	define("HEARTBEAT_TYPE", 0b1);
	define("OS_WIN", 0b10);
	define("OS_APPLE", 0b100);
	define("OS_LINUX", 0b1000);
	define("OS_ARM", 0b10000);
	define("OS_UNIX", 0b100000);

	function getOStype($heartbeat){

		if($heartbeat & OS_WIN)return "WIN";
		if($heartbeat & OS_APPLE)return "APPLE";
		if($heartbeat & OS_LINUX)return "LINUX";
		if($heartbeat & OS_ARM)return "ARM";
		if($heartbeat & OS_UNIX)return "UNIX";
		return "none";
	}


	echo "<table width='75%' border='1' align='center' bgcolor='#11FFff'>";
	echo "<tr>";
	echo "<td><b>#</td></b>" ;
	echo "<td><b>Fen</td></b>" ;
	echo "<td><b>Depth</td></b>" ;
	echo "<td><b>Date</td></b>" ;
	echo "<td><b>Tot</td></b>" ;
	echo "<td><b>Engine</td></b>" ;
	echo "<td><b>Author</td></b>" ;
	echo "<td><b>Time</td></b>" ;
    echo "<td><b>Country</td></b>" ;
	echo "<td><b>OS</td></b>" ;
 	echo "</tr>";
	$lastFen="";
	$color=0;
    while($row = $result->fetch_assoc()) {
		$heartbeat=	$row["heartbeat"];
		echo "<tr>";	
		$fen=$row["fen"];
		if($fen!=$lastFen){$color++;$lastFen=$fen;}
		echo "<td>".$color."</td>" ;
		echo "<td><small>".$fen."</small></td>" ;
		echo "<td>".$row["depth"] ."</td>" ;
		echo "<td>".$row["creation_date"]."</td>" ;
		$tot=$row["tot"];
		if($heartbeat & HEARTBEAT_TYPE)$tot="<i>heartbeat</i>";
		echo "<td>".$tot."</td>" ;
		echo "<td>".$row["engine"] ."</td>" ;
		echo "<td>".$row["author"] ."</td>" ;
		
		$time=$row["time"];
		if($time=="0 min")$time="<1 min";
		echo "<td>".$row["time"]."</td>";		
		echo "<td> <img src='img/flags/".$row["country"].".png'> </td>";
		$OS=getOStype($heartbeat);
		echo "<td> <img src='img/os/".$OS.".png' height='30' width='30' alt='".$OS."' title='".$OS."'> </td>";		
	 	echo "</tr>";
	}
	echo "</table>";


$conn->close();
?> 

  </section>         
        </div>
</section>
<?php include 'footer.html'; ?>   
</body>
</html>
