<html>
 <head>
 <meta charset="UTF-8">
  <meta name="robots" content="noindex">
  <title>Create your Web Site</title>
<link rel="stylesheet"  href="../css/css1.css" type="text/css"/>
<link rel="stylesheet" href="../css/layout.css" type="text/css" />

 </head>
 <body>
<?php include_once("../analyticstracking.php");?>
<?php include '../menu.php';?>
<div class="container2">
<section class="container">
          <hgroup>
            <h1>Create your Web Site</h1> 
          </hgroup>
        <div class="row">
          <section class="col-xs-12 col-sm-6 col-md-6">			
            <section>             			
          			<ul>
                    <li>Install PHP</li>
					<li>Install apache with mod_php</li>
					<li>Install mysql client and server mysql module for PHP</li>
					<li><a href="https://github.com/gekomad/Auriga">Download Auriga source</a></li>					
					<li>Copy all files from <code>Auriga/web_site</code> in your document root</li>
                    <li>Create a new database named auriga and import schema <code>mysql -u$USER -p$PASSWORD auriga < Auriga/web_site/mysql/auriga.sql</code></li>                    
					<li>In your worker file change host/port with your new host</li>
					<li>Define your variables in <code>set_variable.php</code></li>
                  </ul>				
			</section>        
		</section>        
		</div>
</section>

<?php include '../footer.html'; ?>     
</div>
 </body>
</html>
