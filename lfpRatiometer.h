#ifndef LFPRATIOMETER_H
#define LFPRATIOMETER_H

#include <default_gui_model.h>

#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include <complex.h>

#include <vector>
#include <iostream>

#include <fftw3.h>

class lfpRatiometer : public DefaultGUIModel {

    Q_OBJECT

    public:
        // constructor
        lfpRatiometer(int N_input, double sampling_input);

        // destructor
        ~lfpRatiometer(void);

        // execute
        void execute();

        // functions to make GUI
        void createGUI(DefaultGUIModel::variable_t*, int);
        void customizeGUI(void);
        
    protected:

        // update function
        virtual void update(DefaultGUIModel::update_flags_t);

    private:
        // RTXI complains without these
        double some_parameter;
        double some_state;
        double period;
        void initParameters();
    
        int N; // time window in samples
        int f_size; // nonredundant size of DFT
        double sampling; // sampling rate (Hz)
        double *in; // pointer to (windowed) time series
        std::vector<double> in_raw; // vector to hold raw time series
        fftw_complex *out; // pointer to DFT
        fftw_plan p; // stores FFTW3 plan

        std::vector<double> allfreqs;
        std::vector<double> psd;
        double lf_hf_ratio;
        double lf_low;
        double lf_high;
        double hf_low;
        double hf_high;

        std::vector<double> window; // time domain of window
        double s2; // window scaling factor

        // method sets window as rectangle
        void window_rect(){
            window.clear();
            s2 = 0;
            for (int j=0; j<N; j++){
                double val = 1;
                window.push_back(val);
                s2 = s2 + val*val;
            }
        }
        // method sets window as Hamming
        // supposed to be consistent with MATLAB hamming() function
        void window_hamming(){
            window.clear();
            s2 = 0;
            double pi = atan(1) * 4;
            for (int j=0; j<N; j++){
                double z = 2*pi*j/(N-1);
                double val = 0.54 - 0.46*cos(z);
                window.push_back(val);
                s2 = s2 + val*val;
            }
        }

        void makePSD();
        void getRatio();
    
    private slots:
        void aBttn_event(void);
        void bBttn_event(void);


};


#endif