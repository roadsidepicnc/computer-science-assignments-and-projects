<?php

$con = mysqli_connect('localhost', 'root', '', '1901042640');

if (mysqli_connect_errno())
{
	echo "1: Connection failed";
	exit();
}

$customerid = $_POST["customerid"];
settype($result, "integer"); 
$result = 31;

$orderquery = "CALL get_wallet_money('" . $customerid . "', @result);";
mysqli_query($con, $orderquery);

$x = mysqli_query($con, "SELECT @result as output");
$row = $x->fetch_assoc();
$content = "SUCCESSFUL\n" . $row['output'];
echo $content;
?>