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

#include "concatenatetreesproxymodel.h"

#include <QSize>

namespace qtldap_core
{

class ConcatenateTreesProxyModelPrivate
{
    Q_DECLARE_PUBLIC(ConcatenateTreesProxyModel)

public:
    ConcatenateTreesProxyModelPrivate(ConcatenateTreesProxyModel* model)
        : q_ptr(model)
    {}

    ~ConcatenateTreesProxyModelPrivate() {}

private:
    ConcatenateTreesProxyModel *q_ptr;
};

/*!
    \class ConcatenateTreesProxyModel
    \inmodule QtLdap
    \brief The ConcatenateTreesProxyModel class proxies multiple source tree models, concatenating their rows and columns
    \ingroup model-view
    TODO: Write documentation for model
    \sa QAbstractProxyModel, {Model/View Programming}, QIdentityProxyModel, QAbstractItemModel
 */

/*!
    Constructs a concatenate-trees proxy model with the given \a parent.
*/
ConcatenateTreesProxyModel::ConcatenateTreesProxyModel(QObject *parent)
    : QIdentityProxyModel(parent)
    , d_ptr(new ConcatenateTreesProxyModelPrivate(this))
{
}

/*!
    Destroys this proxy model.
*/
ConcatenateTreesProxyModel::~ConcatenateTreesProxyModel()
{
    delete d_ptr;
}

/*!
    Adds a source model \a sourceModel, below all previously added source models.
    The same source model cannot be added more than once.
*/
void ConcatenateTreesProxyModel::addSourceModel(const QSharedPointer<QAbstractItemModel> &sourceModel)
{
    Q_UNUSED(sourceModel);

    // TODO: Implement.
}

/*!
    Removes the source model \a sourceModel, which was previously added to this proxy.
*/
void ConcatenateTreesProxyModel::removeSourceModel(const QSharedPointer<QAbstractItemModel> &sourceModel)
{
    Q_UNUSED(sourceModel);

    // TODO: Implement.
}

/*!
    Returns list of the models added to this proxy model.
*/
QList<QSharedPointer<QAbstractItemModel> > ConcatenateTreesProxyModel::sourceModels() const
{
    // TODO: Implement.

    return QList<QSharedPointer<QAbstractItemModel> >();
}

/*!
    Returns the proxy index for a given \a sourceIndex, which can be from any of the source models.
*/
QModelIndex ConcatenateTreesProxyModel::mapFromSource(const QModelIndex &sourceIndex) const
{
    Q_UNUSED(sourceIndex);

    // TODO: Implement.

    return QModelIndex();
}

/*!
    Returns the source index for a given proxy index.
*/
QModelIndex ConcatenateTreesProxyModel::mapToSource(const QModelIndex &proxyIndex) const
{
    Q_UNUSED(proxyIndex);

    // TODO: Implement.

    return QModelIndex();
}

/*!
  \reimp
*/
bool ConcatenateTreesProxyModel::canDropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column,
                                                 const QModelIndex &parent) const
{
    Q_UNUSED(data);
    Q_UNUSED(action);
    Q_UNUSED(row);
    Q_UNUSED(column);
    Q_UNUSED(parent);

    return false;
}

/*!
  \reimp
*/
int ConcatenateTreesProxyModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    // TODO: Implement.

    return -1;
}

/*!
  \reimp
*/
QVariant ConcatenateTreesProxyModel::data(const QModelIndex &index, int role) const
{
    Q_UNUSED(index);
    Q_UNUSED(role);

    // TODO: Implement.

    return QVariant();
}

/*!
  \reimp
*/
bool ConcatenateTreesProxyModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column,
                                              const QModelIndex &parent)
{
    Q_UNUSED(data);
    Q_UNUSED(action);
    Q_UNUSED(row);
    Q_UNUSED(column);
    Q_UNUSED(parent);

    // TODO: Implement.

    return false;
}

/*!
  \reimp
*/
Qt::ItemFlags ConcatenateTreesProxyModel::flags(const QModelIndex &index) const
{
    Q_UNUSED(index);

    // TODO: Implement.

    return Qt::NoItemFlags;
}

/*!
  \reimp
*/
QVariant ConcatenateTreesProxyModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(section);
    Q_UNUSED(orientation);
    Q_UNUSED(role);

    // TODO: Implement.

    return QVariant();
}

/*!
  \reimp
*/
QModelIndex ConcatenateTreesProxyModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(row);
    Q_UNUSED(column);
    Q_UNUSED(parent);

    // TODO: Implement.

    return QModelIndex();
}

/*!
  \reimp
*/
QMap<int, QVariant> ConcatenateTreesProxyModel::itemData(const QModelIndex &proxyIndex) const
{
    Q_UNUSED(proxyIndex);

    // TODO: Implement.

    return {};
}

/*!
  \reimp
*/
QMimeData *ConcatenateTreesProxyModel::mimeData(const QModelIndexList &indexes) const
{
    Q_UNUSED(indexes);

    // TODO: Implement.

    return nullptr;
}

/*!
  \reimp
*/
QStringList ConcatenateTreesProxyModel::mimeTypes() const
{
    // TODO: Implement.

    return {};
}

/*!
  \reimp
*/
QModelIndex ConcatenateTreesProxyModel::parent(const QModelIndex &index) const
{
    Q_UNUSED(index);

    // TODO: Implement.

    return QModelIndex();
}

/*!
  \reimp
*/
int ConcatenateTreesProxyModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    // TODO: Implement.

    return -1;
}

/*!
  \reimp
*/
bool ConcatenateTreesProxyModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
    Q_UNUSED(role);

    // TODO: Implement.

    return false;
}

/*!
  \reimp
*/
bool ConcatenateTreesProxyModel::setItemData(const QModelIndex &proxyIndex, const QMap<int, QVariant> &roles)
{
    Q_UNUSED(proxyIndex);
    Q_UNUSED(roles);

    // TODO: Implement.

    return false;
}

/*!
  \reimp
*/
QSize ConcatenateTreesProxyModel::span(const QModelIndex &index) const
{
    Q_UNUSED(index);

    // TODO: Implement.

    return {};
}

}
