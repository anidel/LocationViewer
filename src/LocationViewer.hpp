// Navigation pane project template
#ifndef LocationViewer_HPP_
#define LocationViewer_HPP_

#include <QObject>
#include <bb/cascades/QListDataModel>

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

public:
    LocationViewer(bb::cascades::Application *app);
    virtual ~LocationViewer() {}

    // The accessor method for the property
    bb::cascades::DataModel* model() const;

    // This method is called to start the loading of all images.
    Q_INVOKABLE void loadImages();

private:
    // The model that contains the progress and image data
    //bb::cascades::QListDataModel<QObject*>* m_model;
    bb::cascades::QMapListDataModel* m_model;

};

#endif /* LocationViewer_HPP_ */
