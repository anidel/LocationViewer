// Navigation pane project template
#ifndef LocationViewer_HPP_
#define LocationViewer_HPP_

#include <QObject>
#include <bb/cascades/QListDataModel>

#include <libexif/exif-data.h>
#include <libexif/exif-entry.h>
#include <libexif/exif-ifd.h>
#include <libexif/exif-content.h>

namespace bb { namespace cascades { class Application; }}

/*!
 * @brief Application pane object
 *
 *Use this object to create and init app UI, to create context objects, to register the new meta types etc.
 */
class LocationViewer : public QObject
{
    Q_OBJECT

    // The model that contains the progress and image data
    Q_PROPERTY(bb::cascades::DataModel* model READ model CONSTANT)

    Q_PROPERTY(double latitude READ latitude NOTIFY latitudeChanged)
    Q_PROPERTY(double longitude READ longitude NOTIFY longitudeChanged)
    Q_PROPERTY(double altitude READ altitude NOTIFY altitudeChanged)

Q_SIGNALS:
	void latitudeChanged();
    void longitudeChanged();
    void altitudeChanged();

private:
    // The model that contains the progress and image data
    //bb::cascades::QListDataModel<QObject*>* m_model;
    bb::cascades::QMapListDataModel* m_model;
    void retrieveLocation (QString filename);

public:
    LocationViewer(bb::cascades::Application *app);
    virtual ~LocationViewer() {}

    // The accessor method for the property
    bb::cascades::DataModel* model() const;

    // This method is called to start the loading of all images.
    Q_INVOKABLE void loadImages();

    // Location
    double m_latitude;
    double m_longitude;
    double m_altitude;

    double latitude() const;
    double longitude() const;
    double altitude() const;
    double Rational2Double ( unsigned char *, int , ExifByteOrder  ) const;

    Q_INVOKABLE void map (QVariantList idx);
};

#endif /* LocationViewer_HPP_ */
