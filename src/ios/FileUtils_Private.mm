#import "qutils/ios/FileUtils_Private.h"
// Local
#include "qutils/ios/DocumentPicker.h"
#include "qutils/Macros.h"

namespace zmc
{

bool FileUtilsPrivate::m_IsDocumentPickerOpen = false;
static DocumentPicker *m_DocumentPicker = [[DocumentPicker alloc] init];
QList<FileUtilsPrivate *> FileUtilsPrivate::m_Instances = QList<FileUtilsPrivate *>();

FileUtilsPrivate::FileUtilsPrivate()
    : m_InstanceIndex(m_Instances.size())
    , m_IsInvokerInstance(false)
{
    m_Instances.append(this);
}

FileUtilsPrivate::~FileUtilsPrivate()
{
    m_Instances[m_InstanceIndex] = nullptr;
}

bool FileUtilsPrivate::openDocumentPicker(const QStringList &documentTypes, bool selectMultiple)
{
    bool opened = false;
    if (!m_IsDocumentPickerOpen) {
        opened = true;
        m_IsDocumentPickerOpen = true;
        m_IsInvokerInstance = true;
        [m_DocumentPicker showDocumentPicker:documentTypes allowMultiple:selectMultiple];
    }

    return opened;
}

void FileUtilsPrivate::invokeDocumentPickerCanceled()
{
    FileUtilsPrivate *instance = getInvokerInstance();
    if (instance && instance->onDocumentPickerCanceled) {
        instance->onDocumentPickerCanceled();
        instance->m_IsInvokerInstance = false;
        m_IsDocumentPickerOpen = false;
    }
}

void FileUtilsPrivate::invokeDocumentPicked(QStringList paths)
{
    FileUtilsPrivate *instance = getInvokerInstance();
    if (instance && instance->onDocumentPicked) {
        instance->onDocumentPicked(paths);
        instance->m_IsInvokerInstance = false;
        m_IsDocumentPickerOpen = false;
    }
}

FileUtilsPrivate *FileUtilsPrivate::getInvokerInstance()
{
    FileUtilsPrivate *instance = nullptr;
    const int count = m_Instances.size();
    for (int index = 0; index < count; index++) {
        FileUtilsPrivate *tmpInstance = m_Instances.at(index);
        if (tmpInstance && tmpInstance->m_IsInvokerInstance) {
            instance = tmpInstance;
            break;
        }

    }

    return instance;
}

}
