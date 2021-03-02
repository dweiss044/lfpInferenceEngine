#include <lfpRatiometer.h>

using namespace std;


// defining what's in the object's constructor
// user defines time window length (in samples) and sampling rate
lfpRatiometer::lfpRatiometer(int N_input, double sampling_input) {
    // setting user defined variables
    N=N_input;
    f_size=N/2 + 1;
    sampling=sampling_input;

    // setting default variables
    lf_low = 1;
    lf_high = 25;
    hf_low = 30;
    hf_high = 90;
    
    // setting default window
    window_hamming();

    // establishing frequencies that will be associated with DFT output
    for (int n=0; n<f_size;n++){
        allfreqs.push_back(sampling*n/N);
    }

    // allocating space for input time array
    in = (double*) fftw_malloc(sizeof(double)*N);
    // allocating space for N DFT terms
    // note: this has to be N, not N/2 + 1, for fftw_free()
    out = fftw_alloc_complex(N);
    // creating the FFTW3 plan
    // FFTW_MEASURE makes initialization long for faster...
    // calculations
    p = fftw_plan_dft_r2c_1d(N,in,out,FFTW_MEASURE);

    // tmp
    for (int j=0; j<N; j++){
        in_raw.push_back(1);
    }
    in_raw.at(1) = 2;
    in_raw.at(8) = 2;

    // fill_n(in, N, 1); // making input a constant vector for now
    // in[1] = 2;
    // in[8] = 2;
    cout << "constructed\n";
}

// defining what's in the object's destructor
lfpRatiometer::~lfpRatiometer(void) {
    // destroy plan
    fftw_destroy_plan(p);
    // free output array
    fftw_free(out);
    // free input array
    fftw_free(in);

    cout << "destroyed\n";
    return;
}

// function that operates on the input vector
void lfpRatiometer::execute() {
    makePSD();
    getRatio();

    // my observation
    cout << "window:\n";
    for (int i=0; i<N;i++){
        cout << window[i] << "\n";
    }

    cout << "freq, psd\n";
    for (int i=0; i<f_size; i++) {
        cout << allfreqs[i] << "," << psd[i] << "\n";
    }

    cout << "LF/HF Ratio: " << lf_hf_ratio << "\n";
}

// function that calculates the power spectral density
void lfpRatiometer::makePSD() {
    psd.clear(); // clear vector which stores PSD

    // applying window
    for (int n=0; n<N; n++){
        in[n] = in_raw[n]*window[n];
    }

    fftw_execute(p); // FFTW3 calculate the DFT

    // loop through DFT entries
    for (int n=0; n<f_size; n++){
        // calculate |y_n|^2
        double fftsqr = out[n][0]*out[n][0] + out[n][1]*out[n][1];

        // calculate PSD (described in Heinzel et al., 2002)
        // DC & Nyquist freq is scaled to be consistent with MATLAB's spectrogram
        // Usually, the edges should not matter because all frequencies of interest
        // should be contained in the interior
        // WILL CHANGE WITH WINDOWING
        if (n==0 || n==f_size-1) {
            psd.push_back((1/(sampling*s2)) * fftsqr);
        }
        else {
            psd.push_back(2*(1/(sampling*s2)) * fftsqr);
        }
    }
}

// // function that calculates LF/HF ratio
void lfpRatiometer::getRatio() {
    double lf_total = 0;
    double hf_total = 0;

    // iterates over PSD, calculating running sums in each band
    for (int n=0; n<f_size; n++){
        if (allfreqs.at(n)>= lf_low && allfreqs.at(n) <= lf_high) {
            lf_total = lf_total + psd.at(n);
        }
        if (allfreqs.at(n) >= hf_low && allfreqs.at(n) <= hf_high){
            hf_total = hf_total + psd.at(n);
        }
    }

    // take ratio
    lf_hf_ratio = lf_total/hf_total;
}