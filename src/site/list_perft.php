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
		var ca =worker.split('|');
		//alert(ca[0]+ "     "+ca[1]);
		document.getElementById('worker_path').value=ca[0];
		document.getElementById('dir_path').value=ca[1];
    
    }
}

function MyFunction(perft_id,task_uuid)
{
	
	var worker_path=document.getElementById('worker_path').value;
	var dir_path=document.getElementById('dir_path').value;

	setCookie("paths",worker_path+"|"+dir_path) ;
	document.getElementById('t1').value="Copy/paste on your client\n";
	var command="./auriga";
    if(document.getElementById('syst').value=="win") command="auriga.exe";
	var s=command+ " --task "+worker_path+" "+dir_path+" " +perft_id+ " "+task_uuid + " -fetch";
	document.getElementById('t1').value+=s+"\n";

		
}
</script>
</head>
<body onload="checkCookie()">

 <input type="hidden" id="syst">

 complete path worker.ini:<br>
  <input type="text" id="worker_path" value ="yourpath">
  <br>
 complete path data directory:<br>
  <input type="text" id="dir_path" value ="sfsdfd">


<textarea name=type rows=10 cols=150 id=t1 readonly=yes">

</textarea>
<br>


<?php

$perft_uuid=$_GET['id'];

include 'mysql_connect.php';


$sql = "select pt.uuid_perft, pt.uuid_task, sum(not isnull(t.tot)) tot,sum(not isnull(t.partial_moves))partial from perft_tasks pt ". 
"left join tasks t ".
"on  t.uuid_task=pt.uuid_task ".
"where pt.uuid_perft ='".$perft_uuid."' ".
"group by pt.uuid_perft,pt.uuid_task ".
"order by 1,2,3 ";


$result = $conn->query($sql);
echo "Perft uuid: ".$perft_uuid."<br>";
if ($result->num_rows > 0) {
    while($row = $result->fetch_assoc()) {
	   echo '<a " href="#" onclick="MyFunction(\''.$row["uuid_perft"].'\',\''.$row["uuid_task"].'\');return false;">'.$row["uuid_task"].'</a>'.' '. $row["tot"]. " " . $row["partial"].'<br>';
        /*echo "id: " . $row["uuid_perft"]. " - Name: " . $row["uuid_task"]. " " . $row["tot"]. " " . $row["partial"]. "<br>";*/
    }
} else {
    echo "0 results";
}

$conn->close();
?> 
</body>
</html>
