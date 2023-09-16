<?php

$con = mysqli_connect('localhost', 'root', '', '1901042640');

if (mysqli_connect_errno())
{
	echo "1: Connection failed";
	exit();
}

$viewgamesquery = "SELECT * FROM sort_games_by_name;";
$games = mysqli_query($con, $viewgamesquery) or die("4: view games query failed");
$content = "SUCCESSFUL\n";

while ($row = $games -> fetch_row())
{
	$content .= strval($row[0]) . "|";
	$content .= strval($row[1]) . "|";
	$content .= strval($row[2]) . "|";
	$content .= strval($row[3]) . "|";
	$content .= strval($row[4]) . "|";
	$content .= strval($row[5]) . "|";
	$content .= strval($row[6]) . "|";
	$content .= strval($row[7]) . "|";
	$content .= strval($row[8]);
	$content .= "\n";
}

echo $content;
?>