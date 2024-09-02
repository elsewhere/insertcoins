#ifndef _ListaKoskaEngineEiToimi_HPP_
#define _ListaKoskaEngineEiToimi_HPP_

class ListaKoskaEngineEiToiminSolmu
{
public:

	//muodostimet ja tuhoajat
	ListaKoskaEngineEiToiminSolmu();
	ListaKoskaEngineEiToiminSolmu(void *ptr);
	~ListaKoskaEngineEiToiminSolmu();

	//asettaa solmun datan halutuksi
	void set(void *ptr);

	//tulostaa tiedot
	void print();

	//osoittimet seuraaviin osiin
	class ListaKoskaEngineEiToiminSolmu *next;
	class ListaKoskaEngineEiToiminSolmu *previous;

	//data, jota solmu kantaa
	void *payload;

};

class ListaKoskaEngineEiToimi
{
public:
	ListaKoskaEngineEiToimi();
	~ListaKoskaEngineEiToimi();
	
	//lis�t��n ListaKoskaEngineEiToimin loppuun alkio (onnistuu aina)
	void add(void *dataptr);
	//poistetaan ListaKoskaEngineEiToimin viimeinen alkio (palauttaa false jos ListaKoskaEngineEiToimi on tyhj�)
	bool removeLast();
	//lis�t��n ty�pointterin j�lkeen alkio (palauttaa false jos ty�pointteri ei ole kelvollinen)
	bool insert(void *dataptr);
	//poistetaan alkio ty�pointterin kohdalta
	bool remove();

	//tuhotaan koko ListaKoskaEngineEiToimi ja vapautetaan kaikki tiedot
	void destroy();

	//asettaa ty�pointterin ListaKoskaEngineEiToimin alkuun (palauttaa false jos ListaKoskaEngineEiToimi on tyhj�)
	bool goToStart();
	//asettaa ty�pointterin ListaKoskaEngineEiToimin loppuun (palauttaa false jos ListaKoskaEngineEiToimi on tyhj�)
	bool goToEnd();
	//liikuttaa ty�pointteria eteenp�in (palauttaa false jos ListaKoskaEngineEiToimi on loppu)
	bool goForward();
	//liikuttaa ty�pointteria taaksep�in (palauttaa false jos ollaan alussa)
	bool goBackwards();
	//menee haluttuun nodeen (palauttaa false jos ei onnistu)
	bool goToNode(int number); 

	//tutkii, ollaanko ekassa nodessa
	bool isAtEnd();
	//tutkii, ollaanko vikassa nodessa
	bool isAtStart();

	//antaa ensimm�isen noden osoittaman tiedon
	void *getFirst();
	//antaa viimeisen noden osoittaman tiedon
	void *getLast();
	//antaa ty�pointterin osoittaman tiedon
	void *getCurrent();
	//asettaa ty�pointterin nodeen uuden tiedon (tuhoaa vanhan jos siell� sellaista on)
	void setCurrent(void *ptr);
	//kertoo kuinka monta solmua ListaKoskaEngineEiToimissa on
	int getNodeCount();
	//kertoo nykyisen noden
	int getCurrentNodeNumber();

	//tulostetaan ListaKoskaEngineEiToimi lokiin (debug)
	void print();
private:
	int solmuja;
	ListaKoskaEngineEiToiminSolmu *start;
	ListaKoskaEngineEiToiminSolmu *current;
	ListaKoskaEngineEiToiminSolmu *end;

};

#endif