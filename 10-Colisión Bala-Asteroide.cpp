#include<iostream>
#include<stdio.h>
#include<windows.h>
#include<conio.h>
#include<stdlib.h>
#include<list>
//Los numeros son los codigos asociados
#define Arriba 72 //Flecha arriba
#define Izq    75 //Flecha izquierda
#define Der    77//Felcha derecha
#define Abajo  80//Flecha abajo

using namespace std;

void gotoXY(int x, int y){
	HANDLE hCon;
	hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD xyPos;//Estructura de datos
	xyPos.X = x;
	xyPos.Y = y;
	SetConsoleCursorPosition(hCon,xyPos);
}
void ocultarCursor(){
	HANDLE hCon;
	hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci;//Contiene dos atributos con los que se puede 
	//controlar la apariencia del cursor en la salida
	cci.dwSize = 2;//Variable int con valores de 1-100, cada numero es un tamaño distinto del cursor
	cci.bVisible = FALSE;//Atributo bool, true se muestra cursor, false no se muestra cursor
	
	SetConsoleCursorInfo(hCon,&cci);//Manipula las caracteristicas del cursor en la salida pot consola
}
void delimitarEscenario(){
	for(int i=1;i<99; i++){//Pinter lineas horizontales
		gotoXY(i,1);//limite superior
		printf("%c",205);
		gotoXY(i,35);//limite inferior
		printf("%c",205);
	}
	for(int i=2; i<35; i++){//Pinter lineas verticales
		gotoXY(1,i);//limite izq
		printf("%c",186);
		gotoXY(98,i);//limite der
		printf("%c",186);
	}
	gotoXY(1,1);printf("%c",201);//Esquina superior izq
	gotoXY(98,1);printf("%c",187);//Esquina superior der
	gotoXY(1,35);printf("%c",200);//Esquina inferior izq
	gotoXY(98,35);printf("%c",188);//ESquina inferior der
}


class Nave{
	//private:
	int x,y;
	int vida;
	int corazones;
public:	
    Nave(int _x,int _y, int _vida,int _corazones) : x(_x), y(_y),vida(_vida),corazones(_corazones){}
	void pintar();
	int X(){return x;}//Devuelve valor de x
	int Y(){return y;}//Devuelve valor de y
	void borrar();
	void COR(){vida--;}
	void mover();
    void puntosVida();	
	void morir();
	int Corazones(){return corazones;}	
};
class Asteroide{
	int x,y;
public:
	Asteroide(int _x,int _y): x(_x),y(_y){}
	void pintar();
	void mover();
	void choque(/*class*/ Nave &nave1);	
	int X(){return x;}//Devuelve valor de x
	int Y(){return y;}//Devuelve valor de y
};
class Bala{
	int x,y;
public:
	Bala(int _x, int _y) : x(_x),y(_y){};
	int X(){return x;}//Devuelve valor de x
	int Y(){return y;}//Devuelve valor de y
	void mover();
	bool fuera();
	
};


void Nave::pintar(){//Dibuja la nave
	gotoXY(x,y); printf("  %c",30);
	gotoXY(x,y+1); printf(" %c%c%c",40,207,41); 
	gotoXY(x,y+2); printf("%c%c %c%c",30,190,190,30);
}
void Nave::borrar(){//Borra nave para que no deje rastro
	gotoXY(x,y);   cout<<"                      ";
	gotoXY(x,y+1); cout<<"                      ";
	gotoXY(x,y+2); cout<<"                      ";
}
void Nave::mover(){
//Mueve el caracter por la pantalla	dim 120x30
	if(kbhit()){//Detecta si se ha presionado una tecla de conio.h
		char tecla = getch();//getch guarda la tecla que se presiona en variable tecla
	    borrar();//Borra nave	    
		if((tecla == 'D' || tecla == 'd'|| tecla == Der) && x+6<99)x++;
	    if((tecla == 'A' || tecla == 'a'|| tecla == Izq) && x>2)x--;
	    if((tecla == 'W' || tecla == 'w'|| tecla == Arriba) && y>2)y--;//La coordenada y esta invertida
	    if((tecla == 'S' || tecla == 's'|| tecla == Abajo ) && y+3 <35)y++;
	   
		pintar();
	    puntosVida();
	}
}
void Nave::puntosVida(){
	gotoXY(60,2); printf("VIDAS %d",corazones);
	gotoXY(70,2);printf("Salud: ");
	gotoXY(76,2);printf("     ");
	for(int i=0;i<vida;i++){
		gotoXY(76+i,2);printf("%c",3);
	}
} 
//Animacion de explocion
void Nave::morir(){
	if(vida == 0){//Si los corazones son cero la nave explota
		borrar();
		gotoXY(x,y);     printf("     **     ");
		gotoXY(x,y+1);   printf("    ****    ");
		gotoXY(x,y+2);   printf("     **     ");
		Sleep(200);
		borrar();
		gotoXY(x,y);     printf("   * ** *    ");
		gotoXY(x,y+1);   printf("    ****     ");
		gotoXY(x,y+2);   printf("   * ** *    ");
		Sleep(200);
		borrar();
		gotoXY(x,y);     printf("     **     ");
		gotoXY(x,y+1);   printf("    ****    ");
		gotoXY(x,y+2);   printf("     **     ");
		Sleep(200);
		borrar();
		corazones--;
		vida = 4;
		puntosVida();
		pintar();
	}
}
void Asteroide::pintar(){//Pinta un asteroide
	gotoXY(x,y);printf("%c",184);	
}
void Asteroide::mover(){//mueve un asteroide por la pantalla
	gotoXY(x,y); printf(" ");
	y++;
	if(y > 34 ){
		x = 3 + rand() % 95;//rand de stdlib.h
		y = 2;
	}	
	pintar();
}
void Asteroide::choque(/*class*/ Nave &nave1){
	if((x>=nave1.X()) && (x<nave1.X()+5) && (y>=nave1.Y()) && (y<=nave1.Y()+2)){
	    nave1.COR();
	    nave1.borrar();
		nave1.pintar();
		nave1.puntosVida();
		x = 1 + rand() % 97;
		y = 2;		
	}
}

void Bala::mover(){
	gotoXY(x,y);   printf("   ");
	y--;
	gotoXY(x,y);  printf("%c",94);
}

bool Bala::fuera(){
	if(y == 2) return true;
	return false;//else
}

int main(){
	
	ocultarCursor();
	delimitarEscenario();
	int x = 35, y = 30;
	const int cora = 4;
	
	Nave nave1(x,y,cora, 4);
	nave1.pintar();
	nave1.puntosVida();
	
	//Creamos los asteroides con una lista
	list<Asteroide*> A;
	list<Asteroide*>::iterator IT;
	for(int i=0; i<5;i++){
		A.push_back(new Asteroide(rand()%90+3, rand()%5+4));
	}
	
	
	//Crea una lista para las balas
	list<Bala*> B;
	list<Bala*>::iterator it;//Iterador para recorrer la lista
	
	bool game_over = false;
	int puntos = 0;
	
	
	while(!game_over){
		
		gotoXY(84,2); printf("Puntos: %d", puntos);
		
		if(kbhit()){
			char tecla = getch();
			if(tecla == 'r' || tecla == 'R'){
				B.push_back(new Bala(nave1.X()+2, nave1.Y()-1));//Crea una bala cada vez que se precione R
				//en la posicion de la nave (Agreaga bala a la lista)
			}
		}
		//Recorre la lista de Balas
		for(it = B.begin(); it != B.end(); it++){//Recorremos la lista para verificar las posiciones que almacenan balas para moverlas en pantalla
			(*it)->mover();//llamamos metodo mover de la clase bala
			if((*it)->fuera()){//Borramos las balas cuando salen de la pantalla
				gotoXY((*it)->X(),(*it)->Y()); printf(" ");
				delete(*it);
				it = B.erase(it);
			}
		}
		//Recorre la lista de Asteroides 
		for(IT = A.begin(); IT != A.end(); IT++){
			(*IT)->mover();
			(*IT)->choque(nave1);
		}	
		 for(IT = A.begin();IT != A.end(); IT++){ 
		 	for(it = B.begin(); it != B.end(); it++){
		 	    if( ((*IT)->X() == (*it)->X()) && (((*IT)->Y() == (*it)->Y()) || ((*IT)->Y() +1 == (*it)->Y()))){
		 	    	gotoXY((*it)->X(),(*it)->Y()); printf(" ");
		 	    	delete(*it);//Elimina la bala de la lista
		 	    	it = B.erase(it);//Une el it anterior con el it posterior al elemneto eliminado
		 	    	//Creamos asteroide sustituto
		 	    	A.push_back(new Asteroide(rand()%90+3,2));
		 	    	gotoXY((*IT)->X(),(*IT)->Y()); printf(" ");
		 	    	delete(*IT);
		 	    	IT = A.erase(IT);
		 	    	
		 	    	puntos++;
				 } 	
			 }
		 }	
		 
		 if(nave1.Corazones() == 0) game_over = true;
		
		nave1.morir();
		nave1.mover();	
    	Sleep(30);
	}	
	
	
					
	return 0;
}
