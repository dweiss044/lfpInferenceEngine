#ifndef LFPINFERENCEENGINE
#define LFPINFERENCEENGINE

#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include <complex.h>

#include <vector>
#include <iostream>

#include <Python.h>
#include <string>
#include <vector>

class lfpInferenceEngine {
    public:
        // Member functions
        lfpInferenceEngine(); // constructor
        ~lfpInferenceEngine(); // destructor

        int callPythonFunction(std::vector<std::string> args, std::vector<PyObject*> pyArgs);

        void printInPython();

        void load();
        void load_data();

        void setFeats(PyObject *newFeats);
        void setModel(PyObject *newModel);
        void setScaler(PyObject *newFeats);
        //void setData(PyObject *newFeats);

        PyObject* getResult() {return pResult;};
        PyObject* getModel() {return pModel;};
        PyObject* getFeats() {return pFeats;};
        PyObject* getScaler() {return pScaler;};
        PyObject* getInference() {return pInference;};
        
        PyObject* pData;
        
    protected:

    private:
    
        PyObject *pModel;
        PyObject *pFeats;
        PyObject *pScaler;
        PyObject *pInference;
        PyObject *pResult;

};


#endif