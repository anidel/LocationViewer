/*
 * LocImageContainer.cpp
 *
 *  Created on: 2013-01-20
 *      Author: Aniello Del Sorbo
 */
#include "LocImageContainer.hpp"

LocImageContainer::LocImageContainer()
{
}

LocImageContainer::~LocImageContainer()
{
}

LocImageContainer::LocImageContainer(const LocImageContainer &locImageContainer)
{
	m_image = locImageContainer.image();
	m_altitude = locImageContainer.altitude();
	m_latitude = locImageContainer.latitude();
	m_longitude = locImageContainer.longitude();
	m_make = locImageContainer.make();
	m_cameraModel = locImageContainer.cameraModel();
	m_dateTaken = locImageContainer.dateTaken();
	m_locationAvailable = locImageContainer.isLocationAvailable();
	m_exifDataAvailable = locImageContainer.isExifDataAvailable();
}

LocImageContainer::LocImageContainer (const QImage &image)
{
	m_image = image;
}

void LocImageContainer::setLocationAvailable (bool locationAvailable)
{
	m_locationAvailable = locationAvailable;
}

void LocImageContainer::setLatitude(double latitude)
{
	m_latitude = latitude;
}

void LocImageContainer::setLongitude(double longitude)
{
	m_longitude = longitude;
}

void LocImageContainer::setAltitude(double altitude)
{
	m_altitude = altitude;
}

void LocImageContainer::setExifDataAvailable (bool exifDataAvailable)
{
	m_exifDataAvailable = exifDataAvailable;
}

void LocImageContainer::setMake(QString make)
{
	m_make = make;
}

void LocImageContainer::setCameraModel(QString cameraModel)
{
	m_cameraModel = cameraModel;
}

void LocImageContainer::setDateTaken(QString dateTaken)
{
	m_dateTaken = dateTaken;
}

bool LocImageContainer::isLocationAvailable () const
{
	return m_locationAvailable;
}

QImage LocImageContainer::image() const
{
	return m_image;
}

double LocImageContainer::latitude() const
{
	return m_latitude;
}

double LocImageContainer::longitude() const
{
	return m_longitude;
}

double LocImageContainer::altitude() const
{
	return m_altitude;
}

bool LocImageContainer::isExifDataAvailable () const
{
	return m_exifDataAvailable;
}

QString LocImageContainer::make() const
{
	return m_make;
}

QString LocImageContainer::cameraModel() const
{
	return m_cameraModel;
}

QString LocImageContainer::dateTaken() const
{
	return m_dateTaken;
}
