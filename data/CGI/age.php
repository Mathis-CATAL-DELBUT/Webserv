#!/usr/bin/php-cgi
<?php

$nom = isset($_GET['nom']) ? $_GET['nom'] : 'valeur par défaut';
$prenom = isset($_GET['prenom']) ? $_GET['prenom'] : 'default';
$age = isset($_GET['age']) ? intval($_GET['age']) : 0;

$statut_age = ($age >= 18) ? 'majeur' : 'mineur';

$file_path = '.CGI.txt';
$html_content = "<html><head><meta charset='UTF-8'><link rel=\"stylesheet\" href=\"../style.css\"></head><body><h1>Vous êtes $nom $prenom et vous êtes $statut_age.</h1></body></html>";

if (file_put_contents($file_path, $html_content) === false) {
	exit(13);
} 
?>
