<?php

$con = mysqli_connect('localhost', 'root', '', '1901042640');

if (mysqli_connect_errno())
{
	echo "1: Connection failed";
	exit();
}

$customerid = $_POST["customerid"];
$moneyamount = $_POST["moneyamount"];

$viewgamesquery = "CALL add_money_to_wallet( " . $customerid . ", " . $moneyamount . ");";
$games = mysqli_query($con, $viewgamesquery) or die("4: view wallet query failed");
?>