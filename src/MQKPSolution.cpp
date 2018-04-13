/*
 * MQKPSolution.cpp
 *
 * Fichero que define los métodos de la clase MQKPSolution. Forma parte del código esqueleto para el problema de las múltiples mochilas cuadráticas, ofrecido para las prácticas de la asignatura Metaheurísticas del Grado de Ingeniería Informática de la Universidad de Córdoba
 *
 * @author Carlos García cgarcia@uco.es
 */

#include "MQKPSolution.h"
#include "MQKPInstance.h"
#include <iostream>

using namespace std;

MQKPSolution::MQKPSolution(MQKPInstance &instance) {
	// inicializando las variables miembro. Inicialmente, todos los objetos estarán fuera de las mochilas ( = 0)
	_numObjs = instance.getNumObjs();
	_fitness = 0;
	_sol = new int[_numObjs];
	if (!_sol) {
		cerr << "No se ha reservado memoria correctamente para _sol" << endl;
		exit(-1);
	}
	int i;
	for (i = 0; i < _numObjs; i++) {
		_sol[i] = 0;
	}
}

MQKPSolution::~MQKPSolution() {
	//
	_numObjs = 0;
	_fitness = 0;
	delete [] _sol;
}

void MQKPSolution::putObjectIn(int object, int knapsack) {
	_sol[object] = knapsack;
}

int MQKPSolution::whereIsObject(int object) {
	return _sol[object];
}

double MQKPSolution::getFitness() const {
	return _fitness;
}

void MQKPSolution::setFitness(double fitness) {
	_fitness = fitness;
}

void MQKPSolution::copy(MQKPSolution& solution) {

	/* TODO
	 * 1. Copiar las asignaciones de objetos a mochilas
	 * 2. copiar el fitness
	 */

	//_sol=solution._sol; //Copiamos la representacion de la solucion
	for(int i=0;i<_numObjs;i++){
		_sol[i]=solution.whereIsObject(i);
	}
	//_numObjs=solution._numObjs; //Copiamos el numero de objetos
	setFitness(solution.getFitness()); //Copiamos la funcion de fitness
}
