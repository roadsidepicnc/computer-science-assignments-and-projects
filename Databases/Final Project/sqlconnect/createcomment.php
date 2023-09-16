<?php

$con = mysqli_connect('localhost', 'root', '', '1901042640');

if (mysqli_connect_errno())
{
	echo "1: Connection failed";
	exit();
}

$customerid = $_POST["customerid"];
$gameid = $_POST["gameid"];
$content = $_POST["content"];
settype($result, "integer"); 
$result = 31;

$orderquery = "CALL create_comment('" . $customerid . "', '" . $gameid . "', '" . $content .  "', " . "@result);";
mysqli_query($con, $orderquery);

$x = mysqli_query($con, "SELECT @result as output");
$row = $x->fetch_assoc();
echo $row['output'];
?>