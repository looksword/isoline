#ifndef ISOLINETRACKING_H
#define ISOLINETRACKING_H
#include <vector>
#include "ork/math/vec4.h"

struct Point
{
	Point() {
	~Point() {}
	double x;//经度
	double y;//纬度
	double z;//目标数据
};

struct grid
{
	grid() {}
	~grid() {}
	size_t i, j;//网格编号
	Point a, b, c, d;//网格的四个点（从左下开始 逆时针方向）
	size_t gridType;//网格中等值点分布
	size_t gridType15;//处理特殊的等值点分布
	size_t trendType;//网格追踪入口
	Point pin, pout;//网格内等值线出入口
	bool isTraveled;//网格访问状态
	bool isEdge1;// 网格的1边在界上
	bool isEdge2;// 网格的2边在界上
	bool isEdge4;// 网格的4边在界上
	bool isEdge8;// 网格的8边在界上
};

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

/*等值线追踪*/
class ISOLINETracking
{
public:
	ISOLINETracking(){}
	~ISOLINETracking();

	/*生成等值线集合points*/
	//输入：
	//slice:网格每个点的数值
	//longitudeStart：最左边经度值   longitudeEnd：最右边经度值    longitudeStep：从左到右每格的经度间隔
	//latitudeStart： 最下边经度值   latitudeEnd： 最上边经度值    latitudeStep： 从下到上每格的经度间隔
	//isovalue：目标等值
	//输出：
	//points： 等值线集合（二维容器）
	void GenerateTracking(std::vector<float>* slice, double longitudeStart, double longitudeEnd, double longitudeStep,
		double latitudeStart, double latitudeEnd, double latitudeStep, double isovalue,
		std::vector<std::vector<std::vec2f>>& points);

private:
	grid **grid_;//网格网
	size_t width_;//宽度
	size_t height_;//高度

};

#endif // ISOLINETRACKING_H
