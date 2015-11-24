<?PHP
session_start();

$ran=rand(1000, 1999);
$_SESSION['digit'] = (string)$ran;
$image=rand(1, 9999999);
shell_exec("cp captcha/$ran.jpg tmp/$image.jpg");
?>
