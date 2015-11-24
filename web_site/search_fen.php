<html><head>
<meta charset="UTF-8">
<meta name="robots" content="noindex">
<title>Search Fen</title>
<link rel="stylesheet"  href="css/css1.css" type="text/css"/>
<link rel="stylesheet" href="css/layout.css" type="text/css" />
<script src="js/validator.js"></script> 

</head>

<body>
<?php include_once("analyticstracking.php") ;?>
<?php include 'menu.php';?>   

<section class="container">
<hgroup><h1><br>Search Fen</h1><br></hgroup>

<?php $fen=$_POST['fen'];?>

<form method="post" onsubmit="return validate_fen2();" action="search_fen.php" >
Search fen: <input name="fen" size="70" type="text" id="fen" placeholder="Fen string" value="<?php echo $fen;?>" /><input type="submit" class="submit btn outline" id="submit" value="Search" /><br>
<small id="error_fen" class="hidden">fen format is wrong [fen validator: <a href="https://github.com/jhlywa/chess.js">https://github.com/jhlywa/chess.js</a>]</small>

</form>
          <hgroup> 		
         </hgroup><div class="row"><section>             
         <?php  
function getSql($fen){
return "SELECT 'Task' type, uuid_task id,depth FROM task_fens where fen ='".$fen."'
 union 
SELECT 'Perft' type,uuid_perft id,depth FROM perft where fen ='".$fen."'";

}

if($fen!="") {	
	$fen = str_replace("%", "",$fen);
	$fen = str_replace("'", "",$fen);
	$fen = str_replace("\"", "",$fen);
	
	include 'mysql_connect.php';
	$sql = getSql($fen);

	$result = $conn->query($sql);
	if($result->num_rows <= 0){
		preg_match('/(.+) (.+) (.+) (.+) (.+ .+)/', $fen, $re);
		$fen = $re[1]." ".$re[2]." ".$re[3]." ".$re[4]." 0 1";
		$sql = getSql($fen);
		$result = $conn->query($sql);
	}
	if($result->num_rows > 0) {		
		$uuid_task=$row["uuid_task"];
		echo "<div align='center'><img src='http://webchess.freehostia.com/diag/chessdiag.php?fen=".$fen."&amp&size=large&amp&coord=yes&amp&cap=no&amp&stm=yes&amp&fb=no&amp&theme=classic&amp&color1=E3CEAA&amp&color2=635147&amp&color3=000000'  height='300' width='300'></div>";
		echo "<h1>FEN ".$fen."</h1> ";
		echo "<br>";	
		echo "<table>";
		echo "<tr>";
		echo "<td><b>Type</b></td>" ;
		echo "<td><b>ID</b></td>" ;
		echo "<td><b>Depth</b></td>" ;
	 	echo "</tr>";
    	while($row = $result->fetch_assoc()) {
			echo "<tr>";
			echo "<td>".$row["type"]."</td>";
			if($row["type"]=="Task")echo '<td><a href="task.php?uuid_task='.$row["id"].'">'.$row["id"].'</a></td>';
			else echo '<td><a href="perft.php?uuid_perft='.$row["id"].'">'.$row["id"].'</a></td>';
			echo "<td>".$row["depth"]."</td>";
			echo "</tr>";
    	}
		echo "</table>";
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
