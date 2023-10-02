#!/usr/bin/php-cgi
<?php

// Récupération des données du formulaire
$nom = isset($_POST['nom']) ? $_POST['nom'] : 'valeur par défaut';
$prenom = isset($_POST['prenom']) ? $_POST['prenom'] : 'default';
$age = isset($_POST['age']) ? intval($_POST['age']) : 0;

// Vérifie si l'âge de l'utilisateur
$statut_age = ($age >= 18) ? 'majeur' : 'mineur';

// Écriture des données dans le fichier
$file_path = '../CGI/.CGI.txt'; // Utilisez le chemin relatif correct pour accéder au fichier .CGI.txt
$html_content = "<html><head><meta charset='UTF-8'></head><body><h1>Vous êtes $nom $prenom et vous êtes $statut_age.</h1></body></html>";

if (file_put_contents($file_path, $html_content) === false) {
    echo "Échec de l'écriture du fichier.";
} else {
    echo "Données enregistrées avec succès.";
}
