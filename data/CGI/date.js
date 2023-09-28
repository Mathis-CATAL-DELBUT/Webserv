#!/usr/bin/node

const fs = require('fs');

const outputFile = '.CGI.txt';

const stdout = fs.createWriteStream(outputFile, { flags: 'a' }); // 'a' pour ajouter au fichier s'il existe

console.log = function(data) {
    stdout.write(data);
};

// En-têtes HTTP standard pour indiquer que la réponse est au format HTML
// console.log("Content-Type: text/html\n");

// HTML de la page
console.log("<html>");
console.log("<head><title>Script CGI en JavaScript</title><link rel='stylesheet' href='../style.css'></head>");
console.log("<body>");
console.log("<h1>Bienvenue sur mon script CGI en JavaScript</h1>");

// Obtenir la date et l'heure actuelles en JavaScript
const currentDate = new Date();
console.log(`<p>Date et heure actuelles : ${currentDate}</p>`);

console.log("</body>");
console.log("</html>");
