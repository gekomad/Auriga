<?php 

header("Content-Type:text/plain");
include 'set_variable.php';	
echo htmlspecialchars(file_get_contents($auriga_root."/data/".$_GET['id']."/".$_GET['id'].".ini")); 

?>
