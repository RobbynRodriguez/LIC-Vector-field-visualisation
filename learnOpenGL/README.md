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

Appuyer sur echap permet de fermer la fenêtre


## Contenu 

Classe Axes : contient le mesh des axes

Classe Camera : contient toute la gestion de la caméra (génération et déplacement)

Classe Mesh : contient les fonctions pour configurer les buffers des maillages, faire le draw et calculer le champ de
vecteurs tangent à chaque face du maillage.

Classe Shader : contient la gestion des shaders 

Classe Sphere : contient la création du mesh d'une sphère 

Classe BézierSurface : contient la création d'une surface de Bézier à partir de points de contrôle

Structure.h : contient des structures utilitaires

Dossier shaders : contient les différents shaders utilisés, notamment :
-le shader du LIC (qui va permettre de visualiser un champ de vecteurs d'une fonction donnée sur une texture) 
-le shader des glyph qui permet de visualiser un champ de vecteurs discret avec des hedgehog. 

Dossier textures : contient les différentes textures utilisées.