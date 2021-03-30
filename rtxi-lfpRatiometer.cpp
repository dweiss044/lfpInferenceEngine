#include <rtxi-lfpRatiometer.h>

using namespace std;

extern "C" Plugin::Object *createRTXIPlugin(void){
    return new rtxilfpRatiometer();
}

static DefaultGUIModel::variable_t vars[] = {
{ 
  "Time Window", "Time Window (s)", 
  DefaultGUIModel::PARAMETER | DefaultGUIModel::DOUBLE
  },
{ 
  "Sampling Rate", "Sampling Rate (Hz)", 
  DefaultGUIModel::STATE | DefaultGUIModel::DOUBLE,
  },
{ 
  "LF Lower Bound", "LF Lower Bound",
  DefaultGUIModel::PARAMETER | DefaultGUIModel::DOUBLE,
  },
{ 
  "LF Upper Bound", "LF Upper Bound",
  DefaultGUIModel::PARAMETER | DefaultGUIModel::DOUBLE,
  },
{ 
  "HF Lower Bound", "HF Lower Bound",
  DefaultGUIModel::PARAMETER | DefaultGUIModel::DOUBLE,
  },
{ 
  "HF Upper Bound", "HF Upper Bound",
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
};

static size_t num_vars = sizeof(vars) / sizeof(DefaultGUIModel::variable_t);

// defining what's in the object's constructor
// user defines time window length (in samples) and sampling rate
rtxilfpRatiometer::rtxilfpRatiometer(void) :
DefaultGUIModel("lfpRatiometer with Custom GUI", ::vars, ::num_vars),
lfpratiometer(N, sampling) // constructing lfpRatiometer object
{
    setWhatsThis("<p><b>lfpRatiometer:</b><br>Given an input, this module calculates the LF/HF ratio over a specified causal time window.</p>");
    DefaultGUIModel::createGUI(vars, num_vars);
    customizeGUI();
    initParameters();
    update(INIT);
    refresh();
    QTimer::singleShot(0, this, SLOT(resizeMe()));
}

// defining what's in the object's destructor
rtxilfpRatiometer::~rtxilfpRatiometer(void) { }

// real-time RTXI function
void rtxilfpRatiometer::execute(void) {

  // push new time series reading to lfpRatiometer
  lfpratiometer.pushTimeSample(input(0));

  // calculate LF/HF ratio
  lfpratiometer.calcRatio();

  // put the LF/HF ratio into the output
  output(0) = lfpratiometer.getRatio();
    
}

// RTXI function for initializing parameters
void rtxilfpRatiometer::initParameters(void)
{
  some_parameter = 0;
  some_state = 0;
}

// update function (not running in real time)
void rtxilfpRatiometer::update(DefaultGUIModel::update_flags_t flag)
{
  switch (flag) {
    case INIT:
      period = RT::System::getInstance()->getPeriod() * 1e-6; // ms
      setParameter("Time Window", sampling/N);
      setParameter("Sampling Rate", RT::System::getInstance()->getFrequency());
      setParameter("LF Lower Bound", (double)1); // need to amend where these come from
      setParameter("LF Upper Bound", (double)10);
      setParameter("HF Lower Bound", (double)30);
      setParameter("HF Upper Bound", (double)90);
      break;

    case MODIFY:
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
void rtxilfpRatiometer::customizeGUI(void)
{
  QGridLayout* customlayout = DefaultGUIModel::getLayout();

  // adding dropdown menu for choosing FFT window shape
  QLabel* windowLabel = new QLabel("Window:");
  windowShape = new QComboBox;
  windowShape->insertItem(1, "Rectangular")
  windowShape->insertItem(2, "Hamming")
  QObject::connect(windowShape, SIGNAL(activated(int)), this, SLOT(updateWindow(int)));

  customlayout->addWidget(windowLabel, 0, 0);
  customlayout->addWidget(windowShape, 0, 1);
  setLayout(customlayout);
}