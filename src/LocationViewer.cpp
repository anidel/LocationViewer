// Navigation pane project template
#include "LocationViewer.hpp"

#include <bb/ImageData>
#include <bb/cascades/Image>
#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/NavigationPane>
#include <bb/cascades/maps/MapView>
#include <bb/platform/geo/Point.hpp>
#include <bb/cascades/controls/container.h>
#include <bb/cascades/ListView>
#include <bb/cascades/ActionItem>

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
	, m_model(new QListDataModel<QObject*>())
{
    // Register custom type to QML
    qmlRegisterType<ImageLoader>();

	mapView = NULL;

    // create scene document from main.qml asset
    // set parent to created document to ensure it exists for the whole application lifetime
    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);

    qml->setContextProperty("listModel", this);

    // create root object for the UI
    navigationPane = qml->createRootObject<NavigationPane>();

    // set created root object as a scene
    app->setScene(navigationPane);

    QObject::connect(navigationPane, SIGNAL(popTransitionEnded(bb::cascades::Page*)),
        this, SLOT(popFinished(bb::cascades::Page*)));
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
		QFileInfo fileInfo = fileList.at(i);

		m_model->append(new ImageLoader(QString(i), fileInfo.filePath(), this));
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

void LocationViewer::map (QVariantList idx)
{
	if (idx.count() == 0) {
		m_latitude = qobject_cast<ImageLoader*>(m_model->value(0))->latitude();
		m_longitude = qobject_cast<ImageLoader*>(m_model->value(0))->longitude();
	} else {
		m_latitude = qobject_cast<ImageLoader*>(m_model->data(idx).value<QObject*>())->latitude();
		m_longitude = qobject_cast<ImageLoader*>(m_model->data(idx).value<QObject*>())->longitude();
	}
}

void LocationViewer::linkMapData (QObject *mapObject)
{
	Q_UNUSED(mapObject);

//	MapView *mapView = qobject_cast<MapView*>(mapObject);
//	MapData *mapData = mapView->mapData();

//	mapView->children().append((QObject*)mapData);
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
	QPoint ret = mapview->worldToWindow(worldCoordinates);

	return ret;
}

void LocationViewer::selectionChanged(const QVariantList &indexPath, bool selected = true)
{
	if (indexPath.size() != 1 || !selected)
		return; // Not interested

	if (selected)
	{
		QmlDocument *qml = QmlDocument::create("asset:///mapViewPage.qml").parent(this);
	    qml->setContextProperty("mapViewListModel", this);

	    Page *new_page = qml->createRootObject<Page>();

	    navigationPane->push(new_page);

	    ListView *listView = new_page->findChild<ListView *>("listViewMapMode");

	    if (listView != NULL) {
	    	listView->scrollToItem (indexPath);
	    }
	    else
	    	qDebug () << "No listView??";

		map (indexPath);
	}
}

void LocationViewer::popFinished(bb::cascades::Page* page)
{
    delete page;
}
