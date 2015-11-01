<html><head>
<meta name="robots" content="noindex">
<title>Search Fen</title>
<link rel="stylesheet"  href="css/css1.css" type="text/css"/>
<link rel="stylesheet" href="css/layout.css" type="text/css" />

<script type="text/javascript">

function validate() {
	return true;
}
</script>
</head>
  <?php include 'menu.php';?>   
<body>



<section class="container">
<hgroup>
            <h1>Search Fen</h1> 
          </hgroup>
<form method="post" onsubmit="return validate();" action="search_fen.php" >
Search fen: <input name="fen" type="text" id="fen" placeholder="Fen string" />
<input type="submit" class="submit btn outline" id="submit" value="Search" />
</form>
          <hgroup>
<?php   
$fen=$_POST['fen'];
?>
 		
         </hgroup><div class="row"><section>
              <h2>Depsloying code changes</h2>
                <p>OpenShift uses the <a href="http://git-scm.com/">Git version control system</a>
 for your source code, and grants you access to it via the Secure Shell 
(SSH) protocol. In order to upload and download code to your application
 you need to give us your <a href="https://developers.openshift.com/en/managing-remote-connection.html">public SSH key</a>. You can upload it within the web console or install the <a href="https://developers.openshift.com/en/managing-client-tools.html">RHC command line tool</a> and run <code>rhc setup</code> to generate and upload your key automatically.</p>
               

         <?php  

if($fen!="") {	
	echo "<h1>Fen ".$fen."</h1> ";
	include 'mysql_connect.php';
	$sql = "SELECT uuid_task,depth FROM task_fens where fen ='".$fen."'";
	$result = $conn->query($sql);
	if ($result->num_rows > 0) {
		$row = $result->fetch_assoc();
		$uuid_task=$row["uuid_task"];

		echo "<img src='http://webchess.freehostia.com/diag/chessdiag.php?fen=".$fen."&amp&size=large&amp&coord=yes&amp&cap=no&amp&stm=yes&amp&fb=no&amp&theme=classic&amp&color1=E3CEAA&amp&color2=635147&amp&color3=000000'  height='300' width='300'>";
		echo "<br>";	
		echo "<table>";
		echo "<tr>";
		echo "<td><b>Task ID</b></td>" ;
		echo "<td><b>Depth</b></td>" ;
		echo "<td><b>Completed</b></td>" ;
		echo "<td><b>Running</b></td>" ;
	 	echo "</tr>";
    	while($row = $result->fetch_assoc()) {
			echo "<tr>";
			echo '<td><a href="task.php?uuid_task='.$row["uuid_task"].'&uuid_perft='.$uuid_perft.'">'.$row["uuid_task"].'</a></td>';
			echo "<td>".$row["depth"]."</td>";
			$completed=($row["tot"] == 0 ?"no": "yes");
			echo "<td>$completed</td>";
			$running=$completed=="yes"?"":($row["partial"] == 0 ?"no": "yes");
			echo "<td>$running</td>";
			echo "</tr>";
    	}
		echo "</table>";
	}
	else {
    	echo "0 results";	
	}
	$conn->close();	
}



?> 
      </section>         
        </div>
</section>
<?php include 'footer.html'; ?>    
</body>
</html>
