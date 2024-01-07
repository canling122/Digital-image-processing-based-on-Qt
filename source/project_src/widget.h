#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QImage>
#include <QFileDialog>
#include <QTimer>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private:
    QImage Mat2QImage(const cv::Mat &mat);
    void display(QImage image);
    void salt(cv::Mat &image, int n);
    void gray(cv::Mat &image);
    void two(cv::Mat &image, int value);
    QImage avg(cv::Mat &image);
    void reservse(cv::Mat &image);

    void euqalize(cv::Mat &image);


private slots:
    void on_fileselect_clicked();

    void on_saltButton_clicked();

    void on_originalButton_clicked();

    void on_grayButton_clicked();

    void on_twoButton_clicked();

    void on_saveButton_clicked();

    void on_midButton_clicked();

    void on_pushButton_4_clicked();

    void on_reservseButton_clicked();

    void on_avgButton_clicked();

    void on_antiColorButton_clicked();

    void on_equalizeHist_clicked();

private:
    Ui::Widget *ui;
    cv::Mat image;
    cv::Mat originalimg; //保存原始图
    QString fileName;   //图片路径
    QString currentPath;
    QImage qimg;
    QImage imgScaled;
    cv::Mat rgb;

};

#endif // WIDGET_H
