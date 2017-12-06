#include "Element.h"
#include <iostream>


Element::Element(){_key=0;_info=0;}

Element::Element(Key key, unsigned int info) { _key=key; _info=info;}

Element::~Element() {}

Key Element::getKey() const { return _key;}

unsigned int Element::getInfo() const{return _info;}

void Element::affiche() const{
	std::cout<<"Clé : "<<_key<<"\nInfo : "<<_info<<"\n-------------------"<<std::endl;
}

void Element::setKey(Key key) { _key = key; }

void Element::setInfo(unsigned int info) { _info = info; }
