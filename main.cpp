#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <iostream>

QString currentPath() {
    std::array<char, 128> buffer;
    QString result;
    auto pipe = popen("cd", "r");
    while (!feof(pipe)) {
        if (fgets(buffer.data(), 128, pipe) != nullptr){
            result += buffer.data();
        }
    }
    auto rc = pclose(pipe);
    if (rc == EXIT_SUCCESS) { // == 0

    } else if (rc == EXIT_FAILURE) {

    }
    result = result.trimmed();
    return result;
}

void createFiles(
        const QString& originPath,
        const QString& assetsDir) {

    //mkdir
    QString assetsCleanDir = assetsDir;
    assetsCleanDir = assetsCleanDir.remove(":/");
    QString currentDir = QString(originPath + "/" + assetsCleanDir);
    qDebug() << currentDir;
    QDir dirMk(currentDir);
    dirMk.mkdir(currentDir);

    //file
    QDir dir(assetsDir);
    QFileInfoList infoFiles = dir.entryInfoList(QStringList(), QDir::Files);
    QFileInfoList infoDirs = dir.entryInfoList(QStringList(), QDir::Dirs);
    for(auto &&info: infoFiles) {
        QString path = info.absoluteFilePath();
        QString filename = info.fileName();
        QString newFile = currentDir + "/" + filename;
        QFile::copy(path, newFile);
        QFile::setPermissions(newFile, QFileDevice::ExeOther);
        qDebug() << QFile::setPermissions(newFile, QFileDevice::WriteOwner);;
    }

    //dir
    for (auto &&info: infoDirs){
        QString assetsDir = info.absoluteFilePath();
        createFiles(originPath, assetsDir);
//        qDebug() << "path:" << originPath;
    }
}

int main(int argc, char *argv[])
{
//    QCoreApplication a(argc, argv);

    QStringList list;
    list.reserve(2);

    QString ide = "clion";
    QString template_ = "ui";

    for (int i = 1; i < argc; i += 2) {
        QString opt = QString::fromUtf8(argv[i]);
        if (i + 1 >= argc) {
            std::cout << "warning: parametrs is problem for program";
            return -1;
        }
        QString arg = QString::fromUtf8(argv[i + 1]);

        if (opt == "--i") {
            ide = arg;
        } else if (opt == "--t") {
            template_ = arg;
        }
    }

    //createFiles
    //ui-with-file
    QString path = currentPath();
    createFiles(path, ":/" + template_);

    list << ide << path + "/" + template_ + "/CmakeLists.txt";

    std::string runApp = list.join(" ").toStdString();
    qDebug() << QString::fromStdString(runApp);
    system(runApp.c_str());

    return 0;
}
