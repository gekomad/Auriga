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

function writeCommand(perft_id,uuid_task){
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

function writeCommands(perft_id,tasksArray){
	var worker_ini=document.getElementById('worker_ini').value;
	if(worker_ini==""){document.getElementById('t1').value="error. select a worker";return;}

	setCookie("paths",worker_ini) ;
	document.getElementById("label_command").className = 'nohidden';
	var command="./auriga";
	var aurigaroot="$AURIGA_ROOT";
    if(document.getElementById('syst').value=="win") {command="auriga.exe";aurigaroot="%AURIGA_ROOT%";}
	document.getElementById('t1').value="";
	for (var i = 0; i < tasksArray.length; i++) {
		var s=command+ " --task " + aurigaroot + " " + worker_ini + " " + perft_id+ " "+tasksArray[i] + " -fetch\n";
		document.getElementById('t1').value+=s;
	}
}

</script>

worker.ini: <input type="text" id="worker_ini" list="workerName"/>
<datalist id="workerName">
  <select>
 <option value="cheng.auriga.ini">cheng.auriga.ini</option>
	<option value="stockfish.auriga.ini">stockfish.auriga.ini</option>
	<option value="cinnamon.auriga.ini">cinnamon.auriga.ini</option>
	<option value="crafty.auriga.ini">crafty.auriga.ini</option>
    </select>
</datalist>
	<input type="hidden" id="syst">
	<br><br>
	<small id="label_command" class="hidden">Copy/paste on your client</small><br>
    <textarea id=t1 readonly='yes'></textarea>
	<br>
