#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "Element.h"

enum enumTag {LIBRE, OCCUPEE, LIBEREE};

/**
*
**/
class HashTable {

private:

	//Taille maximale
	unsigned int m;
	Element* data;
	enumTag* flagTab;
	unsigned int nbElem;
	//Pointeur sur fct de hachage
	unsigned int (*hashFct)(Key, unsigned int); 
	//Pointeur sur fct de re-hachage
	unsigned int (*reHashFct)(unsigned int, unsigned int, Key, unsigned int);

	/*
	* Retourne l'indice dans data de l'élément de clé 'key' s'il existe
	* Retourne -1 sinon
	*/
	int findIndex(const Key& key) const;
	
public:

	/** 
 	*  Constructeur par defaut.
 	*/
	HashTable();

	/* 
	*  Constructeur.
	*  Initialise data, m et les pointeurs de fonction.
	*/
	HashTable(unsigned int max, unsigned int (*ptrHashFct)(Key, unsigned int),
								unsigned int (*ptrReHashFct)(unsigned int, unsigned int, Key, unsigned int));

	/*
 	* Destructeur.
 	*/
 	~HashTable();

 	/*
 	* Insère l'élément e sans duplication.
 	* Retourne false si la table est pleine, true sinon.
 	* N'utilise pas find.
 	*/
 	bool insert(const Element& e, const Key& key);

 	/*
 	*  Insère l'élément e. Retourne false si la table est pleine, true sinon.
 	*  Utilise la fonction find pour vérifier que l'élément n'existe pas déjà.
 	*/ 
 	bool insertWithFind(const Element& e, const Key& key);

 	/*
 	* Retourne true si l'élément de clé 'key' existe, false sinon.
 	* Utilise la fonction privée findIndex.
 	*/
 	bool find(const Key& key) const;

 	/*
 	* Supprime l'élement de clé key s'il existe.
 	*/
 	void erase(const Key& key);

 	/*
 	* Affiche la table
 	*/
 	void affiche() const;

 	/*
 	* Retourne le nombre d'éléments
 	*/
 	unsigned int getNbElem() const;

 	/*
 	* Retourne une référence vers l'élément de clé key s'il existe,
 	* Sinon, génère une exception et arrête le programme.
 	*/
 	Element& operator [](const Key& key);

 	/*
 	* Retourne une référence constante vers l'élément de clé key s'il existe,
 	* Sinon, génère une exception et arrête le programme.
 	*/
 	const Element& operator [](const Key& key) const;

};

#endif //HASHTABLE_H