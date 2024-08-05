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
#include <QStack>
#include <QUuid>
#include <optional>

namespace qtldap_core
{

struct TreeNode
{
    QVector<const QAbstractItemModel*> sourceModels;

    int row;
    int column;

    QVector<TreeNode*> children;

    bool isValid;

    QModelIndex index;

    explicit TreeNode()
        : sourceModels()
        , row(0)
        , column(0)
        , isValid(false)
        , index()
    {}

    TreeNode(const QAbstractItemModel* sourceModel, int row, int column)
        : sourceModels()
        , row(row)
        , column(column)
        , isValid(true)
        , index()
    {
        sourceModels.append(sourceModel);
    }

    TreeNode(const QAbstractItemModel* sourceModel, int row, int column, QVector<TreeNode*>& children)
        : TreeNode(sourceModel, row, column)
    {
        this->children = children;
    }
};

typedef std::function<void(const QModelIndex & index, const QAbstractItemModel *model, TreeNode *treeNode)> ItemHandler;

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
    void initializeTree(const QAbstractItemModel* model, const TreeNode *rootNodePointer);
    void addTree(const QAbstractItemModel* model);

    void iterateOverTree(const QModelIndex &index, const QAbstractItemModel *model,
                         ItemHandler handler, TreeNode *treeNode);
    void mergeTreeNodes(TreeNode* firstNode, TreeNode* secondNode);

private:
    ConcatenateTreesProxyModel *q_ptr;

    TreeNode rootNode;
};

TreeNode ConcatenateTreesProxyModelPrivate::findSourceModelForRowColumn(int row, int column) const
{

    QStack<TreeNode*> children;

    for (auto child : rootNode.children)
    {
        children.push(child);
    }

    while (children.top())
    {
        TreeNode* child = children.pop();

        if (child->row == row && child->column == column)
        {
            return *child;
        }

        if (!child->children.isEmpty())
        {
            children.append(child->children);
        }
    }

    return TreeNode();
}

void ConcatenateTreesProxyModelPrivate::iterateOverTree(const QModelIndex &index,
                                                        const QAbstractItemModel *model,
                                                        ItemHandler handler,
                                                        TreeNode *treeNode)
{
     if (!index.isValid())
     {
         return;
     }

     if (!model->hasChildren(index) || (index.flags() & Qt::ItemNeverHasChildren))
     {
         return;
     }

     handler(index, model, treeNode);

     for (int i = 0; i < model->rowCount(index); ++i)
     {
         iterateOverTree(model->index(i, 0, index), model, handler, treeNode);
     }
}

void ConcatenateTreesProxyModelPrivate::initializeTree(const QAbstractItemModel* model, const TreeNode* rootNodePointer)
{
    Q_Q(ConcatenateTreesProxyModel);

    ItemHandler handler = [q](const QModelIndex &index, const QAbstractItemModel *model, TreeNode *treeNode)
    {
        TreeNode* child = new TreeNode(model, index.row(), index.column());

        child->index = q->createIndex(index.row(), index.column(), child);

        treeNode->children.push_back(child);

        treeNode = child;
    };

    TreeNode* currentNodePointer = const_cast<TreeNode*>(rootNodePointer);

    for (int i = 0; i < model->rowCount(); ++i)
    {
        iterateOverTree(model->index(i, 0), model, handler, currentNodePointer);
    }
}

bool areEqual(TreeNode* firstNode, TreeNode* secondNode)
{
    // TODO: Create node comparator and use it in this function.
    return false;
}

void ConcatenateTreesProxyModelPrivate::mergeTreeNodes(TreeNode* firstNode, TreeNode* secondNode)
{
    for (auto& secondTreeChild : secondNode->children)
    {
        bool equalNodeFound = false;

        for (auto& firstTreeChild : firstNode->children)
        {
            if (areEqual(firstTreeChild, secondTreeChild))
            {
                firstTreeChild->sourceModels.append(secondTreeChild->sourceModels[0]);

                mergeTreeNodes(firstTreeChild, secondTreeChild);
            }
        }

        if (!equalNodeFound)
        {
            firstNode->children.append(secondTreeChild);
        }
    }
}


void ConcatenateTreesProxyModelPrivate::addTree(const QAbstractItemModel *model)
{
    TreeNode secondModelRoot;

    initializeTree(model, &secondModelRoot);

    mergeTreeNodes(&rootNode, &secondModelRoot);
}

bool ConcatenateTreesProxyModelPrivate::appendModel(const QSharedPointer<QAbstractItemModel> &model)
{

    if (models.contains(model))
    {
        return false;
    }

    if (models.size() == 0)
    {
        initializeTree(model.get(), &rootNode);
    }
    else
    {
        addTree(model.get());
    }

    models.append(model);

    return true;
}

void removeChildren(const QSharedPointer<QAbstractItemModel> &model, TreeNode* rootNode)
{
    auto& children = rootNode->children;

    for (auto& child : children)
    {
        removeChildren(model, child);
    }

    children.erase(std::remove_if(children.begin(), children.end(),
                                  [&model](TreeNode* node)
                                  {
                                      return node->sourceModels.size() == 1 && node->sourceModels.contains(model.get());
                                  }),
                   children.end());

    rootNode->sourceModels.removeOne(model.get());
}

bool ConcatenateTreesProxyModelPrivate::removeModel(const QSharedPointer<QAbstractItemModel> &model)
{
    if (models.size() == 0)
    {
        return false;
    }

    if (models.removeOne(model))
    {
        removeChildren(model, &rootNode);

        return true;
    }

    return false;
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

    if (!d->removeModel(sourceModel))
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
    auto treeNode = d->findSourceModelForRowColumn(sourceIndex.row(), sourceIndex.column());
    if (treeNode.isValid)
    {
        return treeNode.index;
    }

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
    if (sourceIndex.sourceModels.isEmpty())
    {
        return QModelIndex();
    }

    return sourceIndex.sourceModels[0]->index(sourceIndex.row, sourceIndex.column);
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
    Q_D(const ConcatenateTreesProxyModel);
    Q_ASSERT(checkIndex(parent, CheckIndexOption::IndexIsValid));
    if (!parent.isValid())
    {
        return -1;
    }

    TreeNode node = d->findSourceModelForRowColumn(parent.row(), parent.column());
    if (node.isValid)
    {
        return node.children.size();
    }

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
    Q_ASSERT(checkIndex(parent, CheckIndexOption::IndexIsValid));
    const QModelIndex sourceIndex = mapToSource(parent);
    if (!sourceIndex.isValid())
    {
        return -1;
    }

    return sourceIndex.model()->rowCount(sourceIndex);
}

/*!
  \reimp
*/
bool ConcatenateTreesProxyModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    Q_ASSERT(checkIndex(index, CheckIndexOption::IndexIsValid));
    const QModelIndex sourceIndex = mapToSource(index);
    if (!sourceIndex.isValid())
    {
        return false;
    }
    QAbstractItemModel *model = const_cast<QAbstractItemModel*>(sourceIndex.model());
    bool result = model->setData(sourceIndex, value, role);
    if (result)
    {
        emit dataChanged(index, index, { role });
    }

    return result;
}

/*!
  \reimp
*/
bool ConcatenateTreesProxyModel::setItemData(const QModelIndex &proxyIndex, const QMap<int, QVariant> &roles)
{
    Q_ASSERT(checkIndex(proxyIndex, CheckIndexOption::IndexIsValid));
    const QModelIndex sourceIndex = mapToSource(proxyIndex);
    if (!sourceIndex.isValid())
    {
        return false;
    }

    QAbstractItemModel *model = const_cast<QAbstractItemModel*>(sourceIndex.model());
    return model->setItemData(sourceIndex, roles);
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
