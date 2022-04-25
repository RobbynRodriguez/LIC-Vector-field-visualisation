# Visualisation de champs de vecteurs

## Description
Ce projet contient différentes représentations de champs de vecteur.
Nous avons pris pour exemple de représentation des surfaces et des sphères.
Une première représentation consiste à utiliser l'algorithme LIC (Line Integral Convolution) avec une texture de bruit 
et une fonction R2->R2 d'un champ de vecteurs.
Une seconde représentation consiste à créer un champ de vecteurs discret à partir des tangentes aux faces d'un maillage
et à associer chaque vecteur à un sommet pour ensuite représenter le champ vecteurs par des hedgehog glyphs. 

## Installation

Clion : ouvrir le projet, le build et lancer l'éxecutable "main".

En ligne de commande : Se positionner dans le dossier
```bash
mkdir build
cd build
cmake ..
make
./main
```

## Usage

Appuyer sur P permet de lier la souris à la fenêtre, O permet de faire l'inverse.

Les touches Z-Q-S-D et la souris (si la souris est liée à la fenêtre) permettent de se déplacer dans l'espace,
E-A permettent de monter et descendre respectivement.

Appuyer sur W (clavier français) permet d'afficher le maillage triangulaire, X permet de faire l'inverse.

Appuyer sur N permet d'afficher les normales à la surface, B permet de les enlever.

Appuyer sur G permet d'afficher les glyphes des tangentes à la surface, H permet de les enlever.

Appuyer sur 1 permet de faire tourner la lumière, 0 permet de l'arrêter.
