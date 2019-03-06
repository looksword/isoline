#ifndef ISOLINETRACKING_H
#define ISOLINETRACKING_H
#include <vector>
#include "ork/math/vec4.h"

struct Point
{
	Point() {
	~Point() {}
	double x;//����
	double y;//γ��
	double z;//Ŀ������
};

struct grid
{
	grid() {}
	~grid() {}
	size_t i, j;//������
	Point a, b, c, d;//������ĸ��㣨�����¿�ʼ ��ʱ�뷽��
	size_t gridType;//�����е�ֵ��ֲ�
	size_t gridType15;//��������ĵ�ֵ��ֲ�
	size_t trendType;//����׷�����
	Point pin, pout;//�����ڵ�ֵ�߳����
	bool isTraveled;//�������״̬
	bool isEdge1;// �����1���ڽ���
	bool isEdge2;// �����2���ڽ���
	bool isEdge4;// �����4���ڽ���
	bool isEdge8;// �����8���ڽ���
};

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

/*��ֵ��׷��*/
class ISOLINETracking
{
public:
	ISOLINETracking(){}
	~ISOLINETracking();

	/*���ɵ�ֵ�߼���points*/
	//���룺
	//slice:����ÿ�������ֵ
	//longitudeStart������߾���ֵ   longitudeEnd�����ұ߾���ֵ    longitudeStep��������ÿ��ľ��ȼ��
	//latitudeStart�� ���±߾���ֵ   latitudeEnd�� ���ϱ߾���ֵ    latitudeStep�� ���µ���ÿ��ľ��ȼ��
	//isovalue��Ŀ���ֵ
	//�����
	//points�� ��ֵ�߼��ϣ���ά������
	void GenerateTracking(std::vector<float>* slice, double longitudeStart, double longitudeEnd, double longitudeStep,
		double latitudeStart, double latitudeEnd, double latitudeStep, double isovalue,
		std::vector<std::vector<std::vec2f>>& points);

private:
	grid **grid_;//������
	size_t width_;//���
	size_t height_;//�߶�

};

#endif // ISOLINETRACKING_H
