#include "Element.h"
#include "HashTable.h"
#include <cstdio>
#include <iostream>
#include <chrono>
#include <fstream>
#include <cmath> //pow + modf
#include <string>
#include <cstdlib>

//------------------------------------------------------------------------------//
//----------
//----------          CONSTANTES
//----------
//------------------------------------------------------------------------------//

#define NB_INSERTION 32767
#define PAS_TEST_RECHERCHE 100
#define NB_HASHTABLE 200


//#define P 47 //Nombre premier >= m
//#define TAILLE_TABLE 45 //m


const unsigned int TAILLE_TABLE_PREMIER = 33773; //Nombre premier
const unsigned int TAILLE_TABLE_PUISSANCE = 32768; //m = 2**p
const unsigned int P_PUISSANCE = 10; // Plus grande puissance de 2 inférieure ou égale à m


/*
const unsigned int TAILLE_TABLE_PREMIER = 264403; //Nombre premier
const unsigned int TAILLE_TABLE_PUISSANCE = 262144; //m = 2**p
const unsigned int P_PUISSANCE = 18; // Plus grande puissance de 2 inférieure ou égale à m
*/


const float NB_OR = 1.61803398875; //(sqrt(5) + 1) / 2
//const float NB_OR_COMPLEMENTAIRE = 0.61803398875; //(sqrt(5) - 1) / 2

//------------------------------------------------------------------------------//
//----------
//----------          FONCTIONS DE HACHAGE
//----------
//------------------------------------------------------------------------------//

unsigned int hashModulo(Key key, unsigned int max) {
  return key % max;
}

/* Extraction des 2 premiers bits de chaque séquence de 4 bits
*  Et concaténation de ces bits
*/
unsigned int hashExtraction(Key key, unsigned int max) {
  unsigned int res=0;
  unsigned int nbExtrait=0;
  unsigned int b_11 = 3;//binaire : 11
  for(unsigned int i=0; i<(sizeof(Key) * 8)-1; i+=4) {
    res |= (key & (b_11<<i) ) >> (i-nbExtrait);
    nbExtrait+=2;
  }
  return res % max;
}

/* On prend des séquences de K = 2**(P-1) bits sur lesquels on applique un XOR
*  avec P la plus grande puissance de 2 inférieure ou égale à max
*  ici, P est donnée comme constante dans le main
*/
unsigned int hashCompression(Key key, unsigned int max) {
  //unsigned int K = std::pow(2,(P_PUISSANCE))-1; //Nombre de bits à 1 permettant de contenir max
  unsigned int K = max-1;
  unsigned int res=key & K; //La première séquence
  for(unsigned int i=P_PUISSANCE;i<sizeof(Key) * 8;i+=P_PUISSANCE) {
    res ^= ( key & K ) >> i;
  }
  return res;
}

/* On retourne la partie décimale de la clé multipliée
*  par le nombre d'or, multipliée par la taille max de la
*  table.
*/
unsigned int hashMultiplication(Key key, unsigned int max) {
  double a;
  return std::modf((double) key * NB_OR, &a) * max;
}

//------------------------------------------------------------------------------//
//----------
//----------          FONCTIONS DE RE-HACHAGE
//----------
//------------------------------------------------------------------------------//


unsigned int reHashLineaire(unsigned int indice, unsigned int max, Key x, unsigned int nbEssai){
  return (indice+1) % max;
}
unsigned int reHashQuadratique(unsigned int indice, unsigned int max, Key x, unsigned int nbEssai){
  return (indice+2*nbEssai+1) % max;
}

//Double hachage où la deuxième fonction de hachage renvoie un entier dans [1,m-1]
//Prévu initialement avec m premier
unsigned int doubleHashPremier(unsigned int indice, unsigned int max, Key x, unsigned int nbEssai){
  return (indice+(1+ x % (max-1))) % max;
}

//Double hachage où la deuxième fonction de hachage renvoie un entier impair dans [0,(2**p)-1]
//Avec p la plus grande puissance de 2 inférieure ou égale à m
//Prévu initialement pour m=2**p
unsigned int doubleHashPuissance(unsigned int indice, unsigned int max, Key x, unsigned int nbEssai){
  return (indice+(2*x+1) % max-1) % max;
}


//------------------------------------------------------------------------------//
//----------
//----------          FONCTION D'ETUDE DE COMPLEXITE
//----------
//------------------------------------------------------------------------------//

bool testPerformance(const char* filename, unsigned int (*ptrHashFct)(Key, unsigned int),
                                    unsigned int (*ptrReHashFct)(unsigned int, unsigned int, Key, unsigned int), unsigned int taille_table) {
  int n=1;
  std::ofstream file(filename);
  if(!file.is_open()) return false;
  file<<"# \"Nb elem\" \"Temps\"\n";
  HashTable* tab[NB_HASHTABLE];
  for(unsigned int i=0;i<NB_HASHTABLE;++i) {
    tab[i]=new HashTable(taille_table, ptrHashFct, ptrReHashFct);
  }
  unsigned int nbTestMax=NB_INSERTION/PAS_TEST_RECHERCHE;
  int limite=PAS_TEST_RECHERCHE;
  std::chrono::time_point<std::chrono::system_clock> start,end;
  float t;
  
  for (int i = 0; i <= NB_INSERTION; ++i) { 
    if(i==limite) {
      std::cout<<"\rTest numéro : "<<n<<"/"<<nbTestMax;
      n++;
      start=std::chrono::system_clock::now();
      for(unsigned int j=0;j<NB_HASHTABLE;++j) {
        tab[j]->find(rand());
      }
      end=std::chrono::system_clock::now();
      t=std::chrono::duration_cast<std::chrono::microseconds>(end-start).count();
      //Sauvegarde de la ligne
      t=((float) t)/NB_HASHTABLE;
      file<<i<<" "<<t<<"\n";
      limite+=PAS_TEST_RECHERCHE;
    }
    //Insertion des éléments
    for(unsigned int j=0;j<NB_HASHTABLE;++j){
      Element elt(rand(),rand());
      tab[j]->insert(elt,elt.getKey());
    }
  } 


  for(unsigned int i=0;i<NB_HASHTABLE;++i) {
    delete tab[i];
  }
 
  file.close();
  return true;
}



//------------------------------------------------------------------------------//
//----------
//----------          PROGRAMME PRINCIPAL
//----------
//------------------------------------------------------------------------------//

int main(int argc, char** argv) {
  srand(time(NULL));

  /*
  * INSERTION D'ELEMENTS
  */

  std::cout<<std::endl<<std::endl<<"Création d'une table et insertion des éléments de clé 1,123,4587,12,6,58 et 156"<<std::endl<<std::endl<<std::endl;

  HashTable table(100, hashCompression, reHashQuadratique);
  unsigned int addKey [7] = {1,123,4587,12,6,58,156};
  Element elt(0,0);
  for(unsigned int i=0; i<7; ++i) {
    elt.setKey(addKey[i]);
    table.insert(elt,elt.getKey());
  }
  std::cout<<"***************Affichage de la table :***************"<<std::endl;
  table.affiche();

  /*
  * INSERTION EN DOUBLE
  */

  std::cout<<std::endl<<std::endl<<"Insertion en double de 123,12,58"<<std::endl<<std::endl<<std::endl;
  elt.setInfo(2);
  unsigned int add2Key [3] = {123,12,58};
  for(unsigned int i=0; i<3; ++i) {
    elt.setKey(add2Key[i]);
    table.insert(elt,elt.getKey());
  }
  std::cout<<"***************Affichage de la table :***************"<<std::endl;
  table.affiche();

  /*
  * SUPPRESSION
  */

  std::cout<<std::endl<<std::endl<<"Suppression de 123,1,541,3,58,41"<<std::endl<<std::endl<<std::endl;
  unsigned int suppKey [6] = {123,1,541,3,58,41};
  for(unsigned int i=0; i<6; ++i) {
    table.erase(suppKey[i]);
  }
  std::cout<<"***************Affichage de la table :***************"<<std::endl;
  table.affiche();

  /*
  * ACCES AUX ELEMENTS
  */
  std::cout<<std::endl<<std::endl<<"Accès aux éléments existants 4587 et 156"<<std::endl<<std::endl<<std::endl;
  elt = table[4587];
  std::cout<<"*************** Affichage de l'élément de clé 4587 :"<<std::endl;
  elt.affiche();
  elt = table[156];
  std::cout<<"*************** Affichage de l'élément de clé 156 :"<<std::endl;
  elt.affiche();

  /*
  * MODIFICATION DES ELEMENTS
  */
  std::cout<<std::endl<<std::endl<<"Modification de l'info associé aux clés 4587 et 156"<<std::endl<<std::endl<<std::endl;

  table[4587].setInfo(10);
  table[156].setInfo(20);

  std::cout<<"***************Affichage de la table :***************"<<std::endl;
  table.affiche();


  std::cout<<std::endl<<std::endl<<"L'accès à l'élément NON existant 111 gènère une exception avant de quitter"<<std::endl;
  //elt = table[111]; //Génère une exception et quitte le programme

  /*
  * TEST DE PERFORMANCE
  */

  //Test de recherche pour le re-hachage linéaire
  std::cout<<"Test de recherche pour le re-hachage linéaire"<<std::endl;
  if (!testPerformance("plot/testPerfLineaire.txt", hashMultiplication, reHashLineaire, TAILLE_TABLE_PUISSANCE)) {
    std::cerr<<"Erreur de lecture/ouverture du fichier plot/testPerfLineaire"<<std::endl;
    std::cout<<"Existe-t il le dossier plot ?"<<std::endl;
  }

  
  //Test de recherche pour le re-hachage quadratique
  std::cout<<std::endl<<"Test de recherche pour le re-hachage quadratique : "<<std::endl;
  if (!testPerformance("plot/testPerfQuad1.txt", hashMultiplication, reHashQuadratique, TAILLE_TABLE_PUISSANCE)) {
    std::cerr<<"Erreur de lecture/ouverture du fichier plot/testPerfQuad1"<<std::endl;
    std::cout<<"Existe-t il le dossier plot ?"<<std::endl;
  }

  std::cout<<std::endl<<"Test de recherche pour le re-hachage quadratique avec M premier : "<<std::endl;
  if (!testPerformance("plot/testPerfQuad2.txt", hashMultiplication, reHashQuadratique, TAILLE_TABLE_PREMIER)) {
    std::cerr<<"Erreur de lecture/ouverture du fichier plot/testPerfQuad2"<<std::endl;
    std::cout<<"Existe-t il le dossier plot ?"<<std::endl;
  }

  //Test de recherche pour le double-hachage puissance
  std::cout<<std::endl<<"Test de recherche pour le double-hachage puissance avec M non une puissance de deux : "<<std::endl;
  if (!testPerformance("plot/testPerfDoublePuissance1.txt", hashMultiplication, doubleHashPuissance, TAILLE_TABLE_PREMIER)) {
    std::cerr<<"Erreur de lecture/ouverture du fichier plot/testPerfDoublePuissance1"<<std::endl;
    std::cout<<"Existe-t il le dossier plot ?"<<std::endl;
  }

  std::cout<<std::endl<<"Test de recherche pour le double-hachage puissance avec M une puissance de deux : "<<std::endl;
  if (!testPerformance("plot/testPerfDoublePuissance2.txt", hashMultiplication, doubleHashPuissance, TAILLE_TABLE_PUISSANCE)) {
    std::cerr<<"Erreur de lecture/ouverture du fichier plot/testPerfDoublePuissance2"<<std::endl;
    std::cout<<"Existe-t il le dossier plot ?"<<std::endl;
  }

  //Test de recherche pour le double-hachage premier
  std::cout<<std::endl<<"Test de recherche pour le double-hachage premier avec M non premier : "<<std::endl;
  if (!testPerformance("plot/testPerfDoublePremier1.txt", hashMultiplication, doubleHashPremier, TAILLE_TABLE_PUISSANCE)) {
    std::cerr<<"Erreur de lecture/ouverture du fichier plot/testPerfDoublePremier1"<<std::endl;
    std::cout<<"Existe-t il le dossier plot ?"<<std::endl;
  }

  std::cout<<std::endl<<"Test de recherche pour le double-hachage premier avec M premier : "<<std::endl;
  if (!testPerformance("plot/testPerfDoublePremier2.txt", hashMultiplication, doubleHashPremier, TAILLE_TABLE_PREMIER)) {
    std::cerr<<"Erreur de lecture/ouverture du fichier plot/testPerfDoublePremier2"<<std::endl;
    std::cout<<"Existe-t il le dossier plot ?"<<std::endl;
  }
  
  std::cout<<std::endl;

  std::string filename;

  std::cout<<"Voulez vous utiliser le fichier plot/plotFile afin de générer une image png dans le dossier plot ?"<<std::endl;
  std::cout<<"Si oui, écrivez le nom du fichier voulu (sans l'extension), sinon ecrivez 'q' ou 'quit'..."<<std::endl;
  std::cin>>filename;

  if(filename!="q" && filename!="quit" ) {
    std::cout<<"Sauvegarde du fichier plot/"<<filename<<".png"<<std::endl;
    std::string command = "gnuplot plot/plotFile > plot/" + filename + ".png";
    std::system(command.c_str());
  }
  else std::cout<<"Aucune image ne sera créée."<<std::endl;
  return 0;
}
