/* Double Contact
 *
 * Module: Helpers for contact comparison dialog
 *
 * Copyright 2016 Mikhail Y. Zvyozdochkin aka DarkHobbit <pub@zvyozdochkin.ru>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version. See COPYING file for more details.
 *
 */

#ifndef COMPARECONTAINERS_H
#define COMPARECONTAINERS_H

#include <QComboBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QObject>
#include <QStringList>
#include <QToolButton>
#include "contactlist.h"

class ItemPair: public QObject
{
    Q_OBJECT
public:
    ItemPair(const QString& title, QGridLayout* layout, bool multiItem);
protected:
    bool _multiItem;
    int _rowLimit;
    QGridLayout* layLeft;
    QGridLayout* layRight;
    virtual void copyData(bool toLeft)=0;
    virtual void copyOneItem(bool toLeft, int srcIndex)=0;
    virtual bool checkDiff()=0;
    void buildOneItemButtonSide(bool toLeft, int column);
    void buildOneItemButtons(int column);
    void addOneItemButton(bool toLeft, int column);
    void removeOneItemButton(bool toLeft);
    QLineEdit* addEditor(const QString& text);
    QComboBox* addCombo();
    void highlightDiff(bool hasDiff);
    QGroupBox* gbLeft;
private:
    QGroupBox* gbRight;
    QToolButton* btnToLeft;
    QToolButton* btnToRight;
    QList<QToolButton*> btnsOneItemToLeft;
    QList<QToolButton*> btnsOneItemToRight;
private slots:
    void onItemChanged();
    void onToLeftClicked();
    void onToRightClicked();
    void onOneItemToLeftClicked();
    void onOneItemToRightClicked();
};

class StringListPair: public ItemPair
{
    Q_OBJECT
public:
    StringListPair(const QString& title, QGridLayout* layout, bool multiItem = true);
    void setData(const QStringList& leftData, const QStringList& rightData);
    void getData(QStringList& leftData, QStringList& rightData);
protected:
    bool hasLabels;
    virtual void copyData(bool toLeft);
    virtual void copyOneItem(bool toLeft, int srcIndex);
    virtual bool checkDiff();
    virtual QString label(int);
private:
    QList<QLineEdit*> leftSet;
    QList<QLineEdit*> rightSet;
    QList<QLabel*> leftLabels;
    QList<QLabel*> rightLabels;
    void fillBox(QGridLayout* layout, const QStringList& data, QList<QLineEdit*>& edSet, QList<QLabel*>& lbSet);
    void addLine (int row, const QString& text,
        QGridLayout* layout, QList<QLineEdit*>& edSet, QList<QLabel*>& lbSet,
        bool createOneItemButton);
};

class StringPair: public StringListPair
{
    Q_OBJECT
public:
    StringPair(const QString& title, QGridLayout* layout);
    void setData(const QString& leftData, const QString& rightData);
    void getData(QString& leftData, QString& rightData);
};

class NamePair: public StringListPair
{
    Q_OBJECT
public:
    NamePair(const QString& title, QGridLayout* layout);
protected:
    virtual QString label(int row);
};

class TypedPair: public ItemPair
{
    Q_OBJECT
public:
    TypedPair(const QString& title, QGridLayout* layout);
    virtual ~TypedPair();
    template<class T>
    void getData(QList<T>& leftItems, QList<T>& rightItems);
protected:
    StandardTypes* standardTypes;
    void addValue(const TypedStringItem& item, bool toLeft);
    int count(bool onLeft);
    bool getValue(int index, TypedStringItem& item, bool fromLeft);
    virtual void copyData(bool toLeft);
    virtual void copyOneItem(bool toLeft, int srcIndex);
private:
    QList<QLineEdit*> leftEdSet;
    QList<QComboBox*> leftComboSet;
    QList<QLineEdit*> rightEdSet;
    QList<QComboBox*> rightComboSet;
};

class PhonesPair: public TypedPair
{
    Q_OBJECT
public:
    PhonesPair(const QString& title, QGridLayout* layout);
    void setData(const QList<Phone>& leftPhones, const QList<Phone>& rightPhones);
protected:
    virtual bool checkDiff();
};

class EmailsPair: public TypedPair
{
    Q_OBJECT
public:
    EmailsPair(const QString& title, QGridLayout* layout);
    void setData(const QList<Email>& leftEmails, const QList<Email>& rightEmails);
protected:
    virtual bool checkDiff();
};

class MessengersPair: public TypedPair
{
    Q_OBJECT
public:
    MessengersPair(const QString& title, QGridLayout* layout);
    void setData(const QList<Messenger>& leftIMs, const QList<Messenger>& rightIMs);
protected:
    virtual bool checkDiff();
};

class DateItemListPair: public ItemPair
{
    Q_OBJECT
public:
    DateItemListPair(const QString& title, QGridLayout* layout, bool multiItem = true);
    void setData(const QList<DateItem>& leftData, const QList<DateItem>& rightData);
    void getData(QList<DateItem>& leftData, QList<DateItem>& rightData);
protected:
    virtual void copyData(bool toLeft);
    virtual void copyOneItem(bool toLeft, int srcIndex);
    virtual bool checkDiff();
private:
    QList<QLabel*> leftSet;
    QList<QLabel*> rightSet;
    QList<DateItem> leftDataSet;
    QList<DateItem> rightDataSet;
    void fillBox(QGridLayout* layout, const QList<DateItem>& data, QList<QLabel*>& lbSet);
};

class DateItemPair: public DateItemListPair
{
    Q_OBJECT
public:
    DateItemPair(const QString& title, QGridLayout* layout);
    void setData(const DateItem& leftData, const DateItem& rightData);
    void getData(DateItem& leftData, DateItem& rightData);
};

class PostalAddressPair: public TypedPair
{
    Q_OBJECT
public:
    PostalAddressPair(const QString& title, QGridLayout* layout);
    void setData(const QList<PostalAddress>& leftData, const QList<PostalAddress>& rightData);
    void getData(QList<PostalAddress>& leftData, QList<PostalAddress>& rightData);
protected:
    virtual bool checkDiff();
    bool getValue(int index, PostalAddress& item, bool fromLeft);
};

class PhotoPair: public ItemPair
{
    Q_OBJECT
public:
    PhotoPair(const QString& title, QGridLayout* layout);
    void setData(const Photo& leftData, const Photo& rightData);
    void getData(Photo& leftData, Photo& rightData);
protected:
    virtual void copyData(bool toLeft);
    virtual void copyOneItem(bool, int) {};
    virtual bool checkDiff();
private:
    QLabel *lbLeft, *lbRight;
    Photo photoLeft, photoRight;
    void fillPhoto(QGridLayout* layout, const Photo& data, QLabel** lb);
};

#endif // COMPARECONTAINERS_H
