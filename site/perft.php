<html><head>
<meta name="robots" content="noindex">
<title>Perft</title>
<link rel="stylesheet"  href="css/css1.css" type="text/css"/>
<link rel="stylesheet" href="css/layout.css" type="text/css" />
</head>
<body>
  <?php include 'menu.php';?>   
      
<?php

$uuid_perft=$_GET['uuid_perft'];
if($uuid_perft == ""){
	echo "uuid_perft missing<br><br>";
	return;
}

include 'mysql_connect.php';

$sql = "SELECT fen, depth,tasks,creation_date,tot FROM perft where uuid_perft='".$uuid_perft."'";
$result = $conn->query($sql);


$row = $result->fetch_assoc();
$fen =$row["fen"];
$deph =$row["depth"];
$tasks =$row["tasks"];
$creation_date =$row["creation_date"];

$sql = "select pt.uuid_perft,t.fen, pt.uuid_task, min(not isnull(t.tot)) tot,max(not isnull(t.partial_moves))partial from perft_tasks pt ". 
"left join tasks t ".
"on  t.uuid_task=pt.uuid_task ".
"where pt.uuid_perft ='".$uuid_perft."' ".
"group by pt.uuid_perft,pt.uuid_task ".
"order by 1,2,3 ";


$result = $conn->query($sql);
?>

<section class="container">

          <hgroup>
<?php   echo "<h1>Perft id ".$uuid_perft."</h1> ";?>
           
          </hgroup>


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
echo "creation_date: $creation_date<br>";
echo "<br>";

if ($result->num_rows > 0) {
echo "<table>";
		echo "<tr>";
		echo "<td><b>Task ID</b></td>" ;
		echo "<td><b>Completed</b></td>" ;
		echo "<td><b>Running</b></td>" ;
	 	echo "</tr>";
    while($row = $result->fetch_assoc()) {
		echo "<tr>";
		echo '<td><a href="task.php?uuid_task='.$row["uuid_task"].'&uuid_perft='.$uuid_perft.'">'.$row["uuid_task"].'</a></td>';
		$completed=($row["tot"] == 0 ?"no": "yes");
		echo "<td>$completed</td>";
		$running=$completed=="yes"?"":($row["partial"] == 0 ?"no": "yes");
		echo "<td>$running</td>";
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
