<html>
 <head>

  <meta name="robots" content="noindex">
  <title>all perft</title>
<link rel="stylesheet"  href="css/css1.css" type="text/css"/>
<link rel="stylesheet" href="css/layout.css" type="text/css" />


 </head>
 <body>
<?php include_once("analyticstracking.php");?>
<?php include 'menu.php';?>

         
<section class="container">
	<div class="row">
	<hgroup>
		<h1>All Perfts</h1> 
	</hgroup>
<?php include("_command_area.php");?>
<?php echo "<button onclick='writeCommandRandom()'>Generate command for random task</button>";?>
<?php

	include 'mysql_connect.php';

	$sql = "SELECT uuid_perft,fen,depth,tasks,creation_date,tot ,ifnull(perc_completed,0)perc_completed,hours  FROM perft";
	$result = $conn->query($sql);

	if ($result->num_rows > 0) {
		echo "<table>";
		echo "<tr>";
		echo "<td><b>Perft id</b></td>" ;
		echo "<td><b>Fen</b></td>" ;
		echo "<td><b>Depth</b></td>" ;
		echo "<td><b>Tasks</b></td>" ;
		echo "<td><b>Last update</b></td>" ;
		echo "<td><b>Hours</b></td>" ;
		echo "<td><b>Completed</b></td>" ;
		echo "<td><b>Tot</b></td>" ;		
	 	echo "</tr>";
	    while($row = $result->fetch_assoc()) {
			echo "<tr>";
			$mini_uuid_perft=substr($row["uuid_perft"],0,7)."...";
   			echo '<td><a href="perft.php?uuid_perft='.$row["uuid_perft"].'">'.$mini_uuid_perft.'</a></td>';
			echo "<td>".$row["fen"] ."</td>" ;
			echo "<td>".$row["depth"] ."</td>" ;
			echo "<td>".$row["tasks"] ."</td>" ;
			echo "<td>".$row["creation_date"] ."</td>" ;
			echo "<td>".$row["hours"] ."</td>" ;
			echo "<td>".$row["perc_completed"] ."%</td>" ;
			echo "<td>".$row["tot"] ."</td>" ;
			echo "</tr>";
	    }
		echo "</table>";
	} 
	else {
	    echo "0 results";
	}
	$conn->close();

?>
</div>

</section>
<?php include 'footer.html'; ?>     
 </body>
</html>
