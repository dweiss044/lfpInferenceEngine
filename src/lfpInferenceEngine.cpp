#include "lfpInferenceEngine.h"

using namespace std;

// defining what's in the object's constructor
// user defines time window length (in samples) and sampling rate
lfpInferenceEngine::lfpInferenceEngine() {

    int i;

    setenv("PYTHONPATH","./pymodules:~/anaconda3/envs/hmm/lib/python3.10/site-packages",1);
    std::cout << "Initializing python environment.\n";
    Py_Initialize();
	PyRun_SimpleString("import sys, os");
	//PyRun_SimpleString("sys.path.append(\"C:/Users/dweiss38/Documents/GitHub/utils-CorticalState/python\")");
	PyRun_SimpleString("sys.path.append(os.path.dirname(os.getcwd()))");
    /*FILE* fp = fopen("./pymodules/pyfuncs.py", "r");
    int re = PyRun_SimpleFile(fp, "./pymodules/pyfuncs.py");
    */
    PyRun_SimpleString("print('Python Session started.')");

}

// defining what's in the object's destructor
lfpInferenceEngine::~lfpInferenceEngine(void) {
    std::cout << "Ending Python session.\n";
    Py_DECREF(pResult);
    if (Py_FinalizeEx() < 0) {
        exit(120);
    }
    std::cout << "Python session ended. \n";
}
/*
void lfpInferenceEngine::loadModel(void) {
    PyRun_SimpleString("print('Python Session started.')");
}

void lfpInferenceEngine::infer(void) {
    PyRun_SimpleString("print(infer())");
}
*/
void lfpInferenceEngine::printInPython(void) {
    PyRun_SimpleString("print('Python session still on')");
    return;
}


int lfpInferenceEngine::callPythonFunction(vector<string> args, vector<PyObject*> pyArgs = {}) {

    int length = args.size();
    PyObject *pName, *pModule, *pFunc;
    PyObject *pArgs, *pValue;
    int i;

    if (length < 2) {
        fprintf(stderr,"Usage: call pythonfile funcname [args]\n");
        return 1;
    }

	pName = PyUnicode_DecodeFSDefault(args[0].c_str());
    /* Error checking of pName left out */

    pModule = PyImport_Import(pName);
    Py_DECREF(pName);

    if (pModule != NULL) {
        pFunc = PyObject_GetAttrString(pModule, args[1].c_str());
        /* pFunc is a new reference */

        if (pFunc && PyCallable_Check(pFunc)) {
            if (pyArgs.empty()) {
                pArgs = PyTuple_New(length - 2);
                for (i = 0; i < length - 2; ++i) {
                    //pValue = PyLong_FromLong(atoi(args[i + 2].c_str()));
                    pValue = PyUnicode_FromString(args[i + 2].c_str());
                    if (!pValue) {
                        Py_DECREF(pArgs);
                        Py_DECREF(pModule);
                        fprintf(stderr, "Cannot convert argument\n");
                        return 1;
                    }
                    /* pValue reference stolen here: */
                    PyTuple_SetItem(pArgs, i, pValue);
                }
            }
            else {
                pArgs = PyTuple_New(pyArgs.size());
                for (i = 0; i < pyArgs.size(); i++) {
                    PyTuple_SetItem(pArgs, i, pyArgs[i]);
                }
            }
            pValue = PyObject_CallObject(pFunc, pArgs);
            Py_DECREF(pArgs);
            if (pValue != NULL) {
                if (!PyTuple_Check(pValue)) {
                    printf("Result of call: %ld\n", PyLong_AsLong(pValue));
                } 
                else {
                    printf("Result is tuple, will skip outputting result of call.\n");
                }
                this->pResult = Py_NewRef(pValue);

                Py_DECREF(pValue);
                //Py_DECREF(pResult);
            }
            else {
                Py_DECREF(pFunc);
                Py_DECREF(pModule);
                PyErr_Print();
                fprintf(stderr,"Call failed\n");
                return 1;
            }
        }
        else {
            if (PyErr_Occurred())
                PyErr_Print();
            fprintf(stderr, "Cannot find function \"%s\"\n", args[1].c_str());
        }
        Py_XDECREF(pFunc);
        Py_DECREF(pModule);
        
    }
    else {
        PyErr_Print();
        fprintf(stderr, "Failed to load \"%s\"\n", args[0].c_str());
        return 1;
    }
    //return pValue;
    return 0;

}


void lfpInferenceEngine::setModel(PyObject *newModel) {
    pModel = newModel;
}

void lfpInferenceEngine::setFeats(PyObject *newFeats) {
    pFeats = newFeats;
}

void lfpInferenceEngine::setScaler(PyObject *newModel) {
    pModel = newModel;
}

void lfpInferenceEngine::load() {
    if (!PyTuple_Check(this->pResult)) {
        printf("pResult is not a tuple! Make sure to call callPythonFunction correctly before loading.\n");
        return;
    }

    if (!PyArg_ParseTuple(this->pResult,"OOO", &pModel, &pFeats, &pScaler)) {
        printf("Failed to unpack tuple.\n");
        return;
    }
}

void lfpInferenceEngine::load_data() {
    if (PyTuple_Check(this->pResult)) {
        printf("pResult is a tuple! Make sure to call callPythonFunction correctly before loading data.\n");
        return;
    }

    pData = this->pResult;
}