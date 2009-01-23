#include "vca.hpp"

const unsigned int VCAModel::level_max = 100;
const unsigned int VCAModel::env_amount_max = 100;
const unsigned int VCAModel::attack_max = 110;
const unsigned int VCAModel::decay_max = 110;
const unsigned int VCAModel::sustain_max = 100;
const unsigned int VCAModel::release_max = 110;
const unsigned int VCAModel::velocity_max = 100;
const char * VCAModel::pan_type_names[] = {
	"stereo hard pan",
	"stereo some center",
	"stereo more center",
	"mono",
	"rev stereo more center",
	"rev stereo some center",
	"rev stereo hard pan"
};
const unsigned int VCAModel::num_pan_types = 7;

VCAModel::VCAModel(QObject * parent) : QObject(parent){
	mLevel = 0;
	mEnvAmount = 100;
	mAttack = 0;
	mDecay = 0;
	mSustain = sustain_max;
	mRelease = release_max;
	mVelocity = 0;
	mPanType = stereo_one;
}

#include <QComboBox>
#include <QGridLayout>
#include <QLabel>
#include "sliderspinbox.hpp"

VCAView::VCAView(QWidget * parent) : QWidget(parent){
	QLabel * lab;
	//allocate
	mLayout = new QGridLayout(this);

	mLevel = new SliderSpinBox(this);
	mEnvAmount = new SliderSpinBox(this);
	mAttack = new SliderSpinBox(this);
	mDecay = new SliderSpinBox(this);
	mSustain = new SliderSpinBox(this);
	mRelease = new SliderSpinBox(this);
	mVelocity = new SliderSpinBox(this);
	mPanType = new QComboBox(this);

	//set up
	mLevel->setRange(0, VCAModel::level_max);
	mEnvAmount->setRange(0, VCAModel::env_amount_max);
	mAttack->setRange(0, VCAModel::attack_max);
	mDecay->setRange(0, VCAModel::decay_max);
	mSustain->setRange(0, VCAModel::sustain_max);
	mRelease->setRange(0, VCAModel::release_max);
	mVelocity->setRange(0, VCAModel::velocity_max);
	for(unsigned int i = 0; i < VCAModel::num_pan_types; i++)
		mPanType->addItem(QString(VCAModel::pan_type_names[i]));

	//label and plot
	lab = new QLabel(QString("level"));
	mLayout->addWidget(mLevel, 0, 1);
	mLayout->addWidget(lab, 0, 0, Qt::AlignRight);

	lab = new QLabel(QString("env amount"));
	mLayout->addWidget(mEnvAmount, 1, 1);
	mLayout->addWidget(lab, 1, 0, Qt::AlignRight);

	lab = new QLabel(QString("attack"));
	mLayout->addWidget(mAttack, 2, 1);
	mLayout->addWidget(lab, 2, 0, Qt::AlignRight);

	lab = new QLabel(QString("decay"));
	mLayout->addWidget(mDecay, 3, 1);
	mLayout->addWidget(lab, 3, 0, Qt::AlignRight);

	lab = new QLabel(QString("sustain"));
	mLayout->addWidget(mSustain, 4, 1);
	mLayout->addWidget(lab, 4, 0, Qt::AlignRight);

	lab = new QLabel(QString("release"));
	mLayout->addWidget(mRelease, 5, 1);
	mLayout->addWidget(lab, 5, 0, Qt::AlignRight);

	lab = new QLabel(QString("velocity"));
	mLayout->addWidget(mVelocity, 6, 1);
	mLayout->addWidget(lab, 6, 0, Qt::AlignRight);

	lab = new QLabel(QString("pan"));
	mLayout->addWidget(mPanType, 7, 1);
	mLayout->addWidget(lab, 7, 0, Qt::AlignRight);

	mLayout->setSpacing(2);
	mLayout->setRowStretch(10, 1);

	setLayout(mLayout);
}

