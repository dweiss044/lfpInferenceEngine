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
        // I want to get rid of these
        double some_parameter;
        double some_state;
        double period;
        void initParameters();

        // default parameters for lfpRatiometer object
        int N = 2000;
        int sampling = 2000;

        // // lfpRatiometer object
        lfpRatiometer lfpratiometer;
    
    private slots:
        void aBttn_event(void);
        void bBttn_event(void);

};


#endif