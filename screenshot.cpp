#include <QApplication>
#include <QPushButton>
#include <QLabel>
#include <QMouseEvent>
#include <QImage>
#include <QDesktopWidget>
#include <QPainter>
#include <QScreen>
#include <QSize>
#include <QRect>
#include <QImage>
#include <QPixmap>
#include <QIcon>
#include <cstring>
#include <QVector>
#include <QDebug>
#include <QString>
#include <QDir>
#include <QFileDialog>
#include <QTextEdit>
#include <QSignalMapper>
#include <QColor>
#include <cmath>
#include <windows.h>
#include "screenshot.h"

ScreenShot::ScreenShot(QWidget *parent)
    : QWidget(parent),
      leftButtonIsPressed(false),
      selectionDone(false),
      enterDragSizeState(false),
      inSelectionStatus(false),
      leftButtonIsPressedInDragMoveArea(false),
      leftButtonIsPressedInDragResizeArea(false),
      mousePressPosition(QPoint(0, 0)),
      mouseMovePosition(QPoint(0, 0)),
      dragToSelectStartPosition(QPoint(0, 0)),
      dragToSelectEndPosition(QPoint(0,0)),
      dragToMoveStartPosition(QPoint(0, 0)),
      dragToMoveEndPosition(QPoint(0, 0)),
      dragToResizeStartPosition(QPoint(0, 0)),
      dragToResizeEndPosition(QPoint(0, 0)),
      finalWidth(0),
      finalHeight(0),
      finalX(0),
      finalY(0),
      oneSelectionDoneWidth(0),
      oneSelectionDoneHeight(0),
      oneSelectionDoneX(0),
      oneSelectionDoneY(0),
      textSize(12),
      textEditFirstHit(true),
      myPenWidth(2),
      myPenColor(Qt::red),
      imagesAdded(0)
{
    setMouseTracking(true);

    saveAsBtn = new QPushButton(this);
    saveAsBtn->hide();
    saveAsBtn->setFixedSize(32, 25);
    saveAsBtn->setToolTip("保存");

    cancelBtn = new QPushButton(this);
    cancelBtn->hide();
    cancelBtn->setFixedSize(32, 25);
    cancelBtn->setToolTip("取消");

    confirmBtn = new QPushButton(this);
    confirmBtn->hide();
    confirmBtn->setFixedSize(32, 25);
    confirmBtn->setToolTip("确定");

    drawLineBtn = new QPushButton(this);
    drawLineBtn->hide();
    drawLineBtn->setFixedSize(32, 25);
    drawLineBtn->setToolTip("添加线条");
    drawLineBtn->setCheckable(true);

    drawEllipseBtn = new QPushButton(this);
    drawEllipseBtn->hide();
    drawEllipseBtn->setFixedSize(32, 25);
    drawEllipseBtn->setToolTip("添加椭圆");
    drawEllipseBtn->setCheckable(true);

    drawRectBtn = new QPushButton(this);
    drawRectBtn->hide();
    drawRectBtn->setFixedSize(32, 25);
    drawRectBtn->setToolTip("添加矩形");
    drawRectBtn->setCheckable(true);

    textEditBtn = new QPushButton(this);
    textEditBtn->hide();
    textEditBtn->setFixedSize(32, 25);
    textEditBtn->setToolTip("添加文字");
    textEditBtn->setCheckable(true);

    drawArrowBtn = new QPushButton(this);
    drawArrowBtn->hide();
    drawArrowBtn->setFixedSize(32, 25);
    drawArrowBtn->setToolTip("添加箭头");
    drawArrowBtn->setCheckable(true);

    goBackBtn = new QPushButton(this);
    goBackBtn->hide();
    goBackBtn->setFixedSize(32, 25);
    goBackBtn->setToolTip("返回上一步");

    takeSnap();

    sizeLabel = new QLabel(this);
    selectedImageLabel = new QLabel(this);
    Qt::WindowFlags nType = Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint;
    setWindowFlags(Qt::FramelessWindowHint);
    sizeLabel->setWindowFlags(nType);
    selectedImageLabel->setWindowFlags(nType);

    textFont.setFamily(QString("Microsoft YaHei"));
    textFont.setPointSize(textSize);

    textEdit = new QTextEdit(this);
    textEdit->setStyleSheet("QTextEdit{background:transparent;}"
                            "QTextEdit{border: 1px dot-dash black;}");

    textEdit->setCurrentFont(textFont);
    textEdit->setTextColor(myPenColor);
    textEdit->hide();

    colorSignalMapper = new QSignalMapper(this);

    whiteButton = new QPushButton(this);
    colorSignalMapper->setMapping(whiteButton, QString("white"));
    connect(whiteButton, SIGNAL(clicked()), colorSignalMapper, SLOT(map()));
    connect(whiteButton, SIGNAL(clicked()), this, SLOT(setColorToSelectedColorButton()));

    cyanButton = new QPushButton(this);
    colorSignalMapper->setMapping(cyanButton, QString("cyan"));
    connect(cyanButton, SIGNAL(clicked()), colorSignalMapper, SLOT(map()));
    connect(cyanButton, SIGNAL(clicked()), this, SLOT(setColorToSelectedColorButton()));

    greenButton = new QPushButton(this);
    colorSignalMapper->setMapping(greenButton, QString("green"));
    connect(greenButton, SIGNAL(clicked()), colorSignalMapper, SLOT(map()));
    connect(greenButton, SIGNAL(clicked()), this, SLOT(setColorToSelectedColorButton()));

    yellowButton = new QPushButton(this);
    colorSignalMapper->setMapping(yellowButton, QString("yellow"));
    connect(yellowButton, SIGNAL(clicked()), colorSignalMapper, SLOT(map()));
    connect(yellowButton, SIGNAL(clicked()), this, SLOT(setColorToSelectedColorButton()));

    grayButton = new QPushButton(this);
    colorSignalMapper->setMapping(grayButton, QString("gray"));
    connect(grayButton, SIGNAL(clicked()), colorSignalMapper, SLOT(map()));
    connect(grayButton, SIGNAL(clicked()), this, SLOT(setColorToSelectedColorButton()));

    darkGreenButton = new QPushButton(this);
    colorSignalMapper->setMapping(darkGreenButton, QString("darkGreen"));
    connect(darkGreenButton, SIGNAL(clicked()), colorSignalMapper, SLOT(map()));
    connect(darkGreenButton, SIGNAL(clicked()), this, SLOT(setColorToSelectedColorButton()));

    magentaButton = new QPushButton(this);
    colorSignalMapper->setMapping(magentaButton, QString("magenta"));
    connect(magentaButton, SIGNAL(clicked()), colorSignalMapper, SLOT(map()));
    connect(magentaButton, SIGNAL(clicked()), this, SLOT(setColorToSelectedColorButton()));

    redButton = new QPushButton(this);
    colorSignalMapper->setMapping(redButton, QString("red"));
    connect(redButton, SIGNAL(clicked()), colorSignalMapper, SLOT(map()));
    connect(redButton, SIGNAL(clicked()), this, SLOT(setColorToSelectedColorButton()));

    blueButton = new QPushButton(this);
    colorSignalMapper->setMapping(blueButton, QString("blue"));
    connect(blueButton, SIGNAL(clicked()), colorSignalMapper, SLOT(map()));
    connect(blueButton, SIGNAL(clicked()), this, SLOT(setColorToSelectedColorButton()));

    blackButton = new QPushButton(this);
    colorSignalMapper->setMapping(blackButton, QString("black"));
    connect(blackButton, SIGNAL(clicked()), colorSignalMapper, SLOT(map()));
    connect(blackButton, SIGNAL(clicked()), this, SLOT(setColorToSelectedColorButton()));

    connect(colorSignalMapper, SIGNAL(mapped(QString)), this, SLOT(setPenColor(QString)));

    whiteButton->hide();
    cyanButton->hide();
    greenButton->hide();
    yellowButton->hide();
    grayButton->hide();
    darkGreenButton->hide();
    magentaButton->hide();
    redButton->hide();
    blueButton->hide();
    blackButton->hide();

    whiteButton->setFixedSize(16, 16);
    cyanButton->setFixedSize(16, 16);
    greenButton->setFixedSize(16, 16);
    yellowButton->setFixedSize(16, 16);
    grayButton->setFixedSize(16, 16);
    darkGreenButton->setFixedSize(16, 16);
    magentaButton->setFixedSize(16, 16);
    redButton->setFixedSize(16, 16);
    blueButton->setFixedSize(16, 16);
    blackButton->setFixedSize(16, 16);

    whiteButton->setStyleSheet("border: 7px solid white;");
    cyanButton->setStyleSheet("border: 7px solid cyan;");
    greenButton->setStyleSheet("border: 7px solid green;");
    yellowButton->setStyleSheet("border: 7px solid yellow;");
    grayButton->setStyleSheet("border: 7px solid gray;");
    darkGreenButton->setStyleSheet("border: 7px solid darkGreen;");
    magentaButton->setStyleSheet("border: 7px solid magenta;");
    redButton->setStyleSheet("border: 7px solid red;");
    blueButton->setStyleSheet("border: 7px solid blue");
    blackButton->setStyleSheet("border: 7px solid black");

    selectedColorButton = new QPushButton(this);
    selectedColorButton->hide();
    selectedColorButton->setFixedSize(32, 32);
    QPixmap *colorPixmap = new QPixmap(selectedColorButton->size());
    colorPixmap->fill(myPenColor);
    QIcon buttonIcon(*colorPixmap);
    selectedColorButton->setIcon(buttonIcon);

    connect(saveAsBtn, SIGNAL(clicked()), this, SLOT(on_clickSaveAsBtn()));
    connect(cancelBtn, SIGNAL(clicked()), this, SLOT(on_clickCancelBtn()));
    connect(drawLineBtn, SIGNAL(clicked()), this, SLOT(on_clickDrawLineBtn()));
    connect(drawLineBtn, SIGNAL(toggled(bool)), this, SLOT(showAndHideColorOptionsButtons()));
    connect(drawEllipseBtn, SIGNAL(clicked()), this, SLOT(on_clickDrawEllipseBtn()));
    connect(drawEllipseBtn, SIGNAL(toggled(bool)), this, SLOT(showAndHideColorOptionsButtons()));
    connect(drawRectBtn, SIGNAL(clicked()), this, SLOT(on_clickDrawRectBtn()));
    connect(drawRectBtn, SIGNAL(toggled(bool)), this, SLOT(showAndHideColorOptionsButtons()));
    connect(drawArrowBtn, SIGNAL(clicked()), this, SLOT(on_clickDrawArrowBtn()));
    connect(drawArrowBtn, SIGNAL(toggled(bool)), this, SLOT(showAndHideColorOptionsButtons()));
    connect(textEditBtn, SIGNAL(toggled(bool)), this, SLOT(on_textEditBtnToggled()));
    connect(textEditBtn, SIGNAL(toggled(bool)), this, SLOT(showAndHideColorOptionsButtons()));
    connect(goBackBtn, SIGNAL(clicked()), this, SLOT(on_clickGoBackBtn()));
//    connect(confirmBtn, SIGNAL(clicked()), this, SLOT(onclickConfirmBtn()));
    connect(textEdit, SIGNAL(textChanged()), this, SLOT(on_textChanged()));
}

ScreenShot::~ScreenShot()
{
}

void ScreenShot::setBackground(int width, int height)
{
    QScreen *screen = QGuiApplication::primaryScreen();
    background = screen->grabWindow(0).toImage();

    int red, green, blue;
    QImage bg_grayed(width, height, QImage::Format_RGB32);
    for (int x = 0; x < width; ++x){
        for (int y = 0; y < height; ++y){
            red = qRed(background.pixel(x,y))*0.5;
            green = qGreen(background.pixel(x,y))*0.5;
            blue = qBlue(background.pixel(x,y))*0.5;
            bg_grayed.setPixel(x, y, qRgb(red, green, blue));
        }
    }
    grayed_background = bg_grayed;
    QPalette palette;
    palette.setBrush(this->backgroundRole(), QBrush(grayed_background));
    this->setPalette(palette);
}

void ScreenShot::takeSnap()
{
    QDesktopWidget *desktopWidget = QApplication::desktop();
    deskRect = desktopWidget->screenGeometry();
    screen_width = deskRect.width();
    screen_height = deskRect.height();
    this->resize(screen_width, screen_height);
    setBackground(screen_width, screen_height);
}

void ScreenShot::paintEvent(QPaintEvent *)
{
    ScreenShot::CurrentActionStatus status = getCurrentActionStatus();

    if ((status == ScreenShot::FirstSelect)
         && dragToSelectStartPosition != dragToSelectEndPosition)
    {
            setSelectedImageToLabel();
    }
    if (status == ScreenShot::DragToMove ||
         status == ScreenShot::DragToResize)
    {
        setSelectedImageToLabel();
    }
    if (status == ScreenShot::IsToDrawLine ||
        status == ScreenShot::IsToDrawArrow ||
        status == ScreenShot::IsToDrawEllipse ||
        status == ScreenShot::IsToDrawRect ||
        status == ScreenShot::IsToEditText)
    {
        selectedImageLabel->show();
    }
    QPixmap arrow("://icons/arrow.png");
    QPixmap text("://icons/text.png");
    QPixmap ellipse("://icons/ellipse.png");
    QPixmap rect("://icons/rectangle.png");
    QPixmap line("://icons/pen.png");
    QPixmap save("://icons/save.png");
    QPixmap cancel("://icons/cancel.png");
    QPixmap confirm("://icons/ok.png");
    QPixmap back("://icons/back.png");

    drawArrowBtn->setIcon(QIcon(arrow));
    textEditBtn->setIcon(QIcon(text));
    drawEllipseBtn->setIcon(QIcon(ellipse));
    drawRectBtn->setIcon(QIcon(rect));
    drawLineBtn->setIcon(QIcon(line));
    saveAsBtn->setIcon(QIcon(save));
    cancelBtn->setIcon(QIcon(cancel));
    confirmBtn->setIcon(QIcon(confirm));
    goBackBtn->setIcon(QIcon(back));

    //When using style sheets, every widget is treated as a box model, see the documentation for details.
    this->setStyleSheet("QPushButton{background-clip:margin;}"
                        "QPushButton{border:none;}"
                        "QPushButton{margin:1px;}"
                        "QPushButton{background-color:rgb(230, 230, 230);}"
                        "QPushButton:hover{border: 1px solid rgb(125, 125, 125);}"
                        "QPushButton:checked{border: 1px solid rgb(125, 125, 125);}");

    goBackBtn->move(finalX + finalWidth - 9 * 32, finalY + finalHeight + 3);
    drawArrowBtn->move(finalX + finalWidth - 8 * 32, finalY + finalHeight + 3);
    textEditBtn->move(finalX + finalWidth - 7 * 32, finalY + finalHeight + 3);
    drawEllipseBtn->move(finalX + finalWidth - 6 * 32, finalY + finalHeight + 3);
    drawRectBtn->move(finalX + finalWidth - 5 * 32, finalY + finalHeight + 3);
    drawLineBtn->move(finalX + finalWidth - 4 * 32, finalY + finalHeight + 3);
    saveAsBtn->move(finalX + finalWidth - 3 * 32, finalY + finalHeight + 3);
    cancelBtn->move(finalX + finalWidth - 2 * 32, finalY + finalHeight + 3);
    confirmBtn->move(finalX + finalWidth - 32, finalY + finalHeight + 3);

    if (!inSelectionStatus && selectionDone)
    {
        textEditBtn->show();
        drawArrowBtn->show();
        drawEllipseBtn->show();
        drawRectBtn->show();
        drawLineBtn->show();
        saveAsBtn->show();
        cancelBtn->show();
        confirmBtn->show();
        goBackBtn->show();
    }
    else
    {
        textEditBtn->hide();
        drawArrowBtn->hide();
        drawEllipseBtn->hide();
        drawRectBtn->hide();
        drawLineBtn->hide();
        saveAsBtn->hide();
        cancelBtn->hide();
        confirmBtn->hide();
        goBackBtn->hide();
    }
}

void ScreenShot::mousePressEvent(QMouseEvent *me)
{
    leftButtonIsPressed = true;
    mousePressPosition = me->pos();
    mouseMovePosition = mousePressPosition;

    if (!selectionDone)
    {
        dragToSelectStartPosition = me->pos();
        dragToSelectEndPosition = dragToSelectStartPosition;
        finalX = dragToSelectStartPosition.x();
        finalY = dragToSelectStartPosition.y();
    }
    else
    {
        if ((drawLineBtn->isChecked() || drawArrowBtn->isChecked()
             || drawEllipseBtn->isChecked() || drawRectBtn->isChecked())
                && me->button() == Qt::LeftButton)
        {
            lastPenPoint = me->pos();
        }
        else if (textEditBtn->isChecked() && me->button() == Qt::LeftButton
                 && selectedImageLabel->geometry().adjusted(0, 0, -40, -40).contains(me->pos()))
        {
            textEdit->setTextColor(myPenColor);
            textPositions.enqueue(me->pos());

            QRect rectNeedingToBeUpdated;
            if (!textEditFirstHit)
            {
                textString.enqueue(textEdit->toPlainText());

                rectNeedingToBeUpdated = paintText(pixmapForDrawing);
                selectedImageLabel->setPixmap(pixmapForDrawing);
                pixmapStack.push_back(pixmapForDrawing);
                imagesAdded++;
            }

            textEdit->clear();
            textEdit->move(me->pos());
            textEdit->show();
            textEditFirstHit = false;

            update(rectNeedingToBeUpdated);
        }
        else
        {
            if (me->x() < finalX + finalWidth - 2
                && me->x() > finalX + 2
                && me->y() < finalY + finalHeight - 2
                && me->y() > finalY + 2)
            {
                qDebug() << "Hover press"
                         << selectedImageLabel->geometry().adjusted(2, 2, -2, -2).contains(me->pos());

                leftButtonIsPressedInDragMoveArea = true;
                dragToMoveStartPosition = me->pos();
                dragToMoveEndPosition = dragToMoveStartPosition;
            }
            else if (((abs(me->x() - finalX) <= 2 && me->pos().y() >= finalY && me->pos().y() <= finalY + finalHeight)
               || (abs(me->x() - finalX - finalWidth) <= 2 && me->pos().y() >= finalY && me->pos().y() <= finalY + finalHeight)
               || (abs(me->y() - finalY) <= 2 && me->pos().x() >= finalX + 2 && me->pos().x() <= finalX + finalWidth - 2)
               || (abs(me->y() - finalY - finalHeight) <= 2 && me->pos().x() >= finalX + 2 && me->pos().x() <= finalX + finalWidth - 2))
                    && leftButtonIsPressed)
            {
                leftButtonIsPressedInDragResizeArea = true;
                dragToResizeStartPosition = me->pos();
                dragToResizeEndPosition = dragToResizeStartPosition;
            }
            else
            {
                return;
            }
        }
    }
}

void ScreenShot::mouseMoveEvent(QMouseEvent *me)
{
    showSelectedSizeLabel();

    if (me->buttons() == Qt::LeftButton)
    {
        mouseMovePosition = me->pos();
    }

    ScreenShot::CurrentActionStatus status = getCurrentActionStatus();

    if (status == ScreenShot::FirstSelect)
    {
        inSelectionStatus = true;
        dragToSelectEndPosition = me->pos();
        finalX = getRectUpperLeft(dragToSelectStartPosition, dragToSelectEndPosition).x();
        finalY = getRectUpperLeft(dragToSelectStartPosition, dragToSelectEndPosition).y();
        finalWidth = abs(dragToSelectStartPosition.x() - dragToSelectEndPosition.x());
        finalHeight = abs(dragToSelectStartPosition.y() - dragToSelectEndPosition.y());
        update();
    }
    else if (status == ScreenShot::DragToMove)
    {
        dragToMoveEndPosition = me->pos();
        finalX = oneSelectionDoneX + dragToMoveEndPosition.x() - dragToMoveStartPosition.x();
        finalY = oneSelectionDoneY + dragToMoveEndPosition.y() - dragToMoveStartPosition.y();
        update();
    }
    else if (status == MouseHoverAfterSelection)
    {
       /**********************************************
        *              p1 _________ p2               *
        *                |         |                 *
        *                |         |                 *
        *                |         |                 *
        *                |         |                 *
        *             p3 |_________| p4              *
        *                                            *
        **********************************************/

        QPoint p1 = QPoint(finalX, finalY);
        QPoint p2 = QPoint(finalX + finalWidth, finalY);
        QPoint p3 = QPoint(finalX, finalY + finalHeight);
        QPoint p4 = QPoint(finalX + finalWidth, finalY + finalHeight);

        //Top-left point
        if (abs(me->x() - p1.x()) <= 2 && abs(me->y() - p1.y()) <= 2)
        {
            this->setCursor(Qt::SizeFDiagCursor);
        }
        //Top-right point
        else if (abs(me->x() - p2.x()) <= 2 && abs(me->y() - p2.y()) <= 2)
        {
            this->setCursor(Qt::SizeBDiagCursor);
        }
        //Bottom-left point
        else if (abs(me->x() - p3.x()) <= 2 && abs(me->y() - p3.y()) <= 2)
        {
            this->setCursor(Qt::SizeBDiagCursor);
        }
        //Bottom-right point
        else if (abs(me->x() - p4.x()) <= 2 && abs(me->y() - p4.y()) <= 2)
        {
            this->setCursor(Qt::SizeFDiagCursor);
        }
        //Top edge
        else if (me->x() - p1.x() > 2
                 && p2.x() - me->x() > 2
                 && abs(me->y() - p1.y()) < 2)
        {
            this->setCursor(Qt::SizeVerCursor);
        }
        //Left edge
        else if (me->y() - p1.y() > 2
                 && p3.y() - me->y() > 2
                 && abs(me->x() - p1.x()) < 2)
        {
            this->setCursor(Qt::SizeHorCursor);
        }
        //Right edge
        else if (me->y() - p2.y() > 2
                 && p4.y() - me->y() > 2
                 && abs(me->x() - p2.x()) < 2)
        {
            this->setCursor(Qt::SizeHorCursor);
        }
        //Bottom edge
        else if (me->x() - p3.x() > 2
                 && p4.x() - me->x() > 2
                 && abs(me->y() - p3.y()) < 2)
        {
            this->setCursor(Qt::SizeVerCursor);
        }
        //Drag to move area
//        if (me->x() < finalX + finalWidth - 2
//            && me->x() > finalX + 2
//            && me->y() < finalY + finalHeight - 2
//            && me->y() > finalY + 2)
        else if (selectedImageLabel->geometry().adjusted(2, 2, -2, -2).contains(me->pos()))
        {
            this->setCursor(Qt::SizeAllCursor);
        }
        else
        {
            this->setCursor(Qt::ArrowCursor);
        }
    }
    else if (status == ScreenShot::DragToResize)
    {
        inSelectionStatus = true;
        dragToResizeEndPosition = me->pos();
        getPositionWhenDragingToResize();
        update();
    }
    else if (status == ScreenShot::IsToDrawLine && me->buttons() == Qt::LeftButton)
    {
        drawLine(pixmapForDrawing, me->pos());
        selectedImageLabel->setPixmap(pixmapForDrawing);
        QString filename = "D:\\testLine.png";
        (selectedImageLabel->pixmap())->save(filename);
    }
    else if (status == ScreenShot::IsToDrawRect && me->buttons() == Qt::LeftButton)
    {
        pixmapForDrawing = pixmapStack.top().copy(pixmapStack.top().rect());
        drawRectangle(pixmapForDrawing, me->pos());
        selectedImageLabel->setPixmap(pixmapForDrawing);
        QString filename = "D:\\testRect.png";
        (selectedImageLabel->pixmap())->save(filename);
    }
    else if (status == ScreenShot::IsToDrawEllipse && me->buttons() == Qt::LeftButton)
    {
        pixmapForDrawing = pixmapStack.top().copy(pixmapStack.top().rect());
        drawEllipse(pixmapForDrawing, me->pos());
        selectedImageLabel->setPixmap(pixmapForDrawing);
        QString filename = "D:\\testEllipse.png";
        (selectedImageLabel->pixmap())->save(filename);
    }
    else if (status == ScreenShot::IsToDrawArrow && me->buttons() == Qt::LeftButton)
    {
        pixmapForDrawing = pixmapStack.top().copy(pixmapStack.top().rect());
        drawArrow(pixmapForDrawing, me->pos());
        selectedImageLabel->setPixmap(pixmapForDrawing);
    }
    else if (status == ScreenShot::IsToEditText && me->buttons() == Qt::LeftButton)
    {
        return;
    }
    else
    {
        return;
    }

}

void ScreenShot::mouseReleaseEvent(QMouseEvent *me)
{
    mouseMovePosition = me->pos();
    ScreenShot::CurrentActionStatus status = getCurrentActionStatus();

    if (status == ScreenShot::FirstSelect)
    {
        dragToSelectEndPosition = me->pos();
        oneSelectionDoneX = finalX;
        oneSelectionDoneY = finalY;
        oneSelectionDoneWidth = finalWidth;
        oneSelectionDoneHeight = finalHeight;

        if (dragToSelectStartPosition == dragToSelectEndPosition)
        {
            HWND h = GetDesktopWindow();
            h = GetWindow(h, GW_CHILD);
            while (h != NULL) {
                RECT hRect;
                char WND_Title[255];
                GetWindowRect(h, &hRect);
                GetWindowText(h, (LPTSTR) WND_Title, 255);
                QRect windowRect((int)hRect.left, (int)hRect.top,
                                 (int)(hRect.right - hRect.left),
                                 (int)(hRect.bottom - hRect.top));
                if (windowRect.contains(dragToSelectEndPosition) && strcmp(WND_Title, "CaptureScreen") != 0) {
                    finalX = (int) hRect.left;
                    finalY = (int) hRect.top;
                    finalWidth = (int) (hRect.right - hRect.left);
                    finalHeight = (int) (hRect.bottom - hRect.top);
                    update();
                    return;
                }
                h = GetWindow(h, GW_HWNDNEXT);
            }
        }
    }
    if (status == ScreenShot::DragToMove)
    {
        dragToMoveEndPosition = me->pos();
        oneSelectionDoneX = finalX;
        oneSelectionDoneY = finalY;
        oneSelectionDoneWidth = finalWidth;
        oneSelectionDoneHeight = finalHeight;
    }
    if (status == ScreenShot::DragToResize)
    {
        dragToResizeEndPosition = me->pos();
        getPositionWhenDragingToResize();
        oneSelectionDoneX = finalX;
        oneSelectionDoneY = finalY;
        oneSelectionDoneWidth = finalWidth;
        oneSelectionDoneHeight = finalHeight;
    }
    if (status != ScreenShot::IsToDrawLine
        && status != ScreenShot::IsToDrawArrow
        && status != ScreenShot::IsToDrawEllipse
        && status != ScreenShot::IsToDrawRect
        && status != ScreenShot::IsToEditText)
    {
        pixmapForDrawing = finalPixmap.copy(finalPixmap.rect());
        pixmapStack.push_back(finalPixmap);
    }
    if (status == ScreenShot::IsToDrawLine)
    {
        drawLine(pixmapForDrawing, me->pos());
        pixmapStack.push_back(pixmapForDrawing);
        imagesAdded++;
    }
    if (status == ScreenShot::IsToDrawRect)
    {
        drawRectangle(pixmapForDrawing, me->pos());
        pixmapStack.push_back(pixmapForDrawing);
        imagesAdded++;
    }
    if (status == ScreenShot::IsToDrawEllipse)
    {
        drawEllipse(pixmapForDrawing, me->pos());
        pixmapStack.push_back(pixmapForDrawing);
        imagesAdded++;
    }
    if (status == ScreenShot::IsToDrawArrow)
    {
        drawArrow(pixmapForDrawing, me->pos());
        pixmapStack.push_back(pixmapForDrawing);
        imagesAdded++;
    }

    selectionDone = true;
    inSelectionStatus = false;
    leftButtonIsPressedInDragMoveArea = false;
    leftButtonIsPressedInDragResizeArea = false;
    leftButtonIsPressed = false;

    update();
}


void ScreenShot::keyPressEvent(QKeyEvent *ke)
{
    if (ke->key() == Qt::Key_Escape){
        closeAll();
    }
}

void ScreenShot::showSelectedSizeLabel()
{
    QString str = QString("%1 \u00D7 %2").arg(finalWidth).arg(finalHeight);
    sizeLabel->setText(str);

    sizeLabel->resize(76, 24);
    sizeLabel->setAlignment(Qt::AlignCenter);
    sizeLabel->setStyleSheet("QLabel{color:white;}"
                             "QLabel{background-color:black;}");
    sizeLabel->move(QPoint(finalX, finalY - sizeLabel->height() - 3));
    sizeLabel->show();
}


void ScreenShot::setSelectedImageToLabel()
{
    QImage selectedImage = background.copy(finalX, finalY, finalWidth, finalHeight);

    QPixmap imageToPix = QPixmap::fromImage(selectedImage);
    finalPixmap = imageToPix.copy(imageToPix.rect());
    selectedImageLabel->resize(imageToPix.size());
    selectedImageLabel->setPixmap(imageToPix);
    //QString filename = "D:\\testhh.png";
    //selectedImageLabel->pixmap()->save(filename);
    selectedImageLabel->move(QPoint(finalX, finalY));
    selectedImageLabel->show();
}

void ScreenShot::getPositionWhenDragingToResize()
{
    QPoint p1 = QPoint(oneSelectionDoneX, oneSelectionDoneY);
    QPoint p2 = QPoint(oneSelectionDoneX + oneSelectionDoneWidth, oneSelectionDoneY);
    QPoint p3 = QPoint(oneSelectionDoneX, oneSelectionDoneY + oneSelectionDoneHeight);
    QPoint p4 = QPoint(oneSelectionDoneX + oneSelectionDoneWidth, oneSelectionDoneY + oneSelectionDoneHeight);

    // Top-left point
    if (abs(dragToResizeStartPosition.x() - p1.x()) <= 2 && abs(dragToResizeStartPosition.y() - p1.y()) <= 2)
    {
        this->setCursor(Qt::SizeFDiagCursor);
        finalX = getRectUpperLeft(dragToResizeEndPosition, p4).x();
        finalY = getRectUpperLeft(dragToResizeEndPosition, p4).y();
        finalWidth = abs(dragToResizeEndPosition.x() - p4.x());
        finalHeight = abs(dragToResizeEndPosition.y() - p4.y());
    }
    // Top-right point
    else if (abs(dragToResizeStartPosition.x() - p2.x()) <= 2 && abs(dragToResizeStartPosition.y() - p2.y()) <= 2)
    {
        this->setCursor(Qt::SizeBDiagCursor);
        finalX = getRectUpperLeft(dragToResizeEndPosition, p3).x();
        finalY = getRectUpperLeft(dragToResizeEndPosition, p3).y();
        finalWidth = abs(dragToResizeEndPosition.x() - p3.x());
        finalHeight = abs(dragToResizeEndPosition.y() - p3.y());
    }
    // Bottom-left point
    else if (abs(dragToResizeStartPosition.x() - p3.x()) <= 2 && abs(dragToResizeStartPosition.y() - p3.y()) <= 2)
    {
        this->setCursor(Qt::SizeBDiagCursor);
        finalX = getRectUpperLeft(dragToResizeEndPosition, p2).x();
        finalY = getRectUpperLeft(dragToResizeEndPosition, p2).y();
        finalWidth = abs(dragToResizeEndPosition.x() - p2.x());
        finalHeight = abs(dragToResizeEndPosition.y() - p2.y());
    }
    // Bottom-right point
    else if (abs(dragToResizeStartPosition.x() - p4.x()) <= 2 && abs(dragToResizeStartPosition.y() - p4.y()) <= 2)
    {
        this->setCursor(Qt::SizeFDiagCursor);
        finalX = getRectUpperLeft(dragToResizeEndPosition, p1).x();
        finalY = getRectUpperLeft(dragToResizeEndPosition, p1).y();
        finalWidth = abs(dragToResizeEndPosition.x() - p1.x());
        finalHeight = abs(dragToResizeEndPosition.y() - p1.y());
    }
    // Top edge
    else if (dragToResizeStartPosition.x() - p1.x() > 2
             && p2.x() - dragToResizeStartPosition.x() > 2
             && abs(dragToResizeStartPosition.y() - p1.y()) < 2)
    {
        this->setCursor(Qt::SizeVerCursor);
        finalY = dragToResizeEndPosition.y() < p3.y() ? dragToResizeEndPosition.y() : p3.y();
        finalHeight = abs(oneSelectionDoneHeight - (dragToResizeEndPosition.y() - p1.y()));
    }
    // Left edge
    else if (dragToResizeStartPosition.y() - p1.y() > 2
             && p3.y() - dragToResizeStartPosition.y() > 2
             && abs(dragToResizeStartPosition.x() - p1.x()) < 2)
    {
        this->setCursor(Qt::SizeHorCursor);
        finalX = dragToResizeEndPosition.x() < p2.x() ? dragToResizeEndPosition.x() : p2.x();
        finalWidth = abs(oneSelectionDoneWidth - (dragToResizeEndPosition.x() - p1.x()));
    }
    // Right edge
    else if (dragToResizeStartPosition.y() - p2.y() > 2
             && p4.y() - dragToResizeStartPosition.y() > 2
             && abs(dragToResizeStartPosition.x() - p2.x()) < 2)
    {
        this->setCursor(Qt::SizeHorCursor);
        finalX = p1.x() < dragToResizeEndPosition.x() ? p1.x() : dragToResizeEndPosition.x();
        finalWidth = abs(oneSelectionDoneWidth + (dragToResizeEndPosition.x() - p2.x()));
    }
    // Bottom edge
    else if (dragToResizeStartPosition.x() - p3.x() > 2
             && p4.x() - dragToResizeStartPosition.x() > 2
             && abs(dragToResizeStartPosition.y() - p3.y()) < 2)
    {
        this->setCursor(Qt::SizeVerCursor);
        finalY = p1.y() < dragToResizeEndPosition.y() ? p1.y() : dragToResizeEndPosition.y();
        finalHeight = abs(oneSelectionDoneHeight + (dragToResizeEndPosition.y() - p3.y()));
    }
    else
    {
        return;
    }
}

// Get the top-left position of the rectangle from any two given points.
QPoint ScreenShot::getRectUpperLeft(QPoint mouseClickPosition, QPoint mouseReleasePosition)
{
    QPoint result;
    if(mouseClickPosition.x() < mouseReleasePosition.x())
    {
        if(mouseClickPosition.y() < mouseReleasePosition.y())
        {
            result = mouseClickPosition;
        }
        else
        {
            result.setX(mouseClickPosition.x());
            result.setY(mouseReleasePosition.y());
        }
    }
    else
    {
        if(mouseClickPosition.y() < mouseReleasePosition.y())
        {
            result.setX(mouseReleasePosition.x());
            result.setY(mouseClickPosition.y());
        }
        else
        {
            result = mouseReleasePosition;
        }
    }
    return result;
}

ScreenShot::CurrentActionStatus ScreenShot::getCurrentActionStatus()
{
    if (!selectionDone && leftButtonIsPressed)
    {
        return ScreenShot::FirstSelect;
    }
    else if (selectionDone && leftButtonIsPressedInDragMoveArea
             && mousePressPosition != mouseMovePosition)
    {
        return ScreenShot::DragToMove;
    }
    else if (selectionDone && leftButtonIsPressedInDragResizeArea
             && mousePressPosition != mouseMovePosition)
    {
        return ScreenShot::DragToResize;
    }
    else if (selectionDone && !leftButtonIsPressed)
    {
        return ScreenShot::MouseHoverAfterSelection;
    }
    else if (selectionDone && drawLineBtn->isChecked())
    {
        return ScreenShot::IsToDrawLine;
    }
    else if (selectionDone && drawArrowBtn->isChecked())
    {
        return ScreenShot::IsToDrawArrow;
    }
    else if (selectionDone && drawRectBtn->isChecked())
    {
        return ScreenShot::IsToDrawRect;
    }
    else if (selectionDone && drawEllipseBtn->isChecked())
    {
        return ScreenShot::IsToDrawEllipse;
    }
    else if (selectionDone && textEditBtn->isChecked())
    {
        return ScreenShot::IsToEditText;
    }
    else
    {
        return ScreenShot::Invalid;
    }
}

void ScreenShot::closeAll()
{
    if (selectedImageLabel) {
        sizeLabel->close();
        selectedImageLabel->close();
        cancelBtn->close();
        saveAsBtn->close();
        drawLineBtn->close();
        drawRectBtn->close();
        textEditBtn->close();
        drawArrowBtn->close();
        drawEllipseBtn->close();
        confirmBtn->close();
    }
    this->close();
}

void ScreenShot::drawLine(QPixmap &pixmap, const QPoint &endPoint)
{
    QPainter painter(&pixmap);
    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));

    /******************************************************************
     * Don't forget to use **mapFromGlobal()**!!!                     *
     * painter draws on pixmap with paint device local coordinates!!! *
     ******************************************************************/
    painter.drawLine(selectedImageLabel->mapFromGlobal(lastPenPoint),
                     selectedImageLabel->mapFromGlobal(endPoint));

    int rad = (myPenWidth / 2) + 2;
    update(QRect(lastPenPoint, endPoint).normalized().adjusted(-rad, -rad, +rad, +rad));

    lastPenPoint = endPoint;
}

void ScreenShot::drawRectangle(QPixmap &pixmap, const QPoint &endPoint)
{
    QPoint rectXY = getRectUpperLeft(lastPenPoint, endPoint);
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    QPen pen;
    pen.setColor(myPenColor);
    pen.setWidth(myPenWidth);
    painter.setPen(pen);
    painter.drawRect(selectedImageLabel->mapFromGlobal(rectXY).x(), selectedImageLabel->mapFromGlobal(rectXY).y(),
                     abs(lastPenPoint.x() - endPoint.x()),
                     abs(lastPenPoint.y() - endPoint.y()));

    int rad = (myPenWidth / 2) + 2;
    update(QRect(lastPenPoint, endPoint).normalized().adjusted(-rad, -rad, +rad, +rad));
}

void ScreenShot::drawEllipse(QPixmap &pixmap, const QPoint &endPoint)
{
    QPoint rectXY = getRectUpperLeft(lastPenPoint, endPoint);
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    QPen pen;
    pen.setColor(myPenColor);
    pen.setWidth(myPenWidth);
    painter.setPen(pen);
    painter.drawEllipse(QRect(selectedImageLabel->mapFromGlobal(rectXY).x(), selectedImageLabel->mapFromGlobal(rectXY).y(),
                              abs(lastPenPoint.x() - endPoint.x()),
                              abs(lastPenPoint.y() - endPoint.y())));

    int rad = (myPenWidth / 2) + 2;
    update(QRect(lastPenPoint, endPoint).normalized().adjusted(-rad, -rad, +rad, +rad));
}

void ScreenShot::drawArrow(QPixmap &pixmap, const QPoint &endPoint)
{
    /*********************************
     *            endPoint
     *               /\
     *              /  \
     *             /    \
     *    point1  /      \  point2
     *           /__    __\
     *              |  |
     *       point3 |  | point4
     *              |  |
     *              |  |
     *               ..
     *          lastPenPoint
     *********************************/

    double arrowTipLength = 15.0;
    double slop = atan2((endPoint.y() - lastPenPoint.y()), (endPoint.x() - lastPenPoint.x()));
    double slopCos = cos(slop);
    double slopSin = sin(slop);

    //Arrow tip points
    QPoint point1 = QPoint(endPoint.x() + int(-arrowTipLength*slopCos - arrowTipLength/2.0*slopSin),
                           endPoint.y() + int(-arrowTipLength*slopSin + arrowTipLength/2.0*slopCos));
    point1 = selectedImageLabel->mapFromGlobal(point1);
    QPoint point2 = QPoint(endPoint.x() + int(-arrowTipLength*slopCos + arrowTipLength/2.0*slopSin),
                           endPoint.y() - int(arrowTipLength*slopSin + arrowTipLength/2.0*slopCos));
    point2 = selectedImageLabel->mapFromGlobal(point2);

    //Arrow body points
    int offsetX = (int) (arrowTipLength*slopSin/3);
    int offsetY = (int) (arrowTipLength*slopCos/3);
    QPoint point3 = QPoint(endPoint.x() + int(-arrowTipLength*slopCos - (arrowTipLength / 2.0*slopSin)) + offsetX,
                           endPoint.y() + int(-arrowTipLength*slopSin + (arrowTipLength / 2.0*slopCos)) - offsetY);
    point3 = selectedImageLabel->mapFromGlobal(point3);
    QPoint point4 = QPoint(endPoint.x() + int(-arrowTipLength*slopCos + (arrowTipLength / 2.0*slopSin) - offsetX),
                           endPoint.y() - int(arrowTipLength*slopSin + arrowTipLength / 2.0*slopCos) + offsetY);
    point4 = selectedImageLabel->mapFromGlobal(point4);

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);

    QPainterPath path;
    path.moveTo(selectedImageLabel->mapFromGlobal(lastPenPoint));
    path.lineTo(point3);
    path.lineTo(point1);
    path.lineTo(selectedImageLabel->mapFromGlobal(endPoint));
    path.lineTo(point2);
    path.lineTo(point4);
    path.lineTo(selectedImageLabel->mapFromGlobal(lastPenPoint));

    painter.fillPath(path, QBrush(myPenColor));

    int rad = (myPenWidth / 2) + 2;
    update(QRect(lastPenPoint, endPoint).normalized().adjusted(-rad, -rad, +rad, +rad));
}

QRect ScreenShot::paintText(QPixmap &pixmap)
{
    QPainter painter(&pixmap);
    QPen pen;
    pen.setColor(myPenColor);
    painter.setPen(pen);

    QFont font;
    font.setFamily(QString("Microsoft YaHei"));
    font.setPointSize(textSize);
    painter.setFont(font);

    QFontMetrics fontMetrics(textEdit->currentFont());
    int width = fontMetrics.width(textString.head());
    int height = fontMetrics.height();
    QPoint position = textPositions.head();
    QPoint textPos = selectedImageLabel->mapFromGlobal(QPoint(position.x(), position.y()));
    QRect rect(textPos.x(), textPos.y(), width + 10, height + 5);

    painter.drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, textString.head());
    imagesAdded++;

    textString.dequeue();
    textPositions.dequeue();

    return rect;
}

void ScreenShot::on_clickSaveAsBtn()
{
    QString initialPath = QDir::currentPath() + "/untitled.png";

    QString fileName = QFileDialog::getSaveFileName(this, tr("保存"), initialPath);

    if (fileName.isEmpty()) {
        exit(1);
    } else {
        selectedImageLabel->pixmap()->save(fileName);
    }
}

void ScreenShot::on_clickCancelBtn()
{
    closeAll();
}

void ScreenShot::on_clickDrawLineBtn()
{
    if (drawLineBtn->isChecked() == true)
    {
        drawArrowBtn->setChecked(false);
        drawEllipseBtn->setChecked(false);
        drawRectBtn->setChecked(false);
        textEditBtn->setChecked(false);
    }
}

void ScreenShot::on_clickDrawRectBtn()
{
    if (drawRectBtn->isChecked() == true)
    {
        drawArrowBtn->setChecked(false);
        drawEllipseBtn->setChecked(false);
        drawLineBtn->setChecked(false);
        textEditBtn->setChecked(false);
    }
}

void ScreenShot::on_clickDrawEllipseBtn()
{
    if (drawEllipseBtn->isChecked() == true)
    {
        drawArrowBtn->setChecked(false);
        drawRectBtn->setChecked(false);
        drawLineBtn->setChecked(false);
        textEditBtn->setChecked(false);
    }
}

void ScreenShot::on_clickDrawArrowBtn()
{
    if (drawArrowBtn->isChecked() == true)
    {
        drawEllipseBtn->setChecked(false);
        drawRectBtn->setChecked(false);
        drawLineBtn->setChecked(false);
        textEditBtn->setChecked(false);
    }
}

void ScreenShot::on_clickGoBackBtn()
{
    drawArrowBtn->setChecked(false);
    drawEllipseBtn->setChecked(false);
    drawRectBtn->setChecked(false);
    drawLineBtn->setChecked(false);
    textEditBtn->setChecked(false);

    if (pixmapStack.count() > 1 && imagesAdded != 0)
    {
        pixmapStack.pop();
        selectedImageLabel->setPixmap(pixmapStack.top());
        imagesAdded--;
    }
    update();
}

void ScreenShot::on_textChanged()
{
    QFontMetrics fontMetrics(textEdit->currentFont());
    int textWidth = fontMetrics.width(textEdit->toPlainText());
    int textHeight = fontMetrics.height();

    if (textEdit->toPlainText().size() < 3)
    {
        textEdit->resize(40, textHeight + 15);
    }
    else
    {
        textEdit->resize(textWidth + 25, textHeight + 15);
    }
}

void ScreenShot::on_textEditBtnToggled()
{
    if (textEditBtn->isChecked() == true)
    {
        drawEllipseBtn->setChecked(false);
        drawRectBtn->setChecked(false);
        drawLineBtn->setChecked(false);
        drawArrowBtn->setChecked(false);
    }
    else
    {
        textEdit->hide();
        update(textEdit->geometry().adjusted(-1, -1, 1, 1));
    }
}

void ScreenShot::showAndHideColorOptionsButtons()
{
    whiteButton->move(finalX + finalWidth - 9*32, finalY + finalHeight + 25 + 2*3);
    cyanButton->move(finalX + finalWidth - 9*32 + 16, finalY + finalHeight + 25 + 2*3);
    greenButton->move(finalX + finalWidth - 9*32 + 2*16, finalY + finalHeight + 25 + 2*3);
    yellowButton->move(finalX + finalWidth - 9*32 + 3*16, finalY + finalHeight + 25 + 2*3);
    grayButton->move(finalX + finalWidth - 9*32 + 4*16, finalY + finalHeight + 25 + 2*3);
    darkGreenButton->move(finalX + finalWidth - 9*32, finalY + finalHeight + 25 + 16 + 2*3);
    magentaButton->move(finalX + finalWidth - 9*32 + 16, finalY + finalHeight + 25 + 16 + 2*3);
    redButton->move(finalX + finalWidth - 9*32 + 2*16, finalY + finalHeight + 25 + 16 + 2*3);
    blueButton->move(finalX + finalWidth - 9*32 + 3*16, finalY + finalHeight + 25 + 16 + 2*3);
    blackButton->move(finalX + finalWidth - 9*32 + 4*16, finalY + finalHeight + 25 + 16 + 2*3);

    selectedColorButton->move(finalX + finalWidth - 9*32 + 5*16, finalY + finalHeight + 25 + 2*3);

    if (drawArrowBtn->isChecked() || drawLineBtn->isChecked() || drawEllipseBtn->isChecked()
        || drawRectBtn->isChecked() || textEditBtn->isChecked())
    {
        whiteButton->show();
        cyanButton->show();
        greenButton->show();
        yellowButton->show();
        grayButton->show();
        darkGreenButton->show();
        magentaButton->show();
        redButton->show();
        blueButton->show();
        blackButton->show();

        selectedColorButton->show();
    }
    else
    {
        whiteButton->hide();
        cyanButton->hide();
        greenButton->hide();
        yellowButton->hide();
        grayButton->hide();
        darkGreenButton->hide();
        magentaButton->hide();
        redButton->hide();
        blueButton->hide();
        blackButton->hide();

        selectedColorButton->hide();
    }
}

void ScreenShot::setPenColor(QString colorString)
{
    if (colorString == QString("white"))
    {
        myPenColor = Qt::white;
    }
    if (colorString == QString("cyan"))
    {
        myPenColor = Qt::cyan;
    }
    if (colorString == QString("green"))
    {
        myPenColor = Qt::green;
    }
    if (colorString == QString("yellow"))
    {
        myPenColor = Qt::yellow;
    }
    if (colorString == QString("gray"))
    {
        myPenColor = Qt::gray;
    }
    if (colorString == QString("darkGreen"))
    {
        myPenColor = Qt::darkGreen;
    }
    if (colorString == QString("magenta"))
    {
        myPenColor = Qt::magenta;
    }
    if (colorString == QString("red"))
    {
        myPenColor = Qt::red;
    }
    if (colorString == QString("blue"))
    {
        myPenColor = Qt::blue;
    }
    if (colorString == QString("black"))
    {
        myPenColor = Qt::black;
    }
}

void ScreenShot::setColorToSelectedColorButton()
{
    QPixmap *colorPixmap = new QPixmap(selectedColorButton->size());
    colorPixmap->fill(myPenColor);
    QIcon buttonIcon(*colorPixmap);
    selectedColorButton->setIcon(buttonIcon);
}



