#include "IsolineTracking.h"
#include<deque>
#include<algorithm>

/*
							   4
					 d·--------------·c
					  |                |
					  |                |
					 8|                |
					  |                |2
					  |                |
					  |                |
					 a·--------------·b
							  1
*/

/*计算直线中是否包含等值点*/
size_t cal_gT(double x, double y, double w) {
	if ((x - w) * (y - w) < 0) {
		return 1;
	}
	return 0;
}

/*计算网格中等值点分布*/
size_t check_gT(double a, double b, double c, double d, double z) {
	size_t x = 0;
	if (a == -9999 || b == -9999 || c == -9999 || d == -9999){//处理缺失值
		return x; 
	}
	x = cal_gT(a, b, z) + 2 * cal_gT(b, c, z) + 4 * cal_gT(c, d, z) + 8 * cal_gT(a, d, z);
	return x;
}

//按比例计算
double cal_xy(double x1, double x2, double z1, double z2, double z) {
	double x = x1 + (z - z1) * (x2 - x1) / (z2 - z1);
	return x;
}

/*计算入口*//*尚待优化*//*并未考虑三条边上均有等值点的情况（实际中暂未出现）*/
void cal_pin(grid & newgrid, double z) {
	//计算入口的时候顺便给出trendType
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

/*计算出口*//*尚待优化*/
void cal_pout(grid & newgrid, double z) {
	if (newgrid.gridType == 15) {
		switch (newgrid.trendType) {
			//顺时针考虑
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

/*生成等值线,返回二维容器*/
void ISOLINETracking::GenerateTracking(std::vector<float>* slice,//网格每个点的数值
	double longitudeStart, double longitudeEnd, double longitudeStep,	//longitudeStart：最左边经度值   longitudeEnd：最右边经度值    longitudeStep：从左到右每格的经度间隔
	double latitudeStart, double latitudeEnd, double latitudeStep,	    //latitudeStart： 最下边经度值   latitudeEnd： 最上边经度值    latitudeStep： 从下到上每格的经度间隔
	double isovalue,//目标等值
	std::vector<std::vector<std::vec2f>>& points)//等值线集合（二维容器）
{
	width_ = (longitudeEnd - longitudeStart) / longitudeStep;
	height_ = (latitudeEnd - latitudeStart) / latitudeStep;

	float currentvalue = isovalue;//当前目标值

	//网格从左到右从下到上初始化
	grid_ = new grid *[height_ - 1];
	std::deque<size_t> grid_i;//当前等值线网格候选队列编号i
	std::deque<size_t> grid_j;//当前等值线网格候选队列编号j
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
				grid_[i][j].isEdge1 = true;//下边界
			if (j == 0)
				grid_[i][j].isEdge8 = true;//左边界
			if (j == width_ - 2)
				grid_[i][j].isEdge2 = true;//右边界
			if (i == height_ - 2)
				grid_[i][j].isEdge4 = true;//上边界

			//记录网格的四个点
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

			//更新网格中等值点分布
			grid_[i][j].gridType = check_gT(grid_[i][j].a.z, grid_[i][j].b.z, grid_[i][j].c.z, grid_[i][j].d.z, currentvalue);
			if (grid_[i][j].gridType == 15) {
				grid_[i][j].gridType15 = 15;
			}

			//将至少包含两个等值点的网格插入grid_i,grid_j
			if (grid_[i][j].gridType != 0) {
				if (grid_[i][j].isEdge1 || grid_[i][j].isEdge2 || grid_[i][j].isEdge4 || grid_[i][j].isEdge8) {
					grid_i.push_front(i);//边界上的网格从头部插入
					grid_j.push_front(j);//边界上的网格从头部插入
				}
				else {
					grid_i.push_back(i);//内部的网格从尾部插入
					grid_j.push_back(j);//内部的网格从尾部插入
				}
			}
		}
	}

	//开始追踪当前目标值的所有等值线
	bool currentG = false;//追踪状态
	size_t newi;//当前网格编号i
	size_t newj;//当前网格编号j
	std::vector<std::vec2f> pl;//当前等值线
	while (!grid_i.empty() && !grid_j.empty()) {
		if (!currentG) {//非追踪状态时 从grid_i,grid_j读出当前网格(从头部取出)
			newi = grid_i.front();
			newj = grid_j.front();

			//若当前网格已读时跳过
			if (grid_[newi][newj].isTraveled) {
				grid_i.pop_front();
				grid_j.pop_front();
				continue;
			}
		}

		//根据gridType及trendType计算pout(起始网格要顺便计算pin)
		if (grid_[newi][newj].gridType != 0) {
			if (grid_[newi][newj].trendType == 0) {
				cal_pin(grid_[newi][newj], currentvalue);
			}
			cal_pout(grid_[newi][newj], currentvalue);
		}

		//标记isTraveled
		grid_[newi][newj].isTraveled = true;

		//根据gridType及trendType找出下一网格(grid_[x][y])并进行追踪（开始追踪再画点）
		size_t x, y;
		switch (grid_[newi][newj].gridType - grid_[newi][newj].trendType) {
		case 1: {//往下
			x = grid_[newi][newj].i - 1; y = grid_[newi][newj].j;
			if (!grid_[newi][newj].isEdge1 && grid_[x][y].gridType != 0 && grid_[x][y].gridType != 3 && grid_[x][y].gridType != 9 && grid_[x][y].gridType != 10 && !grid_[x][y].isTraveled) {//追踪
				grid_[x][y].trendType = 4;
				grid_[x][y].pin = grid_[newi][newj].pout;

				//追踪
				currentG = true;
				//对应pin,pout加入pl
				pl.push_back(vec2f(grid_[newi][newj].pin.x, grid_[newi][newj].pin.y));
				pl.push_back(vec2f(grid_[newi][newj].pout.x, grid_[newi][newj].pout.y));
				if (grid_[newi][newj].gridType15 == 15) {//处理特殊网格
					grid_[newi][newj].gridType = grid_[newi][newj].gridType15 - grid_[newi][newj].gridType;
					grid_[newi][newj].trendType = 0;
					grid_[newi][newj].isTraveled = false;
				}
				else {
					grid_[newi][newj].gridType = 0;
				}
				newi = x; newj = y;//下一网格
			}
			else if (grid_[newi][newj].isEdge1){//开放式出口
				//对应pin,pout加入pl
				pl.push_back(vec2f(grid_[newi][newj].pin.x, grid_[newi][newj].pin.y));
				pl.push_back(vec2f(grid_[newi][newj].pout.x, grid_[newi][newj].pout.y));
				currentG = false;
				points.push_back(pl);//输出等值线
				pl.clear();//清空容器
			}
			else if (grid_[x][y].isTraveled && pl.size() != 0 && x == pl.begin()->x && y == pl.begin()->y){//封闭式出口
				//对应pin,pout加入pl
				pl.push_back(vec2f(grid_[newi][newj].pin.x, grid_[newi][newj].pin.y));
				pl.push_back(vec2f(grid_[newi][newj].pout.x, grid_[newi][newj].pout.y));
				currentG = false;
				points.push_back(pl);//输出等值线
				pl.clear();//清空容器
			}
			else {//无法继续追踪
				currentG = false;
				if (pl.size() != 0){//输出等值线
					points.push_back(pl);
				}
				pl.clear();//清空容器
			}
		}break;
		case 2: {//往右
			x = grid_[newi][newj].i; y = grid_[newi][newj].j + 1;
			if (!grid_[newi][newj].isEdge2 && grid_[x][y].gridType != 0 && grid_[x][y].gridType != 3 && grid_[x][y].gridType != 5 && grid_[x][y].gridType != 6 && !grid_[x][y].isTraveled) {//追踪
				grid_[x][y].trendType = 8;
				grid_[x][y].pin = grid_[newi][newj].pout;

				//追踪
				currentG = true;
				//对应pin,pout加入pl
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
				newi = x; newj = y;//下一网格
			}
			else if (grid_[newi][newj].isEdge2) {//开放式出口
				//对应pin,pout加入pl
				pl.push_back(vec2f(grid_[newi][newj].pin.x, grid_[newi][newj].pin.y));
				pl.push_back(vec2f(grid_[newi][newj].pout.x, grid_[newi][newj].pout.y));
				currentG = false;
				points.push_back(pl);//输出等值线
				pl.clear();//清空容器
			}
			else if (grid_[x][y].isTraveled && pl.size() != 0 && x == pl.begin()->x && y == pl.begin()->y) {//封闭式出口
				//对应pin,pout加入pl
				pl.push_back(vec2f(grid_[newi][newj].pin.x, grid_[newi][newj].pin.y));
				pl.push_back(vec2f(grid_[newi][newj].pout.x, grid_[newi][newj].pout.y));
				currentG = false;
				points.push_back(pl);//输出等值线
				pl.clear();//清空容器
			}
			else {//无法继续追踪
				currentG = false;
				if (pl.size() != 0) {//输出等值线
					points.push_back(pl);
				}
				pl.clear();//清空容器
			}
		}break;
		case 4: {//往上
			x = grid_[newi][newj].i + 1; y = grid_[newi][newj].j;
			if (!grid_[newi][newj].isEdge4 && grid_[x][y].gridType != 0 && grid_[x][y].gridType != 6 && grid_[x][y].gridType != 10 && grid_[x][y].gridType != 12 && !grid_[x][y].isTraveled) {//追踪
				grid_[x][y].trendType = 1;
				grid_[x][y].pin = grid_[newi][newj].pout;

				//追踪
				currentG = true;
				//对应pin,pout加入Pl
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
				newi = x; newj = y;//下一网格
			}
			else if (grid_[newi][newj].isEdge4) {//开放式出口
				//对应pin,pout加入pl
				pl.push_back(vec2f(grid_[newi][newj].pin.x, grid_[newi][newj].pin.y));
				pl.push_back(vec2f(grid_[newi][newj].pout.x, grid_[newi][newj].pout.y));
				currentG = false;
				points.push_back(pl);//输出等值线
				pl.clear();//清空容器
			}
			else if (grid_[x][y].isTraveled && pl.size() != 0 && x == pl.begin()->x && y == pl.begin()->y) {//封闭式出口
				//对应pin,pout加入pl
				pl.push_back(vec2f(grid_[newi][newj].pin.x, grid_[newi][newj].pin.y));
				pl.push_back(vec2f(grid_[newi][newj].pout.x, grid_[newi][newj].pout.y));
				currentG = false;
				points.push_back(pl);//输出等值线
				pl.clear();//清空容器
			}
			else {//无法继续追踪
				currentG = false;
				if (pl.size() != 0) {//输出等值线
					points.push_back(pl);
				}
				pl.clear();//清空容器
			}
		}break;
		case 8: {//往左
			x = grid_[newi][newj].i; y = grid_[newi][newj].j - 1;
			if (!grid_[newi][newj].isEdge8 && grid_[x][y].gridType != 0 && grid_[x][y].gridType != 5 && grid_[x][y].gridType != 9 && grid_[x][y].gridType != 12 && !grid_[x][y].isTraveled) {//追踪
				grid_[x][y].trendType = 2;
				grid_[x][y].pin = grid_[newi][newj].pout;

				//追踪
				currentG = true;
				//对应pin,pout加入pl
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
				newi = x; newj = y;//下一网格
			}
			else if (grid_[newi][newj].isEdge8) {//开放式出口
				//对应pin,pout加入pl
				pl.push_back(vec2f(grid_[newi][newj].pin.x, grid_[newi][newj].pin.y));
				pl.push_back(vec2f(grid_[newi][newj].pout.x, grid_[newi][newj].pout.y));
				currentG = false;
				points.push_back(pl);//输出等值线
				pl.clear();//清空容器
			}
			else if (grid_[x][y].isTraveled && pl.size() != 0 && x == pl.begin()->x && y == pl.begin()->y) {//封闭式出口
				//对应pin,pout加入pl
				pl.push_back(vec2f(grid_[newi][newj].pin.x, grid_[newi][newj].pin.y));
				pl.push_back(vec2f(grid_[newi][newj].pout.x, grid_[newi][newj].pout.y));
				currentG = false;
				points.push_back(pl);//输出等值线
				pl.clear();//清空容器
			}
			else {//无法继续追踪
				currentG = false;
				if (pl.size() != 0) {//输出等值线
					points.push_back(pl);
				}
				pl.clear();//清空容器
			}
		}break;
		default:
			break;
		}
	}
}
