<html><head>
<meta name="robots" content="noindex">
<title>Perft</title>
<link rel="stylesheet"  href="css/css1.css" type="text/css"/>
<link rel="stylesheet" href="css/layout.css" type="text/css" />
<?php echo "<script>var ut = [];</script>" ?>
</head>
<body onload="checkCookie()">
<?php include_once("analyticstracking.php");?>
<?php include 'menu.php';?>

<?php
$uuid_perft=$_GET['uuid_perft'];
if($uuid_perft == ""){
	echo "uuid_perft missing<br><br>";
	return;
}

include 'mysql_connect.php';

$sql = "SELECT fen, depth,tasks,creation_date,tot,perc_completed FROM perft where uuid_perft='".$uuid_perft."'";
$result = $conn->query($sql);

$row = $result->fetch_assoc();
$fen =$row["fen"];
$deph =$row["depth"];
$tasks =$row["tasks"];
$creation_date =$row["creation_date"];
$tot =$row["tot"];
$perc_completed =$row["perc_completed"];

$sql="select uuid_task,engine n_engine,perc_completed,creation_date,hours from perft_tasks where uuid_perft ='".$uuid_perft."' order by perc_completed asc";

$result = $conn->query($sql);
?>
<section class="container">
          <hgroup><?php echo "<h1>Perft id ".$uuid_perft."</h1> ";?></hgroup>
        <div class="row">        
            <section>
              <h2>Depsloying code changes</h2>
                <p>OpenShift uses the <a href="http://git-scm.com/">Git version control system</a>
 for your source code, and grants you access to it via the Secure Shell 
(SSH) protocol. In order to upload and download code to your application
 you need to give us your <a href="https://developers.openshift.com/en/managing-remote-connection.html">public SSH key</a>. You can upload it within the web console or install the <a href="https://developers.openshift.com/en/managing-client-tools.html">RHC command line tool</a> and run <code>rhc setup</code> to generate and upload your key automatically.</p>

<?php   

echo "<img src='http://webchess.freehostia.com/diag/chessdiag.php?fen=".$fen."&amp&size=large&amp&coord=yes&amp&cap=no&amp&stm=yes&amp&fb=no&amp&theme=classic&amp&color1=E3CEAA&amp&color2=635147&amp&color3=000000'  height='300' width='300'>";
echo "<br><br>fen: $fen<br><br>";
echo "depth: $deph<br>";
echo "tasks: $tasks<br>";
echo "last pudate: $creation_date<br>";
echo "tot: $tot<br>";
echo "completed: $perc_completed%<br>";

echo "<br>";

if($perc_completed!="100"){
	include("_command_area.php");
	echo "<button onclick='writeCommands(\"$uuid_perft\",ut)'>Generate command for unfinished</button>";
}

if ($result->num_rows > 0) {
echo "<table>";
		echo "<tr>";
		echo "<td><b>#</b></td>" ;
		echo "<td><b>Task ID</b></td>" ;
		echo "<td><b>Completed</b></td>" ;
		echo "<td><b>Last update</b></td>" ;
		echo "<td><b># engine</b></td>" ;
		echo "<td><b>Hours</b></td>" ;
	 	echo "</tr>";
		$count=0;
    while($row = $result->fetch_assoc()) {		
		echo "<tr>";
		$count++;
		echo "<td>$count</td>";
		echo '<td><a href="task.php?uuid_task='.$row["uuid_task"].'&uuid_perft='.$uuid_perft.'">'.$row["uuid_task"].'</a></td>';
		$completed=$row["perc_completed"];
		if($completed=="")$completed="0";
		echo "<td>".$completed."%</td>";
		if($completed!="100")echo "<script>ut.push('".$row["uuid_task"]."');</script>";
		$creation_date=$row["creation_date"];
		echo "<td>$creation_date</td>";
		echo "<td>".$row["n_engine"]."</td>";
		$hours=$row["hours"];
		if($hours=="0")$hours="<1";
		echo "<td>".$hours."</td>";
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
