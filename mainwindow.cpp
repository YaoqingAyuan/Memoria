#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QProcess>  // 用于执行外部命令

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("浮黎 V1.0.1"); // 修改此行
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

bool MainWindow::mergeFiles(const QString& video, const QString& audio, const QString& output) {
    QString command = QString("ffmpeg -y -i \"%1\" -i \"%2\" -c copy -map 0:v -map 1:a \"%3\"")
    .arg(video).arg(audio).arg(output);

    QProcess process;
    process.startCommand(command);
    process.waitForFinished();

    return (process.exitCode() == 0);
}

void MainWindow::on_pushButton_clicked()
{
    QString video = ui->videoEdit->text();
    QString audio = ui->audioEdit->text();
    QString output = ui->outputEdit->text();

    if(video.isEmpty() || audio.isEmpty() || output.isEmpty()) {
        QMessageBox::warning(this, "错误", "请先选择所有文件！");
        return;
    }

    if(mergeFiles(video, audio, output)) {
        QMessageBox::information(this, "成功", "文件合并完成！");
    } else {
        QMessageBox::critical(this, "失败", "合并过程中出现错误");
    }
}



