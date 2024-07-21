/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.13
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionQuit;
    QAction *actionSave_As;
    QAction *actionLoad;
    QAction *actionAbout;
    QWidget *centralWidget;
    QPushButton *applyButton;
    QTabWidget *tabWidget;
    QWidget *tab_3;
    QComboBox *sampleFormatCombo;
    QLabel *label_5;
    QLabel *label_6;
    QComboBox *channelConfigCombo;
    QComboBox *sampleRateCombo;
    QLabel *label_7;
    QLabel *label_14;
    QComboBox *stereoModeCombo;
    QGroupBox *groupBox;
    QGroupBox *groupBox_3;
    QWidget *widget;
    QLabel *label_11;
    QSlider *periodCountSlider;
    QLineEdit *periodCountEdit;
    QWidget *widget_2;
    QSlider *periodSizeSlider;
    QLabel *label_10;
    QLineEdit *periodSizeEdit;
    QComboBox *stereoEncodingComboBox;
    QLabel *label_19;
    QLabel *label_30;
    QComboBox *ambiFormatComboBox;
    QCheckBox *outputLimiterCheckBox;
    QCheckBox *outputDitherCheckBox;
    QGroupBox *groupBox_4;
    QLabel *resamplerLabel;
    QSlider *resamplerSlider;
    QLabel *label_9;
    QLabel *label_15;
    QWidget *tab_6;
    QCheckBox *decoderHQModeCheckBox;
    QCheckBox *decoderDistCompCheckBox;
    QGroupBox *groupBox_8;
    QLineEdit *decoderQuadLineEdit;
    QLabel *label_25;
    QPushButton *decoderQuadButton;
    QLineEdit *decoder51LineEdit;
    QPushButton *decoder51Button;
    QLabel *label_26;
    QLabel *label_28;
    QLineEdit *decoder61LineEdit;
    QPushButton *decoder61Button;
    QPushButton *decoder71Button;
    QLineEdit *decoder71LineEdit;
    QLabel *label_29;
    QLabel *label_33;
    QLineEdit *decoder3D71LineEdit;
    QPushButton *decoder3D71Button;
    QCheckBox *decoderNFEffectsCheckBox;
    QWidget *widget_3;
    QLabel *label_27;
    QDoubleSpinBox *decoderSpeakerDistSpinBox;
    QWidget *tab_5;
    QGroupBox *groupBox_2;
    QGroupBox *groupBox_6;
    QListWidget *hrtfFileList;
    QPushButton *hrtfAddButton;
    QCheckBox *defaultHrtfPathsCheckBox;
    QPushButton *hrtfRemoveButton;
    QLabel *label_12;
    QComboBox *preferredHrtfComboBox;
    QGroupBox *groupBox_9;
    QLabel *label_31;
    QLabel *label_32;
    QSlider *hrtfmodeSlider;
    QLabel *hrtfmodeLabel;
    QWidget *tab;
    QListWidget *backendListWidget;
    QStackedWidget *backendStackedWidget;
    QWidget *page;
    QCheckBox *backendCheckBox;
    QListWidget *disabledBackendList;
    QListWidget *enabledBackendList;
    QLabel *label_2;
    QLabel *label;
    QWidget *page_1;
    QCheckBox *pwireAssumeAudioCheckBox;
    QCheckBox *pwireRtMixCheckBox;
    QWidget *page_2;
    QCheckBox *wasapiResamplerCheckBox;
    QWidget *page_8;
    QCheckBox *pulseAutospawnCheckBox;
    QCheckBox *pulseAllowMovesCheckBox;
    QCheckBox *pulseFixRateCheckBox;
    QCheckBox *pulseAdjLatencyCheckBox;
    QWidget *page_7;
    QCheckBox *jackAutospawnCheckBox;
    QGroupBox *groupBox_7;
    QLineEdit *jackBufferSizeLine;
    QSlider *jackBufferSizeSlider;
    QCheckBox *jackConnectPortsCheckBox;
    QCheckBox *jackRtMixCheckBox;
    QWidget *page_3;
    QLabel *label_17;
    QLineEdit *alsaDefaultDeviceLine;
    QLabel *label_18;
    QLineEdit *alsaDefaultCaptureLine;
    QCheckBox *alsaResamplerCheckBox;
    QCheckBox *alsaMmapCheckBox;
    QWidget *page_4;
    QLabel *label_20;
    QLineEdit *ossDefaultDeviceLine;
    QLabel *label_21;
    QLineEdit *ossDefaultCaptureLine;
    QPushButton *ossPlaybackPushButton;
    QPushButton *ossCapturePushButton;
    QWidget *page_5;
    QLineEdit *solarisDefaultDeviceLine;
    QLabel *label_22;
    QPushButton *solarisPlaybackPushButton;
    QWidget *page_6;
    QLabel *label_23;
    QLineEdit *waveOutputLine;
    QLabel *label_24;
    QPushButton *waveOutputButton;
    QCheckBox *waveBFormatCheckBox;
    QWidget *tab_2;
    QLineEdit *srcCountLineEdit;
    QLabel *label_3;
    QLabel *label_4;
    QLineEdit *effectSlotLineEdit;
    QLabel *label_8;
    QLineEdit *srcSendLineEdit;
    QGroupBox *cpuExtGroupBox;
    QCheckBox *enableSSECheckBox;
    QCheckBox *enableSSE2CheckBox;
    QCheckBox *enableNeonCheckBox;
    QCheckBox *enableSSE41CheckBox;
    QCheckBox *enableSSE3CheckBox;
    QLabel *cpuExtDisabledLabel;
    QWidget *tab_4;
    QGroupBox *groupBox_5;
    QCheckBox *enableEaxReverbCheck;
    QCheckBox *enableStdReverbCheck;
    QCheckBox *enableChorusCheck;
    QCheckBox *enableDistortionCheck;
    QCheckBox *enableEchoCheck;
    QCheckBox *enableEqualizerCheck;
    QCheckBox *enableFlangerCheck;
    QCheckBox *enableModulatorCheck;
    QCheckBox *enableDedicatedCheck;
    QCheckBox *enableCompressorCheck;
    QCheckBox *enablePitchShifterCheck;
    QCheckBox *enableFrequencyShifterCheck;
    QCheckBox *enableAutowahCheck;
    QCheckBox *enableVocalMorpherCheck;
    QLabel *label_13;
    QComboBox *defaultReverbComboBox;
    QCheckBox *enableEaxCheck;
    QPushButton *closeCancelButton;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuHelp;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(564, 469);
        MainWindow->setMinimumSize(QSize(564, 460));
        QIcon icon;
        QString iconThemeName = QString::fromUtf8("preferences-desktop-sound");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon = QIcon::fromTheme(iconThemeName);
        } else {
            icon.addFile(QString::fromUtf8("."), QSize(), QIcon::Normal, QIcon::Off);
        }
        MainWindow->setWindowIcon(icon);
        actionQuit = new QAction(MainWindow);
        actionQuit->setObjectName(QString::fromUtf8("actionQuit"));
        QIcon icon1;
        iconThemeName = QString::fromUtf8("application-exit");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon1 = QIcon::fromTheme(iconThemeName);
        } else {
            icon1.addFile(QString::fromUtf8("."), QSize(), QIcon::Normal, QIcon::Off);
        }
        actionQuit->setIcon(icon1);
        actionSave_As = new QAction(MainWindow);
        actionSave_As->setObjectName(QString::fromUtf8("actionSave_As"));
        QIcon icon2;
        iconThemeName = QString::fromUtf8("document-save-as");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon2 = QIcon::fromTheme(iconThemeName);
        } else {
            icon2.addFile(QString::fromUtf8("."), QSize(), QIcon::Normal, QIcon::Off);
        }
        actionSave_As->setIcon(icon2);
        actionLoad = new QAction(MainWindow);
        actionLoad->setObjectName(QString::fromUtf8("actionLoad"));
        QIcon icon3;
        iconThemeName = QString::fromUtf8("document-open");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon3 = QIcon::fromTheme(iconThemeName);
        } else {
            icon3.addFile(QString::fromUtf8("."), QSize(), QIcon::Normal, QIcon::Off);
        }
        actionLoad->setIcon(icon3);
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        applyButton = new QPushButton(centralWidget);
        applyButton->setObjectName(QString::fromUtf8("applyButton"));
        applyButton->setGeometry(QRect(470, 405, 81, 31));
        QIcon icon4;
        iconThemeName = QString::fromUtf8("dialog-ok-apply");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon4 = QIcon::fromTheme(iconThemeName);
        } else {
            icon4.addFile(QString::fromUtf8("."), QSize(), QIcon::Normal, QIcon::Off);
        }
        applyButton->setIcon(icon4);
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(10, 0, 541, 401));
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        sampleFormatCombo = new QComboBox(tab_3);
        sampleFormatCombo->setObjectName(QString::fromUtf8("sampleFormatCombo"));
        sampleFormatCombo->setGeometry(QRect(110, 50, 80, 31));
        sampleFormatCombo->setSizeAdjustPolicy(QComboBox::AdjustToContents);
        label_5 = new QLabel(tab_3);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(0, 50, 101, 31));
        label_5->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_6 = new QLabel(tab_3);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(0, 20, 101, 31));
        label_6->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        channelConfigCombo = new QComboBox(tab_3);
        channelConfigCombo->setObjectName(QString::fromUtf8("channelConfigCombo"));
        channelConfigCombo->setGeometry(QRect(110, 20, 80, 31));
        channelConfigCombo->setSizeAdjustPolicy(QComboBox::AdjustToContents);
        sampleRateCombo = new QComboBox(tab_3);
        sampleRateCombo->addItem(QString());
        sampleRateCombo->addItem(QString());
        sampleRateCombo->addItem(QString());
        sampleRateCombo->addItem(QString());
        sampleRateCombo->addItem(QString());
        sampleRateCombo->addItem(QString());
        sampleRateCombo->addItem(QString());
        sampleRateCombo->addItem(QString());
        sampleRateCombo->setObjectName(QString::fromUtf8("sampleRateCombo"));
        sampleRateCombo->setGeometry(QRect(380, 20, 100, 31));
        sampleRateCombo->setEditable(true);
        sampleRateCombo->setInsertPolicy(QComboBox::NoInsert);
        sampleRateCombo->setSizeAdjustPolicy(QComboBox::AdjustToContents);
        label_7 = new QLabel(tab_3);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(290, 20, 81, 31));
        label_7->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_14 = new QLabel(tab_3);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setGeometry(QRect(290, 50, 81, 31));
        label_14->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        stereoModeCombo = new QComboBox(tab_3);
        stereoModeCombo->setObjectName(QString::fromUtf8("stereoModeCombo"));
        stereoModeCombo->setGeometry(QRect(380, 50, 101, 31));
        groupBox = new QGroupBox(tab_3);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(-11, 180, 551, 201));
        groupBox->setAlignment(Qt::AlignCenter);
        groupBox_3 = new QGroupBox(groupBox);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setGeometry(QRect(20, 30, 511, 81));
        groupBox_3->setAlignment(Qt::AlignCenter);
        widget = new QWidget(groupBox_3);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(260, 20, 241, 51));
        label_11 = new QLabel(widget);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(60, 0, 161, 21));
        label_11->setAlignment(Qt::AlignCenter);
        periodCountSlider = new QSlider(widget);
        periodCountSlider->setObjectName(QString::fromUtf8("periodCountSlider"));
        periodCountSlider->setGeometry(QRect(99, 20, 141, 21));
        periodCountSlider->setMinimum(1);
        periodCountSlider->setMaximum(16);
        periodCountSlider->setSingleStep(1);
        periodCountSlider->setPageStep(2);
        periodCountSlider->setValue(1);
        periodCountSlider->setTracking(true);
        periodCountSlider->setOrientation(Qt::Horizontal);
        periodCountSlider->setTickPosition(QSlider::TicksBelow);
        periodCountSlider->setTickInterval(1);
        periodCountEdit = new QLineEdit(widget);
        periodCountEdit->setObjectName(QString::fromUtf8("periodCountEdit"));
        periodCountEdit->setGeometry(QRect(40, 20, 51, 21));
        widget_2 = new QWidget(groupBox_3);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));
        widget_2->setGeometry(QRect(10, 20, 241, 51));
        periodSizeSlider = new QSlider(widget_2);
        periodSizeSlider->setObjectName(QString::fromUtf8("periodSizeSlider"));
        periodSizeSlider->setGeometry(QRect(60, 20, 191, 21));
        periodSizeSlider->setMinimum(63);
        periodSizeSlider->setMaximum(8192);
        periodSizeSlider->setSingleStep(1);
        periodSizeSlider->setPageStep(1024);
        periodSizeSlider->setValue(63);
        periodSizeSlider->setTracking(true);
        periodSizeSlider->setOrientation(Qt::Horizontal);
        periodSizeSlider->setTickPosition(QSlider::TicksBelow);
        periodSizeSlider->setTickInterval(512);
        label_10 = new QLabel(widget_2);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(10, 0, 201, 21));
        label_10->setAlignment(Qt::AlignCenter);
        periodSizeEdit = new QLineEdit(widget_2);
        periodSizeEdit->setObjectName(QString::fromUtf8("periodSizeEdit"));
        periodSizeEdit->setGeometry(QRect(0, 20, 51, 21));
        stereoEncodingComboBox = new QComboBox(groupBox);
        stereoEncodingComboBox->setObjectName(QString::fromUtf8("stereoEncodingComboBox"));
        stereoEncodingComboBox->setGeometry(QRect(130, 120, 111, 31));
        label_19 = new QLabel(groupBox);
        label_19->setObjectName(QString::fromUtf8("label_19"));
        label_19->setGeometry(QRect(20, 120, 101, 31));
        label_19->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_30 = new QLabel(groupBox);
        label_30->setObjectName(QString::fromUtf8("label_30"));
        label_30->setGeometry(QRect(260, 120, 121, 31));
        label_30->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        ambiFormatComboBox = new QComboBox(groupBox);
        ambiFormatComboBox->setObjectName(QString::fromUtf8("ambiFormatComboBox"));
        ambiFormatComboBox->setGeometry(QRect(390, 120, 131, 31));
        outputLimiterCheckBox = new QCheckBox(groupBox);
        outputLimiterCheckBox->setObjectName(QString::fromUtf8("outputLimiterCheckBox"));
        outputLimiterCheckBox->setGeometry(QRect(30, 160, 231, 20));
        outputLimiterCheckBox->setTristate(true);
        outputDitherCheckBox = new QCheckBox(groupBox);
        outputDitherCheckBox->setObjectName(QString::fromUtf8("outputDitherCheckBox"));
        outputDitherCheckBox->setGeometry(QRect(270, 160, 261, 21));
        outputDitherCheckBox->setTristate(true);
        groupBox_4 = new QGroupBox(tab_3);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        groupBox_4->setGeometry(QRect(60, 90, 421, 81));
        groupBox_4->setAlignment(Qt::AlignCenter);
        resamplerLabel = new QLabel(groupBox_4);
        resamplerLabel->setObjectName(QString::fromUtf8("resamplerLabel"));
        resamplerLabel->setGeometry(QRect(50, 50, 321, 21));
        resamplerLabel->setAlignment(Qt::AlignCenter);
        resamplerSlider = new QSlider(groupBox_4);
        resamplerSlider->setObjectName(QString::fromUtf8("resamplerSlider"));
        resamplerSlider->setGeometry(QRect(80, 30, 251, 23));
        resamplerSlider->setOrientation(Qt::Horizontal);
        label_9 = new QLabel(groupBox_4);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(20, 30, 51, 21));
        label_9->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_15 = new QLabel(groupBox_4);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setGeometry(QRect(340, 30, 51, 21));
        tabWidget->addTab(tab_3, QString());
        tab_6 = new QWidget();
        tab_6->setObjectName(QString::fromUtf8("tab_6"));
        decoderHQModeCheckBox = new QCheckBox(tab_6);
        decoderHQModeCheckBox->setObjectName(QString::fromUtf8("decoderHQModeCheckBox"));
        decoderHQModeCheckBox->setGeometry(QRect(30, 20, 181, 21));
        decoderHQModeCheckBox->setLayoutDirection(Qt::RightToLeft);
        decoderHQModeCheckBox->setTristate(true);
        decoderDistCompCheckBox = new QCheckBox(tab_6);
        decoderDistCompCheckBox->setObjectName(QString::fromUtf8("decoderDistCompCheckBox"));
        decoderDistCompCheckBox->setGeometry(QRect(30, 50, 181, 21));
        decoderDistCompCheckBox->setLayoutDirection(Qt::RightToLeft);
        decoderDistCompCheckBox->setTristate(true);
        groupBox_8 = new QGroupBox(tab_6);
        groupBox_8->setObjectName(QString::fromUtf8("groupBox_8"));
        groupBox_8->setGeometry(QRect(-10, 140, 551, 231));
        groupBox_8->setAlignment(Qt::AlignCenter);
        decoderQuadLineEdit = new QLineEdit(groupBox_8);
        decoderQuadLineEdit->setObjectName(QString::fromUtf8("decoderQuadLineEdit"));
        decoderQuadLineEdit->setGeometry(QRect(130, 30, 301, 25));
        label_25 = new QLabel(groupBox_8);
        label_25->setObjectName(QString::fromUtf8("label_25"));
        label_25->setGeometry(QRect(20, 30, 101, 25));
        label_25->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        decoderQuadButton = new QPushButton(groupBox_8);
        decoderQuadButton->setObjectName(QString::fromUtf8("decoderQuadButton"));
        decoderQuadButton->setGeometry(QRect(440, 30, 91, 25));
        decoder51LineEdit = new QLineEdit(groupBox_8);
        decoder51LineEdit->setObjectName(QString::fromUtf8("decoder51LineEdit"));
        decoder51LineEdit->setGeometry(QRect(130, 70, 301, 25));
        decoder51Button = new QPushButton(groupBox_8);
        decoder51Button->setObjectName(QString::fromUtf8("decoder51Button"));
        decoder51Button->setGeometry(QRect(440, 70, 91, 25));
        label_26 = new QLabel(groupBox_8);
        label_26->setObjectName(QString::fromUtf8("label_26"));
        label_26->setGeometry(QRect(20, 70, 101, 25));
        label_26->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_28 = new QLabel(groupBox_8);
        label_28->setObjectName(QString::fromUtf8("label_28"));
        label_28->setGeometry(QRect(20, 110, 101, 25));
        label_28->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        decoder61LineEdit = new QLineEdit(groupBox_8);
        decoder61LineEdit->setObjectName(QString::fromUtf8("decoder61LineEdit"));
        decoder61LineEdit->setGeometry(QRect(130, 110, 301, 25));
        decoder61Button = new QPushButton(groupBox_8);
        decoder61Button->setObjectName(QString::fromUtf8("decoder61Button"));
        decoder61Button->setGeometry(QRect(440, 110, 91, 25));
        decoder71Button = new QPushButton(groupBox_8);
        decoder71Button->setObjectName(QString::fromUtf8("decoder71Button"));
        decoder71Button->setGeometry(QRect(440, 150, 91, 25));
        decoder71LineEdit = new QLineEdit(groupBox_8);
        decoder71LineEdit->setObjectName(QString::fromUtf8("decoder71LineEdit"));
        decoder71LineEdit->setGeometry(QRect(130, 150, 301, 25));
        label_29 = new QLabel(groupBox_8);
        label_29->setObjectName(QString::fromUtf8("label_29"));
        label_29->setGeometry(QRect(20, 150, 101, 25));
        label_29->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_33 = new QLabel(groupBox_8);
        label_33->setObjectName(QString::fromUtf8("label_33"));
        label_33->setGeometry(QRect(20, 190, 101, 25));
        label_33->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        decoder3D71LineEdit = new QLineEdit(groupBox_8);
        decoder3D71LineEdit->setObjectName(QString::fromUtf8("decoder3D71LineEdit"));
        decoder3D71LineEdit->setGeometry(QRect(130, 190, 301, 25));
        decoder3D71Button = new QPushButton(groupBox_8);
        decoder3D71Button->setObjectName(QString::fromUtf8("decoder3D71Button"));
        decoder3D71Button->setGeometry(QRect(440, 190, 91, 25));
        decoderNFEffectsCheckBox = new QCheckBox(tab_6);
        decoderNFEffectsCheckBox->setObjectName(QString::fromUtf8("decoderNFEffectsCheckBox"));
        decoderNFEffectsCheckBox->setGeometry(QRect(30, 80, 181, 21));
        decoderNFEffectsCheckBox->setLayoutDirection(Qt::RightToLeft);
        decoderNFEffectsCheckBox->setTristate(true);
        widget_3 = new QWidget(tab_6);
        widget_3->setObjectName(QString::fromUtf8("widget_3"));
        widget_3->setGeometry(QRect(30, 110, 471, 21));
        label_27 = new QLabel(widget_3);
        label_27->setObjectName(QString::fromUtf8("label_27"));
        label_27->setGeometry(QRect(45, 0, 111, 21));
        label_27->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        decoderSpeakerDistSpinBox = new QDoubleSpinBox(widget_3);
        decoderSpeakerDistSpinBox->setObjectName(QString::fromUtf8("decoderSpeakerDistSpinBox"));
        decoderSpeakerDistSpinBox->setGeometry(QRect(165, 0, 101, 21));
        decoderSpeakerDistSpinBox->setDecimals(2);
        decoderSpeakerDistSpinBox->setMinimum(0.100000000000000);
        decoderSpeakerDistSpinBox->setMaximum(10.000000000000000);
        decoderSpeakerDistSpinBox->setSingleStep(0.010000000000000);
        decoderSpeakerDistSpinBox->setValue(1.000000000000000);
        tabWidget->addTab(tab_6, QString());
        tab_5 = new QWidget();
        tab_5->setObjectName(QString::fromUtf8("tab_5"));
        groupBox_2 = new QGroupBox(tab_5);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(-10, 200, 551, 181));
        groupBox_2->setAlignment(Qt::AlignCenter);
        groupBox_2->setCheckable(false);
        groupBox_2->setChecked(false);
        groupBox_6 = new QGroupBox(groupBox_2);
        groupBox_6->setObjectName(QString::fromUtf8("groupBox_6"));
        groupBox_6->setGeometry(QRect(20, 30, 511, 141));
        groupBox_6->setAlignment(Qt::AlignCenter);
        hrtfFileList = new QListWidget(groupBox_6);
        hrtfFileList->setObjectName(QString::fromUtf8("hrtfFileList"));
        hrtfFileList->setGeometry(QRect(20, 20, 391, 81));
        hrtfFileList->setDragDropMode(QAbstractItemView::InternalMove);
        hrtfFileList->setAlternatingRowColors(true);
        hrtfFileList->setSelectionMode(QAbstractItemView::ExtendedSelection);
        hrtfFileList->setTextElideMode(Qt::ElideNone);
        hrtfAddButton = new QPushButton(groupBox_6);
        hrtfAddButton->setObjectName(QString::fromUtf8("hrtfAddButton"));
        hrtfAddButton->setGeometry(QRect(420, 20, 81, 21));
        QIcon icon5;
        iconThemeName = QString::fromUtf8("list-add");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon5 = QIcon::fromTheme(iconThemeName);
        } else {
            icon5.addFile(QString::fromUtf8("."), QSize(), QIcon::Normal, QIcon::Off);
        }
        hrtfAddButton->setIcon(icon5);
        hrtfAddButton->setFlat(false);
        defaultHrtfPathsCheckBox = new QCheckBox(groupBox_6);
        defaultHrtfPathsCheckBox->setObjectName(QString::fromUtf8("defaultHrtfPathsCheckBox"));
        defaultHrtfPathsCheckBox->setGeometry(QRect(180, 110, 151, 21));
        defaultHrtfPathsCheckBox->setChecked(true);
        hrtfRemoveButton = new QPushButton(groupBox_6);
        hrtfRemoveButton->setObjectName(QString::fromUtf8("hrtfRemoveButton"));
        hrtfRemoveButton->setGeometry(QRect(420, 50, 81, 21));
        QIcon icon6;
        iconThemeName = QString::fromUtf8("list-remove");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon6 = QIcon::fromTheme(iconThemeName);
        } else {
            icon6.addFile(QString::fromUtf8("."), QSize(), QIcon::Normal, QIcon::Off);
        }
        hrtfRemoveButton->setIcon(icon6);
        label_12 = new QLabel(tab_5);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setGeometry(QRect(30, 20, 91, 31));
        label_12->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        preferredHrtfComboBox = new QComboBox(tab_5);
        preferredHrtfComboBox->setObjectName(QString::fromUtf8("preferredHrtfComboBox"));
        preferredHrtfComboBox->setGeometry(QRect(130, 20, 161, 31));
        groupBox_9 = new QGroupBox(tab_5);
        groupBox_9->setObjectName(QString::fromUtf8("groupBox_9"));
        groupBox_9->setGeometry(QRect(50, 100, 441, 81));
        label_31 = new QLabel(groupBox_9);
        label_31->setObjectName(QString::fromUtf8("label_31"));
        label_31->setGeometry(QRect(20, 30, 51, 21));
        label_31->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_32 = new QLabel(groupBox_9);
        label_32->setObjectName(QString::fromUtf8("label_32"));
        label_32->setGeometry(QRect(340, 30, 51, 21));
        hrtfmodeSlider = new QSlider(groupBox_9);
        hrtfmodeSlider->setObjectName(QString::fromUtf8("hrtfmodeSlider"));
        hrtfmodeSlider->setGeometry(QRect(80, 30, 251, 21));
        hrtfmodeSlider->setOrientation(Qt::Horizontal);
        hrtfmodeLabel = new QLabel(groupBox_9);
        hrtfmodeLabel->setObjectName(QString::fromUtf8("hrtfmodeLabel"));
        hrtfmodeLabel->setGeometry(QRect(50, 50, 321, 21));
        hrtfmodeLabel->setAlignment(Qt::AlignCenter);
        tabWidget->addTab(tab_5, QString());
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        backendListWidget = new QListWidget(tab);
        new QListWidgetItem(backendListWidget);
        new QListWidgetItem(backendListWidget);
        new QListWidgetItem(backendListWidget);
        new QListWidgetItem(backendListWidget);
        new QListWidgetItem(backendListWidget);
        new QListWidgetItem(backendListWidget);
        new QListWidgetItem(backendListWidget);
        new QListWidgetItem(backendListWidget);
        new QListWidgetItem(backendListWidget);
        backendListWidget->setObjectName(QString::fromUtf8("backendListWidget"));
        backendListWidget->setGeometry(QRect(0, 11, 111, 361));
        backendListWidget->setAlternatingRowColors(true);
        backendStackedWidget = new QStackedWidget(tab);
        backendStackedWidget->setObjectName(QString::fromUtf8("backendStackedWidget"));
        backendStackedWidget->setGeometry(QRect(110, 10, 421, 361));
        page = new QWidget();
        page->setObjectName(QString::fromUtf8("page"));
        backendCheckBox = new QCheckBox(page);
        backendCheckBox->setObjectName(QString::fromUtf8("backendCheckBox"));
        backendCheckBox->setGeometry(QRect(20, 190, 391, 21));
        backendCheckBox->setChecked(true);
        disabledBackendList = new QListWidget(page);
        disabledBackendList->setObjectName(QString::fromUtf8("disabledBackendList"));
        disabledBackendList->setGeometry(QRect(220, 30, 191, 151));
        enabledBackendList = new QListWidget(page);
        enabledBackendList->setObjectName(QString::fromUtf8("enabledBackendList"));
        enabledBackendList->setGeometry(QRect(20, 30, 191, 151));
        enabledBackendList->setDragDropMode(QAbstractItemView::InternalMove);
        label_2 = new QLabel(page);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(230, 10, 171, 20));
        label = new QLabel(page);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(30, 10, 171, 20));
        backendStackedWidget->addWidget(page);
        page_1 = new QWidget();
        page_1->setObjectName(QString::fromUtf8("page_1"));
        pwireAssumeAudioCheckBox = new QCheckBox(page_1);
        pwireAssumeAudioCheckBox->setObjectName(QString::fromUtf8("pwireAssumeAudioCheckBox"));
        pwireAssumeAudioCheckBox->setGeometry(QRect(20, 10, 161, 21));
        pwireAssumeAudioCheckBox->setTristate(true);
        pwireRtMixCheckBox = new QCheckBox(page_1);
        pwireRtMixCheckBox->setObjectName(QString::fromUtf8("pwireRtMixCheckBox"));
        pwireRtMixCheckBox->setGeometry(QRect(20, 40, 161, 21));
        pwireRtMixCheckBox->setTristate(true);
        backendStackedWidget->addWidget(page_1);
        page_2 = new QWidget();
        page_2->setObjectName(QString::fromUtf8("page_2"));
        wasapiResamplerCheckBox = new QCheckBox(page_2);
        wasapiResamplerCheckBox->setObjectName(QString::fromUtf8("wasapiResamplerCheckBox"));
        wasapiResamplerCheckBox->setGeometry(QRect(20, 10, 191, 21));
        wasapiResamplerCheckBox->setTristate(true);
        backendStackedWidget->addWidget(page_2);
        page_8 = new QWidget();
        page_8->setObjectName(QString::fromUtf8("page_8"));
        pulseAutospawnCheckBox = new QCheckBox(page_8);
        pulseAutospawnCheckBox->setObjectName(QString::fromUtf8("pulseAutospawnCheckBox"));
        pulseAutospawnCheckBox->setGeometry(QRect(20, 10, 141, 21));
        pulseAutospawnCheckBox->setTristate(true);
        pulseAllowMovesCheckBox = new QCheckBox(page_8);
        pulseAllowMovesCheckBox->setObjectName(QString::fromUtf8("pulseAllowMovesCheckBox"));
        pulseAllowMovesCheckBox->setGeometry(QRect(20, 40, 161, 21));
        pulseAllowMovesCheckBox->setTristate(true);
        pulseFixRateCheckBox = new QCheckBox(page_8);
        pulseFixRateCheckBox->setObjectName(QString::fromUtf8("pulseFixRateCheckBox"));
        pulseFixRateCheckBox->setGeometry(QRect(20, 70, 121, 21));
        pulseFixRateCheckBox->setTristate(true);
        pulseAdjLatencyCheckBox = new QCheckBox(page_8);
        pulseAdjLatencyCheckBox->setObjectName(QString::fromUtf8("pulseAdjLatencyCheckBox"));
        pulseAdjLatencyCheckBox->setGeometry(QRect(20, 100, 111, 21));
        pulseAdjLatencyCheckBox->setTristate(true);
        backendStackedWidget->addWidget(page_8);
        page_7 = new QWidget();
        page_7->setObjectName(QString::fromUtf8("page_7"));
        jackAutospawnCheckBox = new QCheckBox(page_7);
        jackAutospawnCheckBox->setObjectName(QString::fromUtf8("jackAutospawnCheckBox"));
        jackAutospawnCheckBox->setGeometry(QRect(20, 10, 141, 21));
        jackAutospawnCheckBox->setTristate(true);
        groupBox_7 = new QGroupBox(page_7);
        groupBox_7->setObjectName(QString::fromUtf8("groupBox_7"));
        groupBox_7->setGeometry(QRect(10, 110, 401, 80));
        groupBox_7->setAlignment(Qt::AlignCenter);
        jackBufferSizeLine = new QLineEdit(groupBox_7);
        jackBufferSizeLine->setObjectName(QString::fromUtf8("jackBufferSizeLine"));
        jackBufferSizeLine->setGeometry(QRect(320, 30, 71, 21));
        jackBufferSizeSlider = new QSlider(groupBox_7);
        jackBufferSizeSlider->setObjectName(QString::fromUtf8("jackBufferSizeSlider"));
        jackBufferSizeSlider->setGeometry(QRect(10, 30, 301, 21));
        jackBufferSizeSlider->setMaximum(13);
        jackBufferSizeSlider->setSingleStep(1);
        jackBufferSizeSlider->setPageStep(4);
        jackBufferSizeSlider->setOrientation(Qt::Horizontal);
        jackBufferSizeSlider->setTickPosition(QSlider::TicksBelow);
        jackBufferSizeSlider->setTickInterval(1);
        jackConnectPortsCheckBox = new QCheckBox(page_7);
        jackConnectPortsCheckBox->setObjectName(QString::fromUtf8("jackConnectPortsCheckBox"));
        jackConnectPortsCheckBox->setGeometry(QRect(20, 40, 141, 21));
        jackConnectPortsCheckBox->setTristate(true);
        jackRtMixCheckBox = new QCheckBox(page_7);
        jackRtMixCheckBox->setObjectName(QString::fromUtf8("jackRtMixCheckBox"));
        jackRtMixCheckBox->setGeometry(QRect(20, 70, 141, 21));
        jackRtMixCheckBox->setTristate(true);
        backendStackedWidget->addWidget(page_7);
        page_3 = new QWidget();
        page_3->setObjectName(QString::fromUtf8("page_3"));
        label_17 = new QLabel(page_3);
        label_17->setObjectName(QString::fromUtf8("label_17"));
        label_17->setGeometry(QRect(10, 30, 141, 21));
        label_17->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        alsaDefaultDeviceLine = new QLineEdit(page_3);
        alsaDefaultDeviceLine->setObjectName(QString::fromUtf8("alsaDefaultDeviceLine"));
        alsaDefaultDeviceLine->setGeometry(QRect(160, 30, 231, 21));
        label_18 = new QLabel(page_3);
        label_18->setObjectName(QString::fromUtf8("label_18"));
        label_18->setGeometry(QRect(10, 60, 141, 21));
        label_18->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        alsaDefaultCaptureLine = new QLineEdit(page_3);
        alsaDefaultCaptureLine->setObjectName(QString::fromUtf8("alsaDefaultCaptureLine"));
        alsaDefaultCaptureLine->setGeometry(QRect(160, 60, 231, 21));
        alsaResamplerCheckBox = new QCheckBox(page_3);
        alsaResamplerCheckBox->setObjectName(QString::fromUtf8("alsaResamplerCheckBox"));
        alsaResamplerCheckBox->setGeometry(QRect(20, 100, 191, 21));
        alsaResamplerCheckBox->setTristate(true);
        alsaMmapCheckBox = new QCheckBox(page_3);
        alsaMmapCheckBox->setObjectName(QString::fromUtf8("alsaMmapCheckBox"));
        alsaMmapCheckBox->setGeometry(QRect(210, 100, 191, 21));
        alsaMmapCheckBox->setTristate(true);
        backendStackedWidget->addWidget(page_3);
        page_4 = new QWidget();
        page_4->setObjectName(QString::fromUtf8("page_4"));
        label_20 = new QLabel(page_4);
        label_20->setObjectName(QString::fromUtf8("label_20"));
        label_20->setGeometry(QRect(10, 30, 141, 21));
        label_20->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        ossDefaultDeviceLine = new QLineEdit(page_4);
        ossDefaultDeviceLine->setObjectName(QString::fromUtf8("ossDefaultDeviceLine"));
        ossDefaultDeviceLine->setGeometry(QRect(160, 30, 151, 21));
        label_21 = new QLabel(page_4);
        label_21->setObjectName(QString::fromUtf8("label_21"));
        label_21->setGeometry(QRect(10, 60, 141, 21));
        label_21->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        ossDefaultCaptureLine = new QLineEdit(page_4);
        ossDefaultCaptureLine->setObjectName(QString::fromUtf8("ossDefaultCaptureLine"));
        ossDefaultCaptureLine->setGeometry(QRect(160, 60, 151, 21));
        ossPlaybackPushButton = new QPushButton(page_4);
        ossPlaybackPushButton->setObjectName(QString::fromUtf8("ossPlaybackPushButton"));
        ossPlaybackPushButton->setGeometry(QRect(320, 30, 91, 21));
        ossCapturePushButton = new QPushButton(page_4);
        ossCapturePushButton->setObjectName(QString::fromUtf8("ossCapturePushButton"));
        ossCapturePushButton->setGeometry(QRect(320, 60, 91, 21));
        backendStackedWidget->addWidget(page_4);
        page_5 = new QWidget();
        page_5->setObjectName(QString::fromUtf8("page_5"));
        solarisDefaultDeviceLine = new QLineEdit(page_5);
        solarisDefaultDeviceLine->setObjectName(QString::fromUtf8("solarisDefaultDeviceLine"));
        solarisDefaultDeviceLine->setGeometry(QRect(160, 30, 151, 21));
        label_22 = new QLabel(page_5);
        label_22->setObjectName(QString::fromUtf8("label_22"));
        label_22->setGeometry(QRect(10, 30, 141, 21));
        label_22->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        solarisPlaybackPushButton = new QPushButton(page_5);
        solarisPlaybackPushButton->setObjectName(QString::fromUtf8("solarisPlaybackPushButton"));
        solarisPlaybackPushButton->setGeometry(QRect(320, 30, 91, 21));
        backendStackedWidget->addWidget(page_5);
        page_6 = new QWidget();
        page_6->setObjectName(QString::fromUtf8("page_6"));
        label_23 = new QLabel(page_6);
        label_23->setObjectName(QString::fromUtf8("label_23"));
        label_23->setGeometry(QRect(10, 30, 71, 21));
        label_23->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        waveOutputLine = new QLineEdit(page_6);
        waveOutputLine->setObjectName(QString::fromUtf8("waveOutputLine"));
        waveOutputLine->setGeometry(QRect(90, 30, 221, 21));
        label_24 = new QLabel(page_6);
        label_24->setObjectName(QString::fromUtf8("label_24"));
        label_24->setGeometry(QRect(0, 90, 421, 71));
        waveOutputButton = new QPushButton(page_6);
        waveOutputButton->setObjectName(QString::fromUtf8("waveOutputButton"));
        waveOutputButton->setGeometry(QRect(320, 30, 91, 21));
        waveBFormatCheckBox = new QCheckBox(page_6);
        waveBFormatCheckBox->setObjectName(QString::fromUtf8("waveBFormatCheckBox"));
        waveBFormatCheckBox->setGeometry(QRect(120, 60, 191, 21));
        backendStackedWidget->addWidget(page_6);
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        srcCountLineEdit = new QLineEdit(tab_2);
        srcCountLineEdit->setObjectName(QString::fromUtf8("srcCountLineEdit"));
        srcCountLineEdit->setGeometry(QRect(190, 20, 51, 21));
        srcCountLineEdit->setMaxLength(4);
        label_3 = new QLabel(tab_2);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(10, 20, 171, 21));
        label_3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_4 = new QLabel(tab_2);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(10, 50, 171, 21));
        label_4->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        effectSlotLineEdit = new QLineEdit(tab_2);
        effectSlotLineEdit->setObjectName(QString::fromUtf8("effectSlotLineEdit"));
        effectSlotLineEdit->setGeometry(QRect(190, 50, 51, 21));
        effectSlotLineEdit->setMaxLength(3);
        label_8 = new QLabel(tab_2);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(10, 80, 171, 21));
        label_8->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        srcSendLineEdit = new QLineEdit(tab_2);
        srcSendLineEdit->setObjectName(QString::fromUtf8("srcSendLineEdit"));
        srcSendLineEdit->setGeometry(QRect(190, 80, 51, 21));
        srcSendLineEdit->setMaxLength(2);
        cpuExtGroupBox = new QGroupBox(tab_2);
        cpuExtGroupBox->setObjectName(QString::fromUtf8("cpuExtGroupBox"));
        cpuExtGroupBox->setGeometry(QRect(10, 120, 511, 121));
        enableSSECheckBox = new QCheckBox(cpuExtGroupBox);
        enableSSECheckBox->setObjectName(QString::fromUtf8("enableSSECheckBox"));
        enableSSECheckBox->setGeometry(QRect(100, 20, 71, 31));
        enableSSECheckBox->setChecked(true);
        enableSSE2CheckBox = new QCheckBox(cpuExtGroupBox);
        enableSSE2CheckBox->setObjectName(QString::fromUtf8("enableSSE2CheckBox"));
        enableSSE2CheckBox->setGeometry(QRect(180, 20, 71, 31));
        enableSSE2CheckBox->setChecked(true);
        enableNeonCheckBox = new QCheckBox(cpuExtGroupBox);
        enableNeonCheckBox->setObjectName(QString::fromUtf8("enableNeonCheckBox"));
        enableNeonCheckBox->setGeometry(QRect(100, 50, 71, 31));
        enableNeonCheckBox->setChecked(true);
        enableSSE41CheckBox = new QCheckBox(cpuExtGroupBox);
        enableSSE41CheckBox->setObjectName(QString::fromUtf8("enableSSE41CheckBox"));
        enableSSE41CheckBox->setGeometry(QRect(340, 20, 71, 31));
        enableSSE41CheckBox->setChecked(true);
        enableSSE3CheckBox = new QCheckBox(cpuExtGroupBox);
        enableSSE3CheckBox->setObjectName(QString::fromUtf8("enableSSE3CheckBox"));
        enableSSE3CheckBox->setGeometry(QRect(260, 20, 71, 31));
        enableSSE3CheckBox->setChecked(true);
        cpuExtDisabledLabel = new QLabel(cpuExtGroupBox);
        cpuExtDisabledLabel->setObjectName(QString::fromUtf8("cpuExtDisabledLabel"));
        cpuExtDisabledLabel->setGeometry(QRect(101, 80, 311, 31));
        tabWidget->addTab(tab_2, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName(QString::fromUtf8("tab_4"));
        groupBox_5 = new QGroupBox(tab_4);
        groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
        groupBox_5->setGeometry(QRect(10, 60, 511, 241));
        enableEaxReverbCheck = new QCheckBox(groupBox_5);
        enableEaxReverbCheck->setObjectName(QString::fromUtf8("enableEaxReverbCheck"));
        enableEaxReverbCheck->setGeometry(QRect(70, 30, 131, 21));
        enableEaxReverbCheck->setChecked(true);
        enableStdReverbCheck = new QCheckBox(groupBox_5);
        enableStdReverbCheck->setObjectName(QString::fromUtf8("enableStdReverbCheck"));
        enableStdReverbCheck->setGeometry(QRect(70, 60, 131, 21));
        enableStdReverbCheck->setChecked(true);
        enableChorusCheck = new QCheckBox(groupBox_5);
        enableChorusCheck->setObjectName(QString::fromUtf8("enableChorusCheck"));
        enableChorusCheck->setGeometry(QRect(70, 90, 131, 21));
        enableChorusCheck->setChecked(true);
        enableDistortionCheck = new QCheckBox(groupBox_5);
        enableDistortionCheck->setObjectName(QString::fromUtf8("enableDistortionCheck"));
        enableDistortionCheck->setGeometry(QRect(70, 150, 131, 21));
        enableDistortionCheck->setChecked(true);
        enableEchoCheck = new QCheckBox(groupBox_5);
        enableEchoCheck->setObjectName(QString::fromUtf8("enableEchoCheck"));
        enableEchoCheck->setGeometry(QRect(70, 180, 131, 21));
        enableEchoCheck->setChecked(true);
        enableEqualizerCheck = new QCheckBox(groupBox_5);
        enableEqualizerCheck->setObjectName(QString::fromUtf8("enableEqualizerCheck"));
        enableEqualizerCheck->setGeometry(QRect(320, 30, 131, 21));
        enableEqualizerCheck->setChecked(true);
        enableFlangerCheck = new QCheckBox(groupBox_5);
        enableFlangerCheck->setObjectName(QString::fromUtf8("enableFlangerCheck"));
        enableFlangerCheck->setGeometry(QRect(320, 90, 131, 21));
        enableFlangerCheck->setChecked(true);
        enableModulatorCheck = new QCheckBox(groupBox_5);
        enableModulatorCheck->setObjectName(QString::fromUtf8("enableModulatorCheck"));
        enableModulatorCheck->setGeometry(QRect(320, 150, 131, 21));
        enableModulatorCheck->setChecked(true);
        enableDedicatedCheck = new QCheckBox(groupBox_5);
        enableDedicatedCheck->setObjectName(QString::fromUtf8("enableDedicatedCheck"));
        enableDedicatedCheck->setGeometry(QRect(320, 180, 131, 21));
        enableDedicatedCheck->setChecked(true);
        enableCompressorCheck = new QCheckBox(groupBox_5);
        enableCompressorCheck->setObjectName(QString::fromUtf8("enableCompressorCheck"));
        enableCompressorCheck->setGeometry(QRect(70, 120, 111, 21));
        enableCompressorCheck->setChecked(true);
        enablePitchShifterCheck = new QCheckBox(groupBox_5);
        enablePitchShifterCheck->setObjectName(QString::fromUtf8("enablePitchShifterCheck"));
        enablePitchShifterCheck->setGeometry(QRect(320, 120, 131, 21));
        enablePitchShifterCheck->setChecked(true);
        enableFrequencyShifterCheck = new QCheckBox(groupBox_5);
        enableFrequencyShifterCheck->setObjectName(QString::fromUtf8("enableFrequencyShifterCheck"));
        enableFrequencyShifterCheck->setGeometry(QRect(320, 60, 131, 21));
        enableFrequencyShifterCheck->setChecked(true);
        enableAutowahCheck = new QCheckBox(groupBox_5);
        enableAutowahCheck->setObjectName(QString::fromUtf8("enableAutowahCheck"));
        enableAutowahCheck->setGeometry(QRect(70, 210, 131, 21));
        enableAutowahCheck->setChecked(true);
        enableVocalMorpherCheck = new QCheckBox(groupBox_5);
        enableVocalMorpherCheck->setObjectName(QString::fromUtf8("enableVocalMorpherCheck"));
        enableVocalMorpherCheck->setGeometry(QRect(320, 210, 131, 21));
        enableVocalMorpherCheck->setChecked(true);
        label_13 = new QLabel(tab_4);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setGeometry(QRect(10, 20, 141, 31));
        label_13->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        defaultReverbComboBox = new QComboBox(tab_4);
        defaultReverbComboBox->addItem(QString());
        defaultReverbComboBox->addItem(QString());
        defaultReverbComboBox->addItem(QString());
        defaultReverbComboBox->addItem(QString());
        defaultReverbComboBox->addItem(QString());
        defaultReverbComboBox->addItem(QString());
        defaultReverbComboBox->addItem(QString());
        defaultReverbComboBox->addItem(QString());
        defaultReverbComboBox->addItem(QString());
        defaultReverbComboBox->addItem(QString());
        defaultReverbComboBox->addItem(QString());
        defaultReverbComboBox->addItem(QString());
        defaultReverbComboBox->addItem(QString());
        defaultReverbComboBox->addItem(QString());
        defaultReverbComboBox->addItem(QString());
        defaultReverbComboBox->addItem(QString());
        defaultReverbComboBox->addItem(QString());
        defaultReverbComboBox->addItem(QString());
        defaultReverbComboBox->addItem(QString());
        defaultReverbComboBox->addItem(QString());
        defaultReverbComboBox->addItem(QString());
        defaultReverbComboBox->addItem(QString());
        defaultReverbComboBox->addItem(QString());
        defaultReverbComboBox->addItem(QString());
        defaultReverbComboBox->addItem(QString());
        defaultReverbComboBox->addItem(QString());
        defaultReverbComboBox->addItem(QString());
        defaultReverbComboBox->setObjectName(QString::fromUtf8("defaultReverbComboBox"));
        defaultReverbComboBox->setGeometry(QRect(160, 20, 135, 31));
        defaultReverbComboBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
        enableEaxCheck = new QCheckBox(tab_4);
        enableEaxCheck->setObjectName(QString::fromUtf8("enableEaxCheck"));
        enableEaxCheck->setGeometry(QRect(30, 320, 231, 21));
        tabWidget->addTab(tab_4, QString());
        closeCancelButton = new QPushButton(centralWidget);
        closeCancelButton->setObjectName(QString::fromUtf8("closeCancelButton"));
        closeCancelButton->setGeometry(QRect(370, 405, 91, 31));
        QIcon icon7;
        iconThemeName = QString::fromUtf8("window-close");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon7 = QIcon::fromTheme(iconThemeName);
        } else {
            icon7.addFile(QString::fromUtf8("."), QSize(), QIcon::Normal, QIcon::Off);
        }
        closeCancelButton->setIcon(icon7);
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 564, 29));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        MainWindow->setMenuBar(menuBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionLoad);
        menuFile->addAction(actionSave_As);
        menuFile->addSeparator();
        menuFile->addAction(actionQuit);
        menuHelp->addAction(actionAbout);

        retranslateUi(MainWindow);
        QObject::connect(backendListWidget, SIGNAL(currentRowChanged(int)), backendStackedWidget, SLOT(setCurrentIndex(int)));

        tabWidget->setCurrentIndex(0);
        backendStackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "OpenAL Soft Configuration", nullptr));
        actionQuit->setText(QCoreApplication::translate("MainWindow", "&Quit", nullptr));
        actionSave_As->setText(QCoreApplication::translate("MainWindow", "Save &As...", nullptr));
#if QT_CONFIG(tooltip)
        actionSave_As->setToolTip(QCoreApplication::translate("MainWindow", "Save Configuration As", nullptr));
#endif // QT_CONFIG(tooltip)
        actionLoad->setText(QCoreApplication::translate("MainWindow", "&Load...", nullptr));
#if QT_CONFIG(tooltip)
        actionLoad->setToolTip(QCoreApplication::translate("MainWindow", "Load Configuration File", nullptr));
#endif // QT_CONFIG(tooltip)
        actionAbout->setText(QCoreApplication::translate("MainWindow", "&About...", nullptr));
        applyButton->setText(QCoreApplication::translate("MainWindow", "Apply", nullptr));
#if QT_CONFIG(tooltip)
        sampleFormatCombo->setToolTip(QCoreApplication::translate("MainWindow", "The output sample type. Currently, all mixing is done with 32-bit\n"
"float and converted to the output sample type as needed.", nullptr));
#endif // QT_CONFIG(tooltip)
        label_5->setText(QCoreApplication::translate("MainWindow", "Sample Format:", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "Channels:", nullptr));
#if QT_CONFIG(tooltip)
        channelConfigCombo->setToolTip(QCoreApplication::translate("MainWindow", "The default output channel configuration. Note that not all\n"
"backends can properly detect the channel configuration and\n"
"may default to stereo output.", nullptr));
#endif // QT_CONFIG(tooltip)
        sampleRateCombo->setItemText(0, QCoreApplication::translate("MainWindow", "Autodetect", nullptr));
        sampleRateCombo->setItemText(1, QCoreApplication::translate("MainWindow", "8000", nullptr));
        sampleRateCombo->setItemText(2, QCoreApplication::translate("MainWindow", "11025", nullptr));
        sampleRateCombo->setItemText(3, QCoreApplication::translate("MainWindow", "16000", nullptr));
        sampleRateCombo->setItemText(4, QCoreApplication::translate("MainWindow", "22050", nullptr));
        sampleRateCombo->setItemText(5, QCoreApplication::translate("MainWindow", "32000", nullptr));
        sampleRateCombo->setItemText(6, QCoreApplication::translate("MainWindow", "44100", nullptr));
        sampleRateCombo->setItemText(7, QCoreApplication::translate("MainWindow", "48000", nullptr));

#if QT_CONFIG(tooltip)
        sampleRateCombo->setToolTip(QCoreApplication::translate("MainWindow", "The playback/mixing sample rate.", nullptr));
#endif // QT_CONFIG(tooltip)
        label_7->setText(QCoreApplication::translate("MainWindow", "Sample Rate:", nullptr));
        label_14->setText(QCoreApplication::translate("MainWindow", "Stereo Mode:", nullptr));
#if QT_CONFIG(tooltip)
        stereoModeCombo->setToolTip(QCoreApplication::translate("MainWindow", "How to treat stereo output. As headphones, HRTF or crossfeed\n"
"filters may be used to improve binaural quality, which may not\n"
"otherwise be suitable for speakers.", nullptr));
#endif // QT_CONFIG(tooltip)
        groupBox->setTitle(QCoreApplication::translate("MainWindow", "Advanced Settings", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("MainWindow", "Buffer Metrics", nullptr));
#if QT_CONFIG(tooltip)
        widget->setToolTip(QCoreApplication::translate("MainWindow", "The number of update periods. Higher values create a larger\n"
"mix ahead, which helps protect against skips when the CPU is\n"
"under load, but increases the delay between a sound getting\n"
"mixed and being heard.", nullptr));
#endif // QT_CONFIG(tooltip)
        label_11->setText(QCoreApplication::translate("MainWindow", "Period Count", nullptr));
        periodCountEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "3", nullptr));
#if QT_CONFIG(tooltip)
        widget_2->setToolTip(QCoreApplication::translate("MainWindow", "The update period size, in sample frames. This is the number of\n"
"frames needed for each mixing update.", nullptr));
#endif // QT_CONFIG(tooltip)
        label_10->setText(QCoreApplication::translate("MainWindow", "Period Samples", nullptr));
        periodSizeEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "20ms", nullptr));
#if QT_CONFIG(tooltip)
        stereoEncodingComboBox->setToolTip(QCoreApplication::translate("MainWindow", "Basic uses standard amplitude panning (aka\n"
"pair-wise, stereo pair, etc).\n"
"\n"
"UHJ creates a stereo-compatible two-channel\n"
"UHJ mix, which encodes some surround sound\n"
"information into stereo output that can be\n"
"decoded with a surround sound receiver.\n"
"\n"
"Binaural applies HRTF filters to create an\n"
"illusion of 3D placement with headphones.", nullptr));
#endif // QT_CONFIG(tooltip)
        label_19->setText(QCoreApplication::translate("MainWindow", "Stereo Encoding:", nullptr));
        label_30->setText(QCoreApplication::translate("MainWindow", "Ambisonic Format:", nullptr));
#if QT_CONFIG(tooltip)
        outputLimiterCheckBox->setToolTip(QCoreApplication::translate("MainWindow", "Applies a gain limiter on the final mixed output. This reduces the\n"
"volume when the output samples would otherwise be clamped,\n"
"avoiding excessive clipping noise.", nullptr));
#endif // QT_CONFIG(tooltip)
        outputLimiterCheckBox->setText(QCoreApplication::translate("MainWindow", "Enable Gain Limiter", nullptr));
#if QT_CONFIG(tooltip)
        outputDitherCheckBox->setToolTip(QCoreApplication::translate("MainWindow", "Applies dithering on the final mix for 8- and 16-bit output.\n"
"This replaces the distortion created by nearest-value\n"
"quantization with low-level whitenoise.", nullptr));
#endif // QT_CONFIG(tooltip)
        outputDitherCheckBox->setText(QCoreApplication::translate("MainWindow", "Enable Dithering", nullptr));
        groupBox_4->setTitle(QCoreApplication::translate("MainWindow", "Resampler Quality", nullptr));
        resamplerLabel->setText(QCoreApplication::translate("MainWindow", "Default", nullptr));
        label_9->setText(QCoreApplication::translate("MainWindow", "Speed", nullptr));
        label_15->setText(QCoreApplication::translate("MainWindow", "Quality", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QCoreApplication::translate("MainWindow", "Playback", nullptr));
#if QT_CONFIG(tooltip)
        decoderHQModeCheckBox->setToolTip(QCoreApplication::translate("MainWindow", "Enables high-quality ambisonic rendering. This mode is\n"
"capable of frequency-dependent processing, creating a\n"
"better reproduction of 3D sound rendering over\n"
"surround sound speakers.", nullptr));
#endif // QT_CONFIG(tooltip)
        decoderHQModeCheckBox->setText(QCoreApplication::translate("MainWindow", "High Quality Mode:", nullptr));
#if QT_CONFIG(tooltip)
        decoderDistCompCheckBox->setToolTip(QCoreApplication::translate("MainWindow", "This applies the necessary delays and attenuation\n"
"to make the speakers behave as though they are\n"
"all equidistant, which is important for proper\n"
"playback of 3D sound rendering. Requires the\n"
"proper distances to be specified in the decoder\n"
"configuration file.", nullptr));
#endif // QT_CONFIG(tooltip)
        decoderDistCompCheckBox->setText(QCoreApplication::translate("MainWindow", "Distance Compensation:", nullptr));
        groupBox_8->setTitle(QCoreApplication::translate("MainWindow", "Decoder Configurations", nullptr));
        label_25->setText(QCoreApplication::translate("MainWindow", "Quadraphonic:", nullptr));
        decoderQuadButton->setText(QCoreApplication::translate("MainWindow", "Browse...", nullptr));
        decoder51Button->setText(QCoreApplication::translate("MainWindow", "Browse...", nullptr));
        label_26->setText(QCoreApplication::translate("MainWindow", "5.1 Surround:", nullptr));
        label_28->setText(QCoreApplication::translate("MainWindow", "6.1 Surround:", nullptr));
        decoder61Button->setText(QCoreApplication::translate("MainWindow", "Browse...", nullptr));
        decoder71Button->setText(QCoreApplication::translate("MainWindow", "Browse...", nullptr));
        label_29->setText(QCoreApplication::translate("MainWindow", "7.1 Surround:", nullptr));
        label_33->setText(QCoreApplication::translate("MainWindow", "3D7.1 Surround:", nullptr));
        decoder3D71Button->setText(QCoreApplication::translate("MainWindow", "Browse...", nullptr));
#if QT_CONFIG(tooltip)
        decoderNFEffectsCheckBox->setToolTip(QCoreApplication::translate("MainWindow", "Simulates and compensates for low-frequency effects\n"
"caused by the curvature of nearby sound-waves, which\n"
"creates a more realistic perception of sound distance.\n"
"Note that the effect may be stronger or weaker than\n"
"intended if the application doesn't use or specify an\n"
"appropriate unit scale, or if incorrect speaker distances\n"
"are set in the decoder configuration file.", nullptr));
#endif // QT_CONFIG(tooltip)
        decoderNFEffectsCheckBox->setText(QCoreApplication::translate("MainWindow", "Near-Field Effects:", nullptr));
#if QT_CONFIG(tooltip)
        widget_3->setToolTip(QCoreApplication::translate("MainWindow", "Specifies the speaker distance in meters, used by the near-field control\n"
"filters with surround sound output. For ambisonic output modes, this value\n"
"is the basis for the NFC-HOA Reference Delay parameter (calculated as\n"
"delay_seconds = speaker_dist/343.3). This value is not used when a decoder\n"
"configuration is set for the output mode (since they specify the per-speaker\n"
"distances, overriding this setting), or when the NFC filters are off.", nullptr));
#endif // QT_CONFIG(tooltip)
        label_27->setText(QCoreApplication::translate("MainWindow", "Speaker Distance:", nullptr));
        decoderSpeakerDistSpinBox->setSuffix(QCoreApplication::translate("MainWindow", " meters", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_6), QCoreApplication::translate("MainWindow", "Renderer", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("MainWindow", "Advanced Settings", nullptr));
        groupBox_6->setTitle(QCoreApplication::translate("MainWindow", "HRTF Profile Paths", nullptr));
#if QT_CONFIG(tooltip)
        hrtfFileList->setToolTip(QCoreApplication::translate("MainWindow", "A list of additional paths containing HRTF data sets.", nullptr));
#endif // QT_CONFIG(tooltip)
        hrtfAddButton->setText(QCoreApplication::translate("MainWindow", "Add...", nullptr));
#if QT_CONFIG(tooltip)
        defaultHrtfPathsCheckBox->setToolTip(QCoreApplication::translate("MainWindow", "Include the default system paths in addition to any\n"
"listed above.", nullptr));
#endif // QT_CONFIG(tooltip)
        defaultHrtfPathsCheckBox->setText(QCoreApplication::translate("MainWindow", "Include Default Paths", nullptr));
        hrtfRemoveButton->setText(QCoreApplication::translate("MainWindow", "Remove", nullptr));
        label_12->setText(QCoreApplication::translate("MainWindow", "Preferred HRTF:", nullptr));
#if QT_CONFIG(tooltip)
        preferredHrtfComboBox->setToolTip(QCoreApplication::translate("MainWindow", "The default HRTF to use if the application doesn't request one.", nullptr));
#endif // QT_CONFIG(tooltip)
        groupBox_9->setTitle(QCoreApplication::translate("MainWindow", "HRTF Render Method", nullptr));
        label_31->setText(QCoreApplication::translate("MainWindow", "Speed", nullptr));
        label_32->setText(QCoreApplication::translate("MainWindow", "Quality", nullptr));
        hrtfmodeLabel->setText(QCoreApplication::translate("MainWindow", "Default", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_5), QCoreApplication::translate("MainWindow", "HRTF", nullptr));

        const bool __sortingEnabled = backendListWidget->isSortingEnabled();
        backendListWidget->setSortingEnabled(false);
        QListWidgetItem *___qlistwidgetitem = backendListWidget->item(0);
        ___qlistwidgetitem->setText(QCoreApplication::translate("MainWindow", "General", nullptr));
        QListWidgetItem *___qlistwidgetitem1 = backendListWidget->item(1);
        ___qlistwidgetitem1->setText(QCoreApplication::translate("MainWindow", "PipeWire", nullptr));
        QListWidgetItem *___qlistwidgetitem2 = backendListWidget->item(2);
        ___qlistwidgetitem2->setText(QCoreApplication::translate("MainWindow", "WASAPI", nullptr));
        QListWidgetItem *___qlistwidgetitem3 = backendListWidget->item(3);
        ___qlistwidgetitem3->setText(QCoreApplication::translate("MainWindow", "PulseAudio", nullptr));
        QListWidgetItem *___qlistwidgetitem4 = backendListWidget->item(4);
        ___qlistwidgetitem4->setText(QCoreApplication::translate("MainWindow", "JACK", nullptr));
        QListWidgetItem *___qlistwidgetitem5 = backendListWidget->item(5);
        ___qlistwidgetitem5->setText(QCoreApplication::translate("MainWindow", "ALSA", nullptr));
        QListWidgetItem *___qlistwidgetitem6 = backendListWidget->item(6);
        ___qlistwidgetitem6->setText(QCoreApplication::translate("MainWindow", "OSS", nullptr));
        QListWidgetItem *___qlistwidgetitem7 = backendListWidget->item(7);
        ___qlistwidgetitem7->setText(QCoreApplication::translate("MainWindow", "Solaris", nullptr));
        QListWidgetItem *___qlistwidgetitem8 = backendListWidget->item(8);
        ___qlistwidgetitem8->setText(QCoreApplication::translate("MainWindow", "Wave Writer", nullptr));
        backendListWidget->setSortingEnabled(__sortingEnabled);

#if QT_CONFIG(tooltip)
        backendCheckBox->setToolTip(QCoreApplication::translate("MainWindow", "When checked, allows all other available backends not listed in the priority or disabled lists.", nullptr));
#endif // QT_CONFIG(tooltip)
        backendCheckBox->setText(QCoreApplication::translate("MainWindow", "Allow Other Backends", nullptr));
#if QT_CONFIG(tooltip)
        disabledBackendList->setToolTip(QCoreApplication::translate("MainWindow", "Disabled backend driver list.", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(tooltip)
        enabledBackendList->setToolTip(QCoreApplication::translate("MainWindow", "The backend driver list order. Unknown backends and\n"
"duplicated names are ignored.", nullptr));
#endif // QT_CONFIG(tooltip)
        label_2->setText(QCoreApplication::translate("MainWindow", "Disabled Backends:", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Priority Backends:", nullptr));
#if QT_CONFIG(tooltip)
        pwireAssumeAudioCheckBox->setToolTip(QCoreApplication::translate("MainWindow", "Assumes PipeWire has support for audio, allowing\n"
"the backend to initialize even when no audio devices\n"
"are reported.", nullptr));
#endif // QT_CONFIG(tooltip)
        pwireAssumeAudioCheckBox->setText(QCoreApplication::translate("MainWindow", "Assume audio support", nullptr));
#if QT_CONFIG(tooltip)
        pwireRtMixCheckBox->setToolTip(QCoreApplication::translate("MainWindow", "Renders samples directly in the real-time\n"
"processing callback. This allows for lower\n"
"latency and less overall CPU utilization, but\n"
"can increase the risk of underruns when\n"
"increasing the amount of processing the\n"
"mixer needs to do.", nullptr));
#endif // QT_CONFIG(tooltip)
        pwireRtMixCheckBox->setText(QCoreApplication::translate("MainWindow", "Real-time Mixing", nullptr));
#if QT_CONFIG(tooltip)
        wasapiResamplerCheckBox->setToolTip(QCoreApplication::translate("MainWindow", "Specifies whether to allow an extra resampler pass on the output. Enabling\n"
"this will allow the playback device to be set to a different sample rate\n"
"than the actual output can accept, causing the backend to apply its own\n"
"resampling pass after OpenAL Soft mixes the sources and processes effects\n"
"for output.", nullptr));
#endif // QT_CONFIG(tooltip)
        wasapiResamplerCheckBox->setText(QCoreApplication::translate("MainWindow", "Allow Resampler", nullptr));
#if QT_CONFIG(tooltip)
        pulseAutospawnCheckBox->setToolTip(QCoreApplication::translate("MainWindow", "Automatically spawn a PulseAudio server if one\n"
"is not already running.", nullptr));
#endif // QT_CONFIG(tooltip)
        pulseAutospawnCheckBox->setText(QCoreApplication::translate("MainWindow", "AutoSpawn Server", nullptr));
#if QT_CONFIG(tooltip)
        pulseAllowMovesCheckBox->setToolTip(QCoreApplication::translate("MainWindow", "Allows moving PulseAudio streams to different\n"
"devices during playback or capture. Note that the\n"
"device specifier and device format will not change\n"
"to match the new device.", nullptr));
#endif // QT_CONFIG(tooltip)
        pulseAllowMovesCheckBox->setText(QCoreApplication::translate("MainWindow", "Allow Moving Streams", nullptr));
#if QT_CONFIG(tooltip)
        pulseFixRateCheckBox->setToolTip(QCoreApplication::translate("MainWindow", "When checked, fix the OpenAL device's sample\n"
"rate to match the PulseAudio device.", nullptr));
#endif // QT_CONFIG(tooltip)
        pulseFixRateCheckBox->setText(QCoreApplication::translate("MainWindow", "Fix Sample Rate", nullptr));
#if QT_CONFIG(tooltip)
        pulseAdjLatencyCheckBox->setToolTip(QCoreApplication::translate("MainWindow", "Attempts to adjust the overall latency of device\n"
"playback. Note that this may have adverse effects\n"
"on the resulting internal buffer sizes and mixing\n"
"updates, leading to performance problems and\n"
"drop-outs.", nullptr));
#endif // QT_CONFIG(tooltip)
        pulseAdjLatencyCheckBox->setText(QCoreApplication::translate("MainWindow", "Adjust Latency", nullptr));
        jackAutospawnCheckBox->setText(QCoreApplication::translate("MainWindow", "AutoSpawn Server", nullptr));
#if QT_CONFIG(tooltip)
        groupBox_7->setToolTip(QCoreApplication::translate("MainWindow", "The update buffer size, in samples, that the backend\n"
"will keep buffered to handle the server's real-time\n"
"processing requests. Must be a power of 2. Ignored\n"
"when Real-time Mixing is used.", nullptr));
#endif // QT_CONFIG(tooltip)
        groupBox_7->setTitle(QCoreApplication::translate("MainWindow", "Buffer Size", nullptr));
        jackBufferSizeLine->setPlaceholderText(QCoreApplication::translate("MainWindow", "0", nullptr));
        jackConnectPortsCheckBox->setText(QCoreApplication::translate("MainWindow", "AutoConnect Ports", nullptr));
#if QT_CONFIG(tooltip)
        jackRtMixCheckBox->setToolTip(QCoreApplication::translate("MainWindow", "Renders samples directly in the real-time\n"
"processing callback. This allows for lower\n"
"latency and less overall CPU utilization, but\n"
"can increase the risk of underruns when\n"
"increasing the amount of processing the\n"
"mixer needs to do.", nullptr));
#endif // QT_CONFIG(tooltip)
        jackRtMixCheckBox->setText(QCoreApplication::translate("MainWindow", "Real-time Mixing", nullptr));
        label_17->setText(QCoreApplication::translate("MainWindow", "Default Playback Device:", nullptr));
        alsaDefaultDeviceLine->setPlaceholderText(QCoreApplication::translate("MainWindow", "default", nullptr));
        label_18->setText(QCoreApplication::translate("MainWindow", "Default Capture Device:", nullptr));
        alsaDefaultCaptureLine->setPlaceholderText(QCoreApplication::translate("MainWindow", "default", nullptr));
#if QT_CONFIG(tooltip)
        alsaResamplerCheckBox->setToolTip(QCoreApplication::translate("MainWindow", "Allow use of ALSA's software resampler. This lets\n"
"the OpenAL device to be set to a different sample\n"
"rate than the backend device, but incurs another\n"
"resample pass on top of OpenAL's resampler.", nullptr));
#endif // QT_CONFIG(tooltip)
        alsaResamplerCheckBox->setText(QCoreApplication::translate("MainWindow", "Allow Resampler", nullptr));
#if QT_CONFIG(tooltip)
        alsaMmapCheckBox->setToolTip(QCoreApplication::translate("MainWindow", "Accesses the audio device buffer through an mmap,\n"
"potentially avoiding an extra sample buffer copy\n"
"during updates.", nullptr));
#endif // QT_CONFIG(tooltip)
        alsaMmapCheckBox->setText(QCoreApplication::translate("MainWindow", "MMap Buffer", nullptr));
        label_20->setText(QCoreApplication::translate("MainWindow", "Default Playback Device:", nullptr));
        ossDefaultDeviceLine->setPlaceholderText(QCoreApplication::translate("MainWindow", "/dev/dsp", nullptr));
        label_21->setText(QCoreApplication::translate("MainWindow", "Default Capture Device:", nullptr));
        ossDefaultCaptureLine->setPlaceholderText(QCoreApplication::translate("MainWindow", "/dev/dsp", nullptr));
        ossPlaybackPushButton->setText(QCoreApplication::translate("MainWindow", "Browse...", nullptr));
        ossCapturePushButton->setText(QCoreApplication::translate("MainWindow", "Browse...", nullptr));
        solarisDefaultDeviceLine->setPlaceholderText(QCoreApplication::translate("MainWindow", "/dev/audio", nullptr));
        label_22->setText(QCoreApplication::translate("MainWindow", "Default Playback Device:", nullptr));
        solarisPlaybackPushButton->setText(QCoreApplication::translate("MainWindow", "Browse...", nullptr));
        label_23->setText(QCoreApplication::translate("MainWindow", "Output File:", nullptr));
        label_24->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p align=\"center\"><span style=\" font-style:italic;\">Warning: The specified output file will be OVERWRITTEN WITHOUT</span></p><p align=\"center\"><span style=\" font-style:italic;\">QUESTION when the Wave Writer device is opened.</span></p></body></html>", nullptr));
        waveOutputButton->setText(QCoreApplication::translate("MainWindow", "Browse...", nullptr));
        waveBFormatCheckBox->setText(QCoreApplication::translate("MainWindow", "Create .amb (B-Format) files", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("MainWindow", "Backends", nullptr));
#if QT_CONFIG(tooltip)
        srcCountLineEdit->setToolTip(QCoreApplication::translate("MainWindow", "The maximum number of allocatable sources. Lower values may\n"
"help for systems with apps that try to play more sounds than\n"
"the CPU can handle.", nullptr));
#endif // QT_CONFIG(tooltip)
        srcCountLineEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "256", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "Number of Sound Sources:", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "Number of Effect Slots:", nullptr));
#if QT_CONFIG(tooltip)
        effectSlotLineEdit->setToolTip(QCoreApplication::translate("MainWindow", "The maximum number of Auxiliary Effect Slots an app can\n"
"create. A slot can use a non-negligible amount of CPU time if\n"
"an effect is set on it even if no sources are feeding it, so this\n"
"may help when apps use more than the system can handle.", nullptr));
#endif // QT_CONFIG(tooltip)
        effectSlotLineEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "64", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "Number of Source Sends:", nullptr));
#if QT_CONFIG(tooltip)
        srcSendLineEdit->setToolTip(QCoreApplication::translate("MainWindow", "Limits the number of auxiliary sends allowed per source.\n"
"Setting this higher than the default has no effect.", nullptr));
#endif // QT_CONFIG(tooltip)
        srcSendLineEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "16", nullptr));
#if QT_CONFIG(tooltip)
        cpuExtGroupBox->setToolTip(QCoreApplication::translate("MainWindow", "Enables use of specific CPU extensions. Certain methods may\n"
"utilize CPU extensions when detected, and disabling these can\n"
"be useful for preventing those extensions from being used.", nullptr));
#endif // QT_CONFIG(tooltip)
        cpuExtGroupBox->setTitle(QCoreApplication::translate("MainWindow", "CPU Extensions", nullptr));
        enableSSECheckBox->setText(QCoreApplication::translate("MainWindow", "SSE", nullptr));
        enableSSE2CheckBox->setText(QCoreApplication::translate("MainWindow", "SSE2", nullptr));
        enableNeonCheckBox->setText(QCoreApplication::translate("MainWindow", "Neon", nullptr));
        enableSSE41CheckBox->setText(QCoreApplication::translate("MainWindow", "SSE4.1", nullptr));
        enableSSE3CheckBox->setText(QCoreApplication::translate("MainWindow", "SSE3", nullptr));
        cpuExtDisabledLabel->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p align=\"center\"><span style=\" font-style:italic;\">No support enabled for CPU Extensions</span></p></body></html>", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("MainWindow", "Resources", nullptr));
#if QT_CONFIG(tooltip)
        groupBox_5->setToolTip(QCoreApplication::translate("MainWindow", "Specifies which effects apps can recognize. Disabling effects\n"
"can help for apps that try to use ones that are too intensive\n"
"for the system to handle.", nullptr));
#endif // QT_CONFIG(tooltip)
        groupBox_5->setTitle(QCoreApplication::translate("MainWindow", "Enabled Effects", nullptr));
        enableEaxReverbCheck->setText(QCoreApplication::translate("MainWindow", "EAX Reverb", nullptr));
        enableStdReverbCheck->setText(QCoreApplication::translate("MainWindow", "Standard Reverb", nullptr));
        enableChorusCheck->setText(QCoreApplication::translate("MainWindow", "Chorus", nullptr));
        enableDistortionCheck->setText(QCoreApplication::translate("MainWindow", "Distortion", nullptr));
        enableEchoCheck->setText(QCoreApplication::translate("MainWindow", "Echo", nullptr));
        enableEqualizerCheck->setText(QCoreApplication::translate("MainWindow", "Equalizer", nullptr));
        enableFlangerCheck->setText(QCoreApplication::translate("MainWindow", "Flanger", nullptr));
        enableModulatorCheck->setText(QCoreApplication::translate("MainWindow", "Ring Modulator", nullptr));
#if QT_CONFIG(tooltip)
        enableDedicatedCheck->setToolTip(QCoreApplication::translate("MainWindow", "Enables both the Dedicated Dialog and Dedicated LFE effects\n"
"added by the ALC_EXT_DEDICATED extension.", nullptr));
#endif // QT_CONFIG(tooltip)
        enableDedicatedCheck->setText(QCoreApplication::translate("MainWindow", "Dedicated ...", nullptr));
        enableCompressorCheck->setText(QCoreApplication::translate("MainWindow", "Compressor", nullptr));
        enablePitchShifterCheck->setText(QCoreApplication::translate("MainWindow", "Pitch Shifter", nullptr));
        enableFrequencyShifterCheck->setText(QCoreApplication::translate("MainWindow", "Frequency Shifter", nullptr));
        enableAutowahCheck->setText(QCoreApplication::translate("MainWindow", "Autowah", nullptr));
        enableVocalMorpherCheck->setText(QCoreApplication::translate("MainWindow", "Vocal morpher", nullptr));
        label_13->setText(QCoreApplication::translate("MainWindow", "Default Reverb Effect:", nullptr));
        defaultReverbComboBox->setItemText(0, QCoreApplication::translate("MainWindow", "None", nullptr));
        defaultReverbComboBox->setItemText(1, QCoreApplication::translate("MainWindow", "Generic", nullptr));
        defaultReverbComboBox->setItemText(2, QCoreApplication::translate("MainWindow", "PaddedCell", nullptr));
        defaultReverbComboBox->setItemText(3, QCoreApplication::translate("MainWindow", "Room", nullptr));
        defaultReverbComboBox->setItemText(4, QCoreApplication::translate("MainWindow", "Bathroom", nullptr));
        defaultReverbComboBox->setItemText(5, QCoreApplication::translate("MainWindow", "Livingroom", nullptr));
        defaultReverbComboBox->setItemText(6, QCoreApplication::translate("MainWindow", "Stoneroom", nullptr));
        defaultReverbComboBox->setItemText(7, QCoreApplication::translate("MainWindow", "Auditorium", nullptr));
        defaultReverbComboBox->setItemText(8, QCoreApplication::translate("MainWindow", "ConcertHall", nullptr));
        defaultReverbComboBox->setItemText(9, QCoreApplication::translate("MainWindow", "Cave", nullptr));
        defaultReverbComboBox->setItemText(10, QCoreApplication::translate("MainWindow", "Arena", nullptr));
        defaultReverbComboBox->setItemText(11, QCoreApplication::translate("MainWindow", "Hangar", nullptr));
        defaultReverbComboBox->setItemText(12, QCoreApplication::translate("MainWindow", "CarpetedHallway", nullptr));
        defaultReverbComboBox->setItemText(13, QCoreApplication::translate("MainWindow", "Hallway", nullptr));
        defaultReverbComboBox->setItemText(14, QCoreApplication::translate("MainWindow", "StoneCorridor", nullptr));
        defaultReverbComboBox->setItemText(15, QCoreApplication::translate("MainWindow", "Alley", nullptr));
        defaultReverbComboBox->setItemText(16, QCoreApplication::translate("MainWindow", "Forest", nullptr));
        defaultReverbComboBox->setItemText(17, QCoreApplication::translate("MainWindow", "City", nullptr));
        defaultReverbComboBox->setItemText(18, QCoreApplication::translate("MainWindow", "Mountains", nullptr));
        defaultReverbComboBox->setItemText(19, QCoreApplication::translate("MainWindow", "Quarry", nullptr));
        defaultReverbComboBox->setItemText(20, QCoreApplication::translate("MainWindow", "Plain", nullptr));
        defaultReverbComboBox->setItemText(21, QCoreApplication::translate("MainWindow", "ParkingLot", nullptr));
        defaultReverbComboBox->setItemText(22, QCoreApplication::translate("MainWindow", "SewerPipe", nullptr));
        defaultReverbComboBox->setItemText(23, QCoreApplication::translate("MainWindow", "Underwater", nullptr));
        defaultReverbComboBox->setItemText(24, QCoreApplication::translate("MainWindow", "Drugged", nullptr));
        defaultReverbComboBox->setItemText(25, QCoreApplication::translate("MainWindow", "Dizzy", nullptr));
        defaultReverbComboBox->setItemText(26, QCoreApplication::translate("MainWindow", "Psychotic", nullptr));

#if QT_CONFIG(tooltip)
        enableEaxCheck->setToolTip(QCoreApplication::translate("MainWindow", "Enables legacy EAX API support.", nullptr));
#endif // QT_CONFIG(tooltip)
        enableEaxCheck->setText(QCoreApplication::translate("MainWindow", "Enable EAX API support", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_4), QCoreApplication::translate("MainWindow", "Effects", nullptr));
        closeCancelButton->setText(QCoreApplication::translate("MainWindow", "Cancel", nullptr));
        menuFile->setTitle(QCoreApplication::translate("MainWindow", "&File", nullptr));
        menuHelp->setTitle(QCoreApplication::translate("MainWindow", "&Help", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
