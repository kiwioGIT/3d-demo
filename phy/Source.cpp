#include <iostream>
#include "olcConsoleGameEngine.h"
#include <algorithm>
#include <fstream>
#include <strstream>

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

struct triangle
{
	int p1;
	int p2;
	int p3;
	float dist_to_cam = 0;
	int obj;
};

struct mesh {
	vector<vec3d> inpoints;
	vector<triangle> intriangles;
	float size;
	vec3d pos;

	bool loadobjfile(string file) {

		ifstream f(file);
		if (!f.is_open()) {
			return false;
		}

		while (!f.eof())
		{
			char line[128];
			f.getline(line, 128);

			strstream str;
			str << line;
			
			char j;

			if (line[0] == 'v') {
				vec3d vec;
				str >> j >> vec.posx >> vec.posy >> vec.posz;
				vec.should_draw = true;
				vec.posx *= size;
				vec.posy *= size;
				vec.posz *= size;

				vec.posz += pos.posz;
				vec.posx += pos.posx;
				vec.posy += pos.posy;
				inpoints.push_back(vec);
			}

			if (line[0] == 'f') {
				triangle tri;
				int ctyri;

				/*
				str >> j >> tri.p1 >> tri.p2 >> tri.p3 >> ctyri;
				intriangles.push_back(triangle{ tri.p1 - 1,tri.p2 - 1,tri.p3 - 1 });
				intriangles.push_back(triangle{ tri.p1 - 1,tri.p3 - 1,ctyri - 1 });*/

				str >> j >> tri.p1 >> tri.p2 >> tri.p3 >> ctyri;
				intriangles.push_back(triangle{ tri.p1 - 1,tri.p2 - 1,tri.p3 - 1 });
			}


		}

	}
};


struct quad
{
	
	triangle t1;
	triangle t2;
	void construct(int p1,int p2,int p3,int p4) {
		t1.p1 = p1;
		t1.p2 = p2;
		t1.p3 = p3;

		t2.p1 = p1;
		t2.p2 = p3;
		t2.p3 = p4;
	}

};


class Graph : public olcConsoleGameEngine {
	
	
	bool set_px = false;
	vec3d cubepos;
	float camera_rotation = 0;
	float camera_vertical_rotation = 0;
	vec3d camera_pos = {0,0,0};
	float near_view = 400;
	bool indicate = false;
	vec3d lightnormal = vec3d{0.5,-0.7,1};
	vec3d lightsource = vec3d{ 100,-160,300};
	public: 
		vector<vec3d> points;
		vector<triangle> triangles;
		vector<mesh> meshes;
		Graph()
		{}
		virtual bool OnUserCreate() {

			/*
			points.push_back(vec3d{500,200,100});
			points.push_back(vec3d{ -500,200,100 });
			points.push_back(vec3d{ 0,-200,100 });

			
			triangles.push_back(triangle{0,1,2});
			*/
			/*
			mesh m;
			m.pos = vec3d{ 0,0,100 };
			m.size = 30;
			m.loadobjfile("plane.obj");
			meshes.push_back(m);
			InitializeMesh(0);
			*/



			

			mesh m3;
			m3.pos = vec3d{ 0,-15,110 };
			m3.size = 5;
			m3.loadobjfile("rock.obj");
			meshes.push_back(m3);
			InitializeMesh(0);
			
			

			/*
			InitializeMesh(0);
			InitializeMesh(1);*/
			/*
			for (int i = 0; i < meshes.size(); i++) {
				InitializeMesh(i);
			}*/
			
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
			
			Fill(0, 0, ScreenWidth(), ScreenHeight(), ' ');

			
			
			if (isTriangleIntersecting(0, vec3d{ 0,0,0 }, camera_pos)) {
				DrawTriangle(5,5,10,5,5,10, PIXEL_SOLID, FG_RED);
			}
			if (GetNormal(0).posz > 0) {
				indicate = true;
			}
			else {
				indicate = false;
			}
			//DrawCube(30, cubepos.posz, cubepos.posx, cubepos.posy);
			//DrawCube(30, cubepos.posz, cubepos.posx+30, cubepos.posy);
			draw_triangles();
			updatetriangledist();/*
			vector<COLOUR> c = {FG_DARK_GREY,BG_GREY,BG_WHITE,FG_GREY,FG_WHITE,FG_DARK_GREY,FG_GREY,FG_WHITE,BG_GREY,BG_WHITE,FG_GREY,FG_GREY,FG_WHITE,FG_WHITE};
			vector<PIXEL_TYPE> type = {PIXEL_QUARTER,PIXEL_SOLID,PIXEL_SOLID,PIXEL_QUARTER, PIXEL_QUARTER,PIXEL_HALF,PIXEL_HALF,PIXEL_HALF,PIXEL_HALF,PIXEL_HALF,PIXEL_THREEQUARTERS,PIXEL_SOLID,PIXEL_THREEQUARTERS,PIXEL_SOLID};

			vector<COLOUR> c2 = { BG_DARK_GREY,BG_GREY,BG_WHITE,BG_DARK_GREY,BG_GREY,BG_WHITE,BG_GREY,BG_WHITE };
			vector<PIXEL_TYPE> type2 = { PIXEL_QUARTER,PIXEL_QUARTER, PIXEL_QUARTER,PIXEL_HALF,PIXEL_HALF,PIXEL_HALF,PIXEL_SOLID,PIXEL_SOLID };*/

			return true;
		}

		void DrawPointVec(vec3d pos) {
			vec3d p = get_point_draw_position(pos);
			Draw((int)p.posx + (ScreenWidth() / 2), (int)p.posy + (ScreenHeight() / 2), PIXEL_SOLID, FG_RED);
		}

		void updatetriangledist() {
			for (int i = 0; i < triangles.size(); i++) {
				triangles[i].dist_to_cam = getdistancetotriangle(i);
			}
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

		vec3d GetNormal(int t)
		{
			int p1 = triangles[t].p1;
			int p2 = triangles[t].p2;
			int p3 = triangles[t].p3;
			vec3d vec1;
			vec3d vec2;

			vec1.posx = points[p2].posx - points[p1].posx;
			vec1.posy = points[p2].posy - points[p1].posy;
			vec1.posz = points[p2].posz - points[p1].posz;

			vec2.posx = points[p3].posx - points[p1].posx;
			vec2.posy = points[p3].posy - points[p1].posy;
			vec2.posz = points[p3].posz - points[p1].posz;

			
			return normalize(CrossProduct(vec1, vec2));

			/*
			//3 points of a quad
			float v1[] = { 1,  -1, -1 };
			float v2[] = { 1,  -1,  1 };
			float v3[] = { -1, -1,  1 };
			//float v4[] = {-1, -1, -1};

			Vector[0] = v3[0] - v1[0];
			Vector[1] = v3[1] - v1[1];
			Vector[2] = v3[2] - v1[2];

			Vector2[0] = v2[0] - v1[0];
			Vector2[1] = v2[1] - v1[1];
			Vector2[2] = v2[2] - v1[2];

			float* Normal = CrossProduct(Vector, Vector2);
			//Normal should be 0,-1,0 but turns out as 0,4,0*/
		}

		
		
		vec3d GetPoint(int i) {
			return points[i];
		}

		void draw_triangles() {
			
			sort(triangles.begin(), triangles.end(), [](triangle &t1, triangle &t2)
				{
					/*
					float f1 = ((sqrt(t1.p1value.posx* t1.p1value.posx+ t1.p1value.posy * t1.p1value.posy+ t1.p1value.posz * t1.p1value.posz)) + (sqrt(t1.p2value.posx * t1.p2value.posx + t1.p2value.posy * t1.p2value.posy + t1.p2value.posz * t1.p2value.posz)) + (sqrt(t1.p3value.posx * t1.p3value.posx + t1.p3value.posy * t1.p3value.posy + t1.p3value.posz * t1.p3value.posz))) / 3;
					float f2 = ((sqrt(t2.p1value.posx * t2.p1value.posx + t2.p1value.posy * t2.p1value.posy + t2.p1value.posz * t2.p1value.posz)) + (sqrt(t2.p2value.posx * t2.p2value.posx + t2.p2value.posy * t2.p2value.posy + t2.p2value.posz * t2.p2value.posz)) + (sqrt(t2.p3value.posx * t2.p3value.posx + t2.p3value.posy * t2.p3value.posy + t2.p3value.posz * t2.p3value.posz))) / 3;
					*/
					float f1 = t1.dist_to_cam;
					float f2 = t2.dist_to_cam;
					return f1 > f2;
				});
			
			
			for (int i = 0; i < triangles.size(); i++) {
				
				triangle t = triangles[i];
				vec3d normal = GetNormal(i);
				vec3d camconectnormal = vec3d{camera_pos.posx-points[t.p1].posx,camera_pos.posy - points[t.p1].posy ,camera_pos.posz - points[t.p1].posz };
				
				camconectnormal = normalize(camconectnormal);
				normal = vec3d{ normal.posx,normal.posy,normal.posz};
				float dot = DotProduct(camconectnormal, normal);
				
				if (dot <= 0) {
					continue;
				}
				
				FillTriangleV(points[t.p1], points[t.p2], points[t.p3],i);
				
				//DrawLineVec(points[t.p1], points[t.p2]);
				//std::cout << "1t\n";
				//DrawLineVec(points[t.p1], points[t.p3]);
				//DrawLineVec(points[t.p2], points[t.p3]);
				
				
			}
		}
		void FillTriangleV(vec3d pos1,vec3d pos2,vec3d pos3,int c) {
			bool shade = false;
			
			vec3d normal = GetNormal(c);
			/*
			for (int i = 0; i < triangles.size(); i++) {
				triangle t = triangles[i];
				if (triangles[i].obj == triangles[c].obj) {
					continue;
				}
				bool ints = isTriangleIntersecting(i,pos1,lightsource);
				float dist2light = sqrt(points[t.p1].posx * points[t.p1].posx + points[t.p1].posy * points[t.p1].posy + points[t.p1].posz * points[t.p1].posz);
				

				float dist2light2 = sqrt(pos1.posx * pos1.posx + pos1.posy * pos1.posy + pos1.posz * pos1.posz);

					if (ints == true && dist2light > dist2light2 && i != c && (GetNormal(i).posy != normal.posy || GetNormal(i).posx != normal.posx || GetNormal(i).posz != normal.posz)) {
						shade = true;
					}
			}
			*/



			
			vec3d lp = subtractvec(lightsource,/*gettripos(pos1, pos2, pos3)*/pos1);
			float dot = DotProduct(normalize(lp), normal);






			pos1 = get_point_draw_position(pos1);
			pos2 = get_point_draw_position(pos2);
			pos3 = get_point_draw_position(pos3);
			if (pos1.should_draw && pos2.should_draw && pos3.should_draw) {
				if (!shade) {
					FillTriangle(pos1.posx + ScreenWidth() / 2, pos1.posy + ScreenHeight() / 2, pos2.posx + ScreenWidth() / 2, pos2.posy + ScreenHeight() / 2, pos3.posx + ScreenWidth() / 2, pos3.posy + ScreenHeight() / 2, getcorespondingpxtype(dot), getcorespondingcolor(dot));
				}
				else
				{
					FillTriangle(pos1.posx + ScreenWidth() / 2, pos1.posy + ScreenHeight() / 2, pos2.posx + ScreenWidth() / 2, pos2.posy + ScreenHeight() / 2, pos3.posx + ScreenWidth() / 2, pos3.posy + ScreenHeight() / 2, PIXEL_QUARTER, FG_WHITE);
				}
			}
		}

		PIXEL_TYPE getcorespondingpxtype(float dot) {
			PIXEL_TYPE shade;
			float n = 0.14285714285;
			shade = PIXEL_QUARTER;
			if (dot > -(1 - n)) {
				shade = PIXEL_SOLID;
				if (dot > -(1 - 2 * n)) {
					shade = PIXEL_SOLID;
					if (dot > -(1 - 3 * n)) {
						shade = PIXEL_QUARTER;
						if (dot > -(1 - 4 * n)) {
							shade = PIXEL_QUARTER;
							if (dot > -(1 - 5 * n)) {
								shade = PIXEL_HALF;
								if (dot > -(1 - 6 * n)) {
									shade = PIXEL_HALF;
									if (dot > 0) {
										shade = PIXEL_HALF;
										if (dot > n) {
											shade = PIXEL_HALF;
											if (dot > 2 * n) {
												shade = PIXEL_HALF;
												if (dot > 3 * n) {
													shade = PIXEL_THREEQUARTERS;
													if (dot > 4 * n) {
														shade = PIXEL_SOLID;
														if (dot > 5 * n) {
															shade = PIXEL_THREEQUARTERS;
															if (dot > 6 * n) {
																shade = PIXEL_SOLID;

															}
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
			return shade;
		}


		
		COLOUR getcorespondingcolor(float dot) {
			
			COLOUR shade;
			float n = 0.14285714285;

			shade = FG_DARK_GREY;
			if (dot > -(1-n)) {
				shade = BG_GREY;
				if (dot > -(1 - 2*n)) {
					shade = BG_WHITE;
					if (dot > -(1 - 3*n)) {
						shade = FG_GREY;
						if (dot > -(1 - 4*n)) {
							shade = FG_WHITE;
							if (dot > -(1 - 5*n)) {
								shade = FG_DARK_GREY;
								if (dot > -(1 - 6*n)) {
									shade = FG_GREY;
									if (dot > 0) {
										shade = FG_WHITE;
										if (dot > n) {
											shade = BG_GREY;
											if (dot > 2*n) {
												shade = BG_WHITE;
												if (dot > 3*n) {
													shade = FG_GREY;
													if (dot > 4*n) {
														shade = FG_GREY;
														if (dot > 5*n) {
															shade = FG_WHITE;
															if (dot > 6*n) {
																shade = FG_WHITE;

															}
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
			return shade;
		}

		vec3d CrossProduct(vec3d a, vec3d b)
		{
			vec3d result;

			

			

			result.posx = (a.posy * b.posz) - (a.posz * b.posy);
			result.posy = (a.posz * b.posx) - (a.posx * b.posz);
			result.posz = (a.posx * b.posy) - (a.posy * b.posx);


			return result;
		}

		vec3d normalize(vec3d v) {
			float lenght = sqrt(v.posx * v.posx + v.posy * v.posy + v.posz * v.posz);
			v.posx /= lenght;
			v.posy /= lenght;
			v.posz /= lenght;
			return v;
		}

		float getdistancetotriangle(int tt) {
			triangle t = triangles[tt];
			vec3d p1 = points[t.p1];
			vec3d p2 = points[t.p2];
			vec3d p3 = points[t.p3];

			float dist = sqrt(pow(p1.posx - camera_pos.posx, 2) + pow(p1.posy - camera_pos.posy, 2) + pow(p1.posz - camera_pos.posz, 2));
			float dist2 = sqrt(pow(p2.posx - camera_pos.posx, 2) + pow(p2.posy - camera_pos.posy, 2) + pow(p2.posz - camera_pos.posz, 2));
			float dist3 = sqrt(pow(p3.posx - camera_pos.posx, 2) + pow(p3.posy - camera_pos.posy, 2) + pow(p3.posz - camera_pos.posz, 2));
			dist = (dist + dist2 + dist3)/3;
			return dist;
			
		}

		vec3d gettripos(vec3d p1, vec3d p2, vec3d p3) {
			vec3d result = addvec(p1, p2);
			result = addvec(result,p3);
			return result;
		}

		float DotProduct(vec3d a, vec3d b) {
			return a.posx * b.posx + a.posy * b.posy + a.posz * b.posz;
		}



		void InitializeMesh(int m) {
			mesh meshi = meshes[m];
			int s = points.size();
			for (int i = 0; i < meshi.inpoints.size(); i++) {
				points.push_back(meshi.inpoints[i]);
				
			}
			for (int i = 0; i < meshi.intriangles.size(); i++) {
				triangle t = triangle{ meshi.intriangles[i].p1 + s,meshi.intriangles[i].p2 + s,meshi.intriangles[i].p3 + s };
				t.obj = meshes.size();
				triangles.push_back(t);
				
			}
		}


		bool isTriangleIntersecting(int tt, vec3d qq1, vec3d qq2) {

			vec3d q1 = subtractvec(qq1, qq2);
			q1 = normalize(q1);
			q1 = multiplicatevec(q1, vec3d{ 1000,1000,1000 });
			q1 = addvec(q1, qq1);


			vec3d q2 = subtractvec(qq2, qq1);
			q2 = normalize(q2);
			q2 = multiplicatevec(q2, vec3d{ 1000,1000,1000 });
			q2 = addvec(q2, qq2);


			triangle t = triangles[tt];
			vec3d p1 = points[t.p1];
			vec3d p2 = points[t.p2];
			vec3d p3 = points[t.p3];

			if (GetNaturality(volumeOfTetrahedron(q1, p1, p2, p3)) != GetNaturality(volumeOfTetrahedron(q2, p1, p2, p3))) {
				if (GetNaturality(volumeOfTetrahedron(q1, q2, p1, p2)) == GetNaturality(volumeOfTetrahedron(q1, q2, p2, p3)) && GetNaturality(volumeOfTetrahedron(q1, q2, p2, p3)) == GetNaturality(volumeOfTetrahedron(q1, q2, p3, p1))) {
					return true;
				}
			}
			return false;
		}


		float volumeOfTetrahedron(vec3d a, vec3d b, vec3d c, vec3d d) {
			vec3d la;
			vec3d lb;
			vec3d lc;
			la = vec3d{ 0,0,0 };
			lb = subtractvec(b, a);
			lb = subtractvec(b, a);
			return DotProduct(CrossProduct(subtractvec(b, a), subtractvec(c, a)), subtractvec(d, a));
		}

		float GetNaturality(float f) {
			if (f == 0) {
				return 0;
			}
			return f / abs(f);
		}

		vec3d multiplicatevec(vec3d origin, vec3d v) {
			origin.posx *= v.posx;
			origin.posy *= v.posy;
			origin.posz *= v.posz;
			return origin;
		}

		vec3d subtractvec(vec3d origin,vec3d v) {
			origin.posx -= v.posx;
			origin.posy -= v.posy;
			origin.posz -= v.posz;
			return origin;
		
		}

		vec3d addvec(vec3d origin, vec3d v) {
			origin.posx += v.posx;
			origin.posy += v.posy;
			origin.posz += v.posz;
			return origin;

		}




};





int main() {
	Graph demo;
	demo.ConstructConsole(600,400,2,2);
	demo.Start();
	
	return 0;
}


