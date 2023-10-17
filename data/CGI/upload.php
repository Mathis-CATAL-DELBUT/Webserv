#!/usr/bin/php-cgi
<?php

if($_SERVER['REQUEST_METHOD'] == 'POST')
{
	echo 'Request was a POST';
}
if (!empty($_POST))
{
	// echo 'Bonjour ' . $_POST["text1"] . htmlspecialchars($_POST["text2"]) . "!\n";
}
else
{
	echo "\$_POST is empty\n";
}

// UPLOAD
$uploaddir = __DIR__ . '/uploads/';
$uploadfile = $uploaddir . basename($_FILES['fileToUpload']['name']);
$tmp_name = $_FILES['fileToUpload']['tmp_name'];
echo $tmp_name;
echo '<pre>';
if (move_uploaded_file($tmp_name, $uploadfile)) {
	echo "File upload successfully\n";
} else {
	echo "Couldn't upload the file.\n";
}
echo 'Debug infos :';
print_r($_FILES);


?>