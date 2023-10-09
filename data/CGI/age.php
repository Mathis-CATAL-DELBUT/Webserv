#!/usr/bin/php-cgi
<?php

// Read form data from stdin (POST data)
$post_data = file_get_contents("php://stdin");
parse_str($post_data, $form_data);

// Get form data
$nom = isset($form_data['nom']) ? $form_data['nom'] : 'valeur par défaut';
$prenom = isset($form_data['prenom']) ? $form_data['prenom'] : 'default';
$age = isset($form_data['age']) ? intval($form_data['age']) : 0;

// Check user's age
$statut_age = ($age >= 18) ? 'majeur' : 'mineur';

// Write data to the file
$file_path = '.CGI.txt'; // Use the correct relative path to access the .CGI.txt file
$html_content = "<html><head><meta charset='UTF-8'></head><body><h1>Vous êtes $nom $prenom et vous êtes $statut_age.</h1></body></html>";

if (file_put_contents($file_path, $html_content) === false) {
    echo "Échec de l'écriture du fichier.";
} else {
    echo "Données enregistrées avec succès.";
}
?>
