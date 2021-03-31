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
        void initParameters();

        // needed to initialize lfpratiometer object
        int N = 1000; // initialized to 1000 samples
        double sampling; // set based on RT period

        // lfpRatiometer object
        lfpRatiometer lfpratiometer;

};


#endif