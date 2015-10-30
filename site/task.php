<html><head>
<meta name="robots" content="noindex">
<title>Task</title>

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
	document.getElementById('t1').value="Copy/paste on your client\n";
	var command="./auriga";
	var aurigaroot="$AURIGA_ROOT";
    if(document.getElementById('syst').value=="win") {command="auriga.exe";aurigaroot="%AURIGA_ROOT%";}

	var s=command+ " --task " + aurigaroot + " " + worker_ini + " " + perft_id+ " "+uuid_task + " -fetch";
	document.getElementById('t1').value+=s+"\n";

		
}
</script>
</head>
<body onload="checkCookie()">

 <input type="hidden" id="syst">

<textarea name=type rows=10 cols=150 id=t1 readonly=yes"></textarea>

 <br><br>worker.ini: 

<input type="text" id="worker_ini" list="workerName"/>
<datalist id="workerName">
  <select>
 <option value="cheng.auriga.ini">cheng.auriga.ini</option>
	<option value="stockfish.auriga.ini">stockfish.auriga.ini</option>
	<option value="cinnamon.auriga.ini">cinnamon.auriga.ini</option>
	<option value="crafty.auriga.ini">crafty.auriga.ini</option>
    </select>
</datalist>
<br>

<?php

$uuid_task=$_GET['uuid_task'];
$uuid_perft=$_GET['uuid_perft'];
if($uuid_task == "" || $uuid_perft == ""){
	echo "uuid missing<br><br>";
	return;
}


include 'mysql_connect.php';

$sql ="select fen,depth,creation_date,partial_moves,tot,engine,author,minutes from tasks pt where pt.uuid_task ='".$uuid_task."'";

//echo "$sql";

$result = $conn->query($sql);
  echo "perft uuid: ".$uuid_perft."<br><br>";
  
 echo 'task uuid: <a " href="#" onclick="MyFunction(\''.$uuid_perft.'\',\''.$uuid_task.'\');return false;">'.$uuid_task.'</a><br>';
	echo "<br>";
if ($result->num_rows > 0) {
	echo "<table width='75%' border='1' align='center' bgcolor='#11FFff'>";
	echo "<tr>";
	echo "<td>fen</td>" ;
	echo "<td>depth</td>" ;
	echo "<td>creation date</td>" ;
	echo "<td>partial moves</td>" ;
	echo "<td>tot</td>" ;
	echo "<td>engine</td>" ;
	echo "<td>author</td>" ;
	echo "<td>minutes</td>" ;
 	echo "</tr>";

    while($row = $result->fetch_assoc()) {
	 	echo "<tr>";
		echo "<td>".$row["fen"] ."</td>" ;
		echo "<td>".$row["depth"] ."</td>" ;
		echo "<td>".$row["creation_date"]."</td>" ;
		echo "<td>".$row["partial_moves"] ."</td>" ;
		echo "<td>".$row["tot"] ."</td>" ;
		echo "<td>".$row["engine"] ."</td>" ;
		echo "<td>".$row["author"] ."</td>" ;
		echo "<td>".$row["minutes"] ."</td>" ;
	 	echo "</tr>";
    }
	echo "</table>";
} else {
    echo "0 results";
}

$conn->close();
?> 

</body>
</html>
