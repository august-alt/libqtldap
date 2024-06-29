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

#ifndef QT_LDAP_CORE_LDAP_TREE_MODEL_H
#define QT_LDAP_CORE_LDAP_TREE_MODEL_H

#include <QAbstractItemModel>

namespace qtldap_core
{

class LDAPTreeModelPrivate;

class LDAPTreeModel : public QAbstractItemModel
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(LDAPTreeModel)

public:
    explicit LDAPTreeModel(QObject* parent = nullptr);
    ~LDAPTreeModel();

    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;

    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    bool canFetchMore(const QModelIndex &parent) const override;

    void fetchMore(const QModelIndex& parent) override;

    bool hasChildren(const QModelIndex &parent) const override;

private:
    LDAPTreeModelPrivate *d_ptr;
};

}

#endif//QT_LDAP_CORE_LDAP_TREE_MODEL_H
