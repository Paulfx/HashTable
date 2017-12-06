#ifndef ELEMENT_H
#define ELEMENT_H

typedef unsigned int Key;

class Element {
	private:
		Key _key;
		unsigned int _info;
	public:
		//Constructeur par défaut
		Element();

		//Constructeur
		Element(Key key, unsigned int info);

		//Destructeur
		~Element();

		//Retourne la clé
		Key getKey() const;

		//Retourne l'information
		unsigned int getInfo() const;

		//Mutateur de la clé
		void setKey(Key key);

		//Mutateur de l'info
		void setInfo(unsigned int info);

		//Affiche l'élement
		void affiche() const;
		
};

#endif //ELEMENT_H