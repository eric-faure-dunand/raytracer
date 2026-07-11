# Raytracer — README

## Installation (WSL / Linux)

1. Installer les dépendances système :

```bash
sudo apt update
sudo apt install -y cmake libconfig++-dev build-essential
```

2. Générer la build et compiler :

```bash
cmake . -B build
cmake --build build
```

3. (Optionnel) Sur Windows, installez WSL puis suivez les étapes ci‑dessus dans la distribution Linux.

## Structure de projet

Le projet contient :

- `project/` : code source C++
- `plugins/` : dossiers de plugins (un plugin par dossier)
- `shared/` : en-têtes partagés (IObject, ILight, etc.)

---

## Système de plugins (.so)

Le moteur charge dynamiquement des plugins compilés en bibliothèques partagées `.so`. Un plugin doit fournir soit un objet (IObject), soit une lumière (ILight) — compilé en `raytracer_<nom>.so`.

### Arborescence d'un plugin

```
plugins/
  <nom_du_plugin>/
    fichier.cpp
    fichier.hpp
    README.md       # description des paramètres attendus
    (pas de sous-dossiers)
```

Le CMake du projet produit `raytracer_<nom_du_plugin>.so` à partir du dossier.

### Includes

- Inclure `ILight.hpp` ou `IObject.hpp` selon le type du plugin.
- Les en-têtes dans `shared/` sont disponibles globalement — n'utilisez pas de chemins relatifs.

### Fonctions obligatoires

Chaque plugin doit exposer deux fonctions `extern "C"` :

1. `GetSoType()` — renvoie le type du plugin (enum `SoTypeEnum` dans `shared/SoType.hpp`) :

```cpp
extern "C" SoTypeEnum GetSoType(void) {
    return OBJECT; // ou LIGHT
}
```

2. `GetObject()` — constructeur exposé qui prend une `std::map<std::string, std::string>` :

```cpp
extern "C" IObject* GetObject(std::map<std::string, std::string> params) {
    return new MonObjet(params);
}
```

L'instance retournée doit être construite à partir de la map de paramètres (voir section suivante).

### Paramètres — format attendu

Les paramètres sont extraits du fichier `.cfg` et fournis au plugin sous forme d'une `map` clé-valeur.

Exemple `.cfg` :

```
primitives: {
  spheres: [
    { x = 60; y = 5; z = 40; r = 25; color = { r = 255; g = 64; b = 64; }; }
  ]
}
```

La map générée par le parser (notation pointée pour les valeurs imbriquées) :

| Clé | Valeur |
|-----|--------|
| `x` | `60` |
| `y` | `5` |
| `z` | `40` |
| `r` | `25` |
| `color.r` | `255` |
| `color.g` | `64` |
| `color.b` | `64` |

Pour les champs imbriqués, utilisez `.` pour accéder aux sous-champs (ex : `color.r`).

Chaque plugin doit documenter les paramètres attendus dans son propre `README.md` au sein du dossier du plugin.

---

## Compilation des plugins

Après avoir configuré et compilé le projet (voir section Installation), les plugins présents dans `plugins/` seront automatiquement compilés si le CMake du projet est configuré pour ça. Le binaire final du plugin sera nommé `raytracer_<nom>.so`.

Flags de compilation courants : `-Wall -Wextra -Werror`.

---

## Notes utiles

- Les headers dans `shared/` sont fournis au build, pas besoin de chemins relatifs.
- Si un plugin ne se charge pas, vérifiez les messages d'erreur au runtime (le loader affiche les erreurs `dlopen`/`dlsym`).
- Documentez toujours les paramètres de chaque plugin dans son `README.md` pour faciliter l'intégration.
