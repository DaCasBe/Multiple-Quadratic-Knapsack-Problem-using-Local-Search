/*
 * MQKPObjectAssignmentOperation.cpp
 *
 * Fichero que define las funciones de la clase MQKPObjectAssignmentOperation. Forma parte del código esqueleto para el problema de las múltiples mochilas cuadráticas, ofrecido para las prácticas de la asignatura Metaheurísticas del Grado de Ingeniería Informática de la Universidad de Córdoba
 *
 * @author Carlos García cgarcia@uco.es
 */

#include <MQKPObjectAssignmentOperation.h>

MQKPObjectAssignmentOperation::MQKPObjectAssignmentOperation(){
	_indexKnapsack = 0;
	_indexObj = 0;
	_deltaFitness = 0;
}

MQKPObjectAssignmentOperation::~MQKPObjectAssignmentOperation() {
}

void MQKPObjectAssignmentOperation::apply(MQKPSolution& solution) {
	/* TODO
	 * 1. Asigna el objeto de índice _indexObj a la mochila _indexKnapsack en solution
	 * 2. Actualiza el fitness de solution sumándole _deltaFitness
	 */

	solution.putObjectIn(_indexObj,_indexKnapsack); //Guardamos el objeto en la mochila
	solution.setFitness(_deltaFitness+solution.getFitness()); //Modificamos el fitness
}

void MQKPObjectAssignmentOperation::setValues(int indexObject,
		int indexKnapsack, double deltaFitness) {
	/* TODO
	 * Guarda los valores pasados como argumentos en las variables miembro
	 */

	_indexObj=indexObject;
	_indexKnapsack=indexKnapsack;
	_deltaFitness=deltaFitness;
}
