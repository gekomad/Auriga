<script type="text/javascript">

var command;
var aurigaroot;
var worker_ini;

function setCookie(cname, cvalue) {
    var d = new Date();
    d.setTime(d.getTime() + (365*24*60*60*1000));
    var expires = "expires="+d.toUTCString();
    document.cookie = cname + "=" + cvalue + "; " + expires;
}

function getCookie(cname) {	
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

function setCommands(){
	if (navigator.appVersion.indexOf("Win")!=-1) document.getElementById('syst').value="win";
	worker_ini=document.getElementById('worker_ini').value;
	if(worker_ini==""){document.getElementById('t1').value="error. select a worker";return false;}

	setCookie("paths",worker_ini) ;
	document.getElementById("label_command").className = 'nohidden';
    if(document.getElementById('syst').value=="win") {
		aurigaroot="%AURIGA_ROOT%";
		command="%AURIGA_ROOT%/auriga.exe";
	}else{
		aurigaroot="$AURIGA_ROOT";
		command=aurigaroot+"/auriga";
	}
	return true;
}

function writeCommandRandom(){
	if(setCommands()==false)return;

	var s=command+ " --task " + aurigaroot + " " + worker_ini + " -fetch_random";
	document.getElementById('t1').value=s;
}

function writeCommand(perft_id,uuid_task){
	if(setCommands()==false)return;

	var s=command+ " --task " + aurigaroot + " " + worker_ini + " " + perft_id+ " "+uuid_task + " -fetch";
	document.getElementById('t1').value=s;
}

function writeCommands(perft_id,tasksArray){
	if(setCommands()==false)return;
	document.getElementById('t1').value="";
	for (var i = 0; i < tasksArray.length; i++) {
		var s=command+ " --task " + aurigaroot + " " + worker_ini + " " + perft_id+ " "+tasksArray[i] + " -fetch;\n";
		document.getElementById('t1').value+=s;
	}
}

</script>

select a  <a href="/doc/worker.php">worker</a>  <input type="text" id="worker_ini" list="workerName"/>
<datalist id="workerName">
  <select>
 	<option value="cheng">cheng</option>
	<option value="stockfish">stockfish</option>
	<option value="cinnamon">cinnamon</option>
	<option value="crafty">crafty</option>
	<option value="clarabit">clarabit</option>
	<option value="critter">critter</option>
	<option value="daydreamer">daydreamer</option>
	<option value="greko">greko</option>
	<option value="redqueen">redqueen</option>

    </select>
</datalist>
	<input type="hidden" id="syst">
	<br>
	<small id="label_command" class="hidden" style="color:blue">Copy/paste on your client</small><br>
    <textarea id=t1 readonly='yes'></textarea>
	<br>
