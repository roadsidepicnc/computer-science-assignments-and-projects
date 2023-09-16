<?php

$con = mysqli_connect('localhost', 'root', '', '1901042640');

if (mysqli_connect_errno())
{
	echo "1: Connection failed";
	exit();
}

$email = $_POST["email"];
$password = $_POST["password"];

$customeridcheckquery = "SELECT customer_id FROM customers WHERE customer_email = '" . $email . "'AND customer_password = '" . $password . "';";

$customeridcheck = mysqli_query($con, $customeridcheckquery) or die("2");

if (mysqli_num_rows($customeridcheck) != 1)
{
	echo "5: Login Error";
	exit();
}

$row = mysqli_fetch_row($customeridcheck);
echo "SUCCESSFUL|'" . $row[0] . "'";
?>