#include "QHoverLineEdit.h"
#include "QBoxLayout"
#include "QPainter"

class QLineEditOverride :public QLineEdit {
protected:
	virtual void paintEvent(QPaintEvent* event) {
		QLineEdit::paintEvent(event);
		if (text().isEmpty() && !mPlaceholderText.isEmpty()) {
			QPainter painter(this);
			painter.setPen(mPlaceholderColor);
			painter.drawText(rect().adjusted(3, 0, 0, 0), Qt::AlignVCenter | Qt::AlignLeft, mPlaceholderText);
		}
	}
	QString mPlaceholderText;
	QColor mPlaceholderColor;
public:
	QString GetPlaceholderText() const { return mPlaceholderText; }
	void SetPlaceholderText(QString val) { mPlaceholderText = val; }
	QColor GetPlaceholderColor() const { return mPlaceholderColor; }
	void SetPlaceholderColor(QColor val) { mPlaceholderColor = val; }
};

QHoverLineEdit::QHoverLineEdit(QString inText)
	: mLineEdit(new QLineEditOverride())

{
	mLineEdit->setText(inText);
	connect(mLineEdit, &QLineEdit::textChanged, this, &QHoverLineEdit::AsTextChanged);
	connect(mLineEdit, &QLineEdit::editingFinished, this, &QHoverLineEdit::AsEditingFinished);
	QHBoxLayout* h = new QHBoxLayout(this);
	h->addWidget(mLineEdit);
	h->setContentsMargins(2, 2, 2, 2);
}

void QHoverLineEdit::SetText(QString inText)
{
	mLineEdit->setText(inText);
}

QString QHoverLineEdit::GetText() const
{
	return mLineEdit->text();
}

QColor QHoverLineEdit::GetPlaceholdColor() const
{
	return mLineEdit->GetPlaceholderColor();
}

void QHoverLineEdit::SetPlaceholdColor(QColor val)
{
	mLineEdit->SetPlaceholderColor(val);
}

QString QHoverLineEdit::GetPlaceholdText() const
{
	return mLineEdit->GetPlaceholderText();
}

void QHoverLineEdit::SetPlaceholdText(QString val)
{
	mLineEdit->SetPlaceholderText(val);
}

QLineEdit* QHoverLineEdit::GetQLineEdit() const
{
	return mLineEdit;
}
