#include "dialog.h"
#include "ui_dialog.h"
#include "config.h"
#include "universebody.h"
#include "universecomposite.h"
#include "widthvisitor.h"
#include <QKeyEvent>
#include <QPainter>
#include <QPushButton>
#include <QTimer>
#include <QImage>
#include <QPixmap>
#include <iostream>
#include <cmath>

//class WidthVisitor;

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
    , m_width(800)
    , m_height(800)
    , m_zoom(20)
    , m_catastropheTime(100.0)
    , m_inputTime(100)
    , m_catastrophe(false)
    , m_paused(false)
    , m_renderZodiacs(true)
    , m_renderLabels(true)
    , m_timestamp(0)
    , m_config(Config::getInstance())
{
    m_config->read("config.txt");
    m_universe = m_config->parseUniverseBlocks();
    m_zodiacs = m_config->parseZodiacBlocks();
    m_universe->convertRelativeToAbsolute(0,0,0,0);
    m_config->getInstance()->m_trajectory = new std::vector< std::pair<double,double> >;

    //make the window appear
    ui->setupUi(this);
    this->resize(m_width, m_height);

    //create the buttons
    m_buttonPause = new QPushButton("Pause", this);
    m_buttonZodiacs = new QPushButton("Zodiacs", this);
    m_buttonLabels = new QPushButton("Labels", this);
    m_buttonTimer = new QPushButton("Timer", this);
    m_buttonAdjust = new QPushButton("Adjust", this);
    m_buttonReset = new QPushButton("Reset", this);

    //create pixmap
    m_pixmap = new QPixmap;
    m_pixmap->load("Space.bmp");

    //set button properties
    m_buttonPause->setGeometry(QRect(QPoint(0, 0), QSize(100, 50)));
    m_buttonZodiacs->setGeometry(QRect(QPoint(100, 0), QSize(100, 50)));
    m_buttonLabels->setGeometry(QRect(QPoint(200, 0), QSize(100, 50)));
    m_buttonTimer->setGeometry(QRect(QPoint(300, 0), QSize(100, 50)));
    m_buttonAdjust->setGeometry(QRect(QPoint(400, 0), QSize(100, 50)));
    m_buttonReset->setGeometry(QRect(QPoint(500, 0), QSize(100, 50)));
    m_buttonPause->setStyleSheet("background-color: #3cbaa2; color: white");
    m_buttonZodiacs->setStyleSheet("background-color: #3cbaa2; color: white");
    m_buttonLabels->setStyleSheet("background-color: #3cbaa2; color: white");
    m_buttonTimer->setStyleSheet("background-color: #3cbaa2; color: white");
    m_buttonAdjust->setStyleSheet("background-color: #3cbaa2; color: white");
    m_buttonReset->setStyleSheet("background-color: #3cbaa2; color: white");

    //create input dialogs
    m_inputTimer = new QInputDialog();
    m_inputTimer->setOptions(QInputDialog::NoButtons);

    //create the sliders
    m_sliderZoom = new QSlider(Qt::Vertical, this);
    m_sliderWidth = new QSlider(Qt::Horizontal, this);
    m_sliderHeight = new QSlider(Qt::Vertical, this);
    m_sliderZoom->setGeometry(QRect(QPoint(m_width-100, 50), QSize(50, 200)));
    m_sliderWidth->setGeometry(QRect(QPoint(0, 50), QSize(200, 50)));
    m_sliderHeight->setGeometry(QRect(QPoint(0, 100), QSize(50, 200)));
    m_sliderZoom->setValue(20);
    m_sliderWidth->setValue(0);
    m_sliderHeight->setValue(0);
    m_sliderWidth->setRange(-300,300);
    m_sliderHeight->setRange(-300,300);

    //connect gui objects
    connect(m_buttonPause, SIGNAL(released()), this, SLOT(togglePause()));
    connect(m_buttonZodiacs, SIGNAL(released()), this, SLOT(toggleZodiacs()));
    connect(m_buttonLabels, SIGNAL(released()), this, SLOT(toggleLabels()));
    connect(m_buttonTimer, SIGNAL(released()), this, SLOT(toggleTimer()));
    connect(m_buttonAdjust, SIGNAL(released()), this, SLOT(toggleAdjust()));
    connect(m_buttonReset, SIGNAL(released()), this, SLOT(toggleReset()));
    connect(m_sliderZoom, SIGNAL(valueChanged(int)), this, SLOT(toggleZoom(int)));
    connect(m_sliderWidth, SIGNAL(valueChanged(int)), this, SLOT(toggleWidth(int)));
    connect(m_sliderHeight, SIGNAL(valueChanged(int)), this, SLOT(toggleHeight(int)));

    //setup timer
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(nextFrame()));
    pause(false);
}

Dialog::~Dialog()
{
    delete ui;
    delete m_timer;
    delete m_buttonPause;
    delete m_buttonZodiacs;
    delete m_buttonLabels;
    delete m_buttonTimer;
    delete m_buttonAdjust;
    delete m_buttonReset;
    delete m_inputScale;
    delete m_inputTimer;
    delete m_sliderZoom;
    delete m_sliderWidth;
    delete m_sliderHeight;
    delete m_universe;
    delete m_zodiacs;
    delete m_pixmap;
}

void Dialog::toggleZodiacs()
{
    m_renderZodiacs = !m_renderZodiacs;
}

void Dialog::toggleLabels()
{
    m_renderLabels = !m_renderLabels;
}

void Dialog::togglePause()
{
    pause(!m_paused);
}

void Dialog::toggleTimer()
{
    bool ok;
    m_inputTime =  m_inputTimer->getInt(NULL, "Set timer", "Enter a positive integer", 0, 0, 2147483647, 1, &ok, 0);
    if (ok && m_inputTime != 0){
        m_timer->start(m_inputTime / m_config->getFramesPerSecond());
    }
}

void Dialog::toggleZoom(int zoomValue)
{
    m_zoom = zoomValue;
    m_config->setZoomDistanceScale(0.05 * m_zoom);
}

void Dialog::toggleWidth(int widthValue){
    m_widthValue = widthValue;
    WidthVisitor *widthVisitor = new WidthVisitor(widthValue);
    m_width = this->accept(*widthVisitor);
    delete widthVisitor;
}

void Dialog::toggleHeight(int heightValue){
    m_heightValue = heightValue;
    HeightVisitor *heightVisitor = new HeightVisitor(heightValue);
    m_height = this->accept(*heightVisitor);
    delete heightVisitor;
}

void Dialog::toggleAdjust()
{
    m_sliderZoom->setValue(5);
}

void Dialog::toggleReset()
{
    delete m_config->getInstance()->m_trajectory;
    m_config->read("config.txt");
    m_universe = m_config->parseUniverseBlocks();
    m_zodiacs = m_config->parseZodiacBlocks();
    m_universe->convertRelativeToAbsolute(0,0,0,0);
    m_sliderZoom->setValue(20);
    m_sliderWidth->setValue(0);
    m_sliderHeight->setValue(0);
    m_timer->start(100 / m_config->getFramesPerSecond());
    m_config->getInstance()->m_trajectory = new std::vector< std::pair<double,double> >;
}

void Dialog::pause(bool pause)
{
    if(pause)
    {
        m_timer->stop();
        m_paused = true;
    }
    else
    {
        m_timer->start(100 / m_config->getFramesPerSecond());
        m_paused = false;
    }
}

void Dialog::keyPressEvent(QKeyEvent *event)
{
    switch(event->key()) {
    case Qt::Key_Space:
        pause(!m_paused);
        return;
    case Qt::Key_T:
        m_config->setTrajectoryToggle(!m_config->getTrajectoryToggle());
        return;
    case Qt::Key_C:
        m_catastrophe = !m_catastrophe;
        return;
    default:
        return;
    }
}

void Dialog::nextFrame()
{
    //reset the forces stored in every object to 0
    m_universe->resetForces();

    //update the forces acting on every object in the universe,
    //from every object in the universe
    m_universe->addAttractionFrom(*m_universe);

    //update the velocity and position of every object in the universe
    int step = m_config->getPhysicsStepSize() / m_config->getOvercalculatePhysicsAmount();
    for(int i = 0; i < m_config->getPhysicsStepSize(); i += step)
    {
        //update physics
        m_universe->updatePosition(step);
        //some time has passed
        m_timestamp += step;
    }

    //update the window (this will trigger paintEvent)
    update();
}

void Dialog::paintEvent(QPaintEvent *event)
{
    if(m_catastrophe && m_catastropheTime > 0.2){
        m_catastropheTime *= 0.97;
        m_timer->start(m_catastropheTime / m_config->getFramesPerSecond());
    }

    //suppress 'unused variable' warning
    Q_UNUSED(event);

    //redraw the universe
    QPainter painter(this);

    //offset the painter so (0,0) is the center of the window
    painter.translate(m_width/2, m_height/2);

    double xPos = this->pos().x() + m_width/2;
    double yPos = this->pos().y() + m_height/2;

    //Scale pix map
    painter.drawPixmap(0 - xPos, 0 - yPos, m_pixmap->scaled(1000 + 5*m_zoom, 1000 + 5*m_zoom, Qt::KeepAspectRatioByExpanding));

    if(m_renderZodiacs)
    {
        for(auto zodiac : *m_zodiacs)
        {
            zodiac.render(painter);
        }
    }

    m_universe->render(painter);

    if(m_renderLabels)
    {
        m_universe->renderLabel(painter, xPos, yPos);
    }
}
