#!/usr/bin/php-cgi
<?php

// Get form data from the GET request
$nom = isset($_GET['nom']) ? $_GET['nom'] : 'valeur par défaut';
$prenom = isset($_GET['prenom']) ? $_GET['prenom'] : 'default';
$age = isset($_GET['age']) ? intval($_GET['age']) : 0;

// Check user's age
$statut_age = ($age >= 18) ? 'majeur' : 'mineur';

// Write data to the file
$file_path = '.CGI.txt'; // Use the correct relative path to access the .CGI.txt file
$html_content = "<html><head><meta charset='UTF-8'><link rel=\"stylesheet\" href=\"../style.css\"></head><body><h1>Vous êtes $nom $prenom et vous êtes $statut_age.</h1></body></html>";

if (file_put_contents($file_path, $html_content) === false) {
    echo "Échec de l'écriture du fichier.";
} else {
    echo "Données enregistrées avec succès.";
}
?>
