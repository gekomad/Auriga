<html>
 <head>

  <meta name="robots" content="noindex">
  <title>all perft</title>
 </head>
 <body>

<?php

	include 'mysql_connect.php';

	$sql = "SELECT uuid_perft,fen,depth,tasks,creation_date,tot FROM perft";
	$result = $conn->query($sql);

	if ($result->num_rows > 0) {
		echo "<table width='75%' border='1' align='center' bgcolor='#11FFff'>";
		echo "<tr>";
		echo "<td>uuid_perft</td>" ;
		echo "<td>fen</td>" ;
		echo "<td>depth</td>" ;
		echo "<td>tasks</td>" ;
		echo "<td>creation date</td>" ;
		echo "<td>tot</td>" ;		
	 	echo "</tr>";
	    while($row = $result->fetch_assoc()) {
			echo "<tr>";
			
   			echo '<td><a href="perft.php?uuid_perft='.$row["uuid_perft"].'">'.$row["uuid_perft"].'</a></td>';
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

 </body>
</html>
