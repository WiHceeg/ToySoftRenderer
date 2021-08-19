//
// Created by Administrator on 2021/6/15.
//


#include <graphics.h>        // ����ͼ�ο�ͷ�ļ�
#include <conio.h>

#include <array>
#include <iostream>
#include <ctime>

#include "render_preview3.h"
#include "object.h"

#define PI 3.1415926535897932384626433

using namespace std;

int main() {
	// ����render
	RenderPreview3 rd(640, 480);
	//RenderPreview3 rd(800, 600);
	//RenderPreview3 rd(1280, 720);
	rd.scale_ = { 1, 1, 1 };
	rd.rotate_ = { 0, 0, 0 };
	rd.translation_ = { 0, 0, 0 };

	//    rd.eye_ = {-0.7, 0, 1.5};
	//    rd.eye_ = {3, 2, 1.5};
	rd.eye_ = { 0, 3, 0 };
	rd.at_ = { 0, 0, 0 };
	rd.up_ = { 0, 0, 1 };

	rd.fovy_ = PI * 0.5;
	rd.aspect_ = rd.width_ * 1.0 / rd.height_;
	//rd.zn_ = 1.0;
	//rd.zf_ = 500.0;
	rd.zn_ = 0.1;
	rd.zf_ = 10;


	rd.Ia_ = { 0.1, 0.1, 0.1 };
	//rd.Id_ = { 0.5, 0.5, 0.5 };	
	rd.Id_ = { 1, 1, 1 };
	rd.Is_ = rd.Id_;

	rd.light_dir_ = { 0.3, 0.5, 0.7, 0 };    //�Ӵ˷��������



	// ����object
//    Object oct("../texture.jpg", 6, 8);
//    Object oct("../wallhaven-0q3dwq.jpg", 6, 8);
	Object oct("block.jpg", 6, 8);


	// ����ľֲ�����
	oct.vertex_local_ = { {1,  0,  0,  1},    //0
						 {0,  1,  0,  1},    //1
						 {0,  0,  1,  1},    //2
						 {-1, 0,  0,  1},   //3
						 {0,  -1, 0,  1},   //4
						 {0,  0,  -1, 1} };  //5

	// �������������
	oct.vertex_texture_ = { {0.1, 0.4},  //0
						   {0.9, 0.4},  //1
						   {0.5, 0.1},  //2
						   {0.9, 0.6},  //3
						   {0.1, 0.6},  //4
						   {0.5, 0.9} }; //5

	// �������Ӧ�Ķ�������
	oct.face_s_vertex_index_ = { {2, 0, 1},  //0
								{2, 1, 3},  //1
								{2, 3, 4},  //2
								{2, 4, 0},  //3
								{5, 1, 0},  //4
								{5, 3, 1},  //5
								{5, 4, 3},  //6
								{5, 0, 4} }; //7

	// �����Ӧ��������
	oct.vertex_s_face_index_ = { {0, 3, 4, 7},   //0
								{0, 1, 4, 5},   //1
								{0, 1, 2, 3},   //2
								{1, 2, 5, 6},   //3
								{2, 3, 6, 7},   //4
								{4, 5, 6, 7} };  //5



	rd.objects_.push_back(oct);


	rd.refresh();

	int key_input;
	int i = 0;

	//    int fps = 0;
	//    time_t seconds = time(nullptr);     //�Ӽ�Ԫ��ʼ������
	//    char s_seconds[5];

	bool quit = false;
	bool lock = false;   // �Ƿ����� eye ���ڹ۲��λ�ã�������������߾��벻���ˣ�
	cout << "�� 1 ���� eye ���ڹ۲��λ�ã�������������߾��벻����\n�� 2 ������Ĭ�Ͻ�����\n";
	cout << "�� 3 ʹ���淨������ɫ\n�� 4 ʹ�ò�ֵ�ķ�������ɫ��Ĭ�ϲ�ֵ��\n";
	while (true) {
		if (quit) {
			break;
		}
		if (_kbhit()) {
			key_input = _getch();
			//            cout << "���¼�: " << key_input << endl;
			switch (key_input) {
				// �� 1 ������2 ����
			case 49:    //1
				lock = true;
				break;
			case 50:    //2
				lock = false;
				break;

				// �� 3 �� 4
			case 51:    //3
				rd.b_normal_interpolation_ = false;
				break;
			case 52:    //4
				rd.b_normal_interpolation_ = true;
				break;

			//	// �� 5
			//case 53:
			//	rd.b_specular_ = true;
			//	break;

				// �� ju ������ x ����ת
			case 106:   //j
				rd.rotate_.x() -= PI / 180;
				break;
			case 117:   //u
				rd.rotate_.x() += PI / 180;
				break;

				// �� ki ������ y ����ת
			case 107:   //k
				rd.rotate_.y() -= PI / 180;
				break;
			case 105:   //i
				rd.rotate_.y() += PI / 180;
				break;


				// ��lo������ z ����ת
			case 108:   //l
				rd.rotate_.z() -= PI / 180;
				break;
			case 111:   //o
				rd.rotate_.z() += PI / 180;
				break;

				// �� qe��wasd ���������λ��
			case 113:   //q
				rd.eye_.z() -= 0.1;
				if (!lock) {
					rd.at_.z() -= 0.1;
				}
				break;
			case 101:   //e
				rd.eye_.z() += 0.1;
				if (!lock) {
					rd.at_.z() += 0.1;
				}
				break;
			case 119:   //w
				rd.eye_.y() += 0.1;
				if (!lock) {
					rd.at_.y() += 0.1;
				}
				break;
			case 115:   //s
				rd.eye_.y() -= 0.1;
				if (!lock) {
					rd.at_.y() -= 0.1;
				}
				break;
			case 97:    //a
				rd.eye_.x() -= 0.1;
				if (!lock) {
					rd.at_.x() -= 0.1;
				}
				break;
			case 100:   //d
				rd.eye_.x() += 0.1;
				if (!lock) {
					rd.at_.x() += 0.1;
				}
				break;
			case 27:    //esc
				quit = true;
				break;
			}
		}
		rd.refresh();
		//        cout << time(NULL);

	}

	//    _getch();                // �����������
	closegraph();            // �رջ�ͼ����
	return 0;
}