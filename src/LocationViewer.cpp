// Navigation pane project template
#include "LocationViewer.hpp"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/NavigationPane>
#include <bb/cascades/maps/MapView>

#include <libexif/exif-data.h>
#include <libexif/exif-entry.h>
#include <libexif/exif-ifd.h>
#include <libexif/exif-content.h>

using namespace bb::cascades;
using namespace bb::cascades::maps;

QFileInfoList fileList;
NavigationPane *navigationPane;
MapView *mapView;

LocationViewer::LocationViewer(bb::cascades::Application *app)
: QObject(app)
	,m_model(new QMapListDataModel())
	, m_altitude(3000)
{
	mapView = NULL;

    // create scene document from main.qml asset
    // set parent to created document to ensure it exists for the whole application lifetime
    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);
    qml->setContextProperty("listModel", this);

    navigationPane = (NavigationPane*)qml->findChild<NavigationPane *>("NavigationPane");

    // create root object for the UI
    AbstractPane *root = qml->createRootObject<AbstractPane>();
    // set created root object as a scene
    app->setScene(root);
}

bb::cascades::DataModel* LocationViewer::model() const
{
    return m_model;
}

void LocationViewer::loadImages()
{
	QString workingDir = QDir::currentPath();
	QDir pictureFolder = QDir (workingDir+"/shared/camera");
	pictureFolder.setFilter(QDir::Files | QDir::NoSymLinks);
	pictureFolder.setSorting(QDir::Time);

	fileList = pictureFolder.entryInfoList();

	m_model->clear();

	for (int i = 0; i < fileList.size(); ++i) {
		QVariantMap map;

		map.clear();

		QFileInfo fileInfo = fileList.at(i);
	    retrieveLocation (fileInfo.filePath());

	    map["title"] = fileInfo.fileName();
	    map["imageSource"] = "file:/"+fileInfo.filePath();
	    map["idx"] = i;
	    map["latitude"] = m_latitude;
	    map["longitude"] = m_longitude;
	    map["altitude"] = m_altitude;

		qDebug () << "Inserting " << map;

		(*m_model) << map;
	}
}

void LocationViewer::retrieveLocation (QString filename)
{
	m_latitude = -999999.99;
	m_longitude = -999999.99;
	m_altitude = 3000;

	//get exif data
	ExifData *ed = exif_data_new_from_file (filename.toLocal8Bit().constData());

	if (!ed) {
		qDebug () << "no exif?";
		return;
	}
	//save temp
	char value[256]={0,};

	qDebug () << "Retrieving model";
	// Retrieve model
	ExifEntry *ee = exif_content_get_entry(ed->ifd[EXIF_IFD_0], EXIF_TAG_MODEL);
	exif_entry_get_value(ee, value, sizeof(value));
	QString model(value);
	qDebug() << model;

	qDebug () << "Retrieving time it was shot";

	// Retrieve time
	ee = exif_content_get_entry(ed->ifd[EXIF_IFD_0], EXIF_TAG_DATE_TIME);
	exif_entry_get_value(ee, value, sizeof(value));
	QString datetime(value);

	qDebug() << datetime;

	// Retrieve LATITUDE
	ee = exif_content_get_entry (ed->ifd[EXIF_IFD_GPS], (ExifTag)EXIF_TAG_GPS_LATITUDE);
	if (!ee) {
		qDebug() << "Photo does not contain GPS data";
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

			m_latitude = lat;
			qDebug () << "Latitude: " << m_latitude;
		}

		ee = exif_content_get_entry (ed->ifd[EXIF_IFD_GPS], (ExifTag)EXIF_TAG_GPS_LONGITUDE);
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

			m_longitude = lon;
			qDebug () << "Longitude: " << m_longitude;
		}
	}
}
double LocationViewer::latitude() const
{
	qDebug () << "Latitude read: " << m_latitude;
	return m_latitude;
}

double LocationViewer::longitude() const
{
	return m_longitude;
}

double LocationViewer::altitude() const
{
	return m_altitude;
}

double LocationViewer::Rational2Double ( unsigned char *data, int offset, ExifByteOrder order ) const
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

void LocationViewer::map (QVariantList idx)
{
	if (!mapView) {
	    mapView = (MapView*)navigationPane->findChild<MapView *>("mapView");
	    qDebug () << "Found mapView";
	} else {
		qDebug () << "mapView not found";

		return;
	}

	qDebug () << idx;

	if (idx.count() == 0) {
		return;
	}
	qDebug () << idx.at(0).toInt(NULL);

	qDebug () << m_model->data(idx);
	qDebug () << m_model->data(idx).toMap()["latitude"].toDouble(NULL);
	qDebug () << m_model->data(idx).toMap()["longitude"].toDouble(NULL);

	m_longitude = m_model->data(idx).toMap()["longitude"].toDouble(NULL);
	m_latitude = m_model->data(idx).toMap()["latitude"].toDouble(NULL);

//	if (latitude > -999999.99) {
//		mapView->setLatitude(latitude);
//		mapView->setLongitude(longitude);
//	}
}
