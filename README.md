Informations complémentaires sur le module HashTable :

1) Les fonctions de hachage/re-hachage sont codées dans le main.

2) La constante P_PUISSANCE pourrait être calculée dans le constructeur de HashTable,
   mais cela surchargerait les arguments des fonctions de hachage

3) Le fichier plot/plotFile permet de créer une image png contenant les différents fichiers
   proudits par l'executable.
   Usage :
   			$ gnuplot plot/plotFile > nomFichier.png

  Remarque : le programme utilise cette commande après avoir calculé les performances, si
  			 l'utilisateur accepte.


4) La fonction Element::setKey() est utile pour l'exemple dans le main. En réalité,
   On ne devrait pas pouvoir modifier une clé après l'avoir insérée.