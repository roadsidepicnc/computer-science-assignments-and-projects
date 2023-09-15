<?php

$con = mysqli_connect('localhost', 'root', '', '1901042640');

if (mysqli_connect_errno())
{
	echo "1: Connection failed";
	exit();
}

$customerid = $_POST["customerid"];
$viewgamesquery = "CALL delete_customer( " . $customerid . ");";
$games = mysqli_query($con, $viewgamesquery) or die("4: view wallet query failed");
?>