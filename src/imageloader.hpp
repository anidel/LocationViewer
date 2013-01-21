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

#ifndef IMAGELOADER_HPP
#define IMAGELOADER_HPP

#include "imageprocessor.hpp"

#include <QObject>
#include <QByteArray>

#include <bb/cascades/Image>

/*
 * This class retrieves an image from the web, then converts the binary
 * data into a bb::cascades::Image and makes it available through a property.
 */
//! [0]
class ImageLoader : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QVariant image READ image NOTIFY imageChanged)
    Q_PROPERTY(QString label READ label NOTIFY labelChanged)
    Q_PROPERTY(QString id READ label NOTIFY idChanged)
    Q_PROPERTY(bool loading READ loading NOTIFY loadingChanged)
    Q_PROPERTY(double latitude READ latitude NOTIFY latitudeChanged)
    Q_PROPERTY(double longitude READ longitude NOTIFY longitudeChanged)
    Q_PROPERTY(double altitude READ altitude NOTIFY altitudeChanged)
    Q_PROPERTY(QString cameraModel READ cameraModel NOTIFY cameraModelChanged)
    Q_PROPERTY(QString dateTaken READ dateTaken NOTIFY dateTakenChanged)

public:
    /*
     * Creates a new image loader.
     *
     * @param imageUrl The url to load the image from.
     */
    ImageLoader(const QString &id, const QString &imageUrl, QObject* parent = 0);

    /*
     * Destroys the image loader.
     */
    ~ImageLoader();

    double latitude() const;
    double longitude() const;
    double altitude() const;

Q_SIGNALS:
    // The change notification signals of the properties
    void imageChanged();
    void labelChanged();
    void loadingChanged();
    void idChanged();
    void latitudeChanged();
    void longitudeChanged();
    void altitudeChanged();
    void cameraModelChanged();
    void dateTakenChanged();

public Q_SLOTS:
    /*
     * Loads the image.
     */
    void load();

private Q_SLOTS:
    /*
     * Response handler for the network operation.
     */
//    void onReplyFinished();

    /*
     * Response handler for the image process operation.
     */
    void onImageProcessingFinished(const LocImageContainer &imageLocContainer);

private:
    // The accessor methods of the properties
    QVariant image() const;
    QString label() const;
    bool loading() const;
    QString id() const;
    QString cameraModel() const;
    QString dateTaken() const;

    // The property values
    bb::cascades::Image m_image;
    QString m_label;
    bool m_loading;
    // The URL of the image that should be loaded
    QString m_imageUrl;
    QString m_id;
    // Location
    double m_latitude;
    double m_longitude;
    double m_altitude;
    // Photo properties
    QString m_cameraModel;
    QString m_dateTaken;

    // The thread context that processes the image
    QPointer<QThread> m_thread;

    void retrieveLocation (QString filename);
    double Rational2Double ( unsigned char *, int , ExifByteOrder  ) const;

    ImageProcessor *imageProcessor;
};
//! [0]

#endif
