#ifndef SCREENSHOT_H
#define SCREENSHOT_H

#include <QWidget>
#include <QStack>
#include <QQueue>

class QPoint;
class QLabel;
class ScreenLabel;
class QImage;
class QRect;
class QPushButton;
class QPixmap;
class QTimer;
class drawingTools;
class QColor;
class QFont;
class QTextEdit;
class QSignalMapper;

class ScreenShot : public QWidget
{
    Q_OBJECT

public:
    ScreenShot(QWidget *parent = 0);
    ~ScreenShot();
    void setBackground(int width, int height);
    void takeSnap();                  //Take a grayed snap of the screen as background
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *me);
    void mouseMoveEvent(QMouseEvent *me);
    void mouseReleaseEvent(QMouseEvent *me);
    void keyPressEvent(QKeyEvent *ke);
    void showSelectedSizeLabel();
    void setSelectedImageToLabel();
    void paintSelectedImage(QPainter *painter, QPoint topLeftPoint, int w, int h);
    void getPositionWhenDragingToResize();
    QPoint getRectUpperLeft(QPoint mouseClickPosition, QPoint mouseReleasePosition);
    enum CurrentActionStatus { FirstSelect, DragToResize, DragToMove, MouseHoverAfterSelection,
                               IsToDrawEllipse, IsToEditText, IsToDrawArrow, IsToDrawLine, IsToDrawRect, Invalid };
    CurrentActionStatus getCurrentActionStatus();

private:
    bool leftButtonIsPressed;
    bool selectionDone;
    bool enterDragSizeState;
    bool inSelectionStatus;                  // Determine if in dragToSelect, or dragToResize
    bool leftButtonIsPressedInDragMoveArea;
    bool leftButtonIsPressedInDragResizeArea;
    QStack<QPixmap> pixmapStack;
    QPixmap finalPixmap;
    QPixmap pixmapForDrawing;          // Save the selected image as paint device for later drawing operations
    QLabel *sizeLabel;                 // Lable showing the size of captured screen
    QImage background;                 // Screen background image
    QImage grayed_background;          // Grayed screen background image
    QRect capturedRect;                // Rectangle of captured area
    int screen_width;                  // Screen width
    int screen_height;                 // Screen height
    void closeEverything();            // Close all the widgets
    void recoverWindowScreen();        // Recover main screen window after capturing

    drawingTools *dtools;

    QLabel *selectedImageLabel;
    QPoint mousePressPosition;
    QPoint mouseMovePosition;
    QPoint dragToSelectStartPosition;
    QPoint dragToSelectEndPosition;
    QPoint dragToMoveStartPosition;
    QPoint dragToMoveEndPosition;
    QPoint dragToResizeStartPosition;
    QPoint dragToResizeEndPosition;

    QPushButton *cancelBtn, *confirmBtn, *saveAsBtn, *goBackBtn;
    QPushButton *drawEllipseBtn, *drawLineBtn, *drawRectBtn, *textEditBtn, *drawArrowBtn;

    QSignalMapper *colorSignalMapper;
    QPushButton *whiteButton, *cyanButton, *greenButton, *yellowButton, *grayButton,
                *darkGreenButton, *magentaButton,*redButton, *blueButton, *blackButton;
    QPushButton *selectedColorButton;

    int finalWidth, finalHeight, finalX, finalY;
    int oneSelectionDoneWidth, oneSelectionDoneHeight, oneSelectionDoneX, oneSelectionDoneY;

    QFont textFont;
    QTextEdit *textEdit;
    int textSize;
    QQueue<QPoint> textPositions;
    QQueue<QString> textString;
    bool textEditFirstHit;
    void closeAll();

    //void drawFrame();
    void drawLine(QPixmap &pixmap, const QPoint &endPoint);
    void drawRectangle(QPixmap &pixmap, const QPoint &endPoint);
    void drawEllipse(QPixmap &pixmap, const QPoint &endPoint);
    void drawArrow(QPixmap &pixmap, const QPoint &endPoint);
    QRect paintText(QPixmap &pixmap);

    int myPenWidth;
    QColor myPenColor;
    QPoint lastPenPoint;

    int imagesAdded;

    QRect deskRect;

signals:

public slots:
    void on_clickSaveAsBtn();
    void on_clickCancelBtn();
    void on_clickDrawLineBtn();
    void on_clickDrawRectBtn();
    void on_clickDrawEllipseBtn();
    void on_clickDrawArrowBtn();
    void on_clickGoBackBtn();
    void on_textChanged();
    void on_textEditBtnToggled();
    void showAndHideColorOptionsButtons();
    void setPenColor(QString colorString);
    void setColorToSelectedColorButton();
//    void onclickConfirmBtn();
};

#endif // SCREENSHOT_H
