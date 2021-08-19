//
// Created by Administrator on 2021/6/4.
//

#ifndef CGSTUDY_TRANSFORMATION_H
#define CGSTUDY_TRANSFORMATION_H

#include "geometry.h"

//
//{{1,        0,         0,         0},
//{0,         1,         0,         0},
//{0,         0,         1,         0},
//{0,         0,         0,         1}};


// 模型变换矩阵，model/world transform
template<typename T>
Mat<4, 4, T> getMatModel(Vec<3, T> scale, Vec<3, T> rotate, Vec<3, T> translation) {
    Mat<4, 4, T> mat_scale{{scale.x(), 0,         0,         0},
                           {0,         scale.y(), 0,         0},
                           {0,         0,         scale.z(), 0},
                           {0,         0,         0,         1}};

    Mat<4, 4, T> mat_rotate_x{{1, 0,                0,                0},
                              {0, +cos(rotate.x()), -sin(rotate.x()), 0},
                              {0, +sin(rotate.x()), +cos(rotate.x()), 0},
                              {0, 0,                0,                1}};

    Mat<4, 4, T> mat_rotate_y{{+cos(rotate.y()), 0, +sin(rotate.y()), 0},
                              {0,                1, 0,                0},
                              {-sin(rotate.y()), 0, +cos(rotate.y()), 0},
                              {0,                0, 0,                1}};

    Mat<4, 4, T> mat_rotate_z{{+cos(rotate.z()), -sin(rotate.z()), 0, 0},
                              {+sin(rotate.z()), +cos(rotate.z()), 0, 0},
                              {0,                0,                1, 0},
                              {0,                0,                0, 1}};

    Mat<4, 4, T> mat_translation{{1,               0,               0,               0},
                                 {0,               1,               0,               0},
                                 {0,               0,               1,               0},
                                 {translation.x(), translation.y(), translation.z(), 1}};

    return mat_scale * (mat_rotate_x * mat_rotate_y * mat_rotate_z) * mat_translation;

}


// 摄影机变换矩阵, 也有叫 view transform
// 参考 D3DXMatrixLookAtLH
// DX9坐标是行向量，矩阵在右边
// eye: 相机位置
// at：相机看向的方向
// up：相机头顶的方向
template<typename T>
Mat<4, 4, T> getMatLookAtLH(const Vec<3, T> &eye, const Vec<3, T> &at, const Vec<3, T> &up) {
    Vec<3, T> zaxis = vecNormalized(at - eye);
    Vec<3, T> xaxis = vecNormalized(crossProduct(up, zaxis));
    Vec<3, T> yaxis = crossProduct(zaxis, xaxis);

    Mat<4, 4, T> cam{{xaxis.x(),               yaxis.x(),               zaxis.x(),               0},
                     {xaxis.y(),               yaxis.y(),               zaxis.y(),               0},
                     {xaxis.z(),               yaxis.z(),               zaxis.z(),               0},
                     {-dotProduct(xaxis, eye), -dotProduct(yaxis, eye), -dotProduct(zaxis, eye), 1}};

    return cam;
}


// 投影变换矩阵
// 参考 D3DXMatrixPerspectiveFovLH
// aspect: width / height
template<typename T>
Mat<4, 4, T> getMatProjection(T fovy, T aspect, T zn, T zf) {
    T y_scale = 1 / tan(fovy * 0.5);
    T x_scale = y_scale / aspect;

    Mat<4, 4, T> proj{{x_scale, 0,       0,                    0},
                      {0,       y_scale, 0,                    0},
                      {0,       0,       zf / (zf - zn),       1},
                      {0,       0,       -zn * zf / (zf - zn), 0}};

    return proj;
}

//
//// 3D游戏编程大师技巧书上的，有点不太对
//template<typename T>
//Mat<4, 4, T> getMatViewport(T width, T height) {
//    T alpha = 0.5 * width - 0.5;
//    T beta = 0.5 * height - 0.5;
//    Mat<4, 4, T> viewport{{alpha, 0,     0, alpha},
//                          {0,     -beta, 0, beta},
//                          {0,     0,     1, 0},
//                          {0,     0,     0, 1}};
//
//    return viewport;
//}


// wyx
template<typename T>
Mat<4, 4, T> getMatViewport(T width, T height) {
    Mat<4, 4, T> viewport{{0.5f * width, 0.f,           0.f, 0.f},
                          {0.f,          0.5f * height, 0.f, 0.f},
                          {0.f,          0.f,           1.f, 0.f},
                          {0.5f * width, 0.5f * height, 0.f, 1.f}};
    return viewport;
}


#endif //CGSTUDY_TRANSFORMATION_H
