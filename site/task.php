<html><head>
<meta name="robots" content="noindex">
<title>Task</title>
<link rel="stylesheet"  href="css/css1.css" type="text/css"/>
<link rel="stylesheet" href="css/layout.css" type="text/css" />
</head>
<body onload="checkCookie()">
<?php include("setTimezone.php");?>
<?php include_once("analyticstracking.php"); ?>
<?php include 'menu.php';?>
 
<?php

$uuid_task=$_GET['uuid_task'];
$uuid_perft=$_GET['uuid_perft'];
if($uuid_task == ""){
	echo "uuid missing<br><br>";
	return;
}


include 'mysql_connect.php';

if($uuid_perft == ""){
	$sql="SELECT uuid_perft FROM perft_tasks WHERE uuid_task ='".$uuid_task."'";
	$result = $conn->query($sql);
	if ($result->num_rows > 0) {
		$row = $result->fetch_assoc();
		$uuid_perft=$row['uuid_perft'];
	}	
	if($uuid_perft == ""){
		echo "$sql<br>uuid_perft missing<br><br>";
		return;
	}
}

//select  creation_date, CONVERT_TZ(creation_date,'-12:00',@@global.time_zone) from tasks
$sql ="select f.fen,f.depth,CONVERT_TZ(creation_date,'".$time_zone.":00',@@global.time_zone) creation_date,heartbeat,tot,engine,author,minutes,country".
" from task_fens f left join tasks pt ".
" on pt.uuid_task=f.uuid_task and pt.fen=f.fen ".
" where f.uuid_task ='".$uuid_task."' order by fen,ifnull(tot,0) asc,fen, creation_date desc";

$result = $conn->query($sql);
?>
<section class="container">
          <hgroup>

<?php 
  echo "<h1>task id ".$uuid_task."</h1> ";
	echo "perft uuid: <a href='perft.php?uuid_perft=$uuid_perft'>$uuid_perft</a><br><br>";
	?>
          </hgroup><div class="row"><section>
			
              <h2>Deploying code changes</h2>
                <p>OpenShift uses the <a href="http://git-scm.com/">Git version control system</a>
 for your source code, and grants you access to it via the Secure Shell 
(SSH) protocol. In order to upload and download code to your application
 you need to give us your <a href="https://developers.openshift.com/en/managing-remote-connection.html">public SSH key</a>. You can upload it within the web console or install the <a href="https://developers.openshift.com/en/managing-client-tools.html">RHC command line tool</a> and run <code>rhc setup</code> to generate and upload your key automatically.</p>

<?php include("_command_area.php");?>
<?php echo "<button onclick='writeCommand(\"$uuid_perft\",\"$uuid_task\")'>Generate command</button>";?>
<?php if ($result->num_rows > 0) {
	echo "<table width='75%' border='1' align='center' bgcolor='#11FFff'>";
	echo "<tr>";
	echo "<td><b>Fen</td></b>" ;
	echo "<td><b>Depth</td></b>" ;
	echo "<td><b>Date</td></b>" ;
	echo "<td><b>Tot</td></b>" ;
	echo "<td><b>Engine</td></b>" ;
	echo "<td><b>Author</td></b>" ;
	echo "<td><b>Minutes</td></b>" ;
    echo "<td><b>Country</td></b>" ;
 	echo "</tr>";

    while($row = $result->fetch_assoc()) {
		echo "<tr>";	
		echo "<td>".$row["fen"] ."</td>" ;
		echo "<td>".$row["depth"] ."</td>" ;
		echo "<td>".$row["creation_date"]."</td>" ;
		$tot=$row["tot"];
		if($row["heartbeat"]=="1")$tot="<i>heartbeat</i>";
		echo "<td>".$tot."</td>" ;
		echo "<td>".$row["engine"] ."</td>" ;
		echo "<td>".$row["author"] ."</td>" ;
		
		$minutes=$row["minutes"];
		if($minutes=="0")$minutes="<1";
		echo "<td>".$minutes."</td>";		
		echo "<td> <img src='flags/".$row["country"].".png'> </td>";	
	 	echo "</tr>";
    }
	echo "</table>";
} else {
    echo "0 results";
}

$conn->close();
?> 

  </section>         
        </div>
</section>
<?php include 'footer.html'; ?>   
</body>
</html>
