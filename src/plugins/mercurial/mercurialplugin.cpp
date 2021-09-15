    QString trackFile(const QString &repository) override
        return repository + QLatin1String("/.hg/branch");
    QString refreshTopic(const QString &repository) override
        return m_client->branchQuerySync(repository);
    bool isVcsFileOrDirectory(const Utils::FilePath &fileName) const final;
    bool managesDirectory(const QString &filename, QString *topLevel = nullptr) const final;
    bool managesFile(const QString &workingDirectory, const QString &fileName) const final;
    bool vcsOpen(const QString &fileName) final;
    bool vcsAdd(const QString &filename) final;
    bool vcsDelete(const QString &filename) final;
    bool vcsMove(const QString &from, const QString &to) final;
    bool vcsCreateRepository(const QString &directory) final;
    void vcsAnnotate(const QString &file, int line) final;
    void vcsDescribe(const QString &source, const QString &id) final { m_client.view(source, id); }
    QString m_submitRepository;
    const QString msg = tr("Commit changes for \"%1\".").
                        arg(QDir::toNativeSeparators(m_submitRepository));
    commitEditor->setFields(QFileInfo(m_submitRepository), branch,
bool MercurialPluginPrivate::isVcsFileOrDirectory(const Utils::FilePath &fileName) const
    return m_client.isVcsDirectory(fileName);
bool MercurialPluginPrivate::managesDirectory(const QString &directory, QString *topLevel) const
    QFileInfo dir(directory);
    const QString topLevelFound = m_client.findTopLevelForFile(dir);
bool MercurialPluginPrivate::managesFile(const QString &workingDirectory, const QString &fileName) const
bool MercurialPluginPrivate::vcsOpen(const QString &filename)
    Q_UNUSED(filename)
bool MercurialPluginPrivate::vcsAdd(const QString &filename)
    const QFileInfo fi(filename);
    return m_client.synchronousAdd(fi.absolutePath(), fi.fileName());
bool MercurialPluginPrivate::vcsDelete(const QString &filename)
    const QFileInfo fi(filename);
    return m_client.synchronousRemove(fi.absolutePath(), fi.fileName());
bool MercurialPluginPrivate::vcsMove(const QString &from, const QString &to)
    const QFileInfo fromInfo(from);
    const QFileInfo toInfo(to);
    return m_client.synchronousMove(fromInfo.absolutePath(),
                                            fromInfo.absoluteFilePath(),
                                            toInfo.absoluteFilePath());
bool MercurialPluginPrivate::vcsCreateRepository(const QString &directory)
void MercurialPluginPrivate::vcsAnnotate(const QString &file, int line)
    const QFileInfo fi(file);
    m_client.annotate(fi.absolutePath(), fi.fileName(), QString(), line);
    auto command = new VcsBase::VcsCommand(baseDirectory.toString(),
                                           m_client.processEnvironment());
    QString topLevel;
    const bool managed = managesDirectory(fi.absolutePath(), &topLevel);
    const QDir topLevelDir(topLevel);
        emit repositoryChanged(v.toString());