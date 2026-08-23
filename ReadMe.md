# Raytracer

Raytracer est une application C++17 qui affiche une scène ray-tracée par un
compute shader OpenGL. L'interface d'édition est construite avec GLFW et Dear
ImGui.

## Prérequis

L'environnement principal est Linux ou WSL avec un affichage OpenGL disponible.
Il faut également disposer de :

- CMake 3.16 ou plus récent ;
- un compilateur C++17 et `make` ;
- `pkg-config` et `libconfig++` ;
- une carte ou un pilote compatible OpenGL 4.3 ;
- Git, utilisé par CMake pour télécharger GLFW 3.4 lors de la configuration.

Sur Debian ou Ubuntu :

```bash
sudo apt update
sudo apt install -y build-essential cmake git pkg-config libconfig++-dev libgl1-mesa-dev
```

## Compilation

Depuis la racine du dépôt :

```bash
cmake -S . -B build
cmake --build build
```

L'exécutable `raytracer` est généré à la racine du projet. GLFW est récupéré et
compilé automatiquement par CMake ; `glad` et Dear ImGui sont fournis dans
`vendor/`.

## Utilisation

```bash
./raytracer
./raytracer test.cfg
./raytracer --help
```

- sans fichier de scène, une scène de démonstration est créée par le renderer ;
- avec un fichier `.cfg`, la caméra est initialisée à partir de ce fichier ;
- `-h` et `--help` affichent l'aide intégrée.

Le chemin du shader `shaders/raytrace.comp` est relatif au répertoire courant.
Lancer le programme depuis la racine du dépôt garantit donc que le shader sera
trouvé.

## Fichier de scène

Les fichiers utilisent la syntaxe de [libconfig](https://hyperrealm.github.io/libconfig/).
La version actuelle lit les champs suivants :

```cfg
camera:
{
    resolution = { width = 1920; height = 1080; };
    position = { x = 50.0; y = 0.0; z = 0.0; };
    rotation = { x = -1.0; y = 0.0; z = 0.0; };
    fieldOfView = 80.0;
};
```

`camera.resolution` est prévue par le lecteur, mais n'est pas encore utilisée
par `Core` ; la taille de la texture de rendu suit actuellement la taille du
panneau `Viewport` de l'interface.

Les sections `primitives` et `lights` peuvent apparaître dans un fichier de
configuration, mais elles ne sont pas encore chargées dans la scène. Le
renderer construit actuellement deux sphères de démonstration en mémoire.

## Interface et contrôles

L'interface contient les panneaux `Viewport`, `Hierarchy`, `Add Object` et
`Inspector`. Le viewport est recalculé lorsqu'il est redimensionné ou lorsque
la caméra change.

Contrôles caméra :

| Touche | Action |
| --- | --- |
| `Z` / `S` | avancer / reculer |
| `Q` / `D` | se déplacer à gauche / droite |
| `Espace` / `Ctrl gauche` | monter / descendre |
| flèches gauche / droite | tourner horizontalement |
| flèches haut / bas | tourner verticalement |

La vitesse de déplacement et de rotation, ainsi que l'affichage des
coordonnées, sont réglables dans `Inspector`.

## Organisation du code

```text
project/
  includes/   # interfaces et en-têtes
  src/        # implémentation C++
shaders/      # compute shaders OpenGL
vendor/       # glad et Dear ImGui
CMakeLists.txt
test.cfg      # exemple de configuration
```

Les principales responsabilités sont :

- `Core` : initialise l'interface et le renderer, puis pilote la boucle
  principale ;
- `CfgReader` : lit et valide la configuration de la caméra ;
- `Renderer` : prépare le shader, la texture de sortie et la scène GPU ;
- `UI` : gère GLFW, ImGui, les panneaux et les contrôles caméra.

## Dépannage

- Si GLFW ne s'initialise pas, vérifier que la session Linux dispose d'un
  affichage (`DISPLAY`/Wayland) et d'un pilote OpenGL fonctionnel.
- Une erreur de compilation liée à `libconfig++` indique généralement que
  `libconfig++-dev` ou `pkg-config` manque.
- Une erreur de shader ou de fichier introuvable peut venir d'un lancement hors
  de la racine du dépôt.
