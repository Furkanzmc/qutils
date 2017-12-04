#pragma once
#include <QObject>
class QTranslator;

namespace zmc
{

/**
 * @brief TranslationHelper is a utlity class to help you manage the translations for your app.
 * Everytime the language changes, the `LCS` signal is emitted and it causes all of the `qsTr` functions to be re-evaluted If they were used along side of the
 * `LCS` signal.
 *
 * @code
 *     Text {
 *         // When LCS signal is emitted, qsTr's are re-evaluated
 *         text: qsTr("Merhaba Dünya!") + translationTest.LCS
 *         anchors.centerIn: parent
 *     }
 *
 *     Text {
 *         // This will stay the same even when the language changes at runtime because we are not using the `LCS` signal.
 *         text: qsTr("Hello World!")
 *         anchors.centerIn: parent
 *     }
 *
 *     MouseArea {
 *         anchors.fill: parent
 *         acceptedButtons: Qt.LeftButton | Qt.RightButton
 *         onClicked: {
 *             if (mouse.button == Qt.RightButton) {
 *                 translationTest.loadSystemLanguage();
 *             }
 *             else {
 *                 if (turn === 1) {
 *                     translationTest.selectLanguage("en_US");
 *                 }
 *             }
 *         }
 *     }
 * @endcode
 *
 */
class TranslationHelper : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString LCS READ getLanguageChangedSignal NOTIFY languageChanged)

public:
    explicit TranslationHelper(QObject *parent = 0);

    QString getLanguageChangedSignal();

    /**
     * @brief Sets the source language. The source language does not have a *.qm file so If the language you requested to load equals to the source language
     * the translator is removed to return to source language
     * @param lang
     */
    Q_INVOKABLE void setSourceLang(const QString &lang);
    Q_INVOKABLE QString getSourceLang() const;

    Q_INVOKABLE void selectLanguage(const QString &language);
    Q_INVOKABLE QString getCurrentLang() const;
    Q_INVOKABLE void loadSystemLanguage();

    /**
     * @brief Removes the translator from the application, making it return to the source language
     */
    Q_INVOKABLE void removeTranslator();

    Q_INVOKABLE QString getTranslationsDir() const;
    Q_INVOKABLE void setTranslationsDir(const QString &dir);

signals:
    void languageChanged();

private:
    QTranslator *m_Translator;
    QString m_SourceLang,
            m_CurrentLang,
            m_TranslationsDir;
};

}
