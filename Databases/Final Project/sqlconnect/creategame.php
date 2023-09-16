<?php

$con = mysqli_connect('localhost', 'root', '', '1901042640');

if (mysqli_connect_errno())
{
	echo "1: Connection failed";
	exit();
}

$userid = $_POST["userid"];
$gamename = $_POST["gamename"];
$baseprice = $_POST["baseprice"];
$discount = $_POST["discount"];
$releaseyear = $_POST["releaseyear"];
$rating = $_POST["rating"];
$categoryid = $_POST["categoryid"];
$result = 31;

$orderquery = "CALL create_game('" . $userid . "', '" . $gamename . "', '" . $baseprice . "', '" . $discount . "', '" . $releaseyear . "', '"  . $rating . "', '" . $categoryid . "', ". "@result);";
mysqli_query($con, $orderquery);

$x = mysqli_query($con, "SELECT @result as output");
$row = $x->fetch_assoc();
echo $row['output'];
?>