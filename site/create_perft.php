<html>

 <head>
<?php
header("Cache-Control: no-cache, no-store, must-revalidate"); // HTTP 1.1.
header("Pragma: no-cache"); // HTTP 1.0.
header("Expires: 0"); // Proxies.
?>
  <meta name="robots" content="noindex">
  <title>New Perft</title>
 <link rel="stylesheet"  href="css/css1.css" type="text/css"/>
<link rel="stylesheet" href="css/layout.css" type="text/css" />
 <script src="js/validator.js"></script> 
<?php include 'captcha.php';	?>

</head>
 <body>
<?php include_once("analyticstracking.php");?>
<?php include 'menu.php';?>
<section class="container">
          <hgroup>
            <h1>Create a Perft </h1> 
          </hgroup>
        <div class="row">
            <section>
<form method="post" onsubmit="return validate();" action="generate_master_ini.php" name="perftform" id="perftform" class="form form-stacked c-form">
fen: <input name="fen" type="text" id="fen" placeholder="Fen string" />
depth: <input name="depth" type="text" id="depth" placeholder="depth" />
tasks: <input name="tasks" type="text" id="tasks" placeholder="tasks number" />

<br><br><br><br>
<?php echo"<img src=\"tmp/$image.jpg\" width=\"120\" height=\"30\" border=\"1\" alt=\"CAPTCHA\">";?>
<input type="text" size="6" maxlength="5" id="captcha" name="captcha" value="">
<small>copy the digits from the image into this box</small>
<br><br><input type="submit" class="submit btn outline" id="submit" value="Create perft" />
</form>
r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1
<br><br><br><br><br><br><br><br><br><br><br><br>
          </section>
        </div>



</section>
<?php include 'footer.html'; ?>     
 </body>
</html>
