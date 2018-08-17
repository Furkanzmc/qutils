#pragma once
// Qt
#include <QStringList>
#include <QVariantList>
// std
#include <functional>

namespace zmc
{

class FileUtilsPrivate
{
public:
    /**
     * @brief Called when the document picker is cancelled.
     */
    std::function<void()> onDocumentPickerCanceled;

    /**
     * @brief Called when the document is picked.
     */
    std::function<void(const QStringList &)> onDocumentPicked;

public:
    FileUtilsPrivate();
    ~FileUtilsPrivate();

    /**
     * @brief Opens the UIDocumentPickerView on iOS. This will do nothing on any other platform. When the file is picked, documentPicked() signal will be
     * emitted. If the picker was cancelled, documentPickerCanceled() signal will be emitted. Only one document picker can be opened in the app. If one is
     * already open, this will return false. Otherwise, it will return true.
     * The default inMode is UIDocumentPickerModeImport.
     * @return bool
     */
    bool openDocumentPicker(const QStringList &documentTypes, bool selectMultiple);

    /**
     * @brief Calls the onDocumentPickerCanceled function on the instance that called openDocumentPicker().
     */
    static void invokeDocumentPickerCanceled();

    /**
     * @brief Calls the onDocumentPickerCanceled function on the instance that called openDocumentPicker().
     */
    static void invokeDocumentPicked(QStringList paths);

private:
    /*!
     * \variable static QMap<int, CacheManager *> m_Instances
     * \brief This variable is used to keep track of all the instances so that we can send signals to all of them.
     */
    static QMap<int, FileUtilsPrivate *> m_Instances;

    /*!
     * \variable static unsigned int m_NextInstanceID
     * \brief Used to prevent key clashes in m_Instances.
     */
    static unsigned int m_NextInstanceID;

    static bool m_IsDocumentPickerOpen;

    /*!
     * \variable const int m_InstanceIndex
     * \brief This variable is initilized in the constructor to the size value of m_Instances.
     *
     * This is used to identify the current instance in m_Instances.
     */
    const int m_InstanceIndex;

    bool m_IsInvokerInstance;

private:
    /**
     * @brief Returns the instance that opened the document picker. If it is not found, returns nullptr.
     * @return FileUtilsPrivate *
     */
    static FileUtilsPrivate *getInvokerInstance();
};

}
