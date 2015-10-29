<html>
 <head>

  <meta name="robots" content="noindex">
  <title>generate masterfile4</title>
 </head>
 <body>

<?php
$tmp_dir="/tmp/".rand()."/";
mkdir($tmp_dir);
echo "tmp_dir=$tmp_dir<br>";

//$AURIGA_DATA = $_ENV['OPENSHIFT_DATA_DIR']."/auriga_root";
$AURIGA_DATA = "/var/www/html/auriga/auriga_root";
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

$csv=file_get_contents($tmp_dir."/data/".$perft_uuid."/perft_table.tmp");
$iparr = split ("\|", $csv); 


if(count($iparr)==4){
	include 'mysql_connect.php';

	
	echo $db_host."a<br>";
	echo $db_user."b<br>";
	
	echo $db_name."d<br>";
	echo $db_port."e<br>";

	
	$query = "INSERT INTO perft (uuid_perft, fen, depth,tasks)VALUES ('$iparr[0]', '$iparr[1]',$iparr[2],$iparr[3])";
	echo $query;

	if ($conn->query($query) === TRUE) {
	    echo "New record created successfully";		
	} else {
	    echo "Error: " . $query . "<br>" . $conn->error;
	}
	
	
	echo "<br>----end mysql---------";
	
}

$csv2=$tmp_dir."data/".$perft_uuid."/perft_tasks_table.tmp";

$handle = fopen($csv2, "r");
if ($handle) {
    while (($line = fgets($handle)) !== false) {
        $iparr2 = split ("\|", $line); 
		//echo $iparr2[0]."--------".$iparr2[1]."<br>";
		$query = "INSERT INTO perft_tasks (uuid_perft, uuid_task)VALUES ('$iparr2[0]', '$iparr2[1]')";
	
		//echo $query;		
	
		$conn->query($query);
    }

    fclose($handle);
}
$conn->close();

shell_exec("rm -f $tmp_dir/data/$perft_uuid/*.tmp");
shell_exec("mv $tmp_dir/data/$perft_uuid $AURIGA_DATA/data");
shell_exec("rm -fr $tmp_dir");



?>

 </body>
</html>
