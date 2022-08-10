<?php
$file = 'postData.txt'; 
$somecontent = @$_POST['dat'];
if($somecontent != '')
{
	$objDateTime = new DateTime('NOW');
	$somecontent = $objDateTime->format('Y-m-d H:i')."| ".$somecontent."\r\n";
	$current = file_get_contents($file);
	$current .= $somecontent;
	file_put_contents($file, $current);
}

?>