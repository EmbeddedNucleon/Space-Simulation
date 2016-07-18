#ifndef DIALOG_H
#define DIALOG_H

#include "config.h"
#include "universecomponent.h"
#include "zodiac.h"
#include "visitor.h"
#include "visitable.h"
#include <QDialog>
#include <QTimer>
#include <QInputDialog>
#include <QScrollBar>
#include <QSlider>
#include <list>

class Visitor;

namespace Ui {
class Dialog;
}

class Dialog : public QDialog, public Visitable
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    virtual ~Dialog();

    //accept visitor
    virtual int accept(Visitor &visitor) {
        return visitor.visit(*this);
    }

    int getZoomWidth(int widthValue) {return 800 - (800 * (widthValue/100.0));}
    int getZoomHeight(int heightValue) {return 800 - (800 * (heightValue/100.0));}

private slots:
    //prepare the next frame, called by the timer
    void nextFrame();
    //toggle the simultation's pause state
    void togglePause();
    //toggle rendering of Zodiacs
    void toggleZodiacs();
    //toggle rendering of labels
    void toggleLabels();
    //toggle timer settings
    void toggleTimer();
    //toggle screen to include all bodies
    void toggleAdjust();
    //toggle reset
    void toggleReset();
    //toggle zoom settings
    void toggleZoom(int zoomValue);
    //toggle width setting
    void toggleWidth(int widthValue);
    //toggle height setting
    void toggleHeight(int heightValue);

private:
    //method called when the window is being redrawn
    void paintEvent(QPaintEvent *event);
    //pause (or unpause) the simulation
    void pause(bool pause);
    //handle key presses
    void keyPressEvent(QKeyEvent *event);

private:
    Ui::Dialog* ui;
    QTimer* m_timer; //Timer object for triggering updates

    //buttons for UI
    QPushButton* m_buttonPause;
    QPushButton* m_buttonZodiacs;
    QPushButton* m_buttonLabels;
    QPushButton* m_buttonTimer;
    QPushButton* m_buttonAdjust;
    QPushButton* m_buttonReset;

    //input for timer
    QInputDialog* m_inputTimer;
    QInputDialog* m_inputScale;

    //Pixmap
    QPixmap* m_pixmap;

    //input for scaling
    QSlider* m_sliderZoom;
    QSlider* m_sliderWidth;
    QSlider* m_sliderHeight;

    int m_width; //width of the window
    int m_height; //height of the window
    int m_zoom; //zoom value
    int m_inputTime; //time from user input
    int m_widthValue; // width value from scale
    int m_heightValue; //height value from scale
    double m_catastropheTime; //time step in a catastrophe
    bool m_paused; //is the simulation paused?
    bool m_catastrophe; // catastrophe mode
    bool m_renderZodiacs; //should Zodiacs be rendered?
    bool m_renderLabels; //should labels be rendered?
    long m_timestamp; //simulation time since simulation start
    UniverseComponent* m_universe; //The universe
    std::list<Zodiac>* m_zodiacs; //Vector of zodiac lines
    Config* m_config; //the singleton config instance

};

#endif // DIALOG_H
