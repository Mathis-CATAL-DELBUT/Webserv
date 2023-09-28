#!/usr/bin/python3

# En-têtes HTTP standard pour indiquer que la réponse est au format HTML
print("Content-Type: text/html\n")

# HTML de la page
html_content = """
<html>
<head><title>Script CGI en Python</title><link rel="stylesheet" href="../style.css"></head>
<body>
<h1>Bienvenue sur mon script CGI en Python</h1>
"""

# Obtenir la date et l'heure actuelles en Python
import datetime
current_date = datetime.datetime.now()
html_content += "<p>Date et heure actuelles : {}</p>".format(current_date)

html_content += "</body>\n</html>"

# Écrire le contenu HTML dans le fichier de sortie
with open('data/CGI/.CGI.txt', 'a') as file:
    file.write(html_content)

# Afficher le contenu HTML dans la réponse CGI
print(html_content)
