<html><head>
<meta name="robots" content="noindex">
<title>Task</title>
<link rel="stylesheet"  href="css/css1.css" type="text/css"/>
<link rel="stylesheet" href="css/layout.css" type="text/css" />
<script type="text/javascript">

function setCookie(cname, cvalue) {
    var d = new Date();
    d.setTime(d.getTime() + (365*24*60*60*1000));
    var expires = "expires="+d.toUTCString();
    document.cookie = cname + "=" + cvalue + "; " + expires;
}

function getCookie(cname) {
	if (navigator.appVersion.indexOf("Win")!=-1) document.getElementById('syst').value="win";
    var name = cname + "=";
    var ca = document.cookie.split(';');
    for(var i=0; i<ca.length; i++) {
        var c = ca[i];
        while (c.charAt(0)==' ') c = c.substring(1);
        if (c.indexOf(name) == 0) return c.substring(name.length, c.length);
    }
    return "";
}

function checkCookie() {
    var worker = getCookie("paths");

    if (worker != "") {
		document.getElementById('worker_ini').value=worker;    
    }
}

function MyFunction(perft_id,uuid_task)
{
	var worker_ini=document.getElementById('worker_ini').value;
	if(worker_ini==""){document.getElementById('t1').value="error. select a worker";return;}

	setCookie("paths",worker_ini) ;
	document.getElementById("label_command").className = 'nohidden';
	var command="./auriga";
	var aurigaroot="$AURIGA_ROOT";
    if(document.getElementById('syst').value=="win") {command="auriga.exe";aurigaroot="%AURIGA_ROOT%";}

	var s=command+ " --task " + aurigaroot + " " + worker_ini + " " + perft_id+ " "+uuid_task + " -fetch";
	document.getElementById('t1').value=s;

		
}
</script>
</head>
<body onload="checkCookie()">
<?php include_once("analyticstracking.php"); ?>
 <?php include 'menu.php';?>
 <input type="hidden" id="syst">

<?php

$uuid_task=$_GET['uuid_task'];
$uuid_perft=$_GET['uuid_perft'];
if($uuid_task == ""){
	echo "uuid missing<br><br>";
	return;
}


include 'mysql_connect.php';

if($uuid_perft == ""){
	$sql="SELECT uuid_perft FROM perft_tasks WHERE uuid_task ='".$uuid_task."'";
	$result = $conn->query($sql);
	if ($result->num_rows > 0) {
		$row = $result->fetch_assoc();
		$uuid_perft=$row['uuid_perft'];
	}	
	if($uuid_perft == ""){
		echo "$sql<br>uuid_perft missing<br><br>";
		return;
	}
}


$sql ="select f.fen,f.depth,creation_date,heartbeat,tot,engine,author,minutes ".
"from task_fens f left join tasks pt ".
"on pt.uuid_task=f.uuid_task and pt.fen=f.fen ".
"where f.uuid_task ='".$uuid_task."'";

$result = $conn->query($sql);
?>
<section class="container">
          <hgroup>

<?php 
  echo "<h1>task id ".$uuid_task."</h1> ";
	echo "perft uuid: <a href='perft.php?uuid_perft=$uuid_perft'>$uuid_perft</a><br><br>";
	?>

          </hgroup><div class="row"><section>
			
              <h2>Deploying code changes</h2>
                <p>OpenShift uses the <a href="http://git-scm.com/">Git version control system</a>
 for your source code, and grants you access to it via the Secure Shell 
(SSH) protocol. In order to upload and download code to your application
 you need to give us your <a href="https://developers.openshift.com/en/managing-remote-connection.html">public SSH key</a>. You can upload it within the web console or install the <a href="https://developers.openshift.com/en/managing-client-tools.html">RHC command line tool</a> and run <code>rhc setup</code> to generate and upload your key automatically.</p>
worker.ini: 
<input type="text" id="worker_ini" list="workerName"/>
<datalist id="workerName">
  <select>
 <option value="cheng.auriga.ini">cheng.auriga.ini</option>
	<option value="stockfish.auriga.ini">stockfish.auriga.ini</option>
	<option value="cinnamon.auriga.ini">cinnamon.auriga.ini</option>
	<option value="crafty.auriga.ini">crafty.auriga.ini</option>
    </select>
</datalist>

<?php echo " <button onclick='MyFunction(\"$uuid_perft\",\"$uuid_task\")'>Generate command</button> ";?>
	<br><br>
	<small id="label_command" class="hidden">Copy/paste on your client</small><br>
    <textarea id=t1 readonly='yes'></textarea>
	<br>
<?php	

if ($result->num_rows > 0) {
	echo "<table width='75%' border='1' align='center' bgcolor='#11FFff'>";
	echo "<tr>";
	echo "<td><b>Fen</td></b>" ;
	echo "<td><b>Depth</td></b>" ;
	echo "<td><b>Date</td></b>" ;
	echo "<td><b>Heartbeat</td></b>" ;
	echo "<td><b>Tot</td></b>" ;
	echo "<td><b>Engine</td></b>" ;
	echo "<td><b>Author</td></b>" ;
	echo "<td><b>Minutes</td></b>" ;
 	echo "</tr>";

    while($row = $result->fetch_assoc()) {
	 	echo "<tr>";
		echo "<td>".$row["fen"] ."</td>" ;
		echo "<td>".$row["depth"] ."</td>" ;
		echo "<td>".$row["creation_date"]."</td>" ;
		echo "<td>".$row["heartbeat"] ."</td>" ;
		echo "<td>".$row["tot"] ."</td>" ;
		echo "<td>".$row["engine"] ."</td>" ;
		echo "<td>".$row["author"] ."</td>" ;
		
		$minutes=$row["minutes"];
		if($minutes=="0")$minutes="<1";
		echo "<td>".$minutes."</td>";


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
