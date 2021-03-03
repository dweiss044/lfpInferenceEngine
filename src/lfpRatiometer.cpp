#include <lfpRatiometer.h>
#include <main_window.h>

using namespace std;

extern "C" Plugin::Object *createRTXIPlugin(void){
    int N = 10;
    double sampling = 200;
    return new lfpRatiometer(N, sampling);
}

static DefaultGUIModel::variable_t vars[] = {
  {
    "GUI label", "Tooltip description",
    DefaultGUIModel::PARAMETER | DefaultGUIModel::DOUBLE,
  },
{
    "input_LFP", "Input LFP",
    DefaultGUIModel::INPUT | DefaultGUIModel::DOUBLE,
  },
{
    "ratio", "Output LFP Power Ratio",
    DefaultGUIModel::OUTPUT | DefaultGUIModel::DOUBLE,
  },
{
    "A State", "Tooltip description", DefaultGUIModel::STATE,
  },
};

static size_t num_vars = sizeof(vars) / sizeof(DefaultGUIModel::variable_t);

// defining what's in the object's constructor
// user defines time window length (in samples) and sampling rate
lfpRatiometer::lfpRatiometer(int N_input, double sampling_input)
    : DefaultGUIModel("lfpRatiometer with Custom GUI", ::vars, ::num_vars)
{
    setWhatsThis("<p><b>lfpRatiometer:</b><br>QWhatsThis description.</p>");
    DefaultGUIModel::createGUI(vars, num_vars);
    customizeGUI();
    initParameters();
    update(INIT);
    refresh();
    QTimer::singleShot(0, this, SLOT(resizeMe()));

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
}

// defining what's in the object's destructor
lfpRatiometer::~lfpRatiometer(void) {
    // destroy plan
    fftw_destroy_plan(p);
    // free output array
    fftw_free(out);
    // free input array
    fftw_free(in);

    return;
}

// real-time RTXI function
void lfpRatiometer::execute() {

    // get new time series reading
    in_raw.push_back(input(0));

    // if time series is full
    if (in_raw.size() > N) {
        // cut it to size
        in_raw.erase(in_raw.begin());

        // run our LF/HF analysis
        makePSD();
        getRatio();

        // output the calculated lf_hf_ratio
        output(0) = lf_hf_ratio;
        
    }
    
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

// RTXI function for initializing parameters
void lfpRatiometer::initParameters(void)
{
  some_parameter = 0;
  some_state = 0;
}

// update function (not running in real time)
void lfpRatiometer::update(DefaultGUIModel::update_flags_t flag)
{
  switch (flag) {
    case INIT:
      period = RT::System::getInstance()->getPeriod() * 1e-6; // ms
      setParameter("GUI label", some_parameter);
      setState("A State", some_state);
      break;

    case MODIFY:
      some_parameter = getParameter("GUI label").toDouble();
      break;

    case UNPAUSE:
      break;

    case PAUSE:
      break;

    case PERIOD:
      period = RT::System::getInstance()->getPeriod() * 1e-6; // ms
      break;

    default:
      break;
  }
}

// RTXI's customizeGUI function
void lfpRatiometer::customizeGUI(void)
{
  QGridLayout* customlayout = DefaultGUIModel::getLayout();

  QGroupBox* button_group = new QGroupBox;

  QPushButton* abutton = new QPushButton("Button A");
  QPushButton* bbutton = new QPushButton("Button B");
  QHBoxLayout* button_layout = new QHBoxLayout;
  button_group->setLayout(button_layout);
  button_layout->addWidget(abutton);
  button_layout->addWidget(bbutton);
  QObject::connect(abutton, SIGNAL(clicked()), this, SLOT(aBttn_event()));
  QObject::connect(bbutton, SIGNAL(clicked()), this, SLOT(bBttn_event()));

  customlayout->addWidget(button_group, 0, 0);
  setLayout(customlayout);
}

void lfpRatiometer::aBttn_event(void) { }

void lfpRatiometer::bBttn_event(void) { }