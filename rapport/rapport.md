# Compte-rendu du projet Systèmes Embarqués

## Logiciel embarqué de traitement

### Fonctionnement

Chaque groupe de données issues des 50 capteurs est traité simultanément.

Le traitement consiste à considérer les mêmes pixels/lignes de tous les capteurs.
TODO: schéma
Les données sont copiées dans un buffer stockant une ligne de chaque capteur, pour être traitées ensemble.

Ce buffer est alors trié en place en utilisant le moteur de tri fourni.
La médiane est ensuite calculée avec la formule
$$
M =
    \begin{cases}
        arr[\tfrac{size}{2}] + arr[\tfrac{size}{2} - 1] & \text{si}\ size\ \text{est pair} \\
        arr[\tfrac{size}{2}] + arr[\tfrac{size}{2} - 1] & \text{si}\ size\ \text{est impair} \\
    \end{cases}
$$

Les données étant triées, calculer la moyenne avec réjection des valeurs aberrations devient facile.
En effet, la réjection consiste à éliminer les valeurs qui sont trop écartées de la médiane. Les éléments qui sont à sommer sont donc consécutifs dans le tableau, et il suffit donc de déterminer les bornes de ces éléments avant de les sommer, plutôt que de parcourir tout le tableau et vérifier si la condition est remplie sur chaque élément.
Pour déterminer les bornes, nous passont par une dichotomie de chaque côté. Nous avons pu déterminer que cela faisait gagner quelques millisecondes comparé à un parcours complet du tableau, malgré un parcours non linéaire des données en mémoire. Cela s'explique par le fait que le cache peut tenir suffisamment bien les données d'une ligne (50 pixels) pour faire disparaître les latences.
TODO: schéma

Naturellement, il y a quelques situations qui pourraient retourner des données erronnées, notamment lorsque la réjection exclut toutes les données du tableau, donnant une division par zéro dans le calcul de la moyenne. Cela n'est cependant possible que lorsque le tableau est de taille paire avec une tolérance d'erreur inférieure à l'écart entre la médiane et les valeurs utiliées pour calculer cette médiane.
TODO: schéma

La moyenne est ensuite stockée dans un vecteur moyenne, ou dans le vecteur de référence, statique, s'il s'agit de la première itération. La vérification de si le pixel est considéré comme un point chaud est déterminé dans la foulée.

### Algorithme de tri

Trois algorithmes de tri ont été testés: 
- L'algorithme *quicksort* du moteur de tri fourni
- L'algorithme *heapsort* du moteur de tri fourni
- L'algorithme de tri la bibliothèque standard

La mesure des performances et caractéristiques de chacun a conduit au choix de l'algorithme *quicksort* du moteur de tri fourni.

La mesure du temps donnait, sur 50 données, en moyenne, sur les données des samples :
- environ 0.20ms pour le *heapsort*
- environ 0.10ms pour le *quicksort*
- environ 0.09ms pour l'algorithme de tri de la biblio standard (sachant que c'est du *quicksort* principalement sous le capot)

Etant donné que l'algorithme standard ne permet pas d'avoir beaucoup de contrôle sur le code sous-jacent et ne propose qu'un gain anecdotique, celui-ci est éliminé des candidats.
Les deux algorithmes restants ont une complexité moyenne en $\mathcal{O}(n\log n)$. Cependant, la complexité dans le pire cas est $\mathcal{O}(n^2)$ pour le *quicksort*, alors qu'elle reste en $\mathcal{O}(n\log n)$ pour le *heapsort*.
Cependant, des tests sur les cas difficiles et aléatoires ont montrés que le *quicksort* restait proche des 0.10ms et le *heapsort* autour des 0.20ms. Cela s'explique par le faible nombre de données à trier et le passage rapide au tri par insertion sur les appels profonds du *quicksort* qui l'empêche de perdre trop de temps.

L'algorithme retenu est donc le *quicksort*.

### Tests du programme de traitement

Samples, d'abord chaque fonction spécifique isolée puis l'ensemble d'une acquisition.

### Performances et charge CPU

Mesures de performances effectuées sur TSIM avec les samples fournies et des données aléatoires.

Mesure des performances:
| Algorithme | Cycles | Instructions | Temps | Variance | 
|-|-|-|-|
| Copie des pixels dans le buffer | 67460 | 34747 | 1.35ms | 20cycles, 0µs |
| Tri des données | 469774 |  |  | 1474cycles,  |
| Calcul de la médiane |  |  |  |  |
| Calcul des bornes |  |  |  |  |
| Calcul de la moyenne |  |  |  |  |
| Calcul des hotspots |  |  |  |  |
| Traitement total |  |  |  |  |
Charge CPU finale:

On peut noter que sur le traitement total des données, le tri représente 9ms sur 13ms, soit environ 70% de l'occupation CPU par le programme.

Consommation mémoire:
| Variable | Type et taille | Mémoire consommée |
|-|-|-|
| Vecteur de référence |  |  |
| Buffer des pixels |  |  |
| Vecteur des moyennes |  |  |
| Vecteur traité |  |  |
| Autres variables |  |  |

## Architecture

### Overview/Synoptique

Choix du processeur

### Mémoires et justification

On utilise différent types de mémoire chacune choisie pour convenir de manière optimale à l'utilisation prévue

Pour le stockage à court terme des données transférées par SpaceWire/RMAP et contenant la mémoire de travail du processur Leon, on a choisi deux SDRAM de 512Mb chacune pour avoir une redondance. Cette quantité de stockage permets le stockage des l'intégralité des données des capteurs pendant plusieurs secondes tout en conservant de la capacité pour le fonctionnement de l'algorithme.
Les deux mémoires sont utilisées simulatanément en miroir, avec un multiplexage dans les FPGA utilisant le RMAP via le SpaceWire

Stockage de masse : Pour le stockage des données on prévoit  deux NAND FLASH redondantes en mirroir contenant chacune l'intégralité des données pour redondance, avec option d'utilisation séquentielle en cas de problèmes de communication avec le sol. Ayant 8,64GB de données générées par jour, on suppose une fréquence de descente des données de une fois par jour, on doit donc stocker au minimum cette quantité de données dans la mémoire. Pour conserver des marges on choisis deux modules de 16GB chacun (128 Gb). On trouve un composant qualifié spatial adapté : 3D-Plus 3DFN128G08US8761
En cas de problème de télémétrie on peut déparalléliser les deux mémoires et stocker séquentiellement en cas de dépassement des 16 GB disponibles

Stockage du code processeur : EEPROM non recommandée pour nouveaux designs par fabriquant -> Remplacée par MRAM qualifiée spatial de 64Mb, qui permets de stocker le code compilé pour le processeur LEON

### Budget de puissance

### Débit descendant vers la plateforme