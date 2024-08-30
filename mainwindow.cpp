#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QMouseEvent>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::FramelessWindowHint);
    this->resize(800,600);

    this->setMouseTracking(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mousePressEvent(QMouseEvent* ev)
{
	if (ev->button() == Qt::LeftButton) {

		// 如果是鼠标左键
		// 获取当前窗口位置,以窗口左上角
		windowsLastPs = pos();
		// 获取鼠标在屏幕的位置  就是全局的坐标 以屏幕左上角为坐标系
		mousePs = ev->globalPosition().toPoint();
		isPress = true;
		// 获取鼠标在那个区域
		mouse_press_region = GetMouseRegion(ev->pos().x(), ev->pos().y());

	}
}

void MainWindow::mouseReleaseEvent(QMouseEvent* ev)
{
	if (ev->button() == Qt::LeftButton) {
		isPress = false;
	}
	setCursor(QCursor{});
}

void MainWindow::mouseMoveEvent(QMouseEvent* ev)
{
	// 设置鼠标的形状
	SetMouseCursor(ev->pos().x(), ev->pos().y());
	// 计算的鼠标移动偏移量, 就是鼠标全局坐标 - 减去点击时鼠标坐标
	QPoint point_offset = ev->globalPosition().toPoint() - mousePs;
	if ((ev->buttons() == Qt::LeftButton) && isPress)
	{
		if (mouse_press_region == kMousePositionMid)
		{
			// 如果鼠标是在窗口的中间位置,就是移动窗口
			move(windowsLastPs + point_offset);
		}
		else {
			// 其他部分 是拉伸窗口
			// 获取客户区
			QRect rect = geometry();
			switch (mouse_press_region)
			{
				// 左上角
			case kMousePositionLeftTop:
				rect.setTopLeft(rect.topLeft() + point_offset);
				break;
			case kMousePositionTop:
				rect.setTop(rect.top() + point_offset.y());
				break;
			case kMousePositionRightTop:
				rect.setTopRight(rect.topRight() + point_offset);
				break;
			case kMousePositionRight:
				rect.setRight(rect.right() + point_offset.x());
				break;
			case kMousePositionRightBottom:
				rect.setBottomRight(rect.bottomRight() + point_offset);
				break;
			case kMousePositionBottom:
				rect.setBottom(rect.bottom() + point_offset.y());
				break;
			case kMousePositionLeftBottom:
				rect.setBottomLeft(rect.bottomLeft() + point_offset);
				break;
			case kMousePositionLeft:
				rect.setLeft(rect.left() + point_offset.x());
				break;
			default:
				break;
			}
			setGeometry(rect);
			mousePs = ev->globalPosition().toPoint();
		}
	}
}

void MainWindow::SetMouseCursor(int x, int y)
{
	// 鼠标形状对象
	Qt::CursorShape cursor{};
	int region = GetMouseRegion(x, y);
	switch (region)
	{
	case kMousePositionLeftTop:
	case kMousePositionRightBottom:
		cursor = Qt::SizeFDiagCursor; break;
	case kMousePositionRightTop:
	case kMousePositionLeftBottom:
		cursor = Qt::SizeBDiagCursor; break;
	case kMousePositionLeft:
	case kMousePositionRight:
		cursor = Qt::SizeHorCursor; break;
	case kMousePositionTop:
	case kMousePositionBottom:
		cursor = Qt::SizeVerCursor; break;
	case kMousePositionMid:
		cursor = Qt::ArrowCursor; break;
	default:
		break;
	}
	setCursor(cursor);
}

int MainWindow::GetMouseRegion(int x, int y)
{
	int region_x = 0, region_y = 0;
	// 鼠标的X坐标小于 边界5 说明他在最上层区域 第一区域
	if (x < kMouseRegionLeft)
	{
		region_x = 1;
	}
	else if (x > (this->width()/*窗体宽度*/ - kMouseRegionRight/*边界宽度5*/)) {
		// 如果鼠标X坐标 大于 最右侧的边界 说明他在第三区域
		region_x = 3;
	}
	else {
		region_x = 2;
	}
	if (y < kMouseRegionTop)
	{
		// 同理 鼠标Y坐标 小于上层边界5  说明鼠标在第一区域
		region_y = 1;
	}
	else if (y > (this->height() - kMouseRegionBottom)) {
		// 鼠标Y坐标的 大于 最下面的坐标,鼠标就在 第三区
		region_y = 3;
	}
	else {
		region_y = 2;
	}
	// 最后计算 表示区域的 数值 (x=1, y=1) 计算 = 1*10+1 =11 
	// x=2,y=3 = 3*10+2 = 32 在图的 3,2 区域
	return region_y * 10 + region_x;
}