<html>
 <head>

  <meta name="robots" content="noindex">
  <title>generate masterfile4</title>
 </head>
 <body>

<?php

include 'set_variable.php';	

$tmp_dir="/tmp/".rand()."/";
mkdir($tmp_dir);
echo "tmp_dir=$tmp_dir<br>";

$AURIGA_DATA = $openshift_data_dir."/auriga_root";

mkdir($AURIGA_DATA."/data");
$AURIGA_EXE="timeout --signal=9 30s ./auriga";

echo $_POST["fen"];
echo "<br>";

echo $_POST["depth"];
echo "<br>";

echo $_POST["tasks"];
echo "<br>";

$param="--generate-ini ".$tmp_dir." ".$_POST["tasks"]." \"".$_POST["fen"]."\" ".$_POST["depth"];
echo "<br>";
echo "run: exec($AURIGA_EXE $param)";
echo "<br>";
$output=shell_exec($AURIGA_EXE." ".$param);
$array = explode("X", $output);
$perft_uuid=$array[1];
echo "perft_uuid $perft_uuid";
echo "<br>----GENERATED MASTER NODES----------<br>";

$csv=$tmp_dir."/data/".$perft_uuid."/perft.txt";
echo "mysqlimport -u$db_user -p$db_pass --local  --fields-terminated-by='|' $db_name $csv\n";
shell_exec("mysqlimport -u$db_user -p$db_pass --local  --fields-terminated-by='|' $db_name $csv");


$csv2=$tmp_dir."data/".$perft_uuid."/perft_tasks.txt";
echo "mysqlimport -u$db_user -p$db_pass --local  --fields-terminated-by='|' $db_name $csv2\n";
shell_exec("mysqlimport -u$db_user -p$db_pass --local  --fields-terminated-by='|' $db_name $csv2");


shell_exec("rm -f $tmp_dir/data/$perft_uuid/*.tmp");
shell_exec("mv $tmp_dir/data/$perft_uuid $AURIGA_DATA/data");
shell_exec("rm -fr $tmp_dir");


?>

 </body>
</html>
