#pragma once

#include "utils_global.h"

#include <QObject>

namespace Utils {
class FileSystemWatcherPrivate;

// Documentation inside.
class QTCREATOR_UTILS_EXPORT FileSystemWatcher : public QObject
{
    Q_OBJECT

public:
    enum WatchMode
    {
        WatchModifiedDate,
        WatchAllChanges
    };

    explicit FileSystemWatcher(QObject *parent = nullptr);
    explicit FileSystemWatcher(int id, QObject *parent = nullptr);
    ~FileSystemWatcher() override;

    void addFile(const QString &file, WatchMode wm);
    void addFiles(const QStringList &files, WatchMode wm);

    void removeFile(const QString &file);
    void removeFiles(const QStringList &files);

    bool watchesFile(const QString &file) const;
    QStringList files() const;

    void addDirectory(const QString &file, WatchMode wm);
    void addDirectories(const QStringList &files, WatchMode wm);

    void removeDirectory(const QString &file);
    void removeDirectories(const QStringList &files);

    bool watchesDirectory(const QString &file) const;
    QStringList directories() const;

signals:
    void fileChanged(const QString &path);
    void directoryChanged(const QString &path);

private:
    void init();
    void slotFileChanged(const QString &path);
    void slotDirectoryChanged(const QString &path);

    FileSystemWatcherPrivate *d;
};

} // namespace Utils
