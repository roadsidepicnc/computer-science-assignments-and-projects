<?php

$con = mysqli_connect('localhost', 'root', '', '1901042640');

if (mysqli_connect_errno())
{
	echo "1: Connection failed";
	exit();
}

$viewgamesquery = "SELECT * FROM view_all_categories;";
$games = mysqli_query($con, $viewgamesquery) or die("4: view games query failed");
$content = "SUCCESSFUL\n";

while ($row = $games -> fetch_row())
{
	$content .= strval($row[0]) . "|";
	$content .= strval($row[1]) . "|";
	$content .= strval($row[2]);
	$content .= "\n";
}

echo $content;
?>