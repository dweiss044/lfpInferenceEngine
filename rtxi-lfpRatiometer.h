#ifndef RTXILFPRATIOMETER_H
#define RTXILFPRATIOMETER_H

#include <default_gui_model.h>
#include <main_window.h>
#include <lfpRatiometer>

class rtxilfpRatiometer : public DefaultGUIModel {

    Q_OBJECT

    public:
        // constructor
        rtxilfpRatiometer(void);

        // destructor
        virtual ~rtxilfpRatiometer(void);

        // execute
        void execute(void);

        // functions to make GUI
        void createGUI(DefaultGUIModel::variable_t*, int);
        void customizeGUI(void);
        
    protected:

        // update function
        virtual void update(DefaultGUIModel::update_flags_t);

    private:
        double period;
        double sampling_freq;
        void initParameters();

        // default parameters for lfpRatiometer object
        int N = 2000;
        double sampling = 2000;

        // lfpRatiometer object
        lfpRatiometer lfpratiometer;

};


#endif