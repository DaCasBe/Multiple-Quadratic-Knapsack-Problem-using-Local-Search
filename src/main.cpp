/*	main.cpp
 *
 * Fichero que define la función main para ejecutar experimentos sobre el Multiple Quadratic Knapsack Problem.
 * En particular, estos experimentos generan varias optimizaciones locales sobre cuatro instancias y van almacenando el mejor resultado hasta el momento.
 *
 * Forma parte del código esqueleto para el problema de las múltiples mochilas cuadráticas, ofrecido para las prácticas de la asignatura Metaheurísticas del Grado de Ingeniería Informática de la Universidad de Córdoba
 *
 * @author Carlos García cgarcia@uco.es
 */

//Definición de algunos parámetros de la experimentación
#define MAX_SECONS_PER_RUN 5
#define MAX_SOLUTIONS_PER_RUN 100000
#define MAX_INITIAL_SOLUTIONS 5

#include <MQKPEvaluator.h>
#include <MQKPInstance.h>
#include <MQKPSolGenerator.h>
#include <MQKPSolution.h>
#include <MQKPLocalSearch.h>
#include <MQKPSimpleFirstImprovementNO.h>
#include <MQKPSimpleBestImprovementNO.h>
#include <Timer.h>

#include <iostream>
using namespace std;

//Definición externa de las semillas para el generador de números aleatorios (en seeds.cpp)
//extern unsigned int numSeeds;
extern unsigned int seeds[];

/**
 * Función que realiza varias optimizaciones locales de soluciones aleatorias, durante MAX_SECONS_PER_RUN segundos, un máximo de MAX_SOLUTIONS_PER_RUN y un máximo de MAX_INITIAL_SOLUTIONS optimizaciones locales, para la instancia proporcionada
 * @param[out] currentResults vector donde se almacenarán los valores fitness de las soluciones que la búsqueda local va aceptando en todo momento.
 * @param[out] bestSoFarResults vector donde se almacenarán los mejores valores fitness generados hasta el momento
 * @param[in] instance Instancia del problema de la mochila cuadrática múltiple
 * @param[in] explorer Operador de exploración de vecindarios a utilizar
 */
void runALSExperiment(vector<double> &currentResults,
		vector<double> &bestSoFarResults, MQKPInstance &instance,
		MQKPNeighExplorer &explorer) {

	//Inicialización
	MQKPLocalSearch ls;
	MQKPSolution initialSolution(instance);
	Timer t;
	MQKPEvaluator::resetNumEvaluations();

	//Generar una primera solución aleatoria para inicializar bestFitness
	MQKPSolGenerator::genRandomSol(instance, initialSolution);
	double currentFitness = MQKPEvaluator::computeFitness(instance,
			initialSolution);
	double bestFitness = currentFitness;
	initialSolution.setFitness(currentFitness);
	currentResults.push_back(currentFitness);
	bestSoFarResults.push_back(bestFitness);
	int numInitialSolutions = 0;

	//Mientras no se acabe el tiempo y no se generen demasiadas soluciones,	generar solución aleatoria, aplicarle la optimización y guardar el resultado
	while (t.elapsed_time(Timer::VIRTUAL) <= MAX_SECONS_PER_RUN
			&& MQKPEvaluator::getNumEvaluations() < MAX_SOLUTIONS_PER_RUN &&
			numInitialSolutions < MAX_INITIAL_SOLUTIONS) {

		/*
		 * TODO Generar una nueva solución aleatoria en initialSolution
		 * e inicializa su fitness
		 */
		MQKPSolGenerator::genRandomSol(instance, initialSolution);//solución aleatoria en initialSolution

		initialSolution.setFitness(MQKPEvaluator::computeFitness(instance,initialSolution));//Inicializar fitness

		currentResults.push_back(currentFitness);
		bestSoFarResults.push_back(
				max(bestSoFarResults.back(), currentFitness));

		//TODO Invoca a MQKPLocalSearch::optimise para optimizar la solución
		ls.optimise(instance, explorer, initialSolution);

		//Almacenar los resultados
		vector<double> &resultsLS = ls.getResults();

		for (auto aResult : resultsLS) {
			currentResults.push_back(aResult);
			bestSoFarResults.push_back(max(bestSoFarResults.back(), aResult));
		}

		numInitialSolutions++;
	}

}

/**
 * Función que ejecuta todos los experimentos para los argumentos pasados al programa principal
 * @param[out] results matriz 3D donde se almacenarán los resultados. El primer índice será para la instancia considerada. El segundo para el experimento sobre dicha instancia. El tercero para la solución generada en dicho experimento
 * @param[in] mainArgs Argumentos de la función main (argv). En los argumentos vienen, desde el índice 1, <nombre del fichero de la instancia 1> <número de mochilas> <nombre del fichero de la instancia 2> <número de mochilas>...
 * @param[in] numMainArgs Número de argumentos de la función main (argc)
 */
void runExperiments(vector<vector<vector<double>*>*> &results, char **mainArgs,
		unsigned int numMainArgs) {

	MQKPSimpleFirstImprovementNO firstExplorer;
	MQKPSimpleBestImprovementNO bestExplorer;

	//Para cada instancia del problema
	for (unsigned int iInstance = 1; iInstance < numMainArgs; iInstance += 2) {

		//Leer la instancia y crear la estructuras de datos necesarias para guardar los resultados
		MQKPInstance instance;
		vector<vector<double>*>* resultsOnThisInstance = new vector<
				vector<double>*>;
		results.push_back(resultsOnThisInstance);
		char *instanceName = mainArgs[iInstance];
		unsigned int numKnapsacks = atoi(mainArgs[iInstance + 1]);
		instance.readInstance(instanceName, numKnapsacks);

		//Ejecutar la búsqueda local con primera mejora
		vector<double> *theseFirstResults = new vector<double>;
		vector<double> *bestFirstResults = new vector<double>;
		resultsOnThisInstance->push_back(theseFirstResults);
		resultsOnThisInstance->push_back(bestFirstResults);
		runALSExperiment(*theseFirstResults, *bestFirstResults, instance,
				firstExplorer);

		//Ejecutar la búsqueda local con mejor mejora
		vector<double> *theseBestResults = new vector<double>;
		vector<double> *bestBestResults = new vector<double>;
		resultsOnThisInstance->push_back(theseBestResults);
		resultsOnThisInstance->push_back(bestBestResults);
		runALSExperiment(*theseBestResults, *bestBestResults, instance,
				bestExplorer);
	}
}

/**
 * Función que libera la memoria de un vector 2D
 * @param[in] array vector 2D que se va a liberar de memoria
 */
template<typename T>
void free2Darray(vector<vector<T>*> &array) {

	int numVectors = array.size();

	for (int i = 0; i < numVectors; i++) {
		array.at(i)->clear();
		delete (array.at(i));
	}

	array.clear();
}

/**
 * Función que libera la memoria de un vector 3D
 * @param[in] array matriz 3D que se va a liberar de memoria
 */
template<typename T>
void free3Darray(vector<vector<vector<T>*>*> &array) {

	int numArrays = array.size();

	for (int i = 0; i < numArrays; i++) {
		free2Darray(*(array.at(i)));
		delete (array.at(i));
	}

	array.clear();
}

/**
 * Función main que ejecuta la experimentación. Optimización local de soluciones aleatorias para las instancias
 *
 * Finalmente se escribe en la salida estandar los resultados generados en forma de matriz. Por cada instancia se generan cuatro columnas, una con los resultados
 * que va aceptando la búsqueda local de primera mejora en cada momento, otra con los mejores resultados hasta el momento de la búsqueda local anterior,
 * otra con los resultados que va aceptando la búsqueda local de mejor mejora en cada momento y una última con los mejores resultados hasta el momento de la búsqueda local anterior
 */
int main(int argc, char **argv) {

	if (argc < 3 || (argc % 2) != 1) {
		cout << argv[0] << " (<problem instance files> <numKnapsacks>)+"
				<< endl;
		return 0;
	}

	//////////////////////////
	//Ejecución de experimentos
	//////////////////////////
	unsigned int numInstances = (argc - 1) / 2;
	vector<vector<vector<double>*>*> allTheResults;
	srand(seeds[0]);
	//En la matriz allTheResults se almacenarán todos los valores de fitness generados
	//Es tridimensional
	//El primer índice recorre las instancias de los problemas que se han abordado
	//El segundo índice recorre las ejecuciones que se han hecho sobre la misma instancia
	//El tercer índice recorre las soluciones que se han generado en la misma ejecución

	runExperiments(allTheResults, argv, argc);






	//////////////////////
	// Impresión de resultados
	//////////////////////
	vector<double> lastResults;
	for (unsigned int iInstance = 0; iInstance < numInstances; iInstance++){
		cout << argv[iInstance*2+1] << "_" << argv[iInstance*2+2] << "_currentFirst\t";
		cout << argv[iInstance*2+1] << "_" << argv[iInstance*2+2] << "_bestFirst\t";
		cout << argv[iInstance*2+1] << "_" << argv[iInstance*2+2] << "_currentBest\t";
		cout << argv[iInstance*2+1] << "_" << argv[iInstance*2+2] << "_bestBest\t";
		lastResults.push_back(allTheResults.at(iInstance)->at(0)->at(0));
		lastResults.push_back(allTheResults.at(iInstance)->at(1)->at(0));
		lastResults.push_back(allTheResults.at(iInstance)->at(2)->at(0));
		lastResults.push_back(allTheResults.at(iInstance)->at(3)->at(0));
	}
	cout << endl;
	bool allResultsPrinted = false;
	unsigned int iIteration = 0;
	unsigned int indexColumn = 0;

	while (allResultsPrinted == false){
		allResultsPrinted = true;
		for (unsigned int iInstance = 0; iInstance < numInstances; iInstance++){

			if (allTheResults.at(iInstance)->at(0)->size() > iIteration){
				allResultsPrinted = false;
				cout << allTheResults.at(iInstance)->at(0)->at(iIteration) << "\t";
				lastResults[indexColumn] = allTheResults.at(iInstance)->at(0)->at(iIteration);
			} else {
				cout << lastResults[indexColumn] << "\t";
			}
			indexColumn++;

			if (allTheResults.at(iInstance)->at(1)->size() > iIteration){
				allResultsPrinted = false;
				cout << allTheResults.at(iInstance)->at(1)->at(iIteration) << "\t";
				lastResults[indexColumn] = allTheResults.at(iInstance)->at(1)->at(iIteration);
			} else {
				cout << lastResults[indexColumn] << "\t";
			}
			indexColumn++;

			if (allTheResults.at(iInstance)->at(2)->size() > iIteration){
				allResultsPrinted = false;
				cout << allTheResults.at(iInstance)->at(2)->at(iIteration) << "\t";
				lastResults[indexColumn] = allTheResults.at(iInstance)->at(2)->at(iIteration);
			} else {
				cout << lastResults[indexColumn] << "\t";
			}
			indexColumn++;

			if (allTheResults.at(iInstance)->at(3)->size() > iIteration){
				allResultsPrinted = false;
				cout << allTheResults.at(iInstance)->at(3)->at(iIteration) << "\t";
				lastResults[indexColumn] = allTheResults.at(iInstance)->at(3)->at(iIteration);
			} else {
				cout << lastResults[indexColumn] << "\t";
			}
			indexColumn++;
		}
		cout << endl;
		iIteration++;
		indexColumn = 0;
	}




	//////////////////////
	// Liberar memoria
	//////////////////////
	free3Darray(allTheResults);
	return 0;
}
