    FilePath trackFile(const FilePath &repository) override
        return repository.pathAppended(".hg/branch");
    QString refreshTopic(const FilePath &repository) override
        return m_client->branchQuerySync(repository.toString());
    bool isVcsFileOrDirectory(const FilePath &filePath) const final;
    bool managesDirectory(const FilePath &filePath, FilePath *topLevel = nullptr) const final;
    bool managesFile(const FilePath &workingDirectory, const QString &fileName) const final;
    bool vcsOpen(const FilePath &filePath) final;
    bool vcsAdd(const FilePath &filePath) final;
    bool vcsDelete(const FilePath &filePath) final;
    bool vcsMove(const FilePath &from, const FilePath &to) final;
    bool vcsCreateRepository(const FilePath &directory) final;
    void vcsAnnotate(const FilePath &filePath, int line) final;
    void vcsDescribe(const FilePath &source, const QString &id) final { m_client.view(source.toString(), id); }
    FilePath m_submitRepository;
    const QString msg = tr("Commit changes for \"%1\".").arg(m_submitRepository.toUserOutput());
    commitEditor->setFields(QFileInfo(m_submitRepository.toString()), branch,
bool MercurialPluginPrivate::isVcsFileOrDirectory(const FilePath &filePath) const
    return m_client.isVcsDirectory(filePath);
bool MercurialPluginPrivate::managesDirectory(const FilePath &filePath, FilePath *topLevel) const
    const FilePath topLevelFound = m_client.findTopLevelForFile(filePath);
bool MercurialPluginPrivate::managesFile(const FilePath &workingDirectory, const QString &fileName) const
bool MercurialPluginPrivate::vcsOpen(const FilePath &filePath)
    Q_UNUSED(filePath)
bool MercurialPluginPrivate::vcsAdd(const FilePath &filePath)
    return m_client.synchronousAdd(filePath.parentDir(), filePath.fileName());
bool MercurialPluginPrivate::vcsDelete(const FilePath &filePath)
    return m_client.synchronousRemove(filePath.parentDir(), filePath.fileName());
bool MercurialPluginPrivate::vcsMove(const FilePath &from, const FilePath &to)
    const QFileInfo fromInfo = from.toFileInfo();
    const QFileInfo toInfo = to.toFileInfo();
    return m_client.synchronousMove(from.parentDir(),
                                    fromInfo.absoluteFilePath(),
                                    toInfo.absoluteFilePath());
bool MercurialPluginPrivate::vcsCreateRepository(const FilePath &directory)
void MercurialPluginPrivate::vcsAnnotate(const FilePath &filePath, int line)
    m_client.annotate(filePath.parentDir(), filePath.fileName(), QString(), line);
    auto command = new VcsBase::VcsCommand(baseDirectory, m_client.processEnvironment());
    FilePath topLevel;
    const bool managed = managesDirectory(FilePath::fromString(fi.absolutePath()), &topLevel);
    const QDir topLevelDir(topLevel.toString());
        emit repositoryChanged(FilePath::fromVariant(v));