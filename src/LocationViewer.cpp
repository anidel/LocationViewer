// Navigation pane project template
#include "LocationViewer.hpp"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>

using namespace bb::cascades;

QFileInfoList fileList;

LocationViewer::LocationViewer(bb::cascades::Application *app)
: QObject(app)
	,m_model(new QMapListDataModel())
{
    // create scene document from main.qml asset
    // set parent to created document to ensure it exists for the whole application lifetime
    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);
    qml->setContextProperty("listModel", this);

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

	QVariantMap map = QVariantMap();

	for (int i = 0; i < fileList.size(); ++i) {
		QFileInfo fileInfo = fileList.at(i);
		map["title"] = fileInfo.fileName();
	    map["imageSource"] = "file://"+fileInfo.filePath();

		qDebug () << "Inserting " << map["imageSource"];
		m_model->append(map);
	}
}
