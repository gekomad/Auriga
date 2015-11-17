

<?php

include 'set_variable.php';	

session_start();
shell_exec("find tmp/* -type f -mmin +1 -exec rm {} \;");
if($_POST['captcha'] != $_SESSION['digit']) {
	session_destroy();
	sleep(2);
	header("Location: error_captcha.html");//TODO fare pagina
	exit;
}
session_destroy();

$fen=$_POST["fen"];
$fen=trim($fen);
$fen=substr($fen,0,100);

$depth=$_POST["depth"];
$tasks=$_POST["tasks"];
if (!is_numeric($depth)||!is_numeric($tasks)||$tasks>5000||$depth>100) {
    echo "error";
	return;
} 


include 'mysql_connect.php';
$sql = "SELECT count(1)count FROM perft where fen ='".$fen."' and depth=".$depth." and tasks = ".$tasks;

$result = $conn->query($sql);
$row = $result->fetch_assoc();
if($row["count"]!=0){
	$conn->close();
	echo "fen exists<br>";//TODO fare pagina
	return;
}
$conn->close();


$tmp_dir="/tmp/".rand()."/";
mkdir($tmp_dir);
//echo "tmp_dir=$tmp_dir<br>";

$AURIGA_DATA = $auriga_root."/data";
//mkdir($auriga_root);
mkdir($AURIGA_DATA);
$AURIGA_EXE="timeout --signal=9 30s ./auriga";



/*preg_match('/(.+) (.+) (.+) (.+) (.+ .+)/', $fen, $re);
$strs = $re[1]." ".$re[2]." ".$re[3]." ".$re[4];
$fen=$strs ;*/

//echo $fen;
//echo "<br>";


$param="--generate-ini ".$tmp_dir." ".$tasks." \"".$fen."\" ".$depth;
//echo "<br>";
//echo "run: exec($AURIGA_EXE $param)";
//echo "<br>";
$output=shell_exec($AURIGA_EXE." ".$param);
$array = explode("X", $output);
$perft_uuid=$array[1];
//echo "perft_uuid $perft_uuid";

$csv=$tmp_dir."/data/".$perft_uuid."/perft.txt";
//echo "mysqlimport -u$db_user -pdb_pass -h$db_host --local  --fields-terminated-by='|' $db_name $csv<br>";
shell_exec("mysqlimport -u$db_user -p$db_pass -h$db_host --local  --fields-terminated-by='|' $db_name $csv");


$csv=$tmp_dir."data/".$perft_uuid."/perft_tasks.txt";
//echo "mysqlimport -u$db_user -pdb_pass -h$db_host --local  --fields-terminated-by='|' $db_name $csv<br>";
shell_exec("mysqlimport -u$db_user -p$db_pass -h$db_host --local  --fields-terminated-by='|' $db_name $csv");

$csv=$tmp_dir."data/".$perft_uuid."/task_fens.txt";
//echo "mysqlimport -u$db_user -pdb_pass -h$db_host --local  --fields-terminated-by='|' $db_name $csv<br>";
shell_exec("mysqlimport -u$db_user -p$db_pass -h$db_host --local  --fields-terminated-by='|' $db_name $csv");


shell_exec("rm -f $tmp_dir/data/$perft_uuid/*.txt");
shell_exec("mv $tmp_dir/data/$perft_uuid $AURIGA_DATA");
shell_exec("rm -fr $tmp_dir");

header("Location: perft.php?uuid_perft=$perft_uuid");
exit;

?>

