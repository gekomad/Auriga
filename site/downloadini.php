<?php 

header("Content-Type:text/plain");
include 'set_variable.php';	
echo htmlspecialchars(file_get_contents($auriga_root_server."/data/".$_GET['id']."/".$_GET['id'].".ini")); 

?>
