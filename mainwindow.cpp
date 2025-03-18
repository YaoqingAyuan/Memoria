#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QProcess>  // 用于执行外部命令
#include <QRegularExpression>  // 新增此行
#include <QStandardPaths>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("浮黎 V1.2.2"); // 修改此行
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_videoBtn_clicked()
{
    QString path = QFileDialog::getOpenFileName(
        this,
        "选择视频文件",
        "",
        "视频文件 (*.m4s);;所有文件 (*.*)"
        );
    ui->videoEdit->setText(path);
}


void MainWindow::on_audioBtn_clicked()
{
    QString path = QFileDialog::getOpenFileName(
        this, "选择音频文件", "", "音频文件 (*.m4s)"
        );
    ui->audioEdit->setText(path);
}


void MainWindow::on_outputBtn_clicked()
{
    QString path = QFileDialog::getSaveFileName(
        this,
        "选择输出位置",
        "",
        "MP4文件 (*.mp4)"
        );
    ui->outputEdit->setText(path);
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


void MainWindow::on_titlefolderBtn_clicked()
{
    // 选择文件夹
    QString folderPath = QFileDialog::getExistingDirectory(
        this, "选择包含音视频的文件夹", QDir::homePath()
        );
    if (folderPath.isEmpty()) return;

    // 获取文件夹名称
    QDir dir(folderPath);
    QString folderName = dir.dirName();

    // 构建文件路径
    QString videoPath = dir.filePath("video.m4s");
    QString audioPath = dir.filePath("audio.m4s");

    // 验证文件存在性
    if (!QFile::exists(videoPath) || !QFile::exists(audioPath)) {
        QMessageBox::warning(this, "错误", "未找到标准音视频文件");
        return;
    }

    // 生成默认输出路径
    QString downloadPath = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
    QString outputPath = QDir::toNativeSeparators(
        downloadPath + "/" + folderName + ".mp4"
        );

    // 自动创建目录
    QDir().mkpath(QFileInfo(outputPath).path());

    // 填充UI
    ui->videoEdit->setText(videoPath);
    ui->audioEdit->setText(audioPath);
    ui->outputEdit->setText(outputPath);
}

