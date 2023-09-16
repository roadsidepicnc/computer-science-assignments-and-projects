<?php

$con = mysqli_connect('localhost', 'root', '', '1901042640');

if (mysqli_connect_errno())
{
	echo "1: Connection failed";
	exit();
}

$categoryid = $_POST["categoryid"];
$customerid = $_POST["customerid"];
$viewgamesquery = "CALL filter_games_by_category( " . $customerid . ", " . $categoryid . ");";
$games = mysqli_query($con, $viewgamesquery) or die("4: view wallet query failed");
$content = "SUCCESSFUL\n";
while ($row = $games -> fetch_row())
{
	$content .= strval($row[0]) . "|";
	$content .= strval($row[1]) . "|";
	$content .= strval($row[2]) . "|";
	$content .= strval($row[3]) . "|";
	$content .= strval($row[4]) . "|";
	$content .= strval($row[5]);
	$content .= "\n";
}

echo $content;
?>