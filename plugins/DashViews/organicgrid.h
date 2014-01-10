/*
 * Copyright (C) 2014 Canonical, Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef ORGANICGRID_H
#define ORGANICGRID_H

#include <QQuickItem>

class QAbstractItemModel;
class QQmlComponent;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-pedantic"
#if (QT_VERSION < QT_VERSION_CHECK(5, 1, 0))
#include <private/qquickvisualdatamodel_p.h>
#else
#include <private/qqmldelegatemodel_p.h>
#include <qqmlinfo.h>
#endif
#pragma GCC diagnostic pop

class OrganicGrid : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY(QAbstractItemModel *model READ model WRITE setModel NOTIFY modelChanged)
    Q_PROPERTY(QQmlComponent *delegate READ delegate WRITE setDelegate NOTIFY delegateChanged)
    Q_PROPERTY(qreal columnSpacing READ columnSpacing WRITE setColumnSpacing NOTIFY columnSpacingChanged)
    Q_PROPERTY(qreal rowSpacing READ rowSpacing WRITE setRowSpacing NOTIFY rowSpacingChanged)
    Q_PROPERTY(QSizeF smallDelegateSize READ smallDelegateSize WRITE setSmallDelegateSize NOTIFY smallDelegateSizeChanged)
    Q_PROPERTY(QSizeF bigDelegateSize READ bigDelegateSize WRITE setBigDelegateSize NOTIFY bigDelegateSizeChanged)
    Q_PROPERTY(qreal delegateCreationBegin READ delegateCreationBegin
                                           WRITE setDelegateCreationBegin
                                           NOTIFY delegateCreationBeginChanged
                                           RESET resetDelegateCreationBegin)
    Q_PROPERTY(qreal delegateCreationEnd READ delegateCreationEnd
                                         WRITE setDelegateCreationEnd
                                         NOTIFY delegateCreationEndChanged
                                         RESET resetDelegateCreationEnd)

friend class VerticalJournalTest;
friend class HorizontalJournalTest;

public:
    OrganicGrid();

    QAbstractItemModel *model() const;
    void setModel(QAbstractItemModel *model);

    QQmlComponent *delegate() const;
    void setDelegate(QQmlComponent *delegate);

    qreal columnSpacing() const;
    void setColumnSpacing(qreal columnSpacing);

    qreal rowSpacing() const;
    void setRowSpacing(qreal rowSpacing);

    // TODO esto es mejor que lo pongamos en OrganicGrid que sea una subclase de la clase DashView que aun no existe
    // que comparta la ostia con abstractjournal
    QSizeF smallDelegateSize() const;
    void setSmallDelegateSize(const QSizeF &size);

    QSizeF bigDelegateSize() const;
    void setBigDelegateSize(const QSizeF &size);
    // END TODO

    qreal delegateCreationBegin() const;
    void setDelegateCreationBegin(qreal);
    void resetDelegateCreationBegin();

    qreal delegateCreationEnd() const;
    void setDelegateCreationEnd(qreal);
    void resetDelegateCreationEnd();

Q_SIGNALS:
    void modelChanged();
    void delegateChanged();
    void columnSpacingChanged();
    void rowSpacingChanged();
    void smallDelegateSizeChanged();
    void bigDelegateSizeChanged();
    void delegateCreationBeginChanged();
    void delegateCreationEndChanged();

protected Q_SLOTS:
    void relayout();

protected:
    void updatePolish() override;
    void componentComplete() override;

    void releaseItem(QQuickItem *item);
    void setImplicitHeightDirty();

private Q_SLOTS:
#if (QT_VERSION < QT_VERSION_CHECK(5, 1, 0))
    void itemCreated(int modelIndex, QQuickItem *item);
    void onModelUpdated(const QQuickChangeSet &changeSet, bool reset);
#else
    void itemCreated(int modelIndex, QObject *object);
    void onModelUpdated(const QQmlChangeSet &changeSet, bool reset);
#endif
    void onHeightChanged();

private:
    void createDelegateModel();
    void refill();
    bool addVisibleItems(qreal fillFromY, qreal fillToY, bool asynchronous);
    QQuickItem *createItem(int modelIndex, bool asynchronous);

    // TODO esto es mejor que lo pongamos en OrganicGrid que sea una subclase de la clase DashView que aun no existe
    // que comparta la ostia con abstractjournal
//     virtual void findBottomModelIndexToAdd(int *modelIndex, qreal *yPos) = 0;
//     virtual void findTopModelIndexToAdd(int *modelIndex, qreal *yPos) = 0;
//     virtual void positionItem(int modelIndex, QQuickItem *item) = 0;
//     virtual bool removeNonVisibleItems(qreal bufferFromY, qreal bufferToY) = 0;
//     virtual void cleanupExistingItems() = 0;
//     virtual void doRelayout() = 0;
//     virtual void updateItemCulling(qreal visibleFromY, qreal visibleToY) = 0;
//     virtual void calculateImplicitHeight() = 0;
// #if (QT_VERSION < QT_VERSION_CHECK(5, 1, 0))
//     virtual void processModelRemoves(const QVector<QQuickChangeSet::Remove> &removes) = 0;
// #else
//     virtual void processModelRemoves(const QVector<QQmlChangeSet::Remove> &removes) = 0;
// #endif
    
    QPointF positionForIndex(int modelIndex);

    void findBottomModelIndexToAdd(int *modelIndex, qreal *yPos);
    void findTopModelIndexToAdd(int *modelIndex, qreal *yPos);
    void positionItem(int modelIndex, QQuickItem *item);
    bool removeNonVisibleItems(qreal bufferFromY, qreal bufferToY);
    void cleanupExistingItems();
    void doRelayout();
    void updateItemCulling(qreal visibleFromY, qreal visibleToY);
    void calculateImplicitHeight();
#if (QT_VERSION < QT_VERSION_CHECK(5, 1, 0))
    void processModelRemoves(const QVector<QQuickChangeSet::Remove> &removes);
#else
    void processModelRemoves(const QVector<QQmlChangeSet::Remove> &removes);
#endif
    

#if (QT_VERSION < QT_VERSION_CHECK(5, 1, 0))
    QQuickVisualDataModel *m_delegateModel;
#else
    QQmlDelegateModel *m_delegateModel;
#endif

    // Index we are waiting because we requested it asynchronously
    int m_asyncRequestedIndex;

    int m_columnSpacing;
    int m_rowSpacing;
    qreal m_delegateCreationBegin;
    qreal m_delegateCreationEnd;
    bool m_delegateCreationBeginValid;
    bool m_delegateCreationEndValid;
    bool m_needsRelayout;
    bool m_delegateValidated;
    bool m_implicitHeightDirty;
    
    // TODO esto es mejor que lo pongamos en OrganicGrid que sea una subclase de la clase DashView que aun no existe
    // que comparta la ostia con abstractjournal
    QSizeF m_smallDelegateSize;
    QSizeF m_bigDelegateSize;
    int m_firstVisibleIndex;
    int m_numberOfModulesPerRow;
    QList<QQuickItem*> m_visibleItems;
};

#endif
