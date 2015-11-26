<html>
<head>
<meta charset="UTF-8">
<?php
header("Cache-Control: no-cache, no-store, must-revalidate");
header("Pragma: no-cache");
header("Expires: 0");
include_once 'set_variable.php';
?>
    <meta name="robots" content="noindex">
    <title>New Perft</title>
    <link rel="stylesheet" href="css/css1.css" type="text/css"/>
    <link rel="stylesheet" href="css/layout.css" type="text/css"/>
    <script src="js/validator.js"></script>
    <script src="js/cinnamon_engine_1_2b.js"></script>
   <script>
	var gettasks = Module.cwrap('perft', 'number', ['string','number']);
	function calculateTasks(){
		document.getElementById("tree").innerHTML ="";
		if(validate_single_fen()==false)return;		
		fen=document.getElementById("fen").value;
		var rd=1;
		
		res=0;
		pretask=1;
		while(res<= <?php echo "$max_tasks";?> )
		{
			res = gettasks(fen,rd);
			document.getElementById("tree").innerHTML +="tasks >= "+pretask+" and <=" + res+" reduce depth: "+rd+"\n";
			pretask=res+1;
			rd++;
		}
		
	}
	function setCareful(){
		if (navigator.appVersion.indexOf("Win")!=-1){
			document.getElementById("careful").innerHTML ='Before inserting the data you can do tests on your client with the command:<br><br>'
			+ '%AURIGA_ROOT%\\auriga.exe --generate-ini %AURIGA_ROOT% Ntask "FEN" DEPTH<br><br>'
			+'example: %AURIGA_ROOT%\\auriga.exe --generate-ini  %AURIGA_ROOT%  1000 "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" 15';	
		}else{
			document.getElementById("careful").innerHTML ='Please insert data careful, try on your client the command:<br><br>'
			+ '$AURIGA_ROOT/auriga --generate-ini $AURIGA_ROOT Ntask "FEN" DEPTH<br><br>'
			+'example: $AURIGA_ROOT/auriga --generate-ini  $AURIGA_ROOT  1000 "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" 15';	
		}
		
	}

 </script> 
    <?php include 'captcha.php';	?>

</head>
<body onload="setCareful()">

<?php include_once("analyticstracking.php");
include 'menu.php';?>

<section class="container">
    <hgroup>
        <h1><br>Create new Perft</h1><br>
<p>

<pre id="careful" />
</p>

    </hgroup>   
    <div class="row">
        <section>
<br><br>
<aside >

<pre id="tree" style=" height:40%;border: none; background-color: Lavender ;"></pre>
</aside>

            <form method="post" onsubmit='return validate(<?php echo "$max_tasks";?>);' action="generate_master_ini.php" name="perftform" id="perftform" class="form form-stacked c-form">
                Fen:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<input name="fen" type="text" id="fen" placeholder="Fen string"/>
				<input id="calculatebutton" type="button" value="Calculate Tasks" onclick="calculateTasks();" /> 	
                <br><small id="error_fen" class="hidden">fen format is wrong [fen validator: <a href="https://github.com/jhlywa/chess.js">https://github.com/jhlywa/chess.js</a>]</small>
  				
				<br>
			
				<br>Tasks:&nbsp;&nbsp;<input name="tasks" type="text" id="tasks" placeholder="tasks number"/>
                <small id="error_tasks" class="hidden">tasks between 1 and <?php echo "$max_tasks";?></small>
                <br>
                <br>
                Depth:&nbsp;<input name="depth" type="text" id="depth" placeholder="depth"/>
                <small id="error_depth" class="hidden">depth between 2 and 100</small>
                

                <br><br>
                <?php echo"<img src=\"tmp/$image.jpg\" width=\"120\" height=\"30\" border=\"1\" alt=\"CAPTCHA\">";?><br>
                <input type="text" size="6" maxlength="5" id="captcha" name="captcha" value="">
                <small>copy the digits from the image into this box</small>

                <br><br><input type="submit" class="submit btn outline" id="submit" value="Create perft"/>
            </form>
        </section>

    </div>


</section>
<?php include 'footer.html'; ?>
</body>
</html>
