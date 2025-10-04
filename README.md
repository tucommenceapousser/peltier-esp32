# 🌡️ peltier-esp32
Micropython pour contrôler un **Peltier TEC1-12715** avec un **ESP32** via un serveur web.

---

## ⚡ Fonctionnalités principales

| # | Fonctionnalité |
|---|----------------|
| 1 | **Contrôle PWM** du Peltier via slider et boutons +/- |
| 2 | **Affichage temps réel** de la température avec rafraîchissement automatique |
| 3 | **Wi-Fi setup simple** (station mode) |
| 4 | **Protection logicielle** : PWM limité à 1023 max |
| 5 | **Compatible Linux / MicroPython** → pas besoin de C ni Arduino IDE |

---

## 🔹 Détails des fonctionnalités

- **Slider interactif** : contrôle PWM 0-1023  
- **Boutons + / -** : ajustement par pas de 50  
- **Rafraîchissement auto** : page mise à jour toutes les 5 s  
- **Affichage dynamique** : température réelle lue par DS18B20  
- **Design dashboard hacker fluo** : fond sombre, texte vert, titres cyan  

---

## 🛠️ Matériel recommandé

| Composant | Description |
|-----------|-------------|
| **ESP32 WROOM DevKit V1** | Carte MicroPython avec Wi-Fi et PWM |
| **MOSFET IRLZ44N** | Pilote le Peltier via PWM |
| **Fusible 20 A** | Protège le Peltier et l’alimentation 12 V |
| **Peltier TEC1-12715** | Refroidissement principal |
| **Ventilateur 12 V** | Dissipation côté chaud |
| **Dissipateur** | Pour module Peltier |
| **Capteur DS18B20** | Lecture température |
| **Alimentation 12 V** | Capable de fournir courant nécessaire (~15 A max) |

---

## 💡 Notes de montage

- **GND commun** : un point unique où convergent tous les GND pour éviter les problèmes de référence électrique
- **PWM** : GPIO12 de l’ESP32 → Gate MOSFET avec résistance 220 Ω
- **Protection** : fusible 20 A sur le +12 V du Peltier
- **DS18B20** : GPIO13 ESP32, pull-up 4.7kΩ entre Data et VCC 3.3 V

---

## 🚀 Setup logiciel

1. Flasher **MicroPython** sur l’ESP32
2. Installer `ampy` ou `rshell` pour uploader le code Python
3. Modifier `SSID` et `PASSWORD` dans le script
4. Brancher le Peltier, ventilateur et capteur selon le schéma

---
