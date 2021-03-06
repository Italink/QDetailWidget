#ifndef QDetailWidget_h__
#define QDetailWidget_h__

#include "QWidget"
#include "BindingLayer/BindingLayer.h"

class QDetailSearcher;
class QDetailTreeWidget;

class QDetailWidget :public QWidget {
	Q_OBJECT
public:
	using QWidget = QWidget;

	enum Flag {
		DisplaySearcher = 0x0
	};
	Q_DECLARE_FLAGS(Flags, Flag);

	enum Style {
		Unreal = 0,
		Qt,
	};
	QDetailWidget(QDetailWidget::Flags inFlags = QDetailWidget::DisplaySearcher, QDetailWidget::Style inStyle = QDetailWidget::Qt);
	void SetObjects(const QList<BindingLayer::Instance>& inObjects);
	void SetStyle(QDetailWidget::Style inStyle);
	void SearchByKeywords(QString inKeywords);
private:
	QDetailSearcher* mSearcher = nullptr;
	QDetailTreeWidget* mTreeWidget = nullptr;
};

#endif // QDetailWidget_h__
