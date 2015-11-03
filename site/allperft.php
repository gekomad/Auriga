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
<?php

	include 'mysql_connect.php';

	$sql = "SELECT uuid_perft,fen,depth,tasks,creation_date,tot FROM perft";
	$result = $conn->query($sql);

	if ($result->num_rows > 0) {
		echo "<table>";
		echo "<tr>";
		echo "<td>perft id</td>" ;
		echo "<td>fen</td>" ;
		echo "<td>depth</td>" ;
		echo "<td>tasks</td>" ;
		echo "<td>creation date</td>" ;
		echo "<td>tot</td>" ;		
	 	echo "</tr>";
	    while($row = $result->fetch_assoc()) {
			echo "<tr>";
			$mini_uuid_perft=substr($row["uuid_perft"],0,7)."...";
   			echo '<td><a href="perft.php?uuid_perft='.$row["uuid_perft"].'">'.$mini_uuid_perft.'</a></td>';
			echo "<td>".$row["fen"] ."</td>" ;
			echo "<td>".$row["depth"] ."</td>" ;
			echo "<td>".$row["tasks"] ."</td>" ;
			echo "<td>".$row["creation_date"] ."</td>" ;
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
