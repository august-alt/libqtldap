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

#include "concatenatetreesproxymodeltest.h"
#include <libqtldap/concatenatetreesproxymodel.h>

#include <QStandardItemModel>

namespace tests
{

void ConcatenateTreesProxyModelTest::initTestCase()
{
    // TODO: Implement.
}

void ConcatenateTreesProxyModelTest::testEmptyProxy()
{
    qtldap_core::ConcatenateTreesProxyModel proxyModel;
    QAbstractItemModelTester tester(&proxyModel);

    QCOMPARE(proxyModel.rowCount(), 0);
    QCOMPARE(proxyModel.columnCount(), 0);

    QSharedPointer<QStandardItemModel> itemModel = QSharedPointer<QStandardItemModel>(new QStandardItemModel());
    proxyModel.addSourceModel(itemModel);

    QCOMPARE(proxyModel.rowCount(), 0);
    QCOMPARE(proxyModel.columnCount(), 1);
}

void ConcatenateTreesProxyModelTest::testNonEmptyProxy()
{
    qtldap_core::ConcatenateTreesProxyModel proxyModel;
    QAbstractItemModelTester tester(&proxyModel);

    QCOMPARE(proxyModel.rowCount(), 0);
    QCOMPARE(proxyModel.columnCount(), 0);

    QSharedPointer<QStandardItemModel> itemModel1 = createTreeModel(5);
    proxyModel.addSourceModel(itemModel1);

    QCOMPARE(proxyModel.rowCount(), 5);
    QCOMPARE(proxyModel.columnCount(), 3);

    QSharedPointer<QStandardItemModel> itemModel2 = createTreeModel(10);
    proxyModel.addSourceModel(itemModel2);

    QCOMPARE(proxyModel.rowCount(), 10);
    QCOMPARE(proxyModel.columnCount(), 1);
}

void ConcatenateTreesProxyModelTest::dataChanged()
{
    qtldap_core::ConcatenateTreesProxyModel proxyModel;
    QAbstractItemModelTester tester(&proxyModel);

    // TODO: Implement.
}

void ConcatenateTreesProxyModelTest::rowManipulation()
{
    // TODO: Implement.
}

void ConcatenateTreesProxyModelTest::columnManipulation()
{
    // TODO: Implement.
}

void ConcatenateTreesProxyModelTest::layoutManipulation()
{
    // TODO: Implement.
}

void ConcatenateTreesProxyModelTest::setData()
{
    // TODO: Implement.
}

void ConcatenateTreesProxyModelTest::setItemData()
{
    // TODO: Implement.
}

QSharedPointer<QStandardItemModel> ConcatenateTreesProxyModelTest::createTreeModel(uint32_t maxDepth)
{
    QSharedPointer<QStandardItemModel> itemModel = QSharedPointer<QStandardItemModel>(new QStandardItemModel());

    for (uint32_t row = 0; row < maxDepth; ++maxDepth)
    {
        for (uint32_t column = 0; column < maxDepth; ++maxDepth)
        {
            itemModel->setItem(row, column, new QStandardItem(QStringLiteral("%1-%2").arg(row).arg(column)));
        }
    }

    return itemModel;
}

}

QTEST_MAIN(tests::ConcatenateTreesProxyModelTest)
