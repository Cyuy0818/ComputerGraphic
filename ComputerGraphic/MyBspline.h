#pragma once
#define SHOW_CONSOLE
#include<graphics.h>
#include"ApplicationRoot.h"
#include<vector>
#include <iostream>

class MyBspline : public ObjWrapper {
public:
    MyBspline(const std::vector<std::pair<double, double>>& controlPoints, const std::vector<double>& knots, int degree, int color)
        : controlPoints_(controlPoints), knots_(knots), degree_(degree), color_(color) {}

    std::pair<double, double> calculateBsplinePoint(double t);
    // �������п��Ƶ㣬�ж�������ĵ��Ƿ��ڿ��Ƶ㸽��
    bool isMouseNearControlPoint(int mouseX, int mouseY) {
        for (const auto& point : controlPoints_) {
            double distance = std::sqrt(std::pow(point.first - mouseX, 2) + std::pow(point.second - mouseY, 2));
            if (distance < 10) {
                std::cout << "������ĵ��ڿ��Ƶ㸽��" << std::endl;
                return true; // ��������ڿ��Ƶ㸽��
            }
        }
        std::cout << "������ĵ㲻�ڿ��Ƶ㸽��" << std::endl;
        return false; // ������㲻�ڿ��Ƶ㸽��
    }

    // �ڴ�����갴���¼�ʱ���øú������ж�������ĵ��Ƿ��ڿ��Ƶ㸽��
    void handleMouseDown(int mouseX, int mouseY) {
        if (isMouseNearControlPoint(mouseX, mouseY)) {
            bool isSelected = true;
            while (isSelected) {
                MOUSEMSG msgtemp = GetMouseMsg();
                std::cout << "������Ƶ�" << std::endl;
                if (msgtemp.uMsg == WM_LBUTTONUP) {

                    double x = msgtemp.x;
                    double y = msgtemp.y;
                    // �������п��Ƶ㣬�ҵ�����Ŀ��Ƶ㲢�������Ϊ����°��µĵ�
                    for (auto& point : controlPoints_) {
                        double distance = std::sqrt(std::pow(point.first - mouseX, 2) + std::pow(point.second - mouseY, 2));
                        if (distance < 10) {
                            // ���¿��Ƶ��λ��
                            point.first = x;
                            point.second = y;
                            std::cout << point.first << " " << point.second << std::endl;
                            break;
                        }
                    }
                    ObjWrapper::Points.clear();
                    plan();
                    isSelected = false; // �˳�ѭ��
                }
            }
        }
    }
    std::vector<Pixel> Draws();
    void plan();
private:
    std::vector<std::pair<double, double>> controlPoints_;
    std::vector<double> knots_;
    int degree_;
    int color_;
    double basisFunction(int i, int k, double t);
};

