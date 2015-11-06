<?php 
/* non mettere ?> alla fine*/
header('Content-type: application/x-gzip');
include 'set_variable.php';	
echo file_get_contents($auriga_root."/data/".$_GET['id']."/".$_GET['id'].".ini.gz");
