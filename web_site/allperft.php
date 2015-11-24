<?php include("setTimezone.php");?>
<html>
 <head>
  <meta charset="UTF-8">
  <meta name="robots" content="noindex">
  <title>Chess Perft Results</title>
<link rel="stylesheet"  href="css/css1.css" type="text/css"/>
<link rel="stylesheet" href="css/layout.css" type="text/css" />
 </head>
<body onload="checkCookie()">

<?php
echo "<input type='hidden' value=\"".$time_zone."\">";
 include_once("analyticstracking.php");

?>
<?php include 'menu.php';?>

         
<section class="container">
	<div class="row">
	<hgroup>
		<h1><br>Chess Perft Results</h1> <br>
	</hgroup>
<?php include("_command_area.php");?>
<?php echo "<button onclick='writeCommandRandomPerftTask()'>Generate command for random perft/task</button>";?>
<?php

	include 'mysql_connect.php';
	$conn->query("SET time_zone = '{$time_zone}'");
	$sql = "SELECT uuid_perft,fen,depth,tasks,ifnull(perc_completed,0)perc_completed,creation_date,tot,engines,if(minutes=0,' < 1 min', if(minutes>=60*24,concat(round(minutes/60/24),' days'),if(minutes>=60,concat(round(minutes/60,' hours'),' hours'),concat(minutes,' min'))))time,confirmed FROM perft order by creation_date desc";
	$result = $conn->query($sql);

	if ($result->num_rows > 0) {
		echo "<table>";
		echo "<tr>";
		echo "<td><b>Perft id</b></td>" ;
		echo "<td><b>Fen</b></td>" ;
		echo "<td><b>Depth</b></td>" ;
		echo "<td><b>Tasks</b></td>" ;
		echo "<td><b>Last update</b></td>" ;
		echo "<td><b># Engines</b></td>" ;
		echo "<td><b>Time</b></td>" ;
		echo "<td><b>Completed</b></td>" ;
		echo "<td><b>Tot</b></td>" ;
		echo "<td><b>Confirmed</b></td>" ;				
	 	echo "</tr>";
	    while($row = $result->fetch_assoc()) {
			echo "<tr>";
			$uuid_perft=$row["uuid_perft"];
   			echo '<td><font size="1"><a href="perft.php?uuid_perft='.$uuid_perft.'">'.$uuid_perft.'</a></font></td>';
   			echo "<td><small>".$row["fen"]."</small></td>";
			echo "<td>".$row["depth"] ."</td>" ;
			echo "<td>".$row["tasks"] ."</td>" ;
			echo "<td>".$row["creation_date"] ."</td>" ;
			echo "<td>".$row["engines"] ."</td>" ;
			echo "<td>".$row["time"] ."</td>" ;
			echo "<td>".$row["perc_completed"] ."%</td>" ;
			echo "<td>".$row["tot"] ."</td>" ;
			$confirmed="NO";
			if($row["confirmed"]==1)$confirmed="YES";
			echo "<td>".$confirmed."</td>" ;		
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
