#include <iostream>
#include "olcConsoleGameEngine.h"
using namespace std;

struct vec3d
{
	float posx;
	float posy;
	float posz;
};

class Graph : public olcConsoleGameEngine {
	
	vec3d cubepos;

	
	float near_view = 200;
	public: 
		Graph()
		{}
		virtual bool OnUserCreate() {
			cubepos.posx = 0;
			cubepos.posy = 0;
			cubepos.posz = 100;

			return true;

		}

		virtual bool OnUserUpdate(float fElapsedTime) {
			if (m_keys[VK_LEFT].bHeld) {
				cubepos.posx += fElapsedTime * -100;
			}
			if (m_keys[VK_RIGHT].bHeld) {
				cubepos.posx += fElapsedTime * 100;
			}
			if (m_keys[VK_UP].bHeld) {
				cubepos.posy += fElapsedTime * -100;
			}
			if (m_keys[VK_DOWN].bHeld) {
				cubepos.posy += fElapsedTime * 100;
			}
			if (m_keys[VK_F12].bHeld) {
				cubepos.posz += fElapsedTime * 100;
			}
			if (m_keys[VK_F11].bHeld) {
				cubepos.posz += fElapsedTime * -100;
			}
			Fill(0, 0, ScreenWidth(), ScreenHeight(),' ');

			DrawCube(30, cubepos.posz, cubepos.posx, cubepos.posy);


			return true;
		}

		void DrawPointVec(vec3d pos) {
			float scale = pos.posz / near_view;
			float new_x_pos = pos.posx/scale;
			float new_y_pos = pos.posy/scale;
			Draw((int)new_x_pos + (ScreenWidth() / 2), (int)new_y_pos + (ScreenHeight() / 2), PIXEL_SOLID, FG_RED);
		}

		void DrawLineVec(vec3d pos1, vec3d pos2) {
			float scale = pos1.posz / near_view;

			float new_x_pos1 = pos1.posx / scale;
			float new_y_pos1 = pos1.posy / scale;

			float scale2 = pos2.posz / near_view;

			float new_x_pos2 = pos2.posx / scale2;
			float new_y_pos2 = pos2.posy / scale2;
			DrawLine((int)new_x_pos1 + (ScreenWidth() / 2), (int)new_y_pos1 + (ScreenHeight() / 2), (int)new_x_pos2 + (ScreenWidth() / 2), (int)new_y_pos2 + (ScreenHeight() / 2), PIXEL_SOLID, FG_WHITE);
		}

		void DrawCube(float cube_size, float obj_dist, float cubex, float cubey) {
			vec3d point;
			vec3d point2;
			vec3d point3;
			vec3d point4;
			vec3d point5;
			vec3d point6;
			vec3d point7;
			vec3d point8;

			point.posx = 0 + cubex;
			point.posz = obj_dist;
			point.posy = 0 + cubey;

			point2.posx = cube_size + cubex;
			point2.posz = obj_dist;
			point2.posy = 0 + cubey;

			point3.posx = 0 + cubex;
			point3.posz = obj_dist;
			point3.posy = cube_size + cubey;

			point4.posx = cube_size + cubex;
			point4.posz = obj_dist;
			point4.posy = cube_size + cubey;

			point5.posx = 0 + cubex;
			point5.posz = obj_dist + cube_size;
			point5.posy = 0 + cubey;

			point6.posx = cube_size + cubex;
			point6.posz = obj_dist + cube_size;
			point6.posy = 0 + cubey;

			point7.posx = 0 + cubex;
			point7.posz = obj_dist + cube_size;
			point7.posy = cube_size + cubey;

			point8.posx = cube_size + cubex;
			point8.posz = obj_dist + cube_size;
			point8.posy = cube_size + cubey;




			//Draw((int)new_x_pos + ScreenWidth() / 2, (int)new_y_pos + ScreenWidth() / 2, FG_RED, FG_RED);
			DrawPointVec(point);
			DrawPointVec(point2);
			DrawPointVec(point3);
			DrawPointVec(point4);
			DrawPointVec(point5);
			DrawPointVec(point6);
			DrawPointVec(point7);
			DrawPointVec(point8);

			DrawLineVec(point, point2);
			DrawLineVec(point, point3);
			DrawLineVec(point3, point4);
			DrawLineVec(point4, point2);

			DrawLineVec(point5, point6);
			DrawLineVec(point5, point7);
			DrawLineVec(point7, point8);
			DrawLineVec(point8, point6);

			DrawLineVec(point, point5);
			DrawLineVec(point2, point6);
			DrawLineVec(point3, point7);
			DrawLineVec(point4, point8);
		}
};

int main() {
	Graph demo;
	demo.ConstructConsole(400,400,2,2);
	demo.Start();
	
	return 0;
}


