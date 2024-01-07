#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <opencv2/imgproc/types_c.h>
#include <QImage>
#include <QMessageBox>
#include <QFileDialog>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    setWindowTitle("数字图像处理课程程序 [2000080214 康南松]");
}

Widget::~Widget()
{
    delete ui;
}

// 选择图片
void Widget::on_fileselect_clicked()
{
    fileName = QFileDialog::getOpenFileName(this, tr("选择图像"), ".", tr("Images(*.jpg *.png *.bmp)"));

    //这里是显示图片路径
    //qDebug()<<"filenames:"<<fileName;
    ui->imgfilelabel->setText(fileName);

    image = cv::imread(fileName.toLatin1().data());


    //图片备份
    image.copyTo(originalimg);          //copy
    //originalimg = image.clone();      //clone

    //这里图片需要转换成 QImage 类型的才能在 Qt 上展示,因为我自定义 iamge 类型为 cv::Mat 类型
    //一个是 BGR，一个是 RGB
    qimg = Widget::Mat2QImage(image);
    display(qimg);
    // 这里判断为什么时对时错？
    if(image.data)
    {
        ui->saltButton->setEnabled(true);
        ui->originalButton->setEnabled(true);
        ui->grayButton->setEnabled(true);
        ui->twoButton->setEnabled(true);
        ui->saveButton->setEnabled(true);
        ui->equalizeHist->setEnabled(true);

    }

}



//保存图片
void Widget::on_saveButton_clicked()
{
    if(fileName != nullptr)
    {
    QString path = QFileDialog::getSaveFileName(this, tr("图像另存为"), ".", tr("Images(*.jpg *.png *.bmp)"));
    if(!path.isEmpty()){
        QImage img = ui->imagelabel->pixmap()->toImage();
        img.save(path);
        fileName = path;
        QMessageBox::information(this, tr("true"), tr("保存成功！")); //验证操作是否正确
    }
    }
    else {
        QMessageBox::information(this, tr("error"), tr("请选择图片！"));
    }
}

//把Mat 转 QImage
QImage Widget::Mat2QImage(const cv::Mat &mat)
{
    QImage img;
    if(mat.channels()==3)
    {
        //cvt Mat BGR To QImage RGB
        cvtColor(mat,rgb,CV_BGR2RGB);
        img =QImage((const unsigned char*)(rgb.data),
                    rgb.cols,rgb.rows,
                    rgb.cols*rgb.channels(),
                    QImage::Format_RGB888);
    }
    else
    {
        img =QImage((const unsigned char*)(mat.data),
                    mat.cols,mat.rows,
                    mat.cols*mat.channels(),
                    QImage::Format_RGB888);
    }
    return img;
}

//显示处理后的图片
void Widget::display(QImage img)
{
    QImage imgScaled;
//    imgScaled = img.scaled(ui->imagelabel->size(),Qt::KeepAspectRatio);
  imgScaled = img.QImage::scaled(ui->imagelabel->width(),ui->imagelabel->height(),Qt::KeepAspectRatio);
   ui->imagelabel->setPixmap(QPixmap::fromImage(imgScaled));

//    ui->imagelabel->setPixmap(QPixmap::fromImage(img));
//    ui->imagelabel->resize(img.width(), img.height());
}

void Widget::on_originalButton_clicked()
{
    qimg = Widget::Mat2QImage(originalimg);
    display(qimg);
}



//下面实现功能算法的代码大部分都参考于网络以及上机实验的代码


void Widget::on_saltButton_clicked()
{
    salt(image,3000);
    qimg = Widget::Mat2QImage(image);
    display(qimg);
}


//椒盐噪声
void Widget::salt(cv::Mat &image, int n)
{
    if(fileName != nullptr)
    {
    int i,j;
    for (int k=0; k<n; k++)
    {

        i= qrand()%image.cols;
        j= qrand()%image.rows;

        if (image.channels() == 1)
        { // gray-level image

            image.at<uchar>(j,i)= 255;

        }
        else if (image.channels() == 3)
        { // color image

            image.at<cv::Vec3b>(j,i)[0]= 255;
            image.at<cv::Vec3b>(j,i)[1]= 255;
            image.at<cv::Vec3b>(j,i)[2]= 255;
        }
    }
    }
    else {
        QMessageBox::information(this, tr("error"), tr("请选择图片！"));
    }
}


void Widget::on_grayButton_clicked()
{
    gray(image);
    qimg = Widget::Mat2QImage(image);
    display(qimg);
}


// 转灰度图
void Widget::gray(cv::Mat &image)
{

    //灰度处理 灰度是单通道8位 QImage是24位三通道
    cvtColor(image,image,CV_BGR2GRAY);
    cvtColor(image,image,CV_GRAY2BGR);
}

void Widget::on_twoButton_clicked()
{
    two(image,144);
    //qimg = Widget::Mat2QImage(image);
    //display(qimg);
}


//二值化，但这个 value 值怎么自定义选择不知道
void Widget::two(cv::Mat &image, int value)
{
    //QImage image(fileName);
    gray(image);
    QImage img = Widget::Mat2QImage(image);
    int height=img.height();
    int width= img.width();
            int bt;
            QColor oldColor;
            for (int i = 0; i < height; ++i)
            {
                for(int j=0;j<width;++j){
                    oldColor = QColor(img.pixel(j,i));
                    bt = oldColor.red();
                if(bt<value){
                    bt=0;
                }else{
                    bt=255;
                }
                img.setPixel(j,i, qRgb(bt, bt, bt));

                }
            }

        display(img);

}


//中值滤波
void Widget::on_midButton_clicked()
{
    medianBlur(image,image,5);
    qimg = Widget::Mat2QImage(image);
    display(qimg);

}


void Widget::on_pushButton_4_clicked()
{
    QMessageBox::information(this, tr("Please"), tr("图片路径里有中文和空格不知道为什么不能显示，可能Qt编码不对？时间不够就没去研究了，所以请不要打开中文路径的图片 祝老师快乐 o(*￣▽￣*)ブ"));
}


//图像旋转
void Widget::on_reservseButton_clicked()
{
    reservse(image);
}

void Widget::reservse(cv::Mat &image)
{
    QMatrix matrix;
    matrix.rotate(45);
    QImage img = Widget::Mat2QImage(image);
    QImage imgScaled;
    imgScaled = img.scaled(ui->imagelabel->size(),Qt::KeepAspectRatio);
//  imgScaled = img.QImage::scaled(ui->imagelabel->width(),ui->imagelabel->height(),Qt::KeepAspectRatio);
    //ui->imagelabel->setPixmap(QPixmap::fromImage(imgScaled));
    ui->imagelabel->setPixmap(QPixmap::fromImage(imgScaled).transformed(matrix,Qt::SmoothTransformation));

}

//均值滤波
void Widget::on_avgButton_clicked()
{
    display(avg(image));
}


QImage Widget::avg(cv::Mat &image)
{
        int kernel [3][3] = {
            {1,1,1},
            {1,1,1},
            {1,1,1}};
            int sizeKernel = 3;
            int sumKernel = 9;
            QColor color;
            QImage img = Widget::Mat2QImage(image);
             for(int x = sizeKernel/2;x<img.width() - sizeKernel/2;x++)
            {
               for(int y= sizeKernel/2;y<img.height() - sizeKernel/2;y++)
                {
                    int r = 0;
                    int g = 0;
                    int b = 0;
                    for(int i = -sizeKernel/2;i<=sizeKernel/2;i++)
                    {
                       for(int j = -sizeKernel/2;j<=sizeKernel/2;j++)
                        {
                         color = QColor(img.pixel(x+i,y+j));
                         r+=color.red()*kernel[sizeKernel/2+i][sizeKernel/2+j];
                         g+=color.green()*kernel[sizeKernel/2+i][sizeKernel/2+j];
                         b+=color.blue()*kernel[sizeKernel/2+i][sizeKernel/2+j];
                        }
                    }
                    r = qBound(0,r/sumKernel,255);
                    g = qBound(0,g/sumKernel,255);
                    b = qBound(0,b/sumKernel,255);
                    img.setPixel(x,y,qRgb( r,g,b));
                }
            }
             return img;

}

void Widget::on_antiColorButton_clicked()
{

}




void Widget::on_equalizeHist_clicked()
{
    euqalize(image);
//    qimg = Widget::Mat2QImage(image);
//    display(qimg);

}

void Widget::euqalize(cv::Mat &image)
{
////    Mat gray_img;
////    cvtColor(image,gray_img,COLOR_BGR2GRAY);
//    //imshow("gray_img", gray_img);//灰度图像
//    gray(image);
//    Mat dst;
//    equalizeHist(image,dst);
//    qimg = Widget::Mat2QImage(dst);
//    display(qimg);
//    //imshow("hist_equalize", dst);
    Mat gray_img;
    cvtColor(image,gray_img,COLOR_BGR2GRAY);
    //namedWindow("gray_img", WINDOW_FREERATIO);
    //imshow("gray_img", gray_img);//灰度图像
    Mat dst;
    equalizeHist(gray_img,dst);
    namedWindow("hist_equalize", WINDOW_FREERATIO);
    imshow("hist_equalize", dst);
}
