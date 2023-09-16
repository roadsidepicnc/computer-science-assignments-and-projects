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
$location = $_POST["location"];
settype($result, "integer"); 
$result = 31;

$registerquery = "CALL register_publisher('" . $name . "', '" . $password . "', '" . $email . "', " . $phone . ", '" . $location . "', " ."@result);";
mysqli_query($con, $registerquery);

$x = mysqli_query($con, "SELECT @result as output");
$row = $x->fetch_assoc();
echo $row['output'];
?>