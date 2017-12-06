#include "HashTable.h"
#include <iostream>
#include <string>
#include <cassert>

HashTable::HashTable(){}

HashTable::HashTable(unsigned int max, unsigned int (*ptrHashFct)(Key, unsigned int),
									   unsigned int (*ptrReHashFct)(unsigned int, unsigned int, Key, unsigned int) ) {
	nbElem=0;
	m=max;
	data=new Element[m];
	flagTab=new enumTag[m];
	for(unsigned int i=0; i<m; ++i) flagTab[i]=LIBRE;
	hashFct=ptrHashFct;
	reHashFct=ptrReHashFct;
}

HashTable::~HashTable() {
	delete [] data;
	delete [] flagTab;
}

bool HashTable::insert(const Element& e, const Key& key) {
	unsigned int indiceAjout;
	bool trouvePlace=false;
	unsigned int indice=hashFct(key, m);//Première place possible
	if(flagTab[indice] == LIBRE) {
		trouvePlace=true;
		indiceAjout=indice;
	}
	else {
		unsigned int nbEssais=0;
		unsigned int indiceTmp=indice;
		//Tant qu'on a pas fait un tour et qu'on n'est pas sur que la table contienne e,
		do {
			if(data[indiceTmp].getKey()==key) return true;	//On a trouvé notre élément, on arrête (pas de duplication1)
			if(!trouvePlace && flagTab[indiceTmp]==LIBEREE) {	//La première place libérée(sans trouver de case libre) est sauvegardée
				trouvePlace=true;
				indiceAjout=indiceTmp;
			}
			indiceTmp=reHashFct(indiceTmp, m, key, nbEssais);
			nbEssais++;
		}
		while(indiceTmp!=indice && flagTab[indiceTmp]!=LIBRE);

		//Si la première place trouvée est une case LIBRE
		//Alors le dernier indice est l'indice d'ajout
		if(indiceTmp!=indice && !trouvePlace) {
				trouvePlace=true;
				indiceAjout=indiceTmp; 
		}
	}
	if(trouvePlace) {
		nbElem++;
		data[indiceAjout]=e;
		flagTab[indiceAjout]=OCCUPEE;
		return true;
	}
	return false;
}


bool HashTable::insertWithFind(const Element& e, const Key& key) {
	if(find(key)) return true;
	nbElem++;
	unsigned int indice=hashFct(key, m);//Première place possible
	if(flagTab[indice] != OCCUPEE) {
		data[indice]=e;
		flagTab[indice]=OCCUPEE;
	}
	else { //La case est occupée, on re-hache
		unsigned int nbEssais=0;
		unsigned int indiceTmp=indice;
		do {
			indiceTmp=reHashFct(indiceTmp, m, key, nbEssais);
			nbEssais++;
		}
		while(indiceTmp!=indice && flagTab[indiceTmp]==OCCUPEE);
		//tant qu'on a pas fait un tour ET que la case est occupee,
		//on continue

		if(flagTab[indiceTmp]!=OCCUPEE) {//On a une place
			data[indiceTmp]=e;
			flagTab[indiceTmp]=OCCUPEE;
			return true;
		}
	}
	return false;
}

void HashTable::affiche() const {
	for(unsigned int i=0; i<m; ++i) {
		if(flagTab[i]==OCCUPEE) {
			std::cout<<"INDICE "<<i<<std::endl;
			data[i].affiche();
		}
	}
}


int HashTable::findIndex(const Key& key) const {
	unsigned int indice=hashFct(key, m);
	if(flagTab[indice]==LIBRE) return -1;
	else if(data[indice].getKey()==key) return indice;
	else { //Parcours du re-hachage
		unsigned int nbEssais=0;
		unsigned int indiceTmp=indice;
		do {
			indiceTmp=reHashFct(indiceTmp, m, key, nbEssais);
			nbEssais++;
			if(flagTab[indiceTmp]==OCCUPEE && data[indiceTmp].getKey()==key) return indiceTmp; 
		}
		while(indiceTmp != indice && flagTab[indiceTmp]!=LIBRE);//Tant qu'on a pas fait un tour et qu'on 
																//ne sait pas si l'on va trouver e, on continue
	}
	return -1;
}

bool HashTable::find(const Key& key) const {
	int index=findIndex(key);
	if(index != -1) return true;
	return false;
}

Element& HashTable::operator [](const Key& key) {
	try {
		int indice=findIndex(key);
		if(indice==-1) {
			std::string str="Erreur : l'élément de clé "+std::to_string(key)+" n'existe pas";
			throw str;
		}
		else return data[indice];
	}
	catch(const std::string& s) { std::cerr<<s<<std::endl; exit(EXIT_FAILURE);}
}

const Element& HashTable::operator [](const Key& key) const {
	try {
		int indice=findIndex(key);
		if(indice==-1) {
			std::string str="Erreur : l'élément de clé "+std::to_string(key)+" n'existe pas";
			throw str;
		}
		else return data[indice];
	}
	catch(const std::string& s) { std::cerr<<s<<std::endl; exit(EXIT_FAILURE);}
}

void HashTable::erase(const Key& key) {
	int indice=findIndex(key);
	if(indice != -1) {//On a trouvé l'élément
		flagTab[indice]=LIBEREE;
		nbElem--;
	}
}

unsigned int HashTable::getNbElem() const {
	return nbElem;
}
