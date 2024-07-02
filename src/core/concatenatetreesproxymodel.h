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

#ifndef QT_LDAP_CORE_CONCATENATE_TREES_PROXY_MODEL_H
#define QT_LDAP_CORE_CONCATENATE_TREES_PROXY_MODEL_H

#include <QAbstractItemModel>
#include <QIdentityProxyModel>
#include <QSharedPointer>

namespace qtldap_core
{

class ConcatenateTreesProxyModelPrivate;

class ConcatenateTreesProxyModel : public QIdentityProxyModel
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(ConcatenateTreesProxyModel)

public:
    explicit ConcatenateTreesProxyModel(QObject* parent = nullptr);
    ~ConcatenateTreesProxyModel();

    void addSourceModel(const QSharedPointer<QAbstractItemModel> &sourceModel);

    void removeSourceModel(const QSharedPointer<QAbstractItemModel> &sourceModel);

    QList<QSharedPointer<QAbstractItemModel> > sourceModels() const;

    QModelIndex mapFromSource(const QModelIndex &sourceIndex) const override;

    QModelIndex mapToSource(const QModelIndex &proxyIndex) const override;

    bool canDropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column,
                         const QModelIndex &parent) const override;

    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column,
                      const QModelIndex &parent) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;

    QMap<int, QVariant> itemData(const QModelIndex &proxyIndex) const override;

    QMimeData *mimeData(const QModelIndexList &indexes) const override;

    QStringList mimeTypes() const override;

    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    bool setItemData(const QModelIndex &proxyIndex, const QMap<int, QVariant> &roles) override;

    QSize span(const QModelIndex &index) const override;

private:
    ConcatenateTreesProxyModelPrivate *d_ptr;
};

}

#endif//QT_LDAP_CORE_CONCATENATE_TREES_PROXY_MODEL_H
