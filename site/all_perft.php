<html>
 <head>

  <meta name="robots" content="noindex">
  <title>all perft</title>
<link rel="stylesheet"  href="css/css1.css" type="text/css"/>
<link rel="stylesheet" href="css/layout.css" type="text/css" />

 </head>
 <body>

<?php include 'menu.php';?>

<section class="container">

          <hgroup>
            <h1>Auriga </h1> 
          </hgroup>


        <div class="row">
          <section class="col-xs-12 col-sm-6 col-md-6">
            <section>
              <h2>Deploying code changes</h2>
                <p>OpenShift uses the <a href="http://git-scm.com/">Git version control system</a>
 for your source code, and grants you access to it via the Secure Shell 
(SSH) protocol. In order to upload and download code to your application
 you need to give us your <a href="https://developers.openshift.com/en/managing-remote-connection.html">public SSH key</a>. You can upload it within the web console or install the <a href="https://developers.openshift.com/en/managing-client-tools.html">RHC command line tool</a> and run <code>rhc setup</code> to generate and upload your key automatically.</p>

                <h3>Working in your local Git repository</h3>
                <p>If you created your application from the command line
 and uploaded your SSH key, rhc will automatically download a copy of 
that source code repository (Git calls this 'cloning') to your local 
system.</p>

                <p>If you created the application from the web console, 
you'll need to manually clone the repository to your local system. Copy 
the application's source code Git URL and then run:</p>

<pre>$ git clone &lt;git_url&gt; &lt;directory_to_create&gt;

# Within your project directory
# Commit your changes and push to OpenShift

$ git commit -a -m 'Some commit message'
$ git push</pre>


                  <ul>
                    <li><a href="https://developers.openshift.com/en/managing-modifying-applications.html">Learn more about deploying and building your application</a></li>
                    <li>See the README file in your local application Git repository for more information on the options for deploying applications.</li>
                  </ul>
            </section>

          </section>
          <section class="col-xs-12 col-sm-6 col-md-6">
				<img src="img/auriga.jpg" height="252" width="570"> 
                <h2>Managing your application</h2>

                <h3>Web Console</h3>
                <p>You can use the OpenShift web console to enable 
additional capabilities via cartridges, add collaborator access 
authorizations, designate custom domain aliases, and manage domain 
memberships.</p>

                <h3>Command Line Tools</h3>
                <p>Installing the <a href="https://developers.openshift.com/en/managing-client-tools.html">OpenShift RHC client tools</a>
 allows you complete control of your cloud environment. Read more on how
 to manage your application from the command line in our <a href="https://www.openshift.com/user-guide">User Guide</a>.
                </p>

                <h2>Development Resources</h2>
                  <ul>
                    <li><a href="https://developers.openshift.com/en/php-overview.html">Getting Started with PHP on OpenShift</a></li>
                    <li><a href="https://developers.openshift.com/">Developer Center</a></li>
                    <li><a href="https://www.openshift.com/user-guide">User Guide</a></li>
                    <li><a href="https://help.openshift.com/">Help Center</a></li>
                    <li><a href="http://stackoverflow.com/questions/tagged/openshift">Stack Overflow questions for OpenShift</a></li>
                    <li><a href="http://git-scm.com/documentation">Git documentation</a></li>
                  </ul>


          </section>
        </div>



</section>
<section class="container">
 <div class="row">

<?php

	include 'mysql_connect.php';

	$sql = "SELECT uuid_perft,fen,depth,tasks,creation_date,tot FROM perft";
	$result = $conn->query($sql);

	if ($result->num_rows > 0) {
		echo "<table>";
		echo "<tr>";
		echo "<td>perft id</td>" ;
		echo "<td>fen</td>" ;
		echo "<td>depth</td>" ;
		echo "<td>tasks</td>" ;
		echo "<td>creation date</td>" ;
		echo "<td>tot</td>" ;		
	 	echo "</tr>";
	    while($row = $result->fetch_assoc()) {
			echo "<tr>";
			
   			echo '<td><a href="perft.php?uuid_perft='.$row["uuid_perft"].'">'.$row["uuid_perft"].'</a></td>';
			echo "<td>".$row["fen"] ."</td>" ;
			echo "<td>".$row["depth"] ."</td>" ;
			echo "<td>".$row["tasks"] ."</td>" ;
			echo "<td>".$row["creation_date"] ."</td>" ;
			echo "<td>".$row["tot"] ."</td>" ;
			echo "</tr>";
	    }
		echo "</table>";
	} 
	else {
	    echo "0 results";
	}
	$conn->close();

?>
</div>

</section>
<?php include 'footer.html'; ?>     
 </body>
</html>
