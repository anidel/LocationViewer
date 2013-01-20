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

#ifndef QLOCIMAGECONTAINER_HPP
#define QLOCIMAGECONTAINER_HPP

#include <QtGui/QImage>
#include <QMetaType>

class QLocImageContainer
{
public:
	QLocImageContainer();
	QLocImageContainer(const QLocImageContainer &other);
    ~QLocImageContainer();

    QLocImageContainer(const QImage &image);

    void setImage (QImage);
	void setLatitude (double);
	void setLongitude (double);
	void setAltitude (double);
	void setCameraModel (QString);
	void setMake (QString);
	void setDateTaken (QString);

	QImage image () const;
	double latitude () const;
	double longitude () const;
	double altitude () const;
	QString cameraModel () const;
	QString make () const;
	QString dateTaken () const;

private:
	QImage m_image;
	double m_latitude;
	double m_longitude;
	double m_altitude;
	QString m_make;
	QString m_cameraModel;
	QString m_dateTaken;
};

Q_DECLARE_METATYPE(QLocImageContainer);
#endif
