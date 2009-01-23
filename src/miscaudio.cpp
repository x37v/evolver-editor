#include "miscaudio.hpp"

const unsigned int MiscAudioModel::volume_max = 100;
const unsigned int MiscAudioModel::distortion_max = 99;
const unsigned int MiscAudioModel::hack_max = 14;
const unsigned int MiscAudioModel::noise_volume_max = 100;
const unsigned int MiscAudioModel::ext_in_volume_max = 100;

MiscAudioModel::MiscAudioModel(QObject * parent) : QObject(parent){
	mVolume = 100;
	mDistortionType = internal;
	mDistortion = 0;
	mOutputHack = 0;
	mInputHack = 0;
	mNoiseVolume = 0;
	mExtInMode = stereo;
	mExtInVolume = 0;
	mEnvCurve = exponential;
}

#include <QLabel>
#include <QComboBox>
#include <QGridLayout>
#include "sliderspinbox.hpp"

MiscAudioView::MiscAudioView(QWidget * parent) : QWidget(parent){
	QLabel * lab;
	//allocate
	mLayout = new QGridLayout(this);
	mVolume = new SliderSpinBox(this);
	mDistortionType = new QComboBox(this);
	mDistortion = new SliderSpinBox(this);
	mOutputHack = new SliderSpinBox(this);
	mInputHack = new SliderSpinBox(this);
	mNoiseVolume = new SliderSpinBox(this);
	mExtInMode = new QComboBox(this);
	mExtInVolume = new SliderSpinBox(this);
	mEnvCurve = new QComboBox(this);

	//setup
	mVolume->setRange(0, MiscAudioModel::volume_max);

	mDistortionType->addItem("internal");
	mDistortionType->addItem("input");
	mDistortion->setRange(0, MiscAudioModel::distortion_max);
	mOutputHack->setRange(0, MiscAudioModel::hack_max);
	mInputHack->setRange(0, MiscAudioModel::hack_max);
	mNoiseVolume->setRange(0, MiscAudioModel::noise_volume_max);

	mExtInMode->addItem("stereo");
	mExtInMode->addItem("left");
	mExtInMode->addItem("right");
	mExtInMode->addItem("split");
	mExtInVolume->setRange(0, MiscAudioModel::ext_in_volume_max);

	mEnvCurve->addItem("exponential");
	mEnvCurve->addItem("stereo");

	//label and plot
	lab = new QLabel(QString("volume"));
	mLayout->addWidget(mVolume, 0, 1);
	mLayout->addWidget(lab, 0, 0, Qt::AlignRight);

	lab = new QLabel(QString("dist type"));
	mLayout->addWidget(mDistortionType, 1, 1);
	mLayout->addWidget(lab, 1, 0, Qt::AlignRight);

	lab = new QLabel(QString("dist amt"));
	mLayout->addWidget(mDistortion, 2, 1);
	mLayout->addWidget(lab, 2, 0, Qt::AlignRight);

	lab = new QLabel(QString("output hack"));
	mLayout->addWidget(mOutputHack, 3, 1);
	mLayout->addWidget(lab, 3, 0, Qt::AlignRight);

	lab = new QLabel(QString("input hack"));
	mLayout->addWidget(mInputHack, 4, 1);
	mLayout->addWidget(lab, 4, 0, Qt::AlignRight);

	lab = new QLabel(QString("noise vol"));
	mLayout->addWidget(mNoiseVolume, 5, 1);
	mLayout->addWidget(lab, 5, 0, Qt::AlignRight);

	lab = new QLabel(QString("ext in mode"));
	mLayout->addWidget(mExtInMode, 6, 1);
	mLayout->addWidget(lab, 6, 0, Qt::AlignRight);

	lab = new QLabel(QString("ext in vol"));
	mLayout->addWidget(mExtInVolume, 7, 1);
	mLayout->addWidget(lab, 7, 0, Qt::AlignRight);

	lab = new QLabel(QString("env curve"));
	mLayout->addWidget(mEnvCurve, 7, 1);
	mLayout->addWidget(lab, 7, 0, Qt::AlignRight);

	mLayout->setSpacing(2);
	mLayout->setRowStretch(10, 1);

	setLayout(mLayout);
}
