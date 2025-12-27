/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout_Main;
    QWidget *mapContainer;
    QVBoxLayout *verticalLayout_Map;
    QLabel *labelMapTitle;
    QWidget *mapWidget;
    QWidget *controlPanel;
    QVBoxLayout *verticalLayout_Control;
    QGroupBox *groupBoxPathQuery;
    QGridLayout *gridLayout_Path;
    QLabel *labelPathStart;
    QComboBox *comboBoxPathStart;
    QLabel *labelPathEnd;
    QComboBox *comboBoxPathEnd;
    QPushButton *pushButtonQueryPath;
    QGroupBox *groupBoxTour;
    QGridLayout *gridLayout_Tour;
    QLabel *labelTourStart;
    QComboBox *comboBoxTourStart;
    QHBoxLayout *horizontalLayout_Tour;
    QPushButton *pushButtonDFS;
    QPushButton *pushButtonBFS;
    QGroupBox *groupBoxMST;
    QVBoxLayout *verticalLayout_MST;
    QPushButton *pushButtonMST;
    QPushButton *pushButtonClear;
    QGroupBox *groupBoxResult;
    QVBoxLayout *verticalLayout_Result;
    QTextEdit *textEditResult;
    QSpacerItem *verticalSpacer;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1400, 900);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        horizontalLayout_Main = new QHBoxLayout(centralwidget);
        horizontalLayout_Main->setObjectName("horizontalLayout_Main");
        mapContainer = new QWidget(centralwidget);
        mapContainer->setObjectName("mapContainer");
        verticalLayout_Map = new QVBoxLayout(mapContainer);
        verticalLayout_Map->setObjectName("verticalLayout_Map");
        labelMapTitle = new QLabel(mapContainer);
        labelMapTitle->setObjectName("labelMapTitle");
        labelMapTitle->setMaximumSize(QSize(949, 350));
        labelMapTitle->setStyleSheet(QString::fromUtf8("font-size: 16px; font-weight: bold; background-color: #4A6FA5; color: white; padding: 5px;"));
        labelMapTitle->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout_Map->addWidget(labelMapTitle);

        mapWidget = new QWidget(mapContainer);
        mapWidget->setObjectName("mapWidget");

        verticalLayout_Map->addWidget(mapWidget);


        horizontalLayout_Main->addWidget(mapContainer);

        controlPanel = new QWidget(centralwidget);
        controlPanel->setObjectName("controlPanel");
        verticalLayout_Control = new QVBoxLayout(controlPanel);
        verticalLayout_Control->setObjectName("verticalLayout_Control");
        groupBoxPathQuery = new QGroupBox(controlPanel);
        groupBoxPathQuery->setObjectName("groupBoxPathQuery");
        gridLayout_Path = new QGridLayout(groupBoxPathQuery);
        gridLayout_Path->setObjectName("gridLayout_Path");
        labelPathStart = new QLabel(groupBoxPathQuery);
        labelPathStart->setObjectName("labelPathStart");

        gridLayout_Path->addWidget(labelPathStart, 0, 0, 1, 1);

        comboBoxPathStart = new QComboBox(groupBoxPathQuery);
        comboBoxPathStart->setObjectName("comboBoxPathStart");

        gridLayout_Path->addWidget(comboBoxPathStart, 0, 1, 1, 1);

        labelPathEnd = new QLabel(groupBoxPathQuery);
        labelPathEnd->setObjectName("labelPathEnd");

        gridLayout_Path->addWidget(labelPathEnd, 1, 0, 1, 1);

        comboBoxPathEnd = new QComboBox(groupBoxPathQuery);
        comboBoxPathEnd->setObjectName("comboBoxPathEnd");

        gridLayout_Path->addWidget(comboBoxPathEnd, 1, 1, 1, 1);

        pushButtonQueryPath = new QPushButton(groupBoxPathQuery);
        pushButtonQueryPath->setObjectName("pushButtonQueryPath");

        gridLayout_Path->addWidget(pushButtonQueryPath, 2, 0, 1, 2);


        verticalLayout_Control->addWidget(groupBoxPathQuery);

        groupBoxTour = new QGroupBox(controlPanel);
        groupBoxTour->setObjectName("groupBoxTour");
        gridLayout_Tour = new QGridLayout(groupBoxTour);
        gridLayout_Tour->setObjectName("gridLayout_Tour");
        labelTourStart = new QLabel(groupBoxTour);
        labelTourStart->setObjectName("labelTourStart");

        gridLayout_Tour->addWidget(labelTourStart, 0, 0, 1, 1);

        comboBoxTourStart = new QComboBox(groupBoxTour);
        comboBoxTourStart->setObjectName("comboBoxTourStart");

        gridLayout_Tour->addWidget(comboBoxTourStart, 0, 1, 1, 1);

        horizontalLayout_Tour = new QHBoxLayout();
        horizontalLayout_Tour->setObjectName("horizontalLayout_Tour");
        pushButtonDFS = new QPushButton(groupBoxTour);
        pushButtonDFS->setObjectName("pushButtonDFS");

        horizontalLayout_Tour->addWidget(pushButtonDFS);

        pushButtonBFS = new QPushButton(groupBoxTour);
        pushButtonBFS->setObjectName("pushButtonBFS");

        horizontalLayout_Tour->addWidget(pushButtonBFS);


        gridLayout_Tour->addLayout(horizontalLayout_Tour, 1, 0, 1, 2);


        verticalLayout_Control->addWidget(groupBoxTour);

        groupBoxMST = new QGroupBox(controlPanel);
        groupBoxMST->setObjectName("groupBoxMST");
        verticalLayout_MST = new QVBoxLayout(groupBoxMST);
        verticalLayout_MST->setObjectName("verticalLayout_MST");
        pushButtonMST = new QPushButton(groupBoxMST);
        pushButtonMST->setObjectName("pushButtonMST");

        verticalLayout_MST->addWidget(pushButtonMST);


        verticalLayout_Control->addWidget(groupBoxMST);

        pushButtonClear = new QPushButton(controlPanel);
        pushButtonClear->setObjectName("pushButtonClear");
        pushButtonClear->setStyleSheet(QString::fromUtf8("background-color: #DC143C; color: white; font-weight: bold;"));

        verticalLayout_Control->addWidget(pushButtonClear);

        groupBoxResult = new QGroupBox(controlPanel);
        groupBoxResult->setObjectName("groupBoxResult");
        verticalLayout_Result = new QVBoxLayout(groupBoxResult);
        verticalLayout_Result->setObjectName("verticalLayout_Result");
        textEditResult = new QTextEdit(groupBoxResult);
        textEditResult->setObjectName("textEditResult");
        textEditResult->setReadOnly(true);

        verticalLayout_Result->addWidget(textEditResult);


        verticalLayout_Control->addWidget(groupBoxResult);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_Control->addItem(verticalSpacer);


        horizontalLayout_Main->addWidget(controlPanel);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1400, 25));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "\350\245\277\346\271\226\346\231\257\345\214\272\347\256\241\347\220\206\347\263\273\347\273\237", nullptr));
        labelMapTitle->setText(QCoreApplication::translate("MainWindow", "\350\245\277\346\271\226\346\231\257\345\214\272\345\234\260\345\233\276", nullptr));
        groupBoxPathQuery->setTitle(QCoreApplication::translate("MainWindow", "\350\267\257\345\276\204\346\237\245\350\257\242", nullptr));
        labelPathStart->setText(QCoreApplication::translate("MainWindow", "\350\265\267\347\202\271\357\274\232", nullptr));
        labelPathEnd->setText(QCoreApplication::translate("MainWindow", "\347\273\210\347\202\271\357\274\232", nullptr));
        pushButtonQueryPath->setText(QCoreApplication::translate("MainWindow", "\346\237\245\350\257\242\346\234\200\347\237\255\350\267\257\345\276\204", nullptr));
        groupBoxTour->setTitle(QCoreApplication::translate("MainWindow", "\346\231\257\347\202\271\351\201\215\345\216\206", nullptr));
        labelTourStart->setText(QCoreApplication::translate("MainWindow", "\350\265\267\347\202\271\357\274\232", nullptr));
        pushButtonDFS->setText(QCoreApplication::translate("MainWindow", "DFS\351\201\215\345\216\206", nullptr));
        pushButtonBFS->setText(QCoreApplication::translate("MainWindow", "BFS\351\201\215\345\216\206", nullptr));
        groupBoxMST->setTitle(QCoreApplication::translate("MainWindow", "\346\234\200\345\260\217\347\224\237\346\210\220\346\240\221", nullptr));
        pushButtonMST->setText(QCoreApplication::translate("MainWindow", "\347\224\237\346\210\220\346\234\200\345\260\217\347\224\237\346\210\220\346\240\221", nullptr));
        pushButtonClear->setText(QCoreApplication::translate("MainWindow", "\346\270\205\351\231\244\351\253\230\344\272\256", nullptr));
        groupBoxResult->setTitle(QCoreApplication::translate("MainWindow", "\346\237\245\350\257\242\347\273\223\346\236\234", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
