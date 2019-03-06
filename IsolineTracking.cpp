#include "IsolineTracking.h"
#include<deque>
#include<algorithm>

/*
							   4
					 d��--------------��c
					  |                |
					  |                |
					 8|                |
					  |                |2
					  |                |
					  |                |
					 a��--------------��b
							  1
*/

/*����ֱ�����Ƿ������ֵ��*/
size_t cal_gT(double x, double y, double w) {
	if ((x - w) * (y - w) < 0) {
		return 1;
	}
	return 0;
}

/*���������е�ֵ��ֲ�*/
size_t check_gT(double a, double b, double c, double d, double z) {
	size_t x = 0;
	if (a == -9999 || b == -9999 || c == -9999 || d == -9999){//����ȱʧֵ
		return x; 
	}
	x = cal_gT(a, b, z) + 2 * cal_gT(b, c, z) + 4 * cal_gT(c, d, z) + 8 * cal_gT(a, d, z);
	return x;
}

//����������
double cal_xy(double x1, double x2, double z1, double z2, double z) {
	double x = x1 + (z - z1) * (x2 - x1) / (z2 - z1);
	return x;
}

/*�������*//*�д��Ż�*//*��δ�����������Ͼ��е�ֵ��������ʵ������δ���֣�*/
void cal_pin(grid & newgrid, double z) {
	//������ڵ�ʱ��˳�����trendType
	switch (newgrid.gridType) {
	case 3: {
		if (newgrid.isEdge2) {
			newgrid.trendType = 2;
			newgrid.pin.x = newgrid.b.x; newgrid.pin.y = cal_xy(newgrid.b.y, newgrid.c.y, newgrid.b.z, newgrid.c.z, z);//bc
		}
		else {
			newgrid.trendType = 1;
			newgrid.pin.y = newgrid.a.y; newgrid.pin.x = cal_xy(newgrid.a.x, newgrid.b.x, newgrid.a.z, newgrid.b.z, z);//ab
		}
	}break;
	case 5: {
		if (newgrid.isEdge4) {
			newgrid.trendType = 4;
			newgrid.pin.y = newgrid.d.y; newgrid.pin.x = cal_xy(newgrid.d.x, newgrid.c.x, newgrid.d.z, newgrid.c.z, z);//dc
		}
		else {
			newgrid.trendType = 1;
			newgrid.pin.y = newgrid.a.y; newgrid.pin.x = cal_xy(newgrid.a.x, newgrid.b.x, newgrid.a.z, newgrid.b.z, z);//ab
		}
	}break;
	case 6: {
		if (newgrid.isEdge4) {
			newgrid.trendType = 4;
			newgrid.pin.y = newgrid.d.y; newgrid.pin.x = cal_xy(newgrid.d.x, newgrid.c.x, newgrid.d.z, newgrid.c.z, z);//dc
		}
		else {
			newgrid.trendType = 2;
			newgrid.pin.x = newgrid.b.x; newgrid.pin.y = cal_xy(newgrid.b.y, newgrid.c.y, newgrid.b.z, newgrid.c.z, z);//bc
		}
	}break;
	case 9: {
		if (newgrid.isEdge8) {
			newgrid.trendType = 8;
			newgrid.pin.x = newgrid.a.x; newgrid.pin.y = cal_xy(newgrid.a.y, newgrid.d.y, newgrid.a.z, newgrid.d.z, z);//ad
		}
		else {
			newgrid.trendType = 1;
			newgrid.pin.y = newgrid.a.y; newgrid.pin.x = cal_xy(newgrid.a.x, newgrid.b.x, newgrid.a.z, newgrid.b.z, z);//ab
		}
	}break;
	case 10: {
		if (newgrid.isEdge8) {
			newgrid.trendType = 8;
			newgrid.pin.x = newgrid.a.x; newgrid.pin.y = cal_xy(newgrid.a.y, newgrid.d.y, newgrid.a.z, newgrid.d.z, z);//ad
		}
		else {
			newgrid.trendType = 2;
			newgrid.pin.x = newgrid.b.x; newgrid.pin.y = cal_xy(newgrid.b.y, newgrid.c.y, newgrid.b.z, newgrid.c.z, z);//bc
		}
	}break;
	case 12: {
		if (newgrid.isEdge8) {
			newgrid.trendType = 8;
			newgrid.pin.x = newgrid.a.x; newgrid.pin.y = cal_xy(newgrid.a.y, newgrid.d.y, newgrid.a.z, newgrid.d.z, z);//ad
		}
		else {
			newgrid.trendType = 4;
			newgrid.pin.y = newgrid.d.y; newgrid.pin.x = cal_xy(newgrid.d.x, newgrid.c.x, newgrid.d.z, newgrid.c.z, z);//dc
		}
	}break;
	case 15: {
		if (newgrid.isEdge8) {
			newgrid.trendType = 8;
			newgrid.pin.x = newgrid.a.x; newgrid.pin.y = cal_xy(newgrid.a.y, newgrid.d.y, newgrid.a.z, newgrid.d.z, z);//ad
		}
		else if (newgrid.isEdge4) {
			newgrid.trendType = 4;
			newgrid.pin.y = newgrid.d.y; newgrid.pin.x = cal_xy(newgrid.d.x, newgrid.c.x, newgrid.d.z, newgrid.c.z, z);//dc
		}
		else if (newgrid.isEdge2) {
			newgrid.trendType = 2;
			newgrid.pin.x = newgrid.b.x; newgrid.pin.y = cal_xy(newgrid.b.y, newgrid.c.y, newgrid.b.z, newgrid.c.z, z);//bc
		}
		else {
			newgrid.trendType = 1;
			newgrid.pin.y = newgrid.a.y; newgrid.pin.x = cal_xy(newgrid.a.x, newgrid.b.x, newgrid.a.z, newgrid.b.z, z);//ab
		}
	}break;
	default:
		break;
	}
}

/*�������*//*�д��Ż�*/
void cal_pout(grid & newgrid, double z) {
	if (newgrid.gridType == 15) {
		switch (newgrid.trendType) {
			//˳ʱ�뿼��
		case 1:newgrid.gridType = 9; break;
		case 2:newgrid.gridType = 3; break;
		case 4:newgrid.gridType = 6; break;
		case 8:newgrid.gridType = 12; break;
		default:
			break;
		}
	}
	switch (newgrid.gridType - newgrid.trendType) {
	case 1:newgrid.pout.y = newgrid.a.y; newgrid.pout.x = cal_xy(newgrid.a.x, newgrid.b.x, newgrid.a.z, newgrid.b.z, z); break;
	case 2:newgrid.pout.x = newgrid.b.x; newgrid.pout.y = cal_xy(newgrid.b.y, newgrid.c.y, newgrid.b.z, newgrid.c.z, z); break;
	case 4:newgrid.pout.y = newgrid.d.y; newgrid.pout.x = cal_xy(newgrid.d.x, newgrid.c.x, newgrid.d.z, newgrid.c.z, z); break;
	case 8:newgrid.pout.x = newgrid.a.x; newgrid.pout.y = cal_xy(newgrid.a.y, newgrid.d.y, newgrid.a.z, newgrid.d.z, z); break;
	default:
		break;
	}
}

ISOLINETracking::~ISOLINETracking()
{
	for (size_t i = 0; i < height_ - 1; i++) {
		delete[]grid_[i];
	}
	delete[]grid_;
	grid_[1];
}

/*���ɵ�ֵ��,���ض�ά����*/
void ISOLINETracking::GenerateTracking(std::vector<float>* slice,//����ÿ�������ֵ
	double longitudeStart, double longitudeEnd, double longitudeStep,	//longitudeStart������߾���ֵ   longitudeEnd�����ұ߾���ֵ    longitudeStep��������ÿ��ľ��ȼ��
	double latitudeStart, double latitudeEnd, double latitudeStep,	    //latitudeStart�� ���±߾���ֵ   latitudeEnd�� ���ϱ߾���ֵ    latitudeStep�� ���µ���ÿ��ľ��ȼ��
	double isovalue,//Ŀ���ֵ
	std::vector<std::vector<std::vec2f>>& points)//��ֵ�߼��ϣ���ά������
{
	width_ = (longitudeEnd - longitudeStart) / longitudeStep;
	height_ = (latitudeEnd - latitudeStart) / latitudeStep;

	float currentvalue = isovalue;//��ǰĿ��ֵ

	//��������Ҵ��µ��ϳ�ʼ��
	grid_ = new grid *[height_ - 1];
	std::deque<size_t> grid_i;//��ǰ��ֵ�������ѡ���б��i
	std::deque<size_t> grid_j;//��ǰ��ֵ�������ѡ���б��j
	for (size_t i = 0; i < height_ - 1; i++) {
		grid_[i] = new grid[width_ - 1];
		for (size_t j = 0; j < width_ - 1; j++) {
			grid_[i][j].i = i;
			grid_[i][j].j = j;
			grid_[i][j].gridType = 0;
			grid_[i][j].trendType = 0;
			grid_[i][j].isTraveled = false;

			grid_[i][j].isEdge1 = false;
			grid_[i][j].isEdge2 = false;
			grid_[i][j].isEdge4 = false;
			grid_[i][j].isEdge8 = false;
			if (i == 0)
				grid_[i][j].isEdge1 = true;//�±߽�
			if (j == 0)
				grid_[i][j].isEdge8 = true;//��߽�
			if (j == width_ - 2)
				grid_[i][j].isEdge2 = true;//�ұ߽�
			if (i == height_ - 2)
				grid_[i][j].isEdge4 = true;//�ϱ߽�

			//��¼������ĸ���
			grid_[i][j].a.x = longitudeStart + j * longitudeStep;
			grid_[i][j].a.y = latitudeStart + i * latitudeStep;
			grid_[i][j].a.z = (*slice)[i * width_ + j];
			grid_[i][j].b.x = longitudeStart + (j + 1) * longitudeStep;
			grid_[i][j].b.y = latitudeStart + i * latitudeStep;
			grid_[i][j].b.z = (*slice)[i * width_ + (j + 1)];
			grid_[i][j].c.x = longitudeStart + (j + 1) * longitudeStep;
			grid_[i][j].c.y = latitudeStart + (i + 1) * latitudeStep;
			grid_[i][j].c.z = (*slice)[(i + 1) * width_ + (j + 1)];
			grid_[i][j].d.x = longitudeStart + j * longitudeStep;
			grid_[i][j].d.y = latitudeStart + (i + 1) * latitudeStep;
			grid_[i][j].d.z = (*slice)[(i + 1) * width_ + j];

			//���������е�ֵ��ֲ�
			grid_[i][j].gridType = check_gT(grid_[i][j].a.z, grid_[i][j].b.z, grid_[i][j].c.z, grid_[i][j].d.z, currentvalue);
			if (grid_[i][j].gridType == 15) {
				grid_[i][j].gridType15 = 15;
			}

			//�����ٰ���������ֵ����������grid_i,grid_j
			if (grid_[i][j].gridType != 0) {
				if (grid_[i][j].isEdge1 || grid_[i][j].isEdge2 || grid_[i][j].isEdge4 || grid_[i][j].isEdge8) {
					grid_i.push_front(i);//�߽��ϵ������ͷ������
					grid_j.push_front(j);//�߽��ϵ������ͷ������
				}
				else {
					grid_i.push_back(i);//�ڲ��������β������
					grid_j.push_back(j);//�ڲ��������β������
				}
			}
		}
	}

	//��ʼ׷�ٵ�ǰĿ��ֵ�����е�ֵ��
	bool currentG = false;//׷��״̬
	size_t newi;//��ǰ������i
	size_t newj;//��ǰ������j
	std::vector<std::vec2f> pl;//��ǰ��ֵ��
	while (!grid_i.empty() && !grid_j.empty()) {
		if (!currentG) {//��׷��״̬ʱ ��grid_i,grid_j������ǰ����(��ͷ��ȡ��)
			newi = grid_i.front();
			newj = grid_j.front();

			//����ǰ�����Ѷ�ʱ����
			if (grid_[newi][newj].isTraveled) {
				grid_i.pop_front();
				grid_j.pop_front();
				continue;
			}
		}

		//����gridType��trendType����pout(��ʼ����Ҫ˳�����pin)
		if (grid_[newi][newj].gridType != 0) {
			if (grid_[newi][newj].trendType == 0) {
				cal_pin(grid_[newi][newj], currentvalue);
			}
			cal_pout(grid_[newi][newj], currentvalue);
		}

		//���isTraveled
		grid_[newi][newj].isTraveled = true;

		//����gridType��trendType�ҳ���һ����(grid_[x][y])������׷�٣���ʼ׷���ٻ��㣩
		size_t x, y;
		switch (grid_[newi][newj].gridType - grid_[newi][newj].trendType) {
		case 1: {//����
			x = grid_[newi][newj].i - 1; y = grid_[newi][newj].j;
			if (!grid_[newi][newj].isEdge1 && grid_[x][y].gridType != 0 && grid_[x][y].gridType != 3 && grid_[x][y].gridType != 9 && grid_[x][y].gridType != 10 && !grid_[x][y].isTraveled) {//׷��
				grid_[x][y].trendType = 4;
				grid_[x][y].pin = grid_[newi][newj].pout;

				//׷��
				currentG = true;
				//��Ӧpin,pout����pl
				pl.push_back(vec2f(grid_[newi][newj].pin.x, grid_[newi][newj].pin.y));
				pl.push_back(vec2f(grid_[newi][newj].pout.x, grid_[newi][newj].pout.y));
				if (grid_[newi][newj].gridType15 == 15) {//������������
					grid_[newi][newj].gridType = grid_[newi][newj].gridType15 - grid_[newi][newj].gridType;
					grid_[newi][newj].trendType = 0;
					grid_[newi][newj].isTraveled = false;
				}
				else {
					grid_[newi][newj].gridType = 0;
				}
				newi = x; newj = y;//��һ����
			}
			else if (grid_[newi][newj].isEdge1){//����ʽ����
				//��Ӧpin,pout����pl
				pl.push_back(vec2f(grid_[newi][newj].pin.x, grid_[newi][newj].pin.y));
				pl.push_back(vec2f(grid_[newi][newj].pout.x, grid_[newi][newj].pout.y));
				currentG = false;
				points.push_back(pl);//�����ֵ��
				pl.clear();//�������
			}
			else if (grid_[x][y].isTraveled && pl.size() != 0 && x == pl.begin()->x && y == pl.begin()->y){//���ʽ����
				//��Ӧpin,pout����pl
				pl.push_back(vec2f(grid_[newi][newj].pin.x, grid_[newi][newj].pin.y));
				pl.push_back(vec2f(grid_[newi][newj].pout.x, grid_[newi][newj].pout.y));
				currentG = false;
				points.push_back(pl);//�����ֵ��
				pl.clear();//�������
			}
			else {//�޷�����׷��
				currentG = false;
				if (pl.size() != 0){//�����ֵ��
					points.push_back(pl);
				}
				pl.clear();//�������
			}
		}break;
		case 2: {//����
			x = grid_[newi][newj].i; y = grid_[newi][newj].j + 1;
			if (!grid_[newi][newj].isEdge2 && grid_[x][y].gridType != 0 && grid_[x][y].gridType != 3 && grid_[x][y].gridType != 5 && grid_[x][y].gridType != 6 && !grid_[x][y].isTraveled) {//׷��
				grid_[x][y].trendType = 8;
				grid_[x][y].pin = grid_[newi][newj].pout;

				//׷��
				currentG = true;
				//��Ӧpin,pout����pl
				pl.push_back(vec2f(grid_[newi][newj].pin.x, grid_[newi][newj].pin.y));
				pl.push_back(vec2f(grid_[newi][newj].pout.x, grid_[newi][newj].pout.y));
				if (grid_[newi][newj].gridType15 == 15) {
					grid_[newi][newj].gridType = grid_[newi][newj].gridType15 - grid_[newi][newj].gridType;
					grid_[newi][newj].trendType = 0;
					grid_[newi][newj].isTraveled = false;
				}
				else {
					grid_[newi][newj].gridType = 0;
				}
				newi = x; newj = y;//��һ����
			}
			else if (grid_[newi][newj].isEdge2) {//����ʽ����
				//��Ӧpin,pout����pl
				pl.push_back(vec2f(grid_[newi][newj].pin.x, grid_[newi][newj].pin.y));
				pl.push_back(vec2f(grid_[newi][newj].pout.x, grid_[newi][newj].pout.y));
				currentG = false;
				points.push_back(pl);//�����ֵ��
				pl.clear();//�������
			}
			else if (grid_[x][y].isTraveled && pl.size() != 0 && x == pl.begin()->x && y == pl.begin()->y) {//���ʽ����
				//��Ӧpin,pout����pl
				pl.push_back(vec2f(grid_[newi][newj].pin.x, grid_[newi][newj].pin.y));
				pl.push_back(vec2f(grid_[newi][newj].pout.x, grid_[newi][newj].pout.y));
				currentG = false;
				points.push_back(pl);//�����ֵ��
				pl.clear();//�������
			}
			else {//�޷�����׷��
				currentG = false;
				if (pl.size() != 0) {//�����ֵ��
					points.push_back(pl);
				}
				pl.clear();//�������
			}
		}break;
		case 4: {//����
			x = grid_[newi][newj].i + 1; y = grid_[newi][newj].j;
			if (!grid_[newi][newj].isEdge4 && grid_[x][y].gridType != 0 && grid_[x][y].gridType != 6 && grid_[x][y].gridType != 10 && grid_[x][y].gridType != 12 && !grid_[x][y].isTraveled) {//׷��
				grid_[x][y].trendType = 1;
				grid_[x][y].pin = grid_[newi][newj].pout;

				//׷��
				currentG = true;
				//��Ӧpin,pout����Pl
				pl.push_back(vec2f(grid_[newi][newj].pin.x, grid_[newi][newj].pin.y));
				pl.push_back(vec2f(grid_[newi][newj].pout.x, grid_[newi][newj].pout.y));
				if (grid_[newi][newj].gridType15 == 15) {
					grid_[newi][newj].gridType = grid_[newi][newj].gridType15 - grid_[newi][newj].gridType;
					grid_[newi][newj].trendType = 0;
					grid_[newi][newj].isTraveled = false;
				}
				else {
					grid_[newi][newj].gridType = 0;
				}
				newi = x; newj = y;//��һ����
			}
			else if (grid_[newi][newj].isEdge4) {//����ʽ����
				//��Ӧpin,pout����pl
				pl.push_back(vec2f(grid_[newi][newj].pin.x, grid_[newi][newj].pin.y));
				pl.push_back(vec2f(grid_[newi][newj].pout.x, grid_[newi][newj].pout.y));
				currentG = false;
				points.push_back(pl);//�����ֵ��
				pl.clear();//�������
			}
			else if (grid_[x][y].isTraveled && pl.size() != 0 && x == pl.begin()->x && y == pl.begin()->y) {//���ʽ����
				//��Ӧpin,pout����pl
				pl.push_back(vec2f(grid_[newi][newj].pin.x, grid_[newi][newj].pin.y));
				pl.push_back(vec2f(grid_[newi][newj].pout.x, grid_[newi][newj].pout.y));
				currentG = false;
				points.push_back(pl);//�����ֵ��
				pl.clear();//�������
			}
			else {//�޷�����׷��
				currentG = false;
				if (pl.size() != 0) {//�����ֵ��
					points.push_back(pl);
				}
				pl.clear();//�������
			}
		}break;
		case 8: {//����
			x = grid_[newi][newj].i; y = grid_[newi][newj].j - 1;
			if (!grid_[newi][newj].isEdge8 && grid_[x][y].gridType != 0 && grid_[x][y].gridType != 5 && grid_[x][y].gridType != 9 && grid_[x][y].gridType != 12 && !grid_[x][y].isTraveled) {//׷��
				grid_[x][y].trendType = 2;
				grid_[x][y].pin = grid_[newi][newj].pout;

				//׷��
				currentG = true;
				//��Ӧpin,pout����pl
				pl.push_back(vec2f(grid_[newi][newj].pin.x, grid_[newi][newj].pin.y));
				pl.push_back(vec2f(grid_[newi][newj].pout.x, grid_[newi][newj].pout.y));
				if (grid_[newi][newj].gridType15 == 15) {
					grid_[newi][newj].gridType = grid_[newi][newj].gridType15 - grid_[newi][newj].gridType;
					grid_[newi][newj].trendType = 0;
					grid_[newi][newj].isTraveled = false;
				}
				else {
					grid_[newi][newj].gridType = 0;
				}
				newi = x; newj = y;//��һ����
			}
			else if (grid_[newi][newj].isEdge8) {//����ʽ����
				//��Ӧpin,pout����pl
				pl.push_back(vec2f(grid_[newi][newj].pin.x, grid_[newi][newj].pin.y));
				pl.push_back(vec2f(grid_[newi][newj].pout.x, grid_[newi][newj].pout.y));
				currentG = false;
				points.push_back(pl);//�����ֵ��
				pl.clear();//�������
			}
			else if (grid_[x][y].isTraveled && pl.size() != 0 && x == pl.begin()->x && y == pl.begin()->y) {//���ʽ����
				//��Ӧpin,pout����pl
				pl.push_back(vec2f(grid_[newi][newj].pin.x, grid_[newi][newj].pin.y));
				pl.push_back(vec2f(grid_[newi][newj].pout.x, grid_[newi][newj].pout.y));
				currentG = false;
				points.push_back(pl);//�����ֵ��
				pl.clear();//�������
			}
			else {//�޷�����׷��
				currentG = false;
				if (pl.size() != 0) {//�����ֵ��
					points.push_back(pl);
				}
				pl.clear();//�������
			}
		}break;
		default:
			break;
		}
	}
}
