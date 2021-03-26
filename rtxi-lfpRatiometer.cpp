#include <rtxi-lfpRatiometer.h>

using namespace std;

extern "C" Plugin::Object *createRTXIPlugin(void){
    return new rtxilfpRatiometer();
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
rtxilfpRatiometer::rtxilfpRatiometer(void) :
DefaultGUIModel("lfpRatiometer with Custom GUI", ::vars, ::num_vars),
lfpratiometer((int)2000, (double)2000) // constructing lfpRatiometer object
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
void rtxilfpRatiometer::customizeGUI(void)
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

void rtxilfpRatiometer::aBttn_event(void) { }

void rtxilfpRatiometer::bBttn_event(void) { }