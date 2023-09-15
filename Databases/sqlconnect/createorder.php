<?php

$con = mysqli_connect('localhost', 'root', '', '1901042640');

if (mysqli_connect_errno())
{
	echo "1: Connection failed";
	exit();
}

$customerid = $_POST["customerid"];
$gameid = $_POST["gameid"];
$address = $_POST["address"];
$gameprice = $_POST["gameprice"];
settype($result, "integer"); 
$result = 31;

$orderquery = "CALL create_order('" . $customerid . "', '" . $gameid . "', '" . $address . "', " . $gameprice . ", " . "@result);";
mysqli_query($con, $orderquery);

$x = mysqli_query($con, "SELECT @result as output");
$row = $x->fetch_assoc();
echo $row['output'];
?>