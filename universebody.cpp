#include "universebody.h"
#include "config.h"
#include <QDebug>
#include <QWidget>
#include <QMouseEvent>
#include <QGradient>
#include <iostream>
#include <QCursor>
#include <QKeyEvent>
#include <sstream>


UniverseBody::UniverseBody(
        UniverseComponentType type,
        const std::string& name,
        const std::string& parentName)
        : UniverseComponent(type, name, parentName)
        , m_xForce(0)
        , m_yForce(0)
        , m_xVelocity(0.0)
        , m_yVelocity(0.0)
        , m_xPosition(0.0)
        , m_yPosition(0.0)
        , m_radius(1.0)
        , m_mass(1.0)
        , m_color(Qt::white)
{ }

void UniverseBody::render(QPainter& painter) const
{
    Config* config = Config::getInstance();

    //get scaled position and radius
    double x = m_xPosition / config->getZoomDistanceScale();
    double y = m_yPosition / config->getZoomDistanceScale();
    double radius = m_radius / config->getRadiusScale();

    if(config->getUseLogRadius())
    {
        radius = std::log(m_radius / config->getLogPointRadius());
    }

    if(radius < 1)
    {
        painter.setPen(m_color);
        painter.drawPoint(x, y);
    }
    else
    {

        //no outline
        painter.setPen(Qt::NoPen);

        //gradient brush
        QRadialGradient gradient(x, y, radius);
        gradient.setColorAt(0.25, m_color);
        gradient.setColorAt(1, Qt::transparent);

        painter.setBrush(gradient);

        painter.drawEllipse(QPointF(x, y), radius, radius);

    }
}

void UniverseBody::renderLabel(QPainter& painter, double xPos, double yPos) const
{
    Config* config = Config::getInstance();

    //get scaled position
    double x = m_xPosition / config->getZoomDistanceScale();
    double y = m_yPosition / config->getZoomDistanceScale();
    //draw the label
    painter.setPen(m_color);

    QRectF rectName = QRectF(x,y,80.0,60.0);
    painter.drawText(rectName, getName().c_str());

    int xCursor = QCursor::pos().x() - xPos;
    int yCursor = QCursor::pos().y() - yPos;

    // Parse dobles as strings
    double mass = getMass();
    std::stringstream ss("");
    ss << mass << "kg";

    std::stringstream ss1("");
    ss1 << "xPos:" << m_xPosition << "km";

    std::stringstream ss2("");
    ss2 << "yPos:" << m_yPosition << "km";

    if(rectName.contains(xCursor,yCursor)){
        painter.drawText(QRectF(x, y + 20, 150.0, 60.0), ss.str().c_str());
        painter.drawText(QRectF(x, y + 40, 250.0, 60.0), ss1.str().c_str());
        painter.drawText(QRectF(x, y + 60, 250.0, 60.0), ss2.str().c_str());
    }

    if(rectName.contains(xCursor,yCursor) && config->getTrajectoryToggle()){
        config->m_trajectoryName = this->getName();
        config->setTrajectoryToggle(false);
    }

    if(config->getTrajectoryToggle()){
        config->m_trajectoryName = "";
    }

    if(config->m_trajectoryName == this->getName()){
        (*config->m_trajectory).push_back(std::make_pair(x, y));
        painter.setPen(Qt::green);
        for(int i = 0; i < (*config->m_trajectory).size(); i++){
            painter.drawPoint((*config->m_trajectory).at(i).first, (*config->m_trajectory).at(i).second);
        }
    }
}


void UniverseBody::addAttractionTo(UniverseBody &other) const
{
    if(this == &other)
    {
        return;
    }
    double other_mass = other.getMass();
    double dx = m_xPosition - other.getPositionX();
    double dy = m_yPosition - other.getPositionY();
    double distance_sq = dx*dx + dy*dy;
    if(distance_sq == 0.0)
    {
        return;
    }

    double force = GRAVITATIONAL_CONSTANT * m_mass * other_mass / distance_sq;

    double distance = sqrt(distance_sq);

    //normalise the vector {dx, dy} by dividing it by the distance, to get the direction
    other.addForce(force * (dx / distance), force * (dy / distance));
}

void UniverseBody::addAttractionFrom(const UniverseComponent &component)
{
    component.addAttractionTo(*this);
}

void UniverseBody::resetForces()
{
    m_xForce = 0.0;
    m_yForce = 0.0;
}

void UniverseBody::updatePosition(int timestep)
{
     // calculate acceleration
     double xAccel = m_xForce / m_mass;
     double yAccel = m_yForce / m_mass;

     // remember the old velocity
     double oldXVelocity = m_xVelocity;
     double oldYVelocity = m_yVelocity;

     // calculate the new velocity (integrate acceleration)
     m_xVelocity += xAccel * timestep;
     m_yVelocity += yAccel * timestep;

     // calculate the new position (integrate velocity)
     // slightly improved by using the average velocity during this timestep
     m_xPosition += (m_xVelocity + oldXVelocity)/2 * timestep;
     m_yPosition += (m_yVelocity + oldYVelocity)/2 * timestep;

}
