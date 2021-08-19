//
// Created by Administrator on 2021/6/10.
//

#ifndef CGSTUDY_OBJECT_H
#define CGSTUDY_OBJECT_H

#include "geometry.h"
#include "transformation.h"


#include <graphics.h>        // 引用图形库头文件
#include <conio.h>
#include <vector>
#include <algorithm>

class Object {
public:
    Object(const string &texture_filename, const int &number_of_vertex, const int &number_of_face) :
            texture_filename_(texture_filename), number_of_vertex_(number_of_vertex), number_of_face_(number_of_face) {
        // 设置纹理相关变量
        loadimage(&texture_, texture_filename_.c_str());
        texture_width_ = texture_.getwidth();
        texture_height_ = texture_.getheight();
        ptexture_ = GetImageBuffer(&texture_);

        // 初始化顶点相关变量为 0
        vertex_local_.assign(number_of_vertex_, Vec4f());
        vertex_world_.assign(number_of_vertex_, Vec4f());
        vertex_screen_.assign(number_of_vertex_, Vec4f());
        vertex_screen_i_.assign(number_of_vertex_, Vec2i());
        vertex_s_face_index_.assign(number_of_vertex_, vector<int>());
        vertex_texture_.assign(number_of_vertex_, Vec2f());
        vertex_world_normal_.assign(number_of_vertex_, Vec4f());

        // 初始化面相关变量为 0
        face_s_vertex_index_.assign(number_of_face_, Vec3i());
        face_world_normal_.assign(number_of_face, Vec4f());
    }

    // 获取texture指定整数坐标的颜色 Vec3f
    Vec3f getTextureColorInt(int x, int y) {
        COLORREF colorBGR = ptexture_[y * texture_width_ + x];
        COLORREF colorRGB = BGR(colorBGR);
        return Vec3f{static_cast<float>(GetRValue(colorRGB)), static_cast<float>(GetGValue(colorRGB)),
                     static_cast<float>(GetBValue(colorRGB))};
    }

    // 获取texture指定 0-1 小数坐标的颜色
    Vec3f getTextureColor(float x, float y) {
        if (x < 0 || x > 1 || y < 0 || y > 1) {
            return Vec3f{0, 0, 0};
        }
        int x_int = lround((texture_width_ - 1) * x);
        int y_int = lround((texture_height_ - 1) * y);
        return getTextureColorInt(x_int, y_int);
    }

    void refresh() {
        // 用 4 个变换矩阵计算 obj 顶点的世界坐标和屏幕坐标
        for (int i = 0; i < number_of_vertex_; i++) {
            vertex_world_[i] = vertex_local_[i] * model_;
            vertex_screen_[i] = vecHomogenized(vertex_world_[i] * view_ * proj_) * viewport_;
            vertex_screen_i_[i] = Vec2i{lround(vertex_screen_[i][0]), lround(vertex_screen_[i][1])};
        }
        caculateFaceWorldNormal();
        caculateVertexWorldNormal();
    }


    // 计算世界空间里所有面的法向量
    void caculateFaceWorldNormal() {
        for (int i = 0; i < number_of_face_; i++) {
            // 当前要计算的面的 3 个顶点索引
            Vec3i &vertex_index = face_s_vertex_index_[i];

            Vec4f edge1 = vertex_world_[vertex_index[1]] - vertex_world_[vertex_index[0]];
            Vec4f edge2 = vertex_world_[vertex_index[2]] - vertex_world_[vertex_index[1]];
            face_world_normal_[i] = vecNormalized(crossProduct(edge1, edge2));
//            cout << "face_world_normal_" << face_world_normal_[i] << endl;  //没问题
        }
//        cout << "所有面算完了: " << endl; //确认没问题
    }

    // 计算世界空间里所有顶点的法向量，必须先计算了面的法向量才能调用
    void caculateVertexWorldNormal() {
        for (int i = 0; i < number_of_vertex_; i++) {
            // 当前要计算的顶点对应的面索引集
            vector<int> &face_index = vertex_s_face_index_[i];

//            cout << "顶点"<< i <<"对应的面索引";
//            for (int j = 0; j < face_index.size(); j++) {
//                cout << face_index[j] << ' ';       //确认没问题
//            }
//            cout << endl;

            Vec4f vertex_norm;
            for (int j = 0; j < face_index.size(); j++) {
//                cout << i << "    face_world_normal_: " << face_world_normal_[j] << endl;
                vertex_norm += face_world_normal_[face_index[j]];       // 开始这个地方忘记用face_index
            }
            vertex_world_normal_[i] = vecNormalized(vertex_norm);
//            cout << i << "   vertex_world_normal_: " << vertex_world_normal_[i] << endl;
        }
    }

public:
    string texture_filename_;
    IMAGE texture_;                     //纹理图像
    DWORD *ptexture_;                   //指向纹理图像的指针
    int texture_width_;
    int texture_height_;

    int number_of_vertex_;
    int number_of_face_;

    vector <Vec4f> vertex_local_;       //局部空间顶点坐标

    vector <Vec4f> vertex_world_;       ////世界空间顶点坐标。会变
    vector <Vec4f> vertex_screen_;       ////屏幕空间顶点坐标，保留 z。会变
    vector <Vec2i> vertex_screen_i_;     ////屏幕空间顶点的 2d 坐标。会变

    vector <vector<int>> vertex_s_face_index_;   //顶点对应的面索引集

    vector <Vec2f> vertex_texture_;      //顶点对应的纹理坐标，小数，0 ~ 1

    vector <Vec4f> vertex_world_normal_;  ////世界空间顶点的法向量。会变


    vector <Vec3i> face_s_vertex_index_;     //三角面，对应的顶点索引，顺时针
    vector <Vec4f> face_world_normal_;       ////世界空间面的法向量。会变

    // 四个变换矩阵
    Mat44f model_;
    Mat44f view_;
    Mat44f proj_;
    Mat44f viewport_;
};


#endif //CGSTUDY_OBJECT_H
