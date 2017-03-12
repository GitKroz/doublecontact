/* Double Contact
 *
 * Module: HTML file report
 *
 * Copyright 2016 Mikhail Y. Zvyozdochkin aka DarkHobbit <pub@zvyozdochkin.ru>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version. See COPYING file for more details.
 *
 */

#include "htmlfile.h"

HTMLFile::HTMLFile()
{
}

QStringList HTMLFile::supportedExtensions()
{
    return (QStringList() << "html" << "HTML" << "htm" << "HTM");
}

QStringList HTMLFile::supportedFilters()
{
    return (QStringList() << "HTML (*.html *.HTML *.htm *.HTM)");
}


bool HTMLFile::importRecords(const QString &, ContactList &, bool)
{
    return false;
}

bool HTMLFile::exportRecords(const QString &url, ContactList &list)
{
    if (!openFile(url, QIODevice::WriteOnly))
        return false;
    _errors.clear();
    QTextStream stream(&file);
    stream.setCodec("UTF-8");
    stream << QString("<html><head>\n<meta charset=\"utf-8\">\n<title>%1</title>\n</head>\n<body>").arg(url) << endl;
    // General data
    stream << QString("<b>%1</b>: %2<br/>\n").arg(S_ADDRESS_BOOK).arg(url);
    if (!list.extra.model.isEmpty())
        stream << QString("%1<br/>\n").arg(list.extra.model);
    if (!list.extra.timeStamp.isEmpty())
        stream << QString("%1<br/>\n").arg(list.extra.timeStamp);
    stream << endl;
    foreach (const ContactItem& item, list) {
        stream << QString("<p>\n");
        // Name
        stream << QString("<b>%1</b>\n").arg(item.formatNames());
        // Phone(s), email(s)
        exportTypedItem(stream, item.phones, S_PHONE);
        exportTypedItem(stream, item.emails, S_EMAIL);
        // Birthday, anniversary
        exportStringableItem(stream, item.birthday, S_BDAY);
        if (!item.anniversaries.isEmpty()) // TODO simplify, if one ann. will (vCard 4.0)
            exportStringableItem(stream, item.anniversaries[0], S_ANN);
        exportString(stream, item.description, S_DESC);
        if (!item.photo.isEmpty())
            exportString(stream, " ", S_HAS_PHOTO); // space, not empty string!
        // Work
        exportString(stream, item.organization, S_ORG);
        exportString(stream, item.title, S_TITLE);
        // Addresses
        exportStringableItem(stream, item.addrHome, S_ADDR + " " +Phone::standardTypes.translate("home"));
        exportStringableItem(stream, item.addrWork, S_ADDR + " " +Phone::standardTypes.translate("work"));
        // Internet
        exportString(stream, item.nickName, S_NICK);
        exportString(stream, item.url, S_URL);
        exportString(stream, item.jabberName, S_IM_JABBER);
        exportString(stream, item.icqName, S_IM_ICQ);
        exportString(stream, item.skypeName, S_IM_SKYPE);
        stream << QString("</p>\n\n");
    }
    // TODO hr and summary here
    stream << "\n<body>\n<html>" << endl;
    closeFile();
    return true;
}

void HTMLFile::exportString(QTextStream &stream, const QString &field, const QString &title)
{
    if (!field.isEmpty())
            stream << QString("<br/><b>%1:</b> %2\n").arg(title).arg(field);
}

template <class T>
void HTMLFile::exportStringableItem(QTextStream &stream, const T& field, const QString &title)
{
    if (!field.isEmpty())
        exportString(stream, field.toString(), title);
}

template <class T>
void HTMLFile::exportTypedItem(QTextStream &stream, const QList<T> &lst, const QString& title)
{
    if (!lst.isEmpty()) {
        stream << QString("<br/><b>%1:</b> ").arg(title);
        int i=0;
        foreach (const T& it, lst) {
            QString types = "";
            for (int j=0; j<it.types.count(); j++) {
                types += it.standardTypes.translate(it.types[j]).toLower();
                if (j<it.types.count()-1)
                    types += "+";
            }
            stream << QString("%1 (%2)").arg(it.value).arg(types);
            i++;
            if (i<lst.count())
                stream << ", ";
        }
        stream << endl;
    }
}
