<?php 

header('Content-type: application/x-gzip');
include 'set_variable.php';	
echo "XXX".filesize($auriga_root."/data/".$_GET['id']."/".$_GET['id'].".ini.gz")."XXX".file_get_contents($auriga_root."/data/".$_GET['id']."/".$_GET['id'].".ini.gz");?>
