// Navigation pane project template
#include "LocationViewer.hpp"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/NavigationPane>
#include <bb/cascades/maps/MapView>
#include <bb/platform/geo/Point.hpp>
#include <bb/cascades/controls/container.h>

#include <libexif/exif-data.h>
#include <libexif/exif-entry.h>
#include <libexif/exif-ifd.h>
#include <libexif/exif-content.h>

using namespace bb::cascades;
using namespace bb::cascades::maps;
using namespace bb::platform::geo;

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
	    map["cameraModel"] = m_cameraModel;
	    map["dateTaken"] = m_dateTaken;
	    map["latitude"] = m_latitude;
	    map["longitude"] = m_longitude;
	    map["altitude"] = m_altitude;

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

	// Retrieve model
	ExifEntry *ee = exif_content_get_entry(ed->ifd[EXIF_IFD_0], EXIF_TAG_MODEL);
	exif_entry_get_value(ee, value, sizeof(value));
	m_cameraModel = QString(value);
	qDebug () << "Camera model: " << m_cameraModel;

	// Retrieve time
	ee = exif_content_get_entry(ed->ifd[EXIF_IFD_0], EXIF_TAG_DATE_TIME);
	exif_entry_get_value(ee, value, sizeof(value));
	QString dateTime(value);

	QDateTime dateTaken = QDateTime::fromString (dateTime, "yyyy:MM:dd HH:mm:ss");
	m_dateTaken = dateTaken.toString("ddd d MMMM yyyy 'at' HH:mm");

	qDebug() << "Taken on: " << m_dateTaken;

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

QString LocationViewer::cameraModel() const
{
	return m_cameraModel;
}

QString LocationViewer::dateTaken() const
{
	return m_dateTaken;
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
	} else {
		return;
	}

	if (idx.count() == 0) {
		return;
	}

	m_longitude = m_model->data(idx).toMap()["longitude"].toDouble(NULL);
	m_latitude = m_model->data(idx).toMap()["latitude"].toDouble(NULL);
}

QString LocationViewer::worldToPixelInvokable(QObject* mapObject, double lat, double lon) {
	MapView* mapview = qobject_cast<MapView*>(mapObject);
	Point worldCoordinates = Point(lat, lon);
	QPoint pixels = mapview->worldToWindow(worldCoordinates);

	int x = pixels.x();
	int y = pixels.y();

	return (QString::number(x) + " " + QString::number(y));
}

void LocationViewer::updateMarkers(QObject* mapObject, QObject* containerObject) {
	MapView* mapview = qobject_cast<MapView*>(mapObject);
	Container* container = qobject_cast<Container*>(containerObject);
	for (int i = 0; i < container->count(); i++) {
		QPoint xy = worldToPixel(mapview, container->at(i)->property("lat").value<double>(), container->at(i)->property("lon").value<double>());
		container->at(i)->setProperty("x", xy.x());
		container->at(i)->setProperty("y", xy.y());
	}
}

QPoint LocationViewer::worldToPixel(QObject* mapObject, double lat, double lon) {
	MapView* mapview = qobject_cast<MapView*>(mapObject);
	Point worldCoordinates = Point(lat, lon);
	return mapview->worldToWindow(worldCoordinates);
}
