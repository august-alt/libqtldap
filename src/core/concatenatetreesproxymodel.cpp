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

#include <QDebug>
#include <QSize>
#include <QUuid>
#include <optional>

namespace qtldap_core
{

struct TreeNode
{

    QUuid id;

    QAbstractItemModel* sourceModel;

    int row;
    int column;

    QMap<QUuid, TreeNode> children;

    bool isValid;

    explicit TreeNode()
        : id()
        , sourceModel(nullptr)
        , row(0)
        , column(0)
        , isValid(false)
    {}

    TreeNode(QUuid id, QAbstractItemModel* sourceModel, int row, int column)
        : id(id)
        , sourceModel(sourceModel)
        , row(row)
        , column(column)
        , isValid(true)
    {}

    TreeNode(QUuid id, QAbstractItemModel* sourceModel, int row, int column, QMap<QUuid, TreeNode>& children)
        : TreeNode(id, sourceModel, row, column)
    {
        this->children = children;
    }
};

class ConcatenateTreesProxyModelPrivate
{
    Q_DECLARE_PUBLIC(ConcatenateTreesProxyModel)

public:
    ConcatenateTreesProxyModelPrivate(ConcatenateTreesProxyModel* model)
        : q_ptr(model)
    {}

    ~ConcatenateTreesProxyModelPrivate() {}

    TreeNode findSourceModelForRowColumn(int row, int column) const;

    bool appendModel(const QSharedPointer<QAbstractItemModel>& model);
    bool removeModel(const QSharedPointer<QAbstractItemModel>& model);

public:
    QList<QSharedPointer<QAbstractItemModel> > models;

private:
    ConcatenateTreesProxyModel *q_ptr;

    TreeNode rootNode;
};

TreeNode ConcatenateTreesProxyModelPrivate::findSourceModelForRowColumn(int row, int column) const
{
    Q_UNUSED(row);
    Q_UNUSED(column);

    // TODO: Implement.

    return TreeNode();
}

TreeNode mergeSameRoot(const TreeNode& tree1, const TreeNode& tree2)
{
    QMap<QUuid, TreeNode> children;
    for (const auto& pair : tree2.children)
    {
        auto it = tree1.children.find(pair.id);
        if (it != tree1.children.end())
        {
            children[pair.id] = mergeSameRoot(*it, pair);
        }
        else
        {
            children[pair.id] = pair;
        }
    }

    return TreeNode(tree1.id, tree1.sourceModel, tree1.row, tree1.column, children);
}

TreeNode tryMergeIntoFirst(const TreeNode& tree1, const TreeNode& tree2)
{
     if (tree1.id == tree2.id)
     {
         return mergeSameRoot(tree1, tree2);
     }
     else if (tree1.children.empty())
     {
         return TreeNode();
     }
     else
     {
        for (const auto& pair : tree1.children)
        {
            return  tryMergeIntoFirst(pair, tree2);
        }
     }

     return TreeNode();
}

TreeNode tryMergeEither(const TreeNode& tree1, const TreeNode& tree2)
{
    TreeNode result = tryMergeIntoFirst(tree1, tree2);
    if (!result.isValid)
    {
        result = tryMergeIntoFirst(tree2, tree1);
    }

    return result;
}

TreeNode tryMergeOrCreateList(const TreeNode& tree1, const TreeNode& tree2)
{
    TreeNode result = tryMergeEither(tree1, tree2);
    if (result.isValid)
    {
        return result;
    }

    result.children[tree1.id] = tree1;
    result.children[tree2.id] = tree2;

    result.isValid = true;

    return result;
}

bool ConcatenateTreesProxyModelPrivate::appendModel(const QSharedPointer<QAbstractItemModel> &model)
{
    models.append(model);

    TreeNode modelNode(QUuid(), model.get(), 0, 0);

    auto result = tryMergeOrCreateList(rootNode, modelNode);

    if (result.isValid)
    {
        rootNode = result;

        return true;
    }

    return false;
}

bool ConcatenateTreesProxyModelPrivate::removeModel(const QSharedPointer<QAbstractItemModel> &model)
{
    // TODO: Implement tree cleaning.

    return models.removeOne(model);
}

/*!
    \class qtldap_core::ConcatenateTreesProxyModel public
    \inmodule QtLdap
    \brief The ConcatenateTreesProxyModel class proxies multiple source tree models, concatenating their rows and columns.
    \ingroup model-view
    \note This documentation is not yet complete.
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
    Q_D(ConcatenateTreesProxyModel);

    if (!d->models.contains(sourceModel))
    {
        d->appendModel(sourceModel);
    }
    else
    {
        qWarning("ConcatenateTreesProxyModel: Attempt to add a source model that already exists in the proxy model.");
        Q_ASSERT(!"ConcatenateTreesProxyModel: Attempt to add a source model that already exists in the proxy model.");
    }
}

/*!
    Removes the source model \a sourceModel, which was previously added to this proxy.
*/
void ConcatenateTreesProxyModel::removeSourceModel(const QSharedPointer<QAbstractItemModel> &sourceModel)
{
    Q_D(ConcatenateTreesProxyModel);

    if (!d->models.removeOne(sourceModel))
    {
        qWarning("ConcatenateTreesProxyModel: Unable to remove specified model; removeSourceModel failed.");
        Q_ASSERT(!"ConcatenateTreesProxyModel: Unable to remove specified model; removeSourceModel failed.");
    }
}

/*!
    Returns list of the models added to this proxy model.
*/
QList<QSharedPointer<QAbstractItemModel> > ConcatenateTreesProxyModel::sourceModels() const
{
    Q_D(const ConcatenateTreesProxyModel);

    return d->models;
}

/*!
    Returns the proxy index for a given \a sourceIndex, which can be from any of the source models.
*/
QModelIndex ConcatenateTreesProxyModel::mapFromSource(const QModelIndex &sourceIndex) const
{
    Q_D(const ConcatenateTreesProxyModel);
    if (!sourceIndex.isValid())
    {
        return QModelIndex();
    }

    const QSharedPointer<QAbstractItemModel> sourceModel
            = QSharedPointer<QAbstractItemModel>(const_cast<QAbstractItemModel*>(sourceIndex.model()));
    if (!d->models.contains(sourceModel))
    {
        qWarning("ConcatenateTreesProxyModel: Index from wrong model passed to mapFromSource.");
        Q_ASSERT(!"ConcatenateTreesProxyModel: Index from wrong model passed to mapFromSource.");
        return QModelIndex();
    }
    // TODO: Implement actual mapping.

    return QModelIndex();
}

/*!
    Returns the source index for a given proxy index.
*/
QModelIndex ConcatenateTreesProxyModel::mapToSource(const QModelIndex &proxyIndex) const
{
    Q_D(const ConcatenateTreesProxyModel);
    if (!proxyIndex.isValid())
    {
        return QModelIndex();
    }

    const QAbstractItemModel *proxyModel = proxyIndex.model();
    if (proxyModel != this)
    {
        qWarning("ConcatenateTreesProxyModel: Index from wrong model passed to mapToSource.");
        Q_ASSERT(!"ConcatenateTreesProxyModel: Index from wrong model passed to mapToSource.");
        return QModelIndex();
    }
    int row = proxyIndex.row();
    int column = proxyIndex.column();

    TreeNode sourceIndex = d->findSourceModelForRowColumn(row, column);
    if (!sourceIndex.sourceModel)
    {
        return QModelIndex();
    }

    return sourceIndex.sourceModel->index(sourceIndex.row, sourceIndex.column);
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
    Q_ASSERT(checkIndex(index, CheckIndexOption::IndexIsValid));
    const QModelIndex sourceIndex = mapToSource(index);
    if (!sourceIndex.isValid())
    {
        return QVariant();
    }

    return sourceIndex.data(role);
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
    Q_D(const ConcatenateTreesProxyModel);
    if (d->models.isEmpty())
    {
        return Qt::NoItemFlags;
    }

    Q_ASSERT(checkIndex(index, CheckIndexOption::IndexIsValid));
    const QModelIndex sourceIndex = mapToSource(index);
    if (!sourceIndex.isValid())
    {
        return Qt::NoItemFlags;
    }

    return sourceIndex.flags();
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
    Q_ASSERT(checkIndex(proxyIndex, CheckIndexOption::IndexIsValid));
    const QModelIndex sourceIndex = mapToSource(proxyIndex);
    if (!sourceIndex.isValid())
    {
        return {};
    }

    return sourceIndex.model()->itemData(sourceIndex);
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
