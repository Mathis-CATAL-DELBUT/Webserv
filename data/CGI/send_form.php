#!/usr/bin/php-cgi
<?php

// Read form data from stdin (POST data)
$post_data = file_get_contents("php://stdin");
parse_str($post_data, $form_data);

// Get form data
$nom = isset($form_data['nom']) ? $form_data['nom'] : '';
$prenom = isset($form_data['prenom']) ? $form_data['prenom'] : '';
$email = isset($form_data['email']) ? $form_data['email'] : '';
$message = isset($form_data['message']) ? $form_data['message'] : '';

// Create the HTML response
$file_path = ".CGI.txt";
$html_content = "<!DOCTYPE html>
<html lang=\"en\">
<head>
    <link rel=\"icon\" href=\"image/w.webp\">
    <link rel=\"stylesheet\" href=\"style.css\">
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
        <a href=\"index.html\" class=\"go_back\">Go back</a>
    </div>
</body>
</html>";

if (file_put_contents($file_path, $html_content) === false) {
	echo "Échec de l'écriture du fichier.";
} else {
	echo "Données enregistrées avec succès.";
}
?>
