/***********************************************************************************************************************
**
** Copyright (C) 2024 BaseALT Ltd. <org@basealt.ru>
**
** This program is free software; you can redistribute it and/or
** modify it under the terms of the GNU General Public License
** as published by the Free Software Foundation; either version 2
** of the License, or (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
**
***********************************************************************************************************************/

#include "ldaptreemodel.h"
#include "ldaptreeitem.h"

namespace qtldap_core
{

class LDAPTreeModelPrivate
{
    Q_DECLARE_PUBLIC(LDAPTreeModel)

public:
    LDAPTreeModelPrivate(LDAPTreeModel* model)
        : q_ptr(model)
    {}

    ~LDAPTreeModelPrivate()
    {
        if (invisibleRootItem)
        {
            delete invisibleRootItem;
        }
    }


public:
    LDAPTreeItem* invisibleRootItem;

private:
    LDAPTreeModel *q_ptr;
};

LDAPTreeModel::LDAPTreeModel(QObject *parent)
    : QAbstractItemModel(parent)
    , d_ptr(new LDAPTreeModelPrivate(this))
{
    Q_D(LDAPTreeModel);

    Q_UNUSED(d);

    beginResetModel();
    // TODO: Implement.
    endResetModel();
}

LDAPTreeModel::~LDAPTreeModel()
{
    delete d_ptr;
}

QModelIndex LDAPTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_D(const LDAPTreeModel);
    if (!hasIndex(row, column, parent))
    {
        return QModelIndex();
    }

    if (!parent.isValid())
    {
        return createIndex(row, column, d->invisibleRootItem);
    }

    LDAPTreeItem* parentItem = static_cast<LDAPTreeItem*>(parent.internalPointer());
    LDAPTreeItem* childItem = parentItem->getChildren().at(row);
    if (childItem)
    {
        return createIndex(row, column, childItem);
    }

    return QModelIndex();
}

QModelIndex LDAPTreeModel::parent(const QModelIndex &index) const
{
    Q_D(const LDAPTreeModel);
    if (!index.isValid())
    {
        return QModelIndex();
    }

    LDAPTreeItem* childItem = static_cast<LDAPTreeItem*>(index.internalPointer());
    if(!childItem)
    {
        return QModelIndex();
    }

    LDAPTreeItem* parentItem = childItem->getParent();
    if (!parentItem || parentItem == d->invisibleRootItem)
    {
        return QModelIndex();
    }

    if(!parentItem->getParent())
    {
        int index = d->invisibleRootItem->getChildren().indexOf(parentItem);
        return createIndex(index, 0, parentItem);
    }

    QVector<LDAPTreeItem*> items = parentItem->getParent()->getChildren();
    return createIndex(items.indexOf(parentItem), 0, parentItem);
}

int LDAPTreeModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
    {
        return 1;
    }

    LDAPTreeItem* parentItem = static_cast<LDAPTreeItem*>(parent.internalPointer());
    if (parentItem)
    {
        return parentItem->getChildren().size();
    }

    return 0;
}

int LDAPTreeModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 1;
}

QVariant LDAPTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }

    if (index.column() != 0)
    {
        return QVariant();
    }

    LDAPTreeItem* item = static_cast<LDAPTreeItem*>(index.internalPointer());

    Q_UNUSED(item);

    if(role == Qt::DisplayRole)
    {
        // TODO: Implement.
        return QVariant();
    }

    return QVariant();
}

bool LDAPTreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
    Q_UNUSED(role);

    return false;
}

Qt::ItemFlags LDAPTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
    {
        return Qt::NoItemFlags;
    }

     return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

bool LDAPTreeModel::canFetchMore(const QModelIndex &parent) const
{
    if (!parent.isValid())
    {
        return false;
    }

    LDAPTreeItem* parentItem = static_cast<LDAPTreeItem*>(parent.internalPointer());
    if (parentItem)
    {
        // TODO: Implement.
        return false;
    }

    return false;
}

void LDAPTreeModel::fetchMore(const QModelIndex &parent)
{
    LDAPTreeItem* parentItem = static_cast<LDAPTreeItem*>(parent.internalPointer());
    if (parentItem)
    {
        // TODO: Implement.
        //parentItem->load();
    }
}

bool LDAPTreeModel::hasChildren(const QModelIndex &parent) const
{
    LDAPTreeItem* parentItem = static_cast<LDAPTreeItem*>(parent.internalPointer());
    if (parentItem)
    {
        return !parentItem->getChildren().isEmpty();
    }

    return true;
}

}
