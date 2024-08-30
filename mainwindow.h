﻿#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>

namespace {
	/* 这里我们将一个窗口划分为9个区域，分别为
 左上角（1, 1）、中上（1，2）、右上角（1, 3）
 左中  （2, 1）、 中间（2, 2）、右中  （2, 3）
 左下角（3, 1）、中下（3，2）、 右下角（3, 3）*/
	const int kMouseRegionLeft = 5;

	const int kMouseRegionTop = 5;

	const int kMouseRegionRight = 5;

	const int kMouseRegionBottom = 5;

}

// 鼠标的 活动范围的 枚举
enum MousePosition

{
	// 这个是上面图片划分的区域 1,1 区 就用 11 代表 , 1,2 就用12 代表 以此类推
	kMousePositionLeftTop = 11,

	kMousePositionTop = 12,

	kMousePositionRightTop = 13,

	kMousePositionLeft = 21,

	kMousePositionMid = 22,

	kMousePositionRight = 23,

	kMousePositionLeftBottom = 31,

	kMousePositionBottom = 32,

	kMousePositionRightBottom = 33,

};

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget* parent = 0);
	~MainWindow();

protected:
	void mousePressEvent(QMouseEvent* ev)override;
	void mouseReleaseEvent(QMouseEvent* event)override;
	void mouseMoveEvent(QMouseEvent* event)override;

private:
	//根据鼠标的设置鼠标样式，用于拉伸
	void SetMouseCursor(int x, int y);
	//判断鼠标的区域，用于拉伸
	int GetMouseRegion(int x, int y);
private:
	bool isPress = false;
	QPoint windowsLastPs;
	QPoint mousePs;
	int mouse_press_region = kMousePositionMid;
private:
	Ui::MainWindow* ui;

};
#endif // MAINWINDOW_H
