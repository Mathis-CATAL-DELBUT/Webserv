#!/usr/bin/php-cgi
<?php
$uploaddir = __DIR__ . '/uploads/';
$uploadfile = $uploaddir . basename($_FILES['userfile']['name']);
$tmp_name = $_FILES['userfile']['tmp_name'];
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