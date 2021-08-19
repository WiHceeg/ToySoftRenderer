//
// Created by Administrator on 2021/6/10.
//

#ifndef CGSTUDY_OBJECT_H
#define CGSTUDY_OBJECT_H

#include "geometry.h"
#include "transformation.h"


#include <graphics.h>        // ����ͼ�ο�ͷ�ļ�
#include <conio.h>
#include <vector>
#include <algorithm>

class Object {
public:
    Object(const string &texture_filename, const int &number_of_vertex, const int &number_of_face) :
            texture_filename_(texture_filename), number_of_vertex_(number_of_vertex), number_of_face_(number_of_face) {
        // ����������ر���
        loadimage(&texture_, texture_filename_.c_str());
        texture_width_ = texture_.getwidth();
        texture_height_ = texture_.getheight();
        ptexture_ = GetImageBuffer(&texture_);

        // ��ʼ��������ر���Ϊ 0
        vertex_local_.assign(number_of_vertex_, Vec4f());
        vertex_world_.assign(number_of_vertex_, Vec4f());
        vertex_screen_.assign(number_of_vertex_, Vec4f());
        vertex_screen_i_.assign(number_of_vertex_, Vec2i());
        vertex_s_face_index_.assign(number_of_vertex_, vector<int>());
        vertex_texture_.assign(number_of_vertex_, Vec2f());
        vertex_world_normal_.assign(number_of_vertex_, Vec4f());

        // ��ʼ������ر���Ϊ 0
        face_s_vertex_index_.assign(number_of_face_, Vec3i());
        face_world_normal_.assign(number_of_face, Vec4f());
    }

    // ��ȡtextureָ�������������ɫ Vec3f
    Vec3f getTextureColorInt(int x, int y) {
        COLORREF colorBGR = ptexture_[y * texture_width_ + x];
        COLORREF colorRGB = BGR(colorBGR);
        return Vec3f{static_cast<float>(GetRValue(colorRGB)), static_cast<float>(GetGValue(colorRGB)),
                     static_cast<float>(GetBValue(colorRGB))};
    }

    // ��ȡtextureָ�� 0-1 С���������ɫ
    Vec3f getTextureColor(float x, float y) {
        if (x < 0 || x > 1 || y < 0 || y > 1) {
            return Vec3f{0, 0, 0};
        }
        int x_int = lround((texture_width_ - 1) * x);
        int y_int = lround((texture_height_ - 1) * y);
        return getTextureColorInt(x_int, y_int);
    }

    void refresh() {
        // �� 4 ���任������� obj ����������������Ļ����
        for (int i = 0; i < number_of_vertex_; i++) {
            vertex_world_[i] = vertex_local_[i] * model_;
            vertex_screen_[i] = vecHomogenized(vertex_world_[i] * view_ * proj_) * viewport_;
            vertex_screen_i_[i] = Vec2i{lround(vertex_screen_[i][0]), lround(vertex_screen_[i][1])};
        }
        caculateFaceWorldNormal();
        caculateVertexWorldNormal();
    }


    // ��������ռ���������ķ�����
    void caculateFaceWorldNormal() {
        for (int i = 0; i < number_of_face_; i++) {
            // ��ǰҪ�������� 3 ����������
            Vec3i &vertex_index = face_s_vertex_index_[i];

            Vec4f edge1 = vertex_world_[vertex_index[1]] - vertex_world_[vertex_index[0]];
            Vec4f edge2 = vertex_world_[vertex_index[2]] - vertex_world_[vertex_index[1]];
            face_world_normal_[i] = vecNormalized(crossProduct(edge1, edge2));
//            cout << "face_world_normal_" << face_world_normal_[i] << endl;  //û����
        }
//        cout << "������������: " << endl; //ȷ��û����
    }

    // ��������ռ������ж���ķ������������ȼ�������ķ��������ܵ���
    void caculateVertexWorldNormal() {
        for (int i = 0; i < number_of_vertex_; i++) {
            // ��ǰҪ����Ķ����Ӧ����������
            vector<int> &face_index = vertex_s_face_index_[i];

//            cout << "����"<< i <<"��Ӧ��������";
//            for (int j = 0; j < face_index.size(); j++) {
//                cout << face_index[j] << ' ';       //ȷ��û����
//            }
//            cout << endl;

            Vec4f vertex_norm;
            for (int j = 0; j < face_index.size(); j++) {
//                cout << i << "    face_world_normal_: " << face_world_normal_[j] << endl;
                vertex_norm += face_world_normal_[face_index[j]];       // ��ʼ����ط�������face_index
            }
            vertex_world_normal_[i] = vecNormalized(vertex_norm);
//            cout << i << "   vertex_world_normal_: " << vertex_world_normal_[i] << endl;
        }
    }

public:
    string texture_filename_;
    IMAGE texture_;                     //����ͼ��
    DWORD *ptexture_;                   //ָ������ͼ���ָ��
    int texture_width_;
    int texture_height_;

    int number_of_vertex_;
    int number_of_face_;

    vector <Vec4f> vertex_local_;       //�ֲ��ռ䶥������

    vector <Vec4f> vertex_world_;       ////����ռ䶥�����ꡣ���
    vector <Vec4f> vertex_screen_;       ////��Ļ�ռ䶥�����꣬���� z�����
    vector <Vec2i> vertex_screen_i_;     ////��Ļ�ռ䶥��� 2d ���ꡣ���

    vector <vector<int>> vertex_s_face_index_;   //�����Ӧ����������

    vector <Vec2f> vertex_texture_;      //�����Ӧ���������꣬С����0 ~ 1

    vector <Vec4f> vertex_world_normal_;  ////����ռ䶥��ķ����������


    vector <Vec3i> face_s_vertex_index_;     //�����棬��Ӧ�Ķ���������˳ʱ��
    vector <Vec4f> face_world_normal_;       ////����ռ���ķ����������

    // �ĸ��任����
    Mat44f model_;
    Mat44f view_;
    Mat44f proj_;
    Mat44f viewport_;
};


#endif //CGSTUDY_OBJECT_H
