# CRS - Bus CAN

## Objectifs

* 1 - A chaque carte est associé un identifiant Can
* 3pts - Les Données de tous les capteurs sont récupérées et affichées (simultanément)  sur l’IHM du PC Host, au moins en texte
* 1pts - Les échanges entre l’IHM et les cartes sont périodiques
* 2pts - Utilisation de Widgets pour l’affichage des données des capteurs (pression / luminosité / distance / vitesse du vent)
* 2pts - L'orientation de la carte IMU est représentée graphiquement, avec openGL, dans l’IHM
* 2pts - Mise en œuvre de l’algorithme de Madgwick (ou équivalent)
* 3pts - Les données de pression / température / humidité sont récupérées et traitées
* 2pts - Le moteur est contrôlable depuis l’IHM
* 1pts - Le moteur se met à tourner au delà d’une certaine vitesse de vent
* 3pts -  Commutation entre mesure de luminosité et mesure de distance depuis l’IHM

## À faire STM32

- [X] Acquérir les donnés de luminosité et distance (Bus CAN)
- [X] Acquérir les donnés de pression
- [X] Acquérir les donnés de vitesse du vent
- [X] Acquérir les donnés de humidité
- [X] Acquérir les donnés de l’accéléromètre

## À faire IHM

- [X] Montrer les valeurs de luminosité et distance sur l'interface
- [X] Commuter parmi luminosité et distance dans l'interface
- [X] Montrer les donnés de pression
- [X] Montrer les donnés de vitesse du vent
- [X] Montrer les donnés de humidité
- [ ] Montrer l'orientation de la carte IMU
