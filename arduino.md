## 🛠️ Installation et utilisation avec l'IDE Arduino

### 1. Téléchargement du code

Télécharge le fichier principal du projet ici :  
[peltier.ino](https://raw.githubusercontent.com/tucommenceapousser/peltier-esp32/refs/heads/main/peltier.ino)

### 2. Préparation de l'environnement Arduino

#### a. Installation de la carte ESP32

1. Ouvre l'IDE Arduino.
2. Va dans **Fichier > Préférences**.
3. Dans le champ **URL de gestionnaire de cartes supplémentaires**, ajoute l'URL suivante :

```bash
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
```

4. Clique sur **OK** pour enregistrer.
5. Va dans **Outils > Type de carte > Gestionnaire de cartes**.
6. Recherche "ESP32" et installe le paquet **ESP32 by Espressif Systems**.

#### b. Sélection de la carte et du port

1. Va dans **Outils > Type de carte** et sélectionne ton modèle d'ESP32 (ex. **ESP32 Dev Module**).
2. Va dans **Outils > Port** et choisis le port correspondant à ton ESP32.

### 3. Téléversement du code

1. Ouvre le fichier `peltier.ino` dans l'IDE Arduino.
2. Clique sur le bouton **Téléverser** (flèche droite) pour compiler et transférer le code vers l'ESP32.

### 4. Configuration Wi-Fi

Avant de téléverser le code, modifie les lignes suivantes dans le fichier `peltier.ino` avec tes informations Wi-Fi :

```cpp
const char* ssid = "TonSSID";
const char* password = "TonMotDePasse";
```

### 5. Accès au serveur web

Une fois le téléversement terminé et l'ESP32 redémarré, ouvre le Moniteur série dans l'IDE Arduino (vitesse 115200 bauds). Tu verras l'adresse IP attribuée à ton ESP32, par exemple :

```bash
Connecté!
IP: 192.168.1.100
```

Entrez cette adresse IP dans ton navigateur web pour accéder à l'interface de contrôle du Peltier.
