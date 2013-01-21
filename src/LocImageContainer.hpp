/* Copyright (c) 2012 Research In Motion Limited.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef LOCIMAGECONTAINER_HPP
#define LOCIMAGECONTAINER_HPP

#include <QtGui/QImage>
#include <QMetaType>

class LocImageContainer
{
public:
	LocImageContainer();
	LocImageContainer(const LocImageContainer &other);
    ~LocImageContainer();

    LocImageContainer(const QImage &image);

    void setImage (QImage);
    void setLocationAvailable (bool);
	void setLatitude (double);
	void setLongitude (double);
	void setAltitude (double);
	void setExifDataAvailable (bool);
	void setCameraModel (QString);
	void setMake (QString);
	void setDateTaken (QString);

	QImage image () const;
	bool isLocationAvailable() const;
	double latitude () const;
	double longitude () const;
	double altitude () const;
	bool isExifDataAvailable () const;
	QString cameraModel () const;
	QString make () const;
	QString dateTaken () const;

private:
	bool m_locationAvailable;
	QImage m_image;
	double m_latitude;
	double m_longitude;
	double m_altitude;
	bool m_exifDataAvailable;
	QString m_make;
	QString m_cameraModel;
	QString m_dateTaken;
};

Q_DECLARE_METATYPE(LocImageContainer);
#endif
