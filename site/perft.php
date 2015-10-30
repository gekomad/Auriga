<html><head>
<title>(Type a title for your page here)</title>

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

function MyFunction(perft_id,task_uuid)
{	

	var worker_ini=document.getElementById('worker_ini').value;
	if(worker_ini==""){document.getElementById('t1').value="error. select a worker";return;}

	setCookie("paths",worker_ini) ;
	document.getElementById('t1').value="Copy/paste on your client\n";
	var command="./auriga";
	var aurigaroot="$AURIGA_ROOT";
    if(document.getElementById('syst').value=="win") {command="auriga.exe";aurigaroot="%AURIGA_ROOT%";}

	var s=command+ " --task " + aurigaroot + " " + worker_ini + " " + perft_id+ " "+task_uuid + " -fetch";
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

$perft_uuid=$_GET['uuid_perft'];
if($perft_uuid == ""){
	echo "perft_uuid missing<br><br>";
	return;
}


include 'mysql_connect.php';

$sql = "SELECT fen, depth,tasks,creation_date,tot FROM perft where uuid_perft='".$perft_uuid."'";
$result = $conn->query($sql);


$row = $result->fetch_assoc();
$fen =$row["fen"];
$deph =$row["depth"];
$tasks =$row["tasks"];
$creation_date =$row["creation_date"];

$sql = "select pt.uuid_perft, pt.uuid_task, sum(not isnull(t.tot)) tot,sum(not isnull(t.partial_moves))partial from perft_tasks pt ". 
"left join tasks t ".
"on  t.uuid_task=pt.uuid_task ".
"where pt.uuid_perft ='".$perft_uuid."' ".
"group by pt.uuid_perft,pt.uuid_task ".
"order by 1,2,3 ";


$result = $conn->query($sql);
echo "Perft uuid: ".$perft_uuid."<br><br>";
echo "<img src='http://webchess.freehostia.com/diag/chessdiag.php?fen=".$fen."&amp&size=large&amp&coord=yes&amp&cap=no&amp&stm=yes&amp&fb=no&amp&theme=classic&amp&color1=E3CEAA&amp&color2=635147&amp&color3=000000'  height='300' width='300'>";
echo "<br><br>fen: $fen<br><br>";
echo "depth: $deph<br>";
echo "tasks: $tasks<br>";
echo "creation_date: $creation_date<br>";
echo "<br>";

if ($result->num_rows > 0) {
    while($row = $result->fetch_assoc()) {
	   echo '<a " href="#" onclick="MyFunction(\''.$row["uuid_perft"].'\',\''.$row["uuid_task"].'\');return false;">'.$row["uuid_task"].'</a>'.' '. $row["tot"]. " " . $row["partial"].'<br>';
    }
} else {
    echo "0 results";
}

$conn->close();
?> 

</body>
</html>