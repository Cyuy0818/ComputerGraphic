#pragma once
#define SHOW_CONSOLE
#include<iostream>
#include<graphics.h>
#include"Display.h"
#include<vector>
using namespace std;
class MyBezier {
public:
   
    MyBezier() :controlPoints_(), color_(0) {};
    MyBezier(vector<pair<double, double>> controlPoints) : controlPoints_(controlPoints) {}

    // ���㱴���������ϵ������
    pair<double, double> calculatePoint(double t);
    void Draws();

private:
    vector<pair<double, double>> controlPoints_; // ���������ߵĿ��Ƶ�
    // �������ʽϵ��
    int binomialCoefficient(int n, int k);
    int color_;
    friend class Display<MyBezier>;
    friend class MyShapeWrapper;
};

