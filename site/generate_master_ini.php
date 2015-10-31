

<?php

include 'set_variable.php';	

session_start();
shell_exec("find tmp/* -type f -mmin +1 -exec rm {} \;");
if($_POST['captcha'] != $_SESSION['digit']) {
	header("Location: error_captcha.html");
	exit;
}
session_destroy();

echo $_POST["depth"];
echo "<br>";
echo $_POST["tasks"];
echo "<br>";
if (!is_numeric($_POST["depth"])||!is_numeric($_POST["tasks"])) {
    echo "error";
	return;
} 


$tmp_dir="/tmp/".rand()."/";
mkdir($tmp_dir);
echo "tmp_dir=$tmp_dir<br>";


$AURIGA_DATA = $auriga_root."/data";
mkdir($auriga_root);
mkdir($AURIGA_DATA);
$AURIGA_EXE="timeout --signal=9 30s ./auriga";

$fen=$_POST["fen"];
$fen=trim($fen);
$fen=substr($fen,0,100);
echo $fen;
echo "<br>";


$param="--generate-ini ".$tmp_dir." ".$_POST["tasks"]." \"".$fen."\" ".$_POST["depth"];
echo "<br>";
echo "run: exec($AURIGA_EXE $param)";
echo "<br>";
$output=shell_exec($AURIGA_EXE." ".$param);
$array = explode("X", $output);
$perft_uuid=$array[1];
echo "perft_uuid $perft_uuid";
echo "<br>----GENERATED MASTER NODES----------<br>";

$csv=$tmp_dir."/data/".$perft_uuid."/perft.txt";
echo "mysqlimport -u$db_user -pdb_pass -h$db_host --local  --fields-terminated-by='|' $db_name $csv<br>";
shell_exec("mysqlimport -u$db_user -p$db_pass -h$db_host --local  --fields-terminated-by='|' $db_name $csv");


$csv=$tmp_dir."data/".$perft_uuid."/perft_tasks.txt";
echo "mysqlimport -u$db_user -pdb_pass -h$db_host --local  --fields-terminated-by='|' $db_name $csv<br>";
shell_exec("mysqlimport -u$db_user -p$db_pass -h$db_host --local  --fields-terminated-by='|' $db_name $csv");

$csv=$tmp_dir."data/".$perft_uuid."/task_fens.txt";
echo "mysqlimport -u$db_user -pdb_pass -h$db_host --local  --fields-terminated-by='|' $db_name $csv<br>";
shell_exec("mysqlimport -u$db_user -p$db_pass -h$db_host --local  --fields-terminated-by='|' $db_name $csv");


shell_exec("rm -f $tmp_dir/data/$perft_uuid/*.txt");
shell_exec("mv $tmp_dir/data/$perft_uuid $AURIGA_DATA");
shell_exec("rm -fr $tmp_dir");

header("Location: perft.php?uuid_perft=$perft_uuid");
exit;

?>

