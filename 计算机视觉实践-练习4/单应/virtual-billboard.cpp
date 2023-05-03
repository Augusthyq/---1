#include <opencv2/opencv.hpp>
#include<opencv2/imgproc/imgproc_c.h>

using namespace cv;
using namespace std;

struct userdata{
    Mat im;
    vector<Point2f> points;
};

void mouseHandler(int event, int x, int y, int flags, void* data_ptr)
{
    if  ( event == EVENT_LBUTTONDOWN )
    {
        userdata *data = ((userdata *) data_ptr);
        circle(data->im, Point(x,y),3,Scalar(0,255,255), 5, CV_AA);
        imshow("Image", data->im);
        if (data->points.size() < 4)
        {
            data->points.push_back(Point2f(x,y));
        }
    }

}

int main( int argc, char** argv)
{

    // 读取图片
    // 定义vector存储图像的四个角
    Mat im_src = imread("../truck.jpg");

    Size size = im_src.size();
    vector<Point2f> pts_src;
    pts_src.push_back(Point2f(0,0));
    pts_src.push_back(Point2f(size.width - 1, 0));
    pts_src.push_back(Point2f(size.width - 1, size.height -1));
    pts_src.push_back(Point2f(0, size.height - 1 ));

    // 读取广告牌图像
    Mat im_dst = imread("../billboard.jpg");
    Mat im_temp = im_dst.clone();
    // 定义结果图像
    userdata data;
    data.im = im_temp;
    imshow("Image", im_temp);

    // 定义鼠标事件，用于点击广告牌要贴如汽车图片的区域
    cout << "从左上角依次顺时针点击要贴入广告牌的区域" << endl;
    setMouseCallback("Image", mouseHandler, &data);
    waitKey(0);

    // 计算单应矩阵，并将car图片进行透视变换
    Mat H = findHomography(pts_src, data.points, 0); //计算单应矩阵
    warpPerspective(im_src, data.im, H, data.im.size()); //透视变换
    imshow("perspect",data.im);
    imwrite("../truck_perspect.jpg", data.im);

    // 进行图像填充
    Point pts_dst[4];
    for (int i = 0; i < 4; i++)
    {
        pts_dst[i] = data.points[i];
    }
    fillConvexPoly(im_dst, pts_dst, 4, Scalar(0), CV_AA);
    im_dst = im_dst + data.im;

    // 显示结果
    imwrite("../truck_result.jpg", im_dst);
    imshow("Image", im_dst);
    waitKey(0);

    return 0;
}