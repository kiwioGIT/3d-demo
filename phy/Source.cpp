#include <iostream>
#include "olcConsoleGameEngine.h"
using namespace std;

struct vec3d
{
	float posx;
	float posy;
	float posz;
	bool should_draw = true;
	void rotatedxz(float rotation) {
		float distance = sqrt(posx * posx + posz * posz);
		posx = cos(rotation) * distance;
		posz = sin(rotation) * distance;
	}

	float distance2zero() {
		return sqrt(posx * posx + posz * posz);
	}
	float get_rotation() {
		return acos(posx / sqrt(posx * posx + posz * posz));
	}
};

class Graph : public olcConsoleGameEngine {
	
	vec3d cubepos;
	float camera_rotation = 0;
	float camera_vertical_rotation = 0;
	vec3d camera_pos = {0,0,0};
	float near_view = 400;
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
			if (m_keys['A'].bHeld) {
				camera_pos.posz += cos(camera_rotation - 1.57079633) * 10;
				camera_pos.posx += sin(camera_rotation - 1.57079633) * 10;
			}
			if (m_keys['D'].bHeld) {
				camera_pos.posz += cos(camera_rotation + 1.57079633) * 10;
				camera_pos.posx += sin(camera_rotation + 1.57079633) * 10;
			}
			if (m_keys['W'].bHeld) {
				camera_pos.posz += cos(camera_rotation) * 10;
				camera_pos.posx += sin(camera_rotation) * 10;
			}
			if (m_keys['S'].bHeld) {
				camera_pos.posz += cos(camera_rotation) * -10;
				camera_pos.posx += sin(camera_rotation) * -10;
			}
			if (m_keys['Q'].bHeld) {
				camera_pos.posy -= 10;
			}
			if (m_keys['E'].bHeld) {
				camera_pos.posy -= -10;
			}
			if (m_keys[VK_UP].bHeld) {
				//camera_vertical_rotation += fElapsedTime * -2;
			}
			if (m_keys[VK_DOWN].bHeld) {
				//camera_vertical_rotation += fElapsedTime * 2;
			}
			if (m_keys[VK_LEFT].bHeld) {
				camera_rotation += fElapsedTime * -2;
			}
			if (m_keys[VK_RIGHT].bHeld) {
				camera_rotation += fElapsedTime * 2;
			}
			Fill(0, 0, ScreenWidth(), ScreenHeight(),' ');

			DrawCube(30, cubepos.posz, cubepos.posx, cubepos.posy);
			DrawCube(30, cubepos.posz, cubepos.posx+30, cubepos.posy);

			return true;
		}

		void DrawPointVec(vec3d pos) {
			vec3d p = get_point_draw_position(pos);
			Draw((int)p.posx + (ScreenWidth() / 2), (int)p.posy + (ScreenHeight() / 2), PIXEL_SOLID, FG_RED);
		}


		vec3d get_point_draw_position(vec3d origin) {
			vec3d superigin = origin;
			origin.posx -= camera_pos.posx;
			origin.posy -= camera_pos.posy;
			origin.posz -= camera_pos.posz;
			int negus = (2* (superigin.posz > camera_pos.posz))-1;
			origin.posx *= negus;
			origin.posy *= negus;
			origin.posz *= negus;
			float lon = sqrt(pow(origin.posx, 2) + pow(origin.posz, 2));
			float angle = origin.get_rotation() + camera_rotation;
			
			vec3d draw_test = origin;
			draw_test.rotatedxz(draw_test.get_rotation() + camera_rotation);
			vec3d cam_draw_test = camera_pos;
			cam_draw_test.rotatedxz(draw_test.get_rotation() + camera_rotation);
			if (draw_test.posz * negus < 0) {
				origin.should_draw = false;
				return origin;
			}

			float lon_v = sqrt(pow(origin.posy, 2) + pow(origin.posx, 2) + pow(origin.posz, 2));
			float vertical_angle = atan(origin.posy/ sqrt(pow(origin.posx, 2) + pow(origin.posz, 2)));
			
			vertical_angle += camera_vertical_rotation;
			
			origin.posy = sin(vertical_angle) * lon_v;
			float off_value = cos(vertical_angle) * lon_v;
			origin.posx = cos(angle) * off_value;
			origin.posz = sin(angle) * off_value;
			float scale = origin.posz / near_view;
			origin.posz = origin.posz / scale;
			origin.posx = origin.posx / scale;
			origin.posy = origin.posy / scale;
			return origin;
		}

		float lerp(float a, float b, float f)
		{
			return (a * (1.0 - f)) + (b * f);
		}

		void DrawLineVec(vec3d pos1, vec3d pos2) {
			pos1 = get_point_draw_position(pos1);
			pos2 = get_point_draw_position(pos2);
			if (pos1.should_draw && pos2.should_draw) {
				DrawLine((int)pos1.posx + (ScreenWidth() / 2), (int)pos1.posy + (ScreenHeight() / 2), (int)pos2.posx + (ScreenWidth() / 2), (int)pos2.posy + (ScreenHeight() / 2), PIXEL_SOLID, FG_WHITE);
			}
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
	demo.ConstructConsole(600,400,2,2);
	demo.Start();
	
	return 0;
}


