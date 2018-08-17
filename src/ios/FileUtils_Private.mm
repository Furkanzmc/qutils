#import "qutils/ios/FileUtils_Private.h"
// Local
#include "qutils/ios/DocumentPicker.h"
#include "qutils/Macros.h"

namespace zmc
{

bool FileUtilsPrivate::m_IsDocumentPickerOpen = false;
QMap<int, FileUtilsPrivate *> FileUtilsPrivate::m_Instances = QMap<int, FileUtilsPrivate *>();
unsigned int FileUtilsPrivate::m_NextInstanceID = 0;

static DocumentPicker *m_DocumentPicker = nullptr;

FileUtilsPrivate::FileUtilsPrivate()
    : m_InstanceIndex(m_NextInstanceID)
    , m_IsInvokerInstance(false)
{
    m_NextInstanceID++;
    m_Instances.insert(m_InstanceIndex, this);
}

FileUtilsPrivate::~FileUtilsPrivate()
{
    m_Instances.remove(m_InstanceIndex);
}

bool FileUtilsPrivate::openDocumentPicker(const QStringList &documentTypes, bool selectMultiple)
{
    bool opened = false;
    if (!m_IsDocumentPickerOpen) {
        opened = true;
        m_IsDocumentPickerOpen = true;
        m_IsInvokerInstance = true;
        if (m_DocumentPicker == nullptr) {
            m_DocumentPicker = [[DocumentPicker alloc] init];
        }

        [m_DocumentPicker showDocumentPicker: documentTypes allowMultiple: selectMultiple];
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
    auto begin = m_Instances.begin();
    auto end = m_Instances.end();
    for (auto it = begin; it != end; it++) {
        FileUtilsPrivate *tmpInstance = it.value();
        if (tmpInstance && tmpInstance->m_IsInvokerInstance) {
            instance = tmpInstance;
            break;
        }

    }

    return instance;
}

}
