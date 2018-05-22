#include "qutils/TranslationHelper.h"
// Qt
#include <QTranslator>
#include <QGuiApplication>
// Local
#include "qutils/Macros.h"

namespace zmc
{

TranslationHelper::TranslationHelper(QObject *parent)
    : QObject(parent)
    , m_Translator(new QTranslator(this))
    , m_SourceLang("tr_TR")
    , m_CurrentLang(m_SourceLang)
    , m_TranslationsDir("translations")
{
    loadSystemLanguage();
}

QString TranslationHelper::getLanguageChangedSignal()
{
    return "";
}

void TranslationHelper::setSourceLang(const QString &lang)
{
    m_SourceLang = lang;
}

QString TranslationHelper::getSourceLang() const
{
    return m_SourceLang;
}

void TranslationHelper::selectLanguage(const QString &language)
{
    if (m_CurrentLang == language) {
        return;
    }

    if (m_SourceLang == language) {
        removeTranslator();
        return;
    }

    if (m_Translator->load(language, m_TranslationsDir)) {
        m_CurrentLang = language;
        qApp->installTranslator(m_Translator);
        emit languageChanged();
    }
    else {
        LOG_ERROR("Cannot load translation for language '" << language << "' in directory '" << m_TranslationsDir << "'.");
    }
}

QString TranslationHelper::getCurrentLang() const
{
    return m_CurrentLang;
}

void TranslationHelper::loadSystemLanguage()
{
    selectLanguage(QLocale::system().name());
}

void TranslationHelper::removeTranslator()
{
    if (qApp->removeTranslator(m_Translator)) {
        emit languageChanged();
    }
}

QString TranslationHelper::getTranslationsDir() const
{
    return m_TranslationsDir;
}

void TranslationHelper::setTranslationsDir(const QString &dir)
{
    m_TranslationsDir = dir;
}

}
