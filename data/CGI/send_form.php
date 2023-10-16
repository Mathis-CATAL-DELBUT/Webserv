#!/usr/bin/php-cgi
<?php

// Get form data from the POST request
$nom = isset($_POST['nom']) ? $_POST['nom'] : '';
$prenom = isset($_POST['prenom']) ? $_POST['prenom'] : '';
$email = isset($_POST['email']) ? $_POST['email'] : '';
$message = isset($_POST['message']) ? $_POST['message'] : '';

// Create the HTML response
$file_path = ".CGI.txt";
$html_content = "<!DOCTYPE html>
<html lang=\"en\">
<head>
    <link rel=\"icon\" href=\"../image/w.webp\">
    <link rel=\"stylesheet\" href=\"../style.css\">
    <meta charset=\"UTF-8\">
    <title>Form Response</title>
</head>
<body>
    <div id=\"response\">
        <h1>Form Response</h1>
        <p>Nom: $nom</p>
        <p>Prenom: $prenom</p>
        <p>Email: $email</p>
        <p>Message: $message</p>
        <a href=\"../index.html\" class=\"go_back\">Go back</a>
    </div>
</body>
</html>";

if (file_put_contents($file_path, $html_content) === false) {
    echo "Échec de l'écriture du fichier.";
} else {
    echo "Données enregistrées avec succès.";
}
?>
