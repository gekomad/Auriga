<html><head>
<meta charset="UTF-8">
<meta name="robots" content="noindex">
<title>Anomaly</title>
<link rel="stylesheet"  href="css/css1.css" type="text/css"/>
<link rel="stylesheet" href="css/layout.css" type="text/css" />

</head>

<body>

<?php include 'menu.php';?>   

<section class="container">
<hgroup><h1>Anomaly Fen</h1></hgroup>

<?php $fen=$_POST['fen'];?>


</form>
          <hgroup>

 		
         </hgroup><div class="row"><section>
              <h2>Depsloying code changes</h2>
                <p>OpenShift uses the <a href="http://git-scm.com/">Git version control system</a>
 for your source code, and grants you access to it via the Secure Shell 
(SSH) protocol. In order to upload and download code to your application
 you need to give us your <a href="https://developers.openshift.com/en/managing-remote-connection.html">public SSH key</a>. You can upload it within the web console or install the <a href="https://developers.openshift.com/en/managing-client-tools.html">RHC command line tool</a> and run <code>rhc setup</code> to generate and upload your key automatically.</p>
               

         <?php  


	include 'mysql_connect.php';
	$sql = "select uuid_perft,uuid_task,fen,depth,tot from tasks where anomaly !=1";
	
	$result = $conn->query($sql);
	if($result->num_rows >0){
	
	
		echo "<table>";
		echo "<tr>";
		echo "<td><b>Perft ID</b></td>" ;
		echo "<td><b>Task ID</b></td>" ;
		echo "<td><b>Fen</b></td>" ;
		echo "<td><b>Depth</b></td>" ;
		echo "<td><b>Tot</b></td>" ;
	 	echo "</tr>";
    	while($row = $result->fetch_assoc()) {
			$uuid_perft=$row["uuid_perft"];
			$uuid_task=$row["uuid_task"];
			$fen=$row["fen"];
			$depth=$row["depth"];
			$tot=$row["tot"];
	
			echo "<tr>";
			echo '<td><font size="1"><a href="perft.php?uuid_perft='.$uuid_perft.'">'.$uuid_perft.'</a></font></td>';
			echo '<td><font size="1"><a href="task.php?uuid_task='.$row["uuid_task"].'&uuid_perft='.$uuid_perft.'">'.$row["uuid_task"].'</a></font></td>';
			echo "<td>".$fen."</td>";
			echo "<td>".$depth."</td>";
			echo "<td>".$tot."</td>";
			echo "</tr>";
    	}
		echo "</table>";
	}
	
	$conn->close();	


?> 
      </section>         
        </div>
</section>
<?php include 'footer.html'; ?>    
</body>
</html>
