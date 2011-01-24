/*
    This file is part of Cute Chess.

    Cute Chess is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Cute Chess is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Cute Chess.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "boardview.h"
#include <QPainter>
#include <QResizeEvent>
#include <QTimer>


BoardView::BoardView(QGraphicsScene* scene, QWidget* parent)
	: QGraphicsView(scene, parent),
	  m_resizeTimer(new QTimer(this))
{
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	m_resizeTimer->setSingleShot(true);
	m_resizeTimer->setInterval(300);

	connect(m_resizeTimer, SIGNAL(timeout()),
		this, SLOT(fitToRect()));
	connect(scene, SIGNAL(sceneRectChanged(QRectF)),
		this, SLOT(fitToRect()));
}

void BoardView::paintEvent(QPaintEvent* event)
{
	if (!m_resizePixmap.isNull())
	{
		QRect rect(viewport()->rect());
		qreal srcAr = qreal(m_resizePixmap.width()) / m_resizePixmap.height();
		qreal trgAr = qreal(rect.width()) / rect.height();

		if (srcAr > trgAr)
			rect.setHeight(rect.width() / srcAr);
		else if (srcAr < trgAr)
			rect.setWidth(rect.height() * srcAr);
		rect.moveCenter(viewport()->rect().center());

		QPainter painter(viewport());
		painter.drawPixmap(rect, m_resizePixmap);
	}
	else
		QGraphicsView::paintEvent(event);
}

void BoardView::resizeEvent(QResizeEvent* event)
{
	QGraphicsView::resizeEvent(event);

	if (m_resizePixmap.isNull())
	{
		m_resizePixmap = QPixmap(sceneRect().toRect().size());
		QPainter painter(&m_resizePixmap);
		scene()->render(&painter);
	}

	m_resizeTimer->start();
}

void BoardView::fitToRect()
{
	m_resizePixmap = QPixmap();
	fitInView(sceneRect(), Qt::KeepAspectRatio);
}