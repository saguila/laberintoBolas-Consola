#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>
using namespace std;

const int DIM = 20;
typedef int  tTablero[DIM][DIM]; // array bidimensional que contiene los datos del tablero.
typedef enum tDireccion{ derecha, arriba, izquierda, abajo, ninguna };
typedef bool tBolasMarcadas;
bool terminado = false;
typedef struct{
	tTablero tablero;
	int NumBolasTablero, NumTurnos;
	tDireccion DirUltimoTurno;
	string NombreArchivo;
}tJuego;


/* PROTOTIPOS */
int Menu();
void iniciaJuego(tJuego &juegoBolas);
bool cargaJuego(tJuego &juegoBolas);
bool validarFronteras(tJuego juegoBolas);
void muestraLaberinto(tJuego juegoBolas);
void setColor(unsigned int color);
bool desplazaBola(tJuego &juegoBolas, const int fila, const int columna, tDireccion direc);
void inclinaTablero(tJuego &juegoBolas, tDireccion direc, bool &terminado);

int main(){
	tJuego juegoBolas;
	bool terminado = false;
	int opcion;
	iniciaJuego(juegoBolas);
	if (cargaJuego(juegoBolas)){
		muestraLaberinto(juegoBolas);
		do{
			opcion = Menu();
			switch (opcion){
			case 1:
				if (juegoBolas.DirUltimoTurno != 0){
					inclinaTablero(juegoBolas, (tDireccion)0, terminado);
				}
				break;
			case 2:
				if (juegoBolas.DirUltimoTurno != 1){
					inclinaTablero(juegoBolas, (tDireccion)1, terminado);
				}
				break;
			case 3:
				if (juegoBolas.DirUltimoTurno != 2){
					inclinaTablero(juegoBolas, (tDireccion)2, terminado);
				}
				break;
			case 4:
				if (juegoBolas.DirUltimoTurno != 3){
					inclinaTablero(juegoBolas, (tDireccion)3, terminado);
				}
				break;
			case 5:
				cin.sync();
				cargaJuego(juegoBolas);
				break;
			case 6:
				cin.sync();
				iniciaJuego(juegoBolas);
				if (!cargaJuego(juegoBolas)){ // si la carga no es correcta salimos.
					opcion = 0;
				}
				break;
			}
			if (opcion != 0){
				muestraLaberinto(juegoBolas);
			}
		} while (!terminado && opcion != 0);
		if (terminado){
			cout << "Felicidades" << endl;
			cout << "Numero de movimientos invertidos " << juegoBolas.NumTurnos << endl;
		}
	}
	cin.sync();
	do{
		cout << "Saliendo ..." << endl << "pulse INTRO para continuar";
	} while (cin.get() != '\n');
	return 0;
}

/* Contiene el menu principal del programa */
int Menu(){
	setColor(15); // color blanco.
	int opcion;
	cout << endl << " 1 - Inclinar hacia la derecha " << endl;
	cout << " 2 - Inclinar hacia arriba " << endl;
	cout << " 3 - Inclinar hacia la izquierda " << endl;
	cout << " 4 - Inclinar hacia abajo " << endl;
	cout << " 5 - Reiniciar el tablero " << endl;
	cout << " 6 - Cargar nuevo tablero " << endl;
	cout << " 0 - SALIR" << endl;
	cout << "Opcion: ";
	cin >> opcion;
	return opcion;
}
/* Funcion Inicia el juego eligiendo el nombre del archivo para el tablero */
void iniciaJuego(tJuego &juegoBolas){
	string aux;
	juegoBolas.NombreArchivo = "lab.txt"; // por defecto registramos lab.txt a menos que el usuario indique lo contrario.
	cout << " Nombre del laberinto ( INTRO-> archivo por defecto 'lab.txt'): ";
	getline(cin, aux);
	if (aux != ""){
		cin.sync();
		juegoBolas.NombreArchivo = aux;
	}
} // OK
/* Funcion que Carga el tablero desde el archivo y muestra los posibles errores que ocurran durante el proceso  */
bool cargaJuego(tJuego &juegoBolas){
	ifstream lectura;
	int contadorBolas = 0;
	juegoBolas.NumTurnos = 0; // ponemos el contador de turnos a 0.
	juegoBolas.DirUltimoTurno = (tDireccion)4; // inicializamos la posicionanterior a nada,no hubo giro.
	lectura.open(juegoBolas.NombreArchivo);
	bool correcto = true;

	const int INDICE = 20;
	tBolasMarcadas AuxiliarBolasMarcadas[INDICE];
	if (!lectura.is_open()){
		setColor(4);
		correcto = false;
		cout << " No se pudo abrir el archivo  '" << juegoBolas.NombreArchivo << " '" << endl;
	}
	else{

		/* Inicializamos a false el auxiliar de las bolas */
		for (int i = 0; i < INDICE; i++){
			AuxiliarBolasMarcadas[i] = false;
		}

		lectura >> juegoBolas.NumBolasTablero;
		for (int i = 0; i < 20 && correcto; i++){ // i recorre columnas.
			for (int j = 0; j < 20 && correcto; j++){ // j recorre filas.
				lectura >> juegoBolas.tablero[i][j]; // carga el array con la entrada de fichero.

				if (juegoBolas.tablero[i][j] >= -3 && juegoBolas.tablero[i][j] <= juegoBolas.NumBolasTablero){ //Si es mayor o igual que -3 y si es menor que el numero de bolas estara correcto

					if (juegoBolas.tablero[i][j] >= 0){ // si es mayor o igual que 0 es una bola.
						contadorBolas++; // Contamos una bola
						if (AuxiliarBolasMarcadas[juegoBolas.tablero[i][j]] == true){ // Si la posicion ya estaba a true es que hemos vuelto a leer un mismo numero.
							correcto = false;
							setColor(4);
							cout << " El archivo contiene bolas duplicadas,Imposible continuar" << endl;
						}
						AuxiliarBolasMarcadas[juegoBolas.tablero[i][j]] = true; // Aqui estamos relacionando el valor de la bola con la posicion del array de booleanos,si 3 esta a true,es que ha salido un 3.
					}
				}
				else{ // si lo que leemos no es un numero valido para el juego ponemos correcto a false para que no se siga con la ejecucion.
					correcto = false;
					setColor(4);
					cout << "El archivo contiene datos incompatibles con el programa,Imposible continuar" << endl;
				}
			}
		}

		if (contadorBolas != juegoBolas.NumBolasTablero){
			setColor(4);
			cout << "Imposible iniciar,el numero de bolas del tablero es incorrecto" << endl;
			correcto = false;
		}
		if (correcto){
			correcto = validarFronteras(juegoBolas);
		}
	}
	return correcto;
} // OK
/* Funcion que comprueba la estabilidad del juego comprobando si las fronteras estan cerradas */
bool validarFronteras(tJuego juegoBolas){
	bool correcto = true;
	for (int i = 0; i < 20; i++){ // con un for comprobamos las fronteras

		if (juegoBolas.tablero[0][i] != -2){ // lado superior del cuadrante
			correcto = false;
		}
		if (juegoBolas.tablero[i][0] != -2){ // lado izquierdo del cuadrante.
			correcto = false;
		}
		if (juegoBolas.tablero[i][19] != -2){ // lado derecho del cuadrante.
			correcto = false;
		}
		if (juegoBolas.tablero[19][i] != -2){ // lado inferior del cuadrante.
			correcto = false;
		}
	}
	if (!correcto){ // con que haya algun false ya es inestable.
		setColor(4);
		cout << "Imposible continuar,el tablero es inestable." << endl;
	}
	return correcto;
} // OK
/* Funcion que sirve para cambiar de color las letras en consola,color es el color (valores de 0 al 15). */
void setColor(unsigned int color){
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(handle, color);
} // OK
/* Muestra en consola el estado actual del laberinto */
void muestraLaberinto(tJuego juegoBolas){
	setColor(6);
	cout << endl;
	cout << "                                                           " << juegoBolas.NumTurnos << endl;
	setColor(3); // color azul
	for (int i = 0; i < 20; i++){
		for (int j = 0; j < 20; j++){
			if (juegoBolas.tablero[i][j] == -4){ // El -4 es un valor interno del juego que representa una bola metida en un agujero.
				setColor(4); // Color rojo.
				cout << (char)177 << (char)177 << (char)177;
				setColor(3); // Color azul.
			}
			else if (juegoBolas.tablero[i][j] == -3){
				cout << (char)176 << (char)176 << (char)176; // agujero
			}
			else if (juegoBolas.tablero[i][j] == -2){
				cout << (char)219 << (char)219 << (char)219; // pared
			}
			else if (juegoBolas.tablero[i][j] == -1){
				cout << " " << " " << " "; // un espacio
			}
			else{
				setColor(4);
				cout << " " << juegoBolas.tablero[i][j] << " ";
				setColor(3);
			}
		}
		cout << endl;
	}
} //OK
/* Funcion que recibe las coordenadas de la bola y la mueve en el sentido de tDireccion */
bool desplazaBola(tJuego &juegoBolas, const int fila, const int columna, tDireccion direc){
	bool terminado = false;
	bool salir = false; // Salir va a ser el booleano que va indicar al for cuando tiene que salir.
	if (direc == 0){ // a la derecha.OK
		for (int i = fila; !salir; i++){

			if (juegoBolas.tablero[columna][i + 1] == -3 || juegoBolas.tablero[columna][i + 1] == -4){ // Si se mete en un agujero vacio o en un agujero en el que se metio una bola.Esto esta implementado por si se quiere mejorar el juego si obligamos a meter todas las bolas.
				terminado = true;
				salir = true;
				juegoBolas.tablero[columna][fila] = -1; // en la posicion inicial metemos un espacio.
				juegoBolas.tablero[columna][i + 1] = -4; // el menos 4 es un valor para representar un agujero por donde cayo una bola
			}
			else{ // Si no se mete en un agujero vacio o lleno

				if (juegoBolas.tablero[columna][i + 1] == -2 || juegoBolas.tablero[columna][i + 1] >= 0){ // Si el siguiente que leemos es una pared o una bola no se sigue avanzando,copiamos la bola y salimos del bucle.
					if (juegoBolas.tablero[columna][fila] != juegoBolas.tablero[columna][i]){ // Si hay una pared y por lo tanto la posicion de partida y de ida es la misma no hacemos nada.
						juegoBolas.tablero[columna][i] = juegoBolas.tablero[columna][fila]; // movemos la bola a la posicion en la que estamos leyendo.
						juegoBolas.tablero[columna][fila] = -1; // en ls posicion en la cual la bola se encontraba incialmente metemos un espacio -1.

					}
					salir = true; // Independientemente que el siguiente sea una bola o una pared debemos acabar la ejecucion.
				}

			}

		}

	}

	else if (direc == 1){ // a arriba.

		for (int i = columna; !salir; i--){

			if (juegoBolas.tablero[i - 1][fila] == -3 || juegoBolas.tablero[i - 1][fila] == -4){
				terminado = true;
				salir = true;
				juegoBolas.tablero[columna][fila] = -1;
				juegoBolas.tablero[i - 1][fila] = -4; // el menos 4 es un valor para representar un agujero por donde cayo una bola.
			}
			else{

				if (juegoBolas.tablero[i - 1][fila] == -2 || juegoBolas.tablero[i - 1][fila] >= 0){ // Si el siguiente que leemos es una pared hacemos lo siguiente.
					if (juegoBolas.tablero[columna][fila] != juegoBolas.tablero[i][fila]){ // Si la posicion inicial es igual a la anterior de la pared no hacemos nada.
						juegoBolas.tablero[i][fila] = juegoBolas.tablero[columna][fila]; // movemos la bola a la posicion en la que estamos leyendo.
						juegoBolas.tablero[columna][fila] = -1; // en ls posicion en la cual la bola se encontraba incialmente metemos un espacio -1.

					}
					salir = true; // salir es el booleano que le indica al for cuando debe acabar.
				}
			}


		}

	}

	else if (direc == 2){ // a la izquierda OK.
		for (int i = fila; !salir; i--){
			if (juegoBolas.tablero[columna][i - 1] == -3 || juegoBolas.tablero[columna][i - 1] == -4){
				terminado = true;
				salir = true;
				juegoBolas.tablero[columna][fila] = -1; // si se mete en el agujero deja libre.
				juegoBolas.tablero[columna][i - 1] = -4;
			}
			else{
				if (juegoBolas.tablero[columna][i - 1] == -2 || juegoBolas.tablero[columna][i - 1] >= 0){ // Si el siguiente que leemos es una pared hacemos lo siguiente.
					if (juegoBolas.tablero[columna][fila] != juegoBolas.tablero[columna][i]){ // Si la posicion inicial es igual a la anterior de la pared no hacemos nada.
						juegoBolas.tablero[columna][i] = juegoBolas.tablero[columna][fila]; // movemos la bola a la posicion en la que estamos leyendo.
						juegoBolas.tablero[columna][fila] = -1; // en ls posicion en la cual la bola se encontraba incialmente metemos un espacio -1.
					}
					salir = true; // salir es el booleano que le indica al for cuando debe acabar.
				}

			}
		}
	}

	else if (direc == 3){ // a abajo.
		for (int i = columna; !salir; i++){
			if (juegoBolas.tablero[i + 1][fila] == -3 || juegoBolas.tablero[i + 1][fila] == -4){
				terminado = true;
				salir = true;
				juegoBolas.tablero[columna][fila] = -1;
				juegoBolas.tablero[i + 1][fila] = -4; // el menos 4 es un valor para representar un agujero por donde cayo una bola.
			}
			else{
				if (juegoBolas.tablero[i + 1][fila] == -2 || juegoBolas.tablero[i + 1][fila] >= 0){ // Si el siguiente que leemos es una pared hacemos lo siguiente.
					if (juegoBolas.tablero[columna][fila] != juegoBolas.tablero[i][fila]){ // Si la posicion inicial es igual a la anterior de la pared no hacemos nada.
						juegoBolas.tablero[i][fila] = juegoBolas.tablero[columna][fila]; // movemos la bola a la posicion en la que estamos leyendo.
						juegoBolas.tablero[columna][fila] = -1; // en ls posicion en la cual la bola se encontraba incialmente metemos un espacio -1.
					}
					salir = true; // salir es el booleano que le indica al for cuando debe acabar.
				}
			}
		}
	}
	return terminado;
}
/* desplaza todas las bolas y en la parametro terminado devuelve si algunas se han caido al agujero y termina el juego  */
void inclinaTablero(tJuego &juegoBolas, tDireccion direc, bool &terminado){
	int fila = 0, columna = 0;
	terminado = false;
	juegoBolas.DirUltimoTurno = direc; // actualizamos la ultima direccion de giro.
	/* El siguiente codigo es para coger las bolas mas cercanas para que no se pisen unas a otras */
	if (direc == 0){ // Si giramos a la derecha
		for (int i = 0; i < 20; i++){ // Recorremos los dos arrays.
			for (int j = 20; j > 0; j--){
				if (juegoBolas.tablero[i][j] >= 0){ // Al recorrer todos los componentes de los arrays si encontramos uno mayor o igual que 0 es una bola y tenemos que moverla,se lo pasamos a desplaza bola.
					if (desplazaBola(juegoBolas, j, i, direc)){ // si desplazabola devuelve true esque se tiene que acabar el juego porque se ha metido una bola.
						terminado = true;
					}
				}

			}
		}
	}
	else if (direc == 1){
		for (int i = 0; i < 20; i++){ // Recorremos los dos arrays.
			for (int j = 0; j < 20; j++){
				if (juegoBolas.tablero[i][j] >= 0){ // Al recorrer todos los componentes de los arrays si encontramos uno mayor o igual que 0 es una bola y tenemos que moverla,se lo pasamos a desplaza bola.
					if (desplazaBola(juegoBolas, j, i, direc)){ // si desplazabola devuelve true esque se tiene que acabar el juego porque se ha metido una bola.
						terminado = true;
					}
				}

			}
		}
	}
	else if (direc == 2){ // A la izquierda.
		for (int i = 0; i < 20; i++){ // Recorremos los dos arrays.
			for (int j = 0; j < 20; j++){
				if (juegoBolas.tablero[i][j] >= 0){ // Al recorrer todos los componentes de los arrays si encontramos uno mayor o igual que 0 es una bola y tenemos que moverla,se lo pasamos a desplaza bola.
					if (desplazaBola(juegoBolas, j, i, direc)){ // si desplazabola devuelve true esque se tiene que acabar el juego porque se ha metido una bola.
						terminado = true;
					}
				}

			}
		}
	}
	else if (direc == 3){ // abajo.
		for (int i = 20; i > 0; i--){ // Recorremos los dos arrays.
			for (int j = 0; j < 20; j++){
				if (juegoBolas.tablero[i][j] >= 0){ // Al recorrer todos los componentes de los arrays si encontramos uno mayor o igual que 0 es una bola y tenemos que moverla,se lo pasamos a desplaza bola.
					if (desplazaBola(juegoBolas, j, i, direc)){ // si desplazabola devuelve true esque se tiene que acabar el juego porque se ha metido una bola.
						terminado = true;
					}
				}

			}
		}
	}
	juegoBolas.NumTurnos++; // aumentamos el numero de turnos.
}