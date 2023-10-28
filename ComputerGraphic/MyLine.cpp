#include"ApplicationRoot.h"
#include"MyLine.h"
#include<vector>

std::vector<Pixel> MyLine::BresenhamLine(int x1, int y1, int x2, int y2, int color, int skipLength, int wide) {
    std::vector<Pixel> buffer;
    int dx = abs(x2 - x1), sx = x1 < x2 ? 1 : -1;
    int dy = abs(y2 - y1), sy = y1 < y2 ? 1 : -1;
    int erro = (dx > dy ? dx : -dy) / 2;
    int blankCount = 0;

    // �����ߵĳ�ʼ�ͽ����������
    int startWide = -wide / 2;
    int endWide = wide / 2;

    while (x1 != x2 || y1 != y2) {
        if (blankCount < skipLength) {
            blankCount++;
        }
        else {
            // �ڿ�ȷ�Χ�ڻ���
            for (int i = startWide; i <= endWide; i++) {
                buffer.push_back(Pixel(x1, y1 + i, color));
            }
            blankCount = 0;
        }

        int e2 = erro;
        if (e2 > -dx) { erro -= dy; x1 += sx; }
        if (e2 < dy) { erro += dx; y1 += sy; }
    }

    // �����ߵ�ĩβ
    for (int i = startWide; i <= endWide; i++) {
        buffer.push_back(Pixel(x2, y2 + i, color));
    }
    return buffer;
}
std::vector<Pixel> MyLine::MidpointLine(int x0, int y0, int x1, int y1, int color, int skipLength, int lineWidth) {
    std::vector<Pixel> buffer;
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = x0 < x1 ? 1 : -1;
    int sy = y0 < y1 ? 1 : -1;
    int err = dx - dy;
    int x = x0;
    int y = y0;

    buffer.push_back(Pixel(x, y, color));

    while (x != x1 || y != y1) {
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x += sx;
        }
        if (e2 < dx) {
            err += dx;
            y += sy;
        }
        for (int i = 0; i < skipLength; ++i) {
            if ((x != x1 || y != y1)) {
                e2 = 2 * err;
                if (e2 > -dy) {
                    err -= dy;
                    x += sx;
                }
                if (e2 < dx) {
                    err += dx;
                    y += sy;
                }
            }
        }
        for (int i = -lineWidth / 2; i <= lineWidth / 2; ++i) {
            buffer.push_back(Pixel(x, y + i, color));
        }
    }
    return buffer;
}


void MyLine::CS_clip(int xMin, int yMin, int xMax, int yMax) {

    // �����߶����˵��������
    int code1 = computeCode(x1_, y1_, xMin, yMin, xMax, yMax);
    int code2 = computeCode(x2_, y2_, xMin, yMin, xMax, yMax);

    // ѭ��ֱ���ü����
    bool accept = false;
    while (true) {
        if (!(code1 | code2)) {
            // �߶���ȫ�ڲü�������
            accept = true;
            break;
        }
        else if (code1 & code2) {
    
            break;
        }
        else {
            // �߶���ü������ཻ������ü���
            int x, y;

            // ѡ�������벻Ϊ0�ĵ�
            int codeOut = code1 ? code1 : code2;

            // �����������ҵ����������
            if (codeOut & TOP) {
                x = x1_ + (x2_ - x1_) * (yMax - y1_) / (y2_ - y1_);
                y = yMax;
            }
            else if (codeOut & BOTTOM) {
                x = x1_ + (x2_ - x1_) * (yMin - y1_) / (y2_ - y1_);
                y = yMin;
            }
            else if (codeOut & RIGHT) {
                y = y1_ + (y2_ - y1_) * (xMax - x1_) / (x2_ - x1_);
                x = xMax;
            }
            else if (codeOut & LEFT) {
                y = y1_ + (y2_ - y1_) * (xMin - x1_) / (x2_ - x1_);
                x = xMin;
            }

            // �������滻�������벻Ϊ0�ĵ�
            if (codeOut == code1) {
                x1_ = x;
                y1_ = y;
                code1 = computeCode(x1_, y1_, xMin, yMin, xMax, yMax);
            }
            else {
                x2_ = x;
                y2_ = y;
                code2 = computeCode(x2_, y2_, xMin, yMin, xMax, yMax);
            }
        }
    }

    // ����߶��ڲü������ڣ������µ��߶�����
    if (accept) {
        Points = BresenhamLine(x1_, y1_, x2_, y2_, color_, skipLength_, wide_);
    }
    else {
        // �߶α���ȫ�ü������������ Points
        Points.clear();
    }
}

void MyLine::plan() {
    if(flag_==1)
        ObjWrapper::Points = MidpointLine(x1_, y1_, x2_, y2_, LIGHTBLUE, skipLength_, wide_);
    else if (flag_ == 2)
        ObjWrapper::Points = BresenhamLine(x1_, y1_, x2_, y2_, color_, skipLength_, wide_);
}

int MyLine::computeCode(int x, int y, int xMin, int yMin, int xMax, int yMax) {
    int code = INSIDE;
    if (x < xMin) {
        code |= LEFT;
    }
    else if (x > xMax) {
        code |= RIGHT;
    }
    if (y < yMin) {
        code |= BOTTOM;
    }
    else if (y > yMax) {
        code |= TOP;
    }
    return code;
}


void MyLine::Mid_clip(int x1, int y1, int x2, int y2, int xmin, int ymin, int xmax, int ymax) {
    std::vector<Pixel> buffer;
    buffer.push_back(Pixel(x1, y1, color_)); // ����ʼ�����buffer

    while (!buffer.empty()) {
        Pixel current = buffer.back();
        buffer.pop_back();

        int midX = (current.x + x2) / 2;
        int midY = (current.y + y2) / 2;

        // �ж��е��Ƿ��ڴ�����
        if (midX >= xmin && midX <= xmax && midY >= ymin && midY <= ymax) {
            // ����е��ڴ����ڣ����е����buffer
            buffer.push_back(Pixel(midX, midY, color_));
        }

        // �жϵ�ǰ���Ƿ��ڴ�����
        if (current.x >= xmin && current.x <= xmax && current.y >= ymin && current.y <= ymax) {
            // �����ǰ���ڴ����ڣ�����ObjWrapper::Points
            ObjWrapper::Points.push_back(Pixel(current.x, current.y, color_));
        }

        // �ж���һ�����Ƿ��ڴ�����
        if (x2 >= xmin && x2 <= xmax && y2 >= ymin && y2 <= ymax) {
            // �����һ�����ڴ����ڣ�����ObjWrapper::Points���˳�ѭ��
            ObjWrapper::Points.push_back(Pixel(x2, y2, color_));
            break;
        }

        // ������һ����Ϊ�е�
        x2 = midX;
        y2 = midY;
    }
}
