#!/usr/bin/php-cgi
<?php


if (!empty($_POST))
{
	// echo 'Bonjour ' . $_POST["text1"] . htmlspecialchars($_POST["text2"]) . "!\n";
}
else
{
	exit(13);
}

// UPLOAD
$uploaddir = __DIR__ . '/uploads/';
$uploadfile = $uploaddir . basename($_FILES['fileToUpload']['name']);
$tmp_name = $_FILES['fileToUpload']['tmp_name'];
if (move_uploaded_file($tmp_name, $uploadfile)) {
	exit(13);
} 


?>