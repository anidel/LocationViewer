/*
 * QLocImageContainer.cpp
 *
 *  Created on: 2013-01-20
 *      Author: Aniello
 */
#include "QLocImageContainer.hpp"

QLocImageContainer::QLocImageContainer()
{
}

QLocImageContainer::~QLocImageContainer()
{
}

QLocImageContainer::QLocImageContainer(const QLocImageContainer &locImageContainer)
{
	m_image = locImageContainer.image();
	m_altitude = locImageContainer.altitude();
	m_latitude = locImageContainer.latitude();
	m_longitude = locImageContainer.longitude();
	m_make = locImageContainer.make();
	m_cameraModel = locImageContainer.cameraModel();
	m_dateTaken = locImageContainer.dateTaken();
}

QLocImageContainer::QLocImageContainer (const QImage &image)
{
	m_image = image;
}

void QLocImageContainer::setLatitude(double latitude)
{
	m_latitude = latitude;
}

void QLocImageContainer::setLongitude(double longitude)
{
	m_longitude = longitude;
}

void QLocImageContainer::setAltitude(double altitude)
{
	m_altitude = altitude;
}

void QLocImageContainer::setMake(QString make)
{
	m_make = make;
}

void QLocImageContainer::setCameraModel(QString cameraModel)
{
	m_cameraModel = cameraModel;
}

void QLocImageContainer::setDateTaken(QString dateTaken)
{
	m_dateTaken = dateTaken;
}

QImage QLocImageContainer::image() const
{
	return m_image;
}

double QLocImageContainer::latitude() const
{
	return m_latitude;
}

double QLocImageContainer::longitude() const
{
	return m_longitude;
}

double QLocImageContainer::altitude() const
{
	return m_altitude;
}

QString QLocImageContainer::make() const
{
	return m_make;
}

QString QLocImageContainer::cameraModel() const
{
	return m_cameraModel;
}

QString QLocImageContainer::dateTaken() const
{
	return m_dateTaken;
}
