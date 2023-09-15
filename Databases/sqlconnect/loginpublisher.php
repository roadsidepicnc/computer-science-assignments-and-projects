<?php

$con = mysqli_connect('localhost', 'root', '', '1901042640');

if (mysqli_connect_errno())
{
	echo "1: Connection failed";
	exit();
}

$email = $_POST["email"];
$password = $_POST["password"];

$publisheridcheckquery = "SELECT publisher_id FROM publishers WHERE publisher_email = '" . $email . "'AND publisher_password = '" . $password . "';";

$publisheridcheck = mysqli_query($con, $publisheridcheckquery) or die("2");

if (mysqli_num_rows($publisheridcheck) != 1)
{
	echo "5: Login Error";
	exit();
}

$row = mysqli_fetch_row($publisheridcheck);
echo "SUCCESSFUL|'" . $row[0] . "'";
?>