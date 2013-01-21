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
// EXIF
#include <QtCore>

#include <libexif/exif-data.h>
#include <libexif/exif-entry.h>
#include <libexif/exif-ifd.h>
#include <libexif/exif-content.h>

#include "imageprocessor.hpp"

//! [0]
ImageProcessor::ImageProcessor(const QString &imageData, QObject *parent)
: QObject(parent)
, m_data(imageData)
{
}
//! [0]

//! [1]
void ImageProcessor::start()
{
	LocImageContainer locImageContainer = process (m_data);

	emit finished(locImageContainer);
}
//! [1]

LocImageContainer ImageProcessor::process (QString filename)
{
	QImage image (m_data);

	if (!image.isNull()) {
		image = image.scaled(720, 500, Qt::KeepAspectRatioByExpanding);
	}

	LocImageContainer locImageContainer(image);

	locImageContainer.setAltitude(3000.00);

	//get exif data
	ExifData *ed = exif_data_new_from_file (filename.toLocal8Bit().constData());

	if (!ed) {
		locImageContainer.setLocationAvailable(false);
		locImageContainer.setExifDataAvailable(false);

		return locImageContainer;
	}

	locImageContainer.setExifDataAvailable(true);

	//save temp
	char value[256]={0,};

	// Retrieve make
	QString make;
	ExifEntry *ee = exif_content_get_entry(ed->ifd[EXIF_IFD_0], EXIF_TAG_MAKE);
	if (!ee) {
		make = "";
	} else {
		exif_entry_get_value(ee, value, sizeof(value));
		locImageContainer.setMake (QString(value));
	}

	// Retrieve model
	ee = exif_content_get_entry(ed->ifd[EXIF_IFD_0], EXIF_TAG_MODEL);
	if (!ee) {
		locImageContainer.setCameraModel ("");
	} else {
		exif_entry_get_value(ee, value, sizeof(value));
		locImageContainer.setCameraModel ("Taken with the "+QString(value));
	}

	// Retrieve time
	ee = exif_content_get_entry(ed->ifd[EXIF_IFD_EXIF], EXIF_TAG_DATE_TIME_ORIGINAL);
	if (!ee) {
		locImageContainer.setDateTaken ("");
	} else {
		exif_entry_get_value(ee, value, sizeof(value));
		QString dateTime(value);

		QDateTime dateTaken = QDateTime::fromString (dateTime, "yyyy:MM:dd HH:mm:ss");
		locImageContainer.setDateTaken (dateTaken.toString("'on 'dddd d, MMMM yyyy 'at' HH:mm"));
	}

	// Retrieve LATITUDE
	ee = exif_content_get_entry (ed->ifd[EXIF_IFD_GPS], (ExifTag)EXIF_TAG_GPS_LATITUDE);
	if (!ee) {
		locImageContainer.setLocationAvailable(false);
		locImageContainer.setLatitude(-9999.99);
	} else {
		if ( ( ee && ee->components == 3 && ee->format == EXIF_FORMAT_RATIONAL ) )
		{
			double lat = Rational2Double ( ee->data,
					exif_format_get_size(ee->format),
					exif_data_get_byte_order(ed) );

			ee = exif_content_get_entry (ed->ifd[EXIF_IFD_GPS], (ExifTag)EXIF_TAG_GPS_LATITUDE_REF);
			if ( ee ) {
				exif_entry_get_value ( ee, value, 128 );
				if ( value[0] == 'S' )
					lat = -lat;
			}

			locImageContainer.setLocationAvailable(true);
			locImageContainer.setLatitude (lat);
		}
	}

	ee = exif_content_get_entry (ed->ifd[EXIF_IFD_GPS], (ExifTag)EXIF_TAG_GPS_LONGITUDE);
	if (!ee) {
		locImageContainer.setLocationAvailable(false);
		locImageContainer.setLongitude(-9999.99);
	} else {
		if ( ( ee && ee->components == 3 && ee->format == EXIF_FORMAT_RATIONAL ) )
		{
			double lon = Rational2Double ( ee->data,
					exif_format_get_size(ee->format),
					exif_data_get_byte_order(ed) );

			ee = exif_content_get_entry (ed->ifd[EXIF_IFD_GPS], (ExifTag)EXIF_TAG_GPS_LONGITUDE_REF);
			if ( ee ) {
				exif_entry_get_value ( ee, value, 128 );
				if ( value[0] == 'W' )
					lon = -lon;
			}

			locImageContainer.setLocationAvailable(true);
			locImageContainer.setLongitude (lon);
		}
	}

	exif_data_free(ed);

	return locImageContainer;
}

double ImageProcessor::Rational2Double ( unsigned char *data, int offset, ExifByteOrder order ) const
{
	// Explaination from GPS Correlate 'exif-gps.cpp' v 1.6.1
	// What we are trying to do here is convert the three rationals:
	//    dd/v mm/v ss/v
	// To a decimal
	//    dd.dddddd...
	// dd/v is easy: result = dd/v.
	// mm/v is harder:
	//    mm
	//    -- / 60 = result.
	//     v
	// ss/v is sorta easy.
	//     ss
	//     -- / 3600 = result
	//      v
	// Each part is added to the final number.
	double ans;
	ExifRational er;
	er = exif_get_rational (data, order);
	ans = (double)er.numerator / (double)er.denominator;
	if (offset <= 0)
		return ans;

	er = exif_get_rational (data+(1*offset), order);
	ans = ans + ( ( (double)er.numerator / (double)er.denominator ) / 60.0 );
	er = exif_get_rational (data+(2*offset), order);
	ans = ans + ( ( (double)er.numerator / (double)er.denominator ) / 3600.0 );

	return ans;
}
