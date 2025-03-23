#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QProcess>  // 用于执行外部命令
#include <QRegularExpression>  // 新增此行
#include <QStandardPaths>
#include <QSettings>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("浮黎 V1.4.8"); // 修改此行
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_videoBtn_clicked()
{
    QSettings settings;
    QString lastPath = settings.value("Last/VideoPath", QDir::homePath()).toString();

    QString path = QFileDialog::getOpenFileName(
        this, "选择视频文件", lastPath, "视频文件 (*.m4s);;所有文件 (*.*)");

    if (!path.isEmpty()) {
        ui->videoEdit->setText(path);
        settings.setValue("Last/VideoPath", QFileInfo(path).absolutePath());
    }
}


void MainWindow::on_audioBtn_clicked()
{
    QSettings settings;
    QString lastPath = settings.value("Last/AudioPath", QDir::homePath()).toString();

    QString path = QFileDialog::getOpenFileName(
        this, "选择音频文件", lastPath, "音频文件 (*.m4s);;所有文件 (*.*)");

    if (!path.isEmpty()) {
        ui->audioEdit->setText(path);
        settings.setValue("Last/AudioPath", QFileInfo(path).absolutePath());
    }
}


void MainWindow::on_outputBtn_clicked()
{
    QSettings settings;
    QString lastDir = settings.value("Last/OutputPath",
                                     QStandardPaths::writableLocation(QStandardPaths::DownloadLocation)).toString();

    // 关键修改：将默认路径改为纯目录路径（末尾添加斜杠）
    QString defaultPath = QDir::toNativeSeparators(lastDir + "/");

    QString path = QFileDialog::getSaveFileName(
        this,
        "选择输出位置",
        defaultPath,  // 使用纯目录路径
        "MP4文件 (*.mp4)"
        );

    if (!path.isEmpty()) {
        ui->outputEdit->setText(path);
        settings.setValue("Last/OutputPath", QFileInfo(path).absolutePath());
    }
}

//修改合并函数为异步执行
bool MainWindow::mergeFiles(const QString& video, const QString& audio, const QString& output) {
    QProcess *process = new QProcess(this);
    connect(process, &QProcess::readyReadStandardError, [=]() {
        QString output = QString::fromLocal8Bit(process->readAllStandardError());
        updateProgress(output);
    });

    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            [=](int exitCode, QProcess::ExitStatus status) {
                process->deleteLater();
                if (exitCode == 0) {
                    QMessageBox::information(this, "成功", "文件合并完成！");

                    //新增清空操作(合并完成，清空所有文本框)
                    ui->videoEdit->clear();
                    ui->audioEdit->clear();
                    ui->outputEdit->clear();
                } else {
                    QMessageBox::critical(this, "失败", "合并过程中出现错误");
                }
                ui->progressBar->setValue(0); // 重置进度条
            });

    QString command = QString("ffmpeg -y -hide_banner -stats -i \"%1\" -i \"%2\" -c copy -map 0:v -map 1:a \"%3\"")
                          .arg(video).arg(audio).arg(output);

    process->start(command);
    return process->waitForStarted(); // 立即返回，不阻塞
}

//修改合并按钮的槽函数
void MainWindow::on_pushButton_clicked()
{
    QString video = ui->videoEdit->text();
    QString audio = ui->audioEdit->text();
    QString output = ui->outputEdit->text();

    if(video.isEmpty() || audio.isEmpty() || output.isEmpty()) {
        QMessageBox::warning(this, "错误", "请先选择所有文件！");
        return;
    }

    ui->progressBar->setValue(0); // 开始前重置进度
    if(!mergeFiles(video, audio, output)) {
        QMessageBox::critical(this, "错误", "无法启动FFmpeg进程");
    }
}

//实现进度解析逻辑（新增函数）
void MainWindow::updateProgress(const QString &output) {
    static qint64 duration = 0;
    QRegularExpression reDuration(R"(Duration: (\d{2}):(\d{2}):(\d{2})\.\d{2})");
    QRegularExpression reTime(R"(time=(\d{2}):(\d{2}):(\d{2})\.\d{2})");

    // 解析总时长
    if (duration == 0) {
        auto match = reDuration.match(output);
        if (match.hasMatch()) {
            duration = match.captured(1).toInt() * 3600 +
                       match.captured(2).toInt() * 60 +
                       match.captured(3).toInt();
        }
    }

    // 解析当前时间
    auto timeMatch = reTime.match(output);
    if (timeMatch.hasMatch() && duration > 0) {
        qint64 current = timeMatch.captured(1).toInt() * 3600 +
                         timeMatch.captured(2).toInt() * 60 +
                         timeMatch.captured(3).toInt();

        int progress = static_cast<int>((current * 100) / duration);
        ui->progressBar->setValue(progress);
    }
}

//1.3.6版本：经过测试，输出路径的记忆功能很不错，非常实用
//但是“标题文件夹一键导入”模式的“输出路径”功能经过测试一团乱麻，我需要好好梳理“记忆路径”与“默认路径”逻辑了！
void MainWindow::on_titlefolderBtn_clicked()
{
    QSettings settings;
    QString lastFolder = settings.value("Last/TitleFolderPath", QDir::homePath()).toString();

    QString folderPath = QFileDialog::getExistingDirectory(
        this, "选择包含音视频的文件夹", lastFolder);

    if (folderPath.isEmpty()) return;

    // 获取文件夹名称
    QDir dir(folderPath);
    QString folderName = dir.dirName();

    // 验证音视频文件存在性
    QString videoPath = dir.filePath("video.m4s");
    QString audioPath = dir.filePath("audio.m4s");
    if (!QFile::exists(videoPath) || !QFile::exists(audioPath)) {
        QMessageBox::warning(this, "错误", "未找到标准音视频文件");
        return;
    }

    // 更新记忆
    settings.setValue("Last/TitleFolderPath", folderPath);

    // 生成默认输出路径（不记忆）
    QString defaultDir = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation)
                         + "/" + QDir(folderPath).dirName();
    QString defaultPath = defaultDir + "/" + QDir(folderPath).dirName() + ".mp4";

    // 弹出保存对话框（会话级记忆）
    QString outputPath = QFileDialog::getSaveFileName(
        this, "保存合并文件",
        m_sessionTitleOutputPath.isEmpty() ? defaultPath : m_sessionTitleOutputPath,
        "MP4文件 (*.mp4)"
        );

    if (!outputPath.isEmpty()) {
        m_sessionTitleOutputPath = outputPath; // 会话级变量
        ui->outputEdit->setText(outputPath);

        // 目录操作逻辑...
    }

    // 弹出保存对话框后
    if (!outputPath.isEmpty()) {
        settings.setValue("LastOutputPath", QFileInfo(outputPath).absolutePath()); // 更新记忆路径
        // 填充UI
        if (!outputPath.isEmpty()) {
            ui->videoEdit->setText(videoPath);
            ui->audioEdit->setText(audioPath);
            ui->outputEdit->setText(outputPath);
        }
    }

}

