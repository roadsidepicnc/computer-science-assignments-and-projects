<?php

$con = mysqli_connect('localhost', 'root', '', '1901042640');

if (mysqli_connect_errno())
{
	echo "1: Connection failed";
	exit();
}

$userid = $_POST["userid"];
$gameid = $_POST["gameid"];
$baseprice = $_POST["baseprice"];
$discount = $_POST["discount"];
$result = 31;

$orderquery = "CALL update_game('" . $userid . "', '" . $gameid . "', '" . $baseprice . "', '" . $discount . "', " . "@result);";
mysqli_query($con, $orderquery);

$x = mysqli_query($con, "SELECT @result as output");
$row = $x->fetch_assoc();
echo $row['output'];
?>