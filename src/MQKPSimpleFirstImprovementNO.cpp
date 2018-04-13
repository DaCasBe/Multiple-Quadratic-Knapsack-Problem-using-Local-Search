/*
 * MQKPSimpleFirstImprovementNO.cpp
 *
 * Fichero que define las funciones de la clase MQKPSimpleFirstImprovementNO.Forma parte del código esqueleto para el problema de las múltiples mochilas cuadráticas, ofrecido para las prácticas de la asignatura Metaheurísticas del Grado de Ingeniería Informática de la Universidad de Córdoba
 *
 * @author Carlos García cgarcia@uco.es
 */

#include <MQKPSimpleFirstImprovementNO.h>
#include <MQKPInstance.h>
#include <MQKPEvaluator.h>
#include <MQKPObjectAssignmentOperation.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

bool MQKPSimpleFirstImprovementNO::findOperation(MQKPInstance &instance, MQKPSolution &solution, MQKPChangeOperation &operation) {

	MQKPObjectAssignmentOperation *oaOperation = dynamic_cast<MQKPObjectAssignmentOperation*>(&operation);
	if (oaOperation == NULL){
		cerr << "MQKPSimpleFirstImprovementNO::findOperation recibió un objeto operation que no es de la clase MQKPObjectAssignmentOperation" << endl;
		exit(1);
	}

	//Crear una permutación de los índices de los objetos e inicializar algunas variables
	vector<int> perm;
	int numObjs = instance.getNumObjs();
	int numKnapsacks = instance.getNumKnapsacks();
	MQKPInstance::randomPermutation(numObjs, perm);

	/* TODO
	 * 1. Para todo objeto del problema (accediendo en el orden indicado en perm)
	 *   a. Para toda mochila del problema (Nota: no te olvides de ninguna)
	 *     i. Obtener el deltaFitness de asignar dicho objeto a dicha mochila en solution
	 *
	 *     ii. Si el deltaFitness es positivo
	 *       . actualizar los valores de la operación en oaOperation
	 *       . Salir devolviendo true
	 *
	 * 2. Si se llega a este punto, no se encontró ningún deltaPositivo y se devuelve false
	 *
	 */
	MQKPEvaluator evaluator;
	double EvaluatorFitness;

	for(int i=0;i<numObjs;i++){ //Exploramos todos los objeto
		for(int j=0;j<=numKnapsacks;j++){ //Exploramos todas las mochilas
			EvaluatorFitness =evaluator.computeDeltaFitness(instance,solution,perm[i],j);
			if(EvaluatorFitness>0){ //El deltaFitness es positivo
				oaOperation->setValues(perm[i],j,EvaluatorFitness); //Actualizamos los valores de la operacion

				return true;
			}
		}
	}

	return false;
}



