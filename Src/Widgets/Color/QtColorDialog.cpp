#include "QtColorDialog.h"
#include "QBoxLayout"
#include "Component/QColorPicker.h"

QtColorDialog::QtColorDialog()
	: mColorWheel(new ColorWheel)
	, mColorPreview(new ColorPreview)
	, mRedBox(new QColorChannelSlider("R"))
	, mGreenBox(new QColorChannelSlider("G"))
	, mBlueBox(new QColorChannelSlider("B"))
	, mAlphaBox(new QColorChannelSlider("A"))
	, mHueBox(new QColorChannelSlider("H"))
	, mSaturationBox(new QColorChannelSlider("S"))
	, mValueBox(new QColorChannelSlider("V"))
	, mPbPick(new QPushButton("Pick"))
	, mPbOk(new QPushButton("OK"))
	, mPbCancel(new QPushButton("Cancel"))
	, mLeHex(new ColorLineEdit())
{
	CreateUI();
	ConnectUI();
}

QtColorDialog::~QtColorDialog()
{
	Current = nullptr;
}

void QtColorDialog::SetColor(QColor color) {
	mLastColor = color;
	mColorPreview->setComparisonColor(color);
	SetCurrentColorInternal(color);
}

int QtColorDialog::CreateAndShow(QColor color)
{
	QtColorDialog* dialog = QtColorDialog::Current;
	dialog->disconnect();
	if (dialog == nullptr) {
		dialog = new QtColorDialog;
		QtColorDialog::Current = dialog;
	}
	dialog->setAttribute(Qt::WA_DeleteOnClose);
	dialog->SetColor(color);
	dialog->activateWindow();
	dialog->setFocus();
	dialog->show();
	return 0;
}

void QtColorDialog::CreateUI()
{
	mColorWheel->setSelectorShape(ColorWheel::ShapeSquare);
	mColorWheel->setFocusPolicy(Qt::NoFocus);
	QVBoxLayout* v = new QVBoxLayout(this);
	QHBoxLayout* h = new QHBoxLayout();
	h->addWidget(mColorWheel,5);
	QVBoxLayout* tool = new QVBoxLayout;
	tool->setAlignment(Qt::AlignTop);
	tool->addSpacing(40);
	tool->addWidget(mColorPreview);
	tool->addWidget(mLeHex);
	tool->addWidget(mPbPick);
	h->addLayout(tool,2);

	v->addLayout(h);
	QHBoxLayout* sliderPanle = new QHBoxLayout();
	sliderPanle->setAlignment(Qt::AlignBottom);
	QVBoxLayout* RGBA = new QVBoxLayout;
	QVBoxLayout* HSV = new QVBoxLayout;

	RGBA->setAlignment(Qt::AlignTop);
	RGBA->addWidget(mRedBox);
	RGBA->addWidget(mGreenBox);
	RGBA->addWidget(mBlueBox);
	RGBA->addWidget(mAlphaBox);

	HSV->setAlignment(Qt::AlignTop);
	HSV->addWidget(mHueBox);
	HSV->addWidget(mSaturationBox);
	HSV->addWidget(mValueBox);
	QHBoxLayout* buttonPanel = new QHBoxLayout();
	buttonPanel->setAlignment(Qt::AlignRight);
	buttonPanel->addWidget(mPbOk);
	buttonPanel->addWidget(mPbCancel);
	HSV->addLayout(buttonPanel);

	sliderPanle->addLayout(RGBA);
	sliderPanle->addLayout(HSV);
	v->addLayout(sliderPanle);

	mLeHex->setFixedHeight(25);
	mColorPreview->setFixedHeight(40);
	mColorPreview->setDisplayMode(ColorPreview::SplitColor);
	mColorWheel->setMinimumSize(150, 150);
	mColorWheel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	mPbOk->setFocusPolicy(Qt::NoFocus);
	mPbCancel->setFocusPolicy(Qt::NoFocus);
}

void QtColorDialog::ConnectUI()
{
	connect(mColorWheel, &ColorWheel::OnColorChanged, this, [this](QColor color) {
		SetCurrentColorInternal(color);
	});

	connect(mPbPick, &QPushButton::clicked, this, [this]() {
		QColor color = QColorPicker::Pick();
		if (color.isValid()) {
			SetCurrentColorInternal(color);
		}
		this->activateWindow();
		this->setFocus();
	});

	connect(mLeHex, &ColorLineEdit::OnColorChanged, this, [this](QColor color) {
		SetCurrentColorInternal(color);
	});

	connect(mRedBox, &QColorChannelSlider::AsValueChanged, this, [this](float var) {
		QColor next = mCurrentColor;
		next.setRedF(var);
		SetCurrentColorInternal(next);
	});
	connect(mGreenBox, &QColorChannelSlider::AsValueChanged, this, [this](float var) {
		QColor next = mCurrentColor;
		next.setGreenF(var);
		SetCurrentColorInternal(next);
	});
	connect(mBlueBox, &QColorChannelSlider::AsValueChanged, this, [this](float var) {
		QColor next = mCurrentColor;
		next.setBlueF(var);
		SetCurrentColorInternal(next);
	});
	connect(mAlphaBox, &QColorChannelSlider::AsValueChanged, this, [this](float var) {
		QColor next = mCurrentColor;
		next.setAlphaF(var);
		SetCurrentColorInternal(next);
	});

	connect(mHueBox, &QColorChannelSlider::AsValueChanged, this, [this](float var) {
		QColor next = mCurrentColor;
		next.setHsvF(var, mCurrentColor.hsvSaturationF(), mCurrentColor.valueF());
		SetCurrentColorInternal(next);
	});
	connect(mSaturationBox, &QColorChannelSlider::AsValueChanged, this, [this](float var) {
		QColor next = mCurrentColor;
		next.setHsvF(mCurrentColor.hsvHueF(), var, mCurrentColor.valueF());
		SetCurrentColorInternal(next);
	});
	connect(mValueBox, &QColorChannelSlider::AsValueChanged, this, [this](float var) {
		QColor next = mCurrentColor;
		next.setHsvF(mCurrentColor.hsvHueF(), mCurrentColor.hsvSaturationF(), var);
		SetCurrentColorInternal(next);
	});

	connect(mPbOk, &QPushButton::clicked, this, [this]() {
		close();
	});
	connect(mPbCancel, &QPushButton::clicked, this, [this]() {
		SetCurrentColorInternal(mLastColor);
		close();
	});
}

void QtColorDialog::SetCurrentColorInternal(QColor color)
{
	if (mCurrentColor == color)
		return;
	mCurrentColor = color;
	RefleshChannelGradiant();

	mColorWheel->blockSignals(true);
	mColorPreview->blockSignals(true);
	mRedBox->blockSignals(true);
	mGreenBox->blockSignals(true);
	mBlueBox->blockSignals(true);
	mAlphaBox->blockSignals(true);
	mHueBox->blockSignals(true);
	mSaturationBox->blockSignals(true);
	mValueBox->blockSignals(true);
	mLeHex->blockSignals(true);

	mColorWheel->SetColor(color);
	mColorPreview->SetColor(color);
	mLeHex->SetColor(color);

	mRedBox->SetChannelValue(color.redF());
	mGreenBox->SetChannelValue(color.greenF());
	mBlueBox->SetChannelValue(color.blueF());
	mAlphaBox->SetChannelValue(color.alphaF());
	mHueBox->SetChannelValue(color.hsvHueF());
	mSaturationBox->SetChannelValue(color.hsvSaturationF());
	mValueBox->SetChannelValue(color.valueF());

	mColorWheel->blockSignals(false);
	mColorPreview->blockSignals(false);
	mRedBox->blockSignals(false);
	mGreenBox->blockSignals(false);
	mBlueBox->blockSignals(false);
	mAlphaBox->blockSignals(false);
	mHueBox->blockSignals(false);
	mSaturationBox->blockSignals(false);
	mValueBox->blockSignals(false);
	mLeHex->blockSignals(false);

	Q_EMIT AsColorChanged(mCurrentColor);
}

void QtColorDialog::RefleshChannelGradiant()
{
	QGradientStops stops;
	QColor begin = mCurrentColor;
	QColor end = mCurrentColor;

	begin.setRedF(0.0f);
	end.setRedF(1.0f);
	stops.push_back(QGradientStop{ 0.0f,begin });
	stops.push_back(QGradientStop{ 1.0f,end });
	mRedBox->SetGradientStops(stops);
	stops.clear();

	begin = mCurrentColor;
	end = mCurrentColor;
	begin.setGreenF(0.0f);
	end.setGreenF(1.0f);
	stops.push_back(QGradientStop{ 0.0f,begin });
	stops.push_back(QGradientStop{ 1.0f,end });
	mGreenBox->SetGradientStops(stops);
	stops.clear();

	begin = mCurrentColor;
	end = mCurrentColor;
	begin.setBlueF(0.0f);
	end.setBlueF(1.0f);
	stops.push_back(QGradientStop{ 0.0f,begin });
	stops.push_back(QGradientStop{ 1.0f,end });
	mBlueBox->SetGradientStops(stops);
	stops.clear();

	begin = mCurrentColor;
	end = mCurrentColor;
	begin.setAlphaF(0.0f);
	end.setAlphaF(1.0f);
	stops.push_back(QGradientStop{ 0.0f,begin });
	stops.push_back(QGradientStop{ 1.0f,end });
	mAlphaBox->SetGradientStops(stops);
	stops.clear();

	begin = mCurrentColor;
	end = mCurrentColor;
	begin.setHsvF(0.0f, mCurrentColor.hsvSaturationF(), mCurrentColor.valueF());
	end.setHsvF(1.0f, mCurrentColor.hsvSaturationF(), mCurrentColor.valueF());
	stops.push_back(QGradientStop{ 0.0f,begin });
	stops.push_back(QGradientStop{ 1.0f,end });
	mHueBox->SetGradientStops(stops);
	stops.clear();

	begin = mCurrentColor;
	end = mCurrentColor;
	begin.setHsvF(mCurrentColor.hsvHueF(), 0.0f, mCurrentColor.valueF());
	end.setHsvF(mCurrentColor.hsvHueF(), 1.0f, mCurrentColor.valueF());
	stops.push_back(QGradientStop{ 0.0f,begin });
	stops.push_back(QGradientStop{ 1.0f,end });
	mSaturationBox->SetGradientStops(stops);
	stops.clear();

	begin = mCurrentColor;
	end = mCurrentColor;
	begin.setHsvF(mCurrentColor.hsvHueF(),mCurrentColor.hsvSaturationF() , 0.0f);
	end.setHsvF(mCurrentColor.hsvHueF(), mCurrentColor.hsvSaturationF(), 1.0f);
	stops.push_back(QGradientStop{ 0.0f,begin });
	stops.push_back(QGradientStop{ 1.0f,end });
	mValueBox->SetGradientStops(stops);
}
