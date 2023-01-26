#include <lfpRatiometer.h>
#include "lfpInferenceEngine.h"

#include <chrono>
#include <iostream>
#include <fstream>

int main () {

    // instantiate an object
    //std::ofstream FILE ("time_predict2.csv",std::ios_base::app);
    //FILE << "Constructor, Model Load, Data load, Predict, Infer\n";
    int N = 10;
    double sampling = 200;
    lfpRatiometer lfpratiometer(N, sampling);
    auto start = std::chrono::high_resolution_clock::now();
    lfpInferenceEngine inferenceEngine;
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    //FILE << duration.count() << ",";
    std::cout << "Constructor time: " << duration.count() << std::endl;

    int i;
    //inferenceEngine.printInPython();
    
    auto start1 = std::chrono::high_resolution_clock::now();
    std::vector<std::string> arguments_load = {"pyfuncs","get_model","AP103_1","4b-bp-03200-fs500-win512ms-step40ms"};
    inferenceEngine.init("AP103_1","4b-bp-03200-fs500-win512ms-step40ms");
    auto stop1 = std::chrono::high_resolution_clock::now();
    auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(stop1 - start1);
    //FILE << duration1.count() << ",";
    std::cout << "Init time: " << duration1.count() << std::endl;


    auto start3 = std::chrono::high_resolution_clock::now();
    std::vector<std::string> arguments_predict = {"pyfuncs","predict"};
    std::vector<PyObject*> pyArgs = {inferenceEngine.getModel(), inferenceEngine.getFeats(), inferenceEngine.getScaler(), inferenceEngine.getData()};
    i = inferenceEngine.callPythonFunction(arguments_predict, pyArgs);
    auto stop3 = std::chrono::high_resolution_clock::now();
    auto duration3 = std::chrono::duration_cast<std::chrono::microseconds>(stop3 - start3);
    //FILE << duration3.count() << ",";
    std::cout << "Predict time: " << duration3.count() << std::endl;

    start3 = std::chrono::high_resolution_clock::now();
    pyArgs = {inferenceEngine.getModel(), inferenceEngine.getFeats(), inferenceEngine.getScaler(), inferenceEngine.getData()};
    i = inferenceEngine.callPythonFunction(arguments_predict, pyArgs);
    stop3 = std::chrono::high_resolution_clock::now();
    duration3 = std::chrono::duration_cast<std::chrono::microseconds>(stop3 - start3);
    //FILE << duration3.count() << ",";
    std::cout << "Predict time2: " << duration3.count() << std::endl;

    //printf("Result of call2: %ld\n", PyLong_AsLong(inferenceEngine.getResult()));
    
    auto start4 = std::chrono::high_resolution_clock::now();
    std::vector<std::string> arguments = {"pyfuncs","infer"};
    i = inferenceEngine.callPythonFunction(arguments,{});
    auto stop4 = std::chrono::high_resolution_clock::now();
    auto duration4 = std::chrono::duration_cast<std::chrono::microseconds>(stop4 - start4);
    //FILE << duration4.count() << "\n";
    std::cout << "Infer time: " << duration4.count() << std::endl;
    Py_INCREF(inferenceEngine.getResult());
    printf("Result of Infer: %ld\n", PyLong_AsLong(inferenceEngine.getResult()));
    
    //FILE.close();
    //std::cout << "Infer time: " << duration4.count() << std::endl;
    //std::cout << "pResult: " << inferenceEngine.pResult << std::endl;

    //Py_INCREF(inferenceEngine.getResult());
    //printf("Result of call3: %ld\n", PyLong_AsLong(inferenceEngine.getResult()));
    //inferenceEngine.printInPython();
    //Py_DECREF(&inferenceEngine.pResult);

    return 0;

}