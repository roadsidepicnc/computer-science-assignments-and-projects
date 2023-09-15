<?php

$con = mysqli_connect('localhost', 'root', '', '1901042640');

if (mysqli_connect_errno())
{
	echo "1: Connection failed";
	exit();
}

$name = $_POST["name"];
$email = $_POST["email"];
$phone = (int) $_POST["phone"];
$password = $_POST["password"];
settype($result, "integer"); 
$result = 31;

$registerquery = "CALL register_customer('" . $name . "', '" . $password . "', '" . $email . "', " . $phone . ", " . "@result);";
mysqli_query($con, $registerquery);

$x = mysqli_query($con, "SELECT @result as output");
$row = $x->fetch_assoc();
echo $row['output'];
?>