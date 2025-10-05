#ifndef GRIDCONTAINER_H
#define GRIDCONTAINER_H

#include <QWidget>
#include <QResizeEvent>
#include <QDebug>
#include <QGridLayout>

class GridContainer : public QWidget
{
public:
	explicit GridContainer(QWidget *parent = nullptr);

private:
	QWidget *child;

protected:
	void resizeEvent(QResizeEvent *event) override;

};

#endif // GRIDCONTAINER_H
