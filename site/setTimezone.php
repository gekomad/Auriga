<?php 
setcookie("time_zone", "pippo", time()+(3600*24*365));
if (isset($_COOKIE['timezone'])) {
  $timezone = $_COOKIE['timezone'];
}else{
 include 'mysql_connect.php';
 $ip = $_SERVER['REMOTE_ADDR'];
 if (array_key_exists('HTTP_X_FORWARDED_FOR', $_SERVER)) {
    $ip = array_pop(explode(',', $_SERVER['HTTP_X_FORWARDED_FOR']));
 }

 preg_match('/^(\d{1,3}\.\d{1,3}\.\d{1,3})\.\d{1,3}\z/', $ip, $re);
 $miniip = $re[1];

 $country="";
 $sql = "select time_zone from ip_country_timezone where network ='$miniip'";
 $result = $conn->query($sql);
 $time_zone="+0";
 setcookie("time_zone", "pippo", time()+3600*24*365);
 if ($result->num_rows > 0) {
	$row = $result->fetch_assoc();
	$time_zone=$row['time_zone'];
	if($time_zone>=0)$time_zone="+".$time_zone; 
	setcookie("time_zone", $time_zone, time()+3600*24*365);
 }
 $conn->close();
}
?>
