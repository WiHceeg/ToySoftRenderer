//
// Created by Administrator on 2021/6/11.
//

#ifndef CGSTUDY_RENDER_PREVIEW3_H
#define CGSTUDY_RENDER_PREVIEW3_H

#include "geometry.h"
#include "transformation.h"
#include "object.h"

#include <graphics.h>        // 引用图形库头文件
#include <conio.h>
#include <vector>
#include <algorithm>

#include <string>
#include <cstring>
#include <ctime>

// 计算 p 在 pts 三角形的重心坐标
Vec3f barycentricCoordinates(const vector <Vec2i> &pts, const Vec2i &p) {
    Vec2f a = vecToFloat(pts[0]);
    Vec2f b = vecToFloat(pts[1]);
    Vec2f c = vecToFloat(pts[2]);
    float gamma = ((a.y() - b.y()) * p.x() + (b.x() - a.x()) * p.y() + a.x() * b.y() - b.x() * a.y())
                  / ((a.y() - b.y()) * c.x() + (b.x() - a.x()) * c.y() + a.x() * b.y() - b.x() * a.y());
    float beta = ((a.y() - c.y()) * p.x() + (c.x() - a.x()) * p.y() + a.x() * c.y() - c.x() * a.y())
                 / ((a.y() - c.y()) * b.x() + (c.x() - a.x()) * b.y() + a.x() * c.y() - c.x() * a.y());
    float alpha = 1 - beta - gamma;
    return Vec3f({alpha, beta, gamma});
}

// 用重心坐标透视校正插值得到Zt
float perspectiveCorrectInterpolationZt(const Vec3f &barycentric_coordinates, const float &ZA, const float &ZB,
                                        const float &ZC) {
    return 1 / (barycentric_coordinates[0] / ZA + barycentric_coordinates[1] / ZB + barycentric_coordinates[2] / ZC);
}

// 用重心坐标透视校正插值得到任意属性，例如法向量、纹理坐标、viewspace坐标
template<typename T>
T perspectiveCorrectInterpolationIt(const Vec3f &barycentric_coordinates,
                                    const float &ZA, const float &ZB, const float &ZC, const float &Zt,
                                    const T &IA, const T &IB, const T &IC) {
    return (barycentric_coordinates[0] * IA / ZA + barycentric_coordinates[1] * IB / ZB +
            barycentric_coordinates[2] * IC / ZC) * Zt;
}


class RenderPreview3 {
public:

    vector<vector<float>> zbuffer_;
    vector<vector<float>> zbuffer_origin_;

    Vec3f Ia_;
    Vec3f Id_;
    Vec3f Is_;

    Vec3f p_;

    Vec4f light_dir_;

    // model
    Vec3f scale_;
    Vec3f rotate_;
    Vec3f translation_;

    // veiw
    Vec3f eye_;
    Vec3f at_;
    Vec3f up_;

    // proj
    float fovy_;
    float aspect_;
    float zn_;
    float zf_;

    // viewport
    int width_;
    int height_;

    // 四个变换矩阵
    Mat44f model_;
    Mat44f view_;
    Mat44f proj_;
    Mat44f viewport_;


    vector<Object> objects_;

    bool b_normal_interpolation_ = false;   //法向量是否用顶点法向量插值得到
    bool b_specular_ = false;

    string fps_;
    int frames_in_a_second_ = 0;
    time_t second_ = time(nullptr);     //从纪元开始的秒数


    RenderPreview3(int width, int height) : width_(width), height_(height) {
        initgraph(width_, height_);    // 创建绘图窗口
        zbuffer_.assign(width, vector<float>(height, 2));
        zbuffer_origin_.assign(width, vector<float>(height, 2));
	}



    // 画 obj 的第 i 个三角形面
    void drawTexuturedTriangle(Object &obj, const int &i) {
        // 先用面的法向量判断要不要画，在正面才画
        Vec4f &face_world_normal_i = obj.face_world_normal_[i];
        if (dotProduct(vecDimensionReducedDir(face_world_normal_i), eye_) > 0) {
            // 此面对应顶点的索引
            Vec3i &face_s_vertex_index_i = obj.face_s_vertex_index_[i];

            // 此面对应顶点在屏幕空间的 2d 整数坐标，三角形 ABC 顺时针
            Vec2i &A2i = obj.vertex_screen_i_.at(face_s_vertex_index_i[0]);
            Vec2i &B2i = obj.vertex_screen_i_.at(face_s_vertex_index_i[1]);
            Vec2i &C2i = obj.vertex_screen_i_.at(face_s_vertex_index_i[2]);

            // 此面对应顶点在屏幕空间的 3d 4f坐标，三角形 ABC 顺时针
            Vec4f &A4f = obj.vertex_screen_.at(face_s_vertex_index_i[0]);
            Vec4f &B4f = obj.vertex_screen_.at(face_s_vertex_index_i[1]);
            Vec4f &C4f = obj.vertex_screen_.at(face_s_vertex_index_i[2]);

            // 此面对应顶点在世界空间的 3d 4f法向量，三角形 ABC 顺时针
            Vec4f &A_world_norm_4f = obj.vertex_world_normal_.at(face_s_vertex_index_i[0]);
            Vec4f &B_world_norm_4f = obj.vertex_world_normal_.at(face_s_vertex_index_i[1]);
            Vec4f &C_world_norm_4f = obj.vertex_world_normal_.at(face_s_vertex_index_i[2]);

//            cout << "A_world_norm_4f" << A_world_norm_4f << endl; //没问题，是会变
//            cout << "B_world_norm_4f" << B_world_norm_4f << endl; //这三个结果都一样，很怪
//            cout << "C_world_norm_4f" << C_world_norm_4f << endl; //

            // 此面对应顶点的 2d 2f纹理坐标， 三角形 ABC 顺时针
            Vec2f &A_texture_2f = obj.vertex_texture_.at(face_s_vertex_index_i[0]);
            Vec2f &B_texture_2f = obj.vertex_texture_.at(face_s_vertex_index_i[1]);
            Vec2f &C_texture_2f = obj.vertex_texture_.at(face_s_vertex_index_i[2]);


            Vec2i bboxmin;
            Vec2i bboxmax;
            bboxmin.x() = max(0, min<int>({A2i.x(), B2i.x(), C2i.x()}));
            bboxmin.y() = max(0, min<int>({A2i.y(), B2i.y(), C2i.y()}));
            bboxmax.x() = min(width_ - 1, max<int>({A2i.x(), B2i.x(), C2i.x()}));
            bboxmax.y() = min(height_ - 1, max<int>({A2i.y(), B2i.y(), C2i.y()}));

            Vec2i P;    // 三角形 ABC 所在 bbox 里面的像素 P
            for (P.x() = bboxmin.x(); P.x() <= bboxmax.x(); P.x()++) {
                for (P.y() = bboxmin.y(); P.y() <= bboxmax.y(); P.y()++) {
                    // P 在三角形 ABC 的重心坐标
                    Vec3f bc = barycentricCoordinates({A2i, B2i, C2i}, P);
//                cout  << "Pcoordinate: " << P << endl;
                    if (bc.x() >= 0 && bc.y() >= 0 && bc.z() >= 0) {
                        // P 透视校正插值得到的真实 Zt
                        float zt = perspectiveCorrectInterpolationZt(bc, A4f.z(), B4f.z(), C4f.z());
//                        cout << "zt: " << zt << endl;

                        if (zt < zbuffer_[P.x()][P.y()]) {
                            zbuffer_[P.x()][P.y()] = zt;

                            // 插值得到 P 的纹理坐标（小数0-1）
                            Vec2f texture_coordinate_f = perspectiveCorrectInterpolationIt(bc,
                                                                                           A4f.z(), B4f.z(), C4f.z(),
                                                                                           zt,
                                                                                           A_texture_2f, B_texture_2f, C_texture_2f);
                            // 获取 P 的颜色
                            Vec3f P_texture_color_origin_f = obj.getTextureColor(texture_coordinate_f.x(), texture_coordinate_f.y());

//                        cout  << "origincolor: " << coordinate_texture_color_origin_f << endl;

                            Vec3i result_color;

                            if (b_specular_) {
                                //// TODO，生成高光
                                //Vec4f P_world_norm_4f = perspectiveCorrectInterpolationIt(bc,
                                //    A4f.z(), B4f.z(), C4f.z(),
                                //    zt,
                                //    A_world_norm_4f, B_world_norm_4f, C_world_norm_4f);
                                ////                            cout << "P_world_norm_4f" << P_world_norm_4f << endl;
                                //result_color = phongShadingWithSpecular(P_texture_color_origin_f, P_texture_color_origin_f, vecNormalized(P_world_norm_4f));

                            }
                            else {
                                // 不生成高光
                                if (b_normal_interpolation_) {
                                    // 插值得到世界空间中 P 的法向量
                                    Vec4f P_world_norm_4f = perspectiveCorrectInterpolationIt(bc,
                                        A4f.z(), B4f.z(), C4f.z(),
                                        zt,
                                        A_world_norm_4f, B_world_norm_4f, C_world_norm_4f);
                                    //                            cout << "P_world_norm_4f" << P_world_norm_4f << endl;
                                    result_color = phongShadingWithoutSpecular(P_texture_color_origin_f, P_texture_color_origin_f, vecNormalized(P_world_norm_4f));
                                }
                                else {
                                    result_color = gouraudShading(P_texture_color_origin_f, P_texture_color_origin_f, obj.face_world_normal_[i]);
                                }
                                //                        cout  << "coordinate: " << coordinate << endl << "result_color: "<< result_color << endl;
                            }
                            putpixel(P.x(), P.y(), RGB(result_color[0], result_color[1], result_color[2]));
                        }

                    }
                }
            }

        }
    }

    void refresh() {

        zbuffer_ = zbuffer_origin_;

        model_ = getMatModel<float>(scale_, rotate_, translation_);
        view_ = getMatLookAtLH<float>(eye_, at_, up_);
        proj_ = getMatProjection<float>(fovy_, aspect_, zn_, zf_);
        viewport_ = getMatViewport<float>(static_cast<float>(width_), static_cast<float>(height_));

        BeginBatchDraw();   // 这个函数用于开始批量绘图。执行后，任何绘图操作都将暂时不输出到绘图窗口上，直到执行 FlushBatchDraw 或 EndBatchDraw 才将之前的绘图输出。
        cleardevice();      // 这个函数使用当前背景色清空绘图设备。

        for (Object &obj: objects_) {
            obj.model_ = model_;
            obj.view_ = view_;
            obj.proj_ = proj_;
            obj.viewport_ = viewport_;
            obj.refresh();
            for (int i = 0; i < obj.number_of_face_; i++) {
                drawTexuturedTriangle(obj, i);
            }
        }

        frames_in_a_second_++;

        if (second_ != time(nullptr)) {
            second_ = time(nullptr);
            fps_ = to_string(frames_in_a_second_);
            frames_in_a_second_ = 0;

        }
        outtextxy(10, 10, fps_.c_str());    // 这个函数用于在指定位置输出字符串。

        EndBatchDraw();     // 这个函数用于结束批量绘制，并执行未完成的绘制任务。
    }

    // Ia_, Id_ , light_dir_是类成员变量
    Vec3i gouraudShading(const Vec3f &ka, const Vec3f &kd, const Vec4f &n) {
        Vec3f La = componentWiseProduct(ka, Ia_);
        //         Eigen::Vector3f Ld = kd.cwiseProduct(light.intensity / point2light.dot(point2light)) * std::max(0.0f, normal.normalized().dot(point2light.normalized()));
//        cout << "light_dir_: " << light_dir_ << endl;
//        cout << "n: " << n << endl;
//        cout << "dotProduct(vecNormalized(n), vecNormalized(light_dir_))" << dotProduct(vecNormalized(n), vecNormalized(light_dir_)) << endl;
        Vec3f Ld = componentWiseProduct(kd, Id_) * max(0.0f, dotProduct((n), vecNormalized(light_dir_)));
//        cout << "La: " << La << endl << "Ld: " << Ld << endl;
        Vec3i result_color =  vecToInt(La + Ld);
        for (int i = 0; i < 3; i++) {
            if (result_color[i] > 255) {
                result_color[i] = 255;
            }
        }
        return result_color;
    }

    
    Vec3i phongShadingWithoutSpecular(const Vec3f& ka, const Vec3f& kd, const Vec4f& n) {
        return gouraudShading(ka, kd, n);
    }

    // TODO
//    Vec3i phongShadingWithSpecular(const Vec3f& ka, const Vec3f& kd, const Vec4f& n) {
//        Vec3f La = componentWiseProduct(ka, Ia_);
//        //         Eigen::Vector3f Ld = kd.cwiseProduct(light.intensity / point2light.dot(point2light)) * std::max(0.0f, normal.normalized().dot(point2light.normalized()));
////        cout << "light_dir_: " << light_dir_ << endl;
////        cout << "n: " << n << endl;
////        cout << "dotProduct(vecNormalized(n), vecNormalized(light_dir_))" << dotProduct(vecNormalized(n), vecNormalized(light_dir_)) << endl;
//        Vec3f Ld = componentWiseProduct(kd, Id_) * max(0.0f, dotProduct((n), vecNormalized(light_dir_)));
//        //        cout << "La: " << La << endl << "Ld: " << Ld << endl;
//        
//        Vec3f Ls = 
//        Vec3i result_color = vecToInt(La + Ld);
//        for (int i = 0; i < 3; i++) {
//            if (result_color[i] > 255) {
//                result_color[i] = 255;
//            }
//        }
//        return result_color;
//    }
//
//
};

#endif //CGSTUDY_RENDER_PREVIEW3_H
