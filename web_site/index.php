<html>
<head>
	<meta charset="UTF-8">
    <meta name="robots" content="noindex">

	<meta name="description" content="Distributed and collaborative perft chess" />
	<meta name="keywords" content="chess,perft,perft results,chess engine,open source" />
	<meta name="author" content="Giuseppe Cannella" />

    <title>Auriga</title>
    <link rel="stylesheet" href="css/css1.css" type="text/css"/>
    <link rel="stylesheet" href="css/layout.css" type="text/css"/>
</head>
<body>
<?php include_once("analyticstracking.php");?>
<?php include 'menu.php';?>
<div class="container2">
    <section class="container">
        <hgroup>
            <h1><br>Auriga </h1><br>
        </hgroup>
        <div class="row">
            <section class="col-xs-12 col-sm-6 col-md-6">

                <img src="img/auriga.png" ALIGN="right" border="60px"/>
                <section>
					<a href="https://en.wikipedia.org/wiki/Auriga_%28slave%29"><img src="img/logo.jpg"/></a>
                    <h2>Overview</h2>
                    

                    <p>Auriga is an open source distributed collaborative and multi platform system for the calculation of the function <a href="https://chessprogramming.wikispaces.com/Perft">Perft</a>.

                    <h3>How does it work</h3>
					<p>The tree is divided into tasks, each task will be analyzed from a single machine (<b>worker</b>) and the result will be sent automatically to this site. When all tasks are processed you will have the result of perft.


                    <h3>Install</h3>
                    <a href="download.php">Download</a> auriga for your platform and copy the folder somewhere in your file system, set the environment variable AURIGA_ROOT example in ubuntu insert in the file ~/.bashrc <br><br>
                    <code>export AURIGA_ROOT="/your/path/auriga_root"</code>
                    for windows click <a href="https://support.microsoft.com/en-us/kb/310519">here.</a><br>
					<br>The directory <i>auriga_root</i> contains another folder named <i>worker</i> with some configuration engines, see <a href="doc/worker.php">here</a> for the configuration of the worker.
                    </p>

                    <h3>Launch a worker</h3>
                    Before launching a worker you must have <a href="doc/worker.php">configured</a>.<br><br>

                    <p>You can launch a worker on:
                    <ul>							
                        <li>A random perft and task not yet completed, go <a href="allperft.php">here</a> select a worker and click on <code>Generate command for random perft/task</code> copy and paste the generated command in a shell on your machine.
                        </li>
                        <li>A random task on a given perft not yet completed, go <a href="allperft.php">here</a> click on a perft and click on <code>Generate command for random tasks</code> copy and paste the generated command shell on your machine.</li>

                        <li>A specific task - the previous page click on a task and then on <code>Generate command</code> copy and paste the generated command shell on your machine.</li>
                    </p>
                    </ul>
				  <h2>See also</h2>
                  <ul>
                    <li><a href="doc/create_web_site.php">Create your Web Site</a></li>
                    <li><a href="doc/inside_auriga.php">Inside Auriga</a></li>                    
                  </ul>                  
                </section>
        </div>
    </section>
    <?php include 'footer.html'; ?>
</div>
</body>
</html>
