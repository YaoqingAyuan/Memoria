#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>  // 文件对话框
#include <QMessageBox>  // 提示框

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_videoBtn_clicked();

    void on_audioBtn_clicked();

    void on_outputBtn_clicked();

    void on_pushButton_clicked();

    // 新增合并函数声明
    bool mergeFiles(const QString& video, const QString& audio, const QString& output);

    //实现进度解析逻辑（新增函数）
    void updateProgress(const QString &output);

private:

    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
