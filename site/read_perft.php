<html>
 <head>

  <meta name="robots" content="noindex">
  <title>read perft</title>
 </head>
 <body>

<?php

	include 'mysql_connect.php';

	$sql = "SELECT uuid_perft, tasks FROM perft";
	$result = $conn->query($sql);

	if ($result->num_rows > 0) {
	    while($row = $result->fetch_assoc()) {
	         echo "id: " . $row["uuid_perft"]. " - tasks: " . $row["tasks"]. "<br>";
	    }
	} else {
	    echo "0 results";
	}
	$conn->close();

?>

 </body>
</html>
