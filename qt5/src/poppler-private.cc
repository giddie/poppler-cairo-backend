/* poppler-private.cc: qt interface to poppler
 * Copyright (C) 2005, Net Integration Technologies, Inc.
 * Copyright (C) 2006, 2011, 2015, 2017 by Albert Astals Cid <aacid@kde.org>
 * Copyright (C) 2008, 2010, 2011, 2014 by Pino Toscano <pino@kde.org>
 * Copyright (C) 2013 by Thomas Freitag <Thomas.Freitag@alfa.de>
 * Copyright (C) 2013 Adrian Johnson <ajohnson@redneon.com>
 * Copyright (C) 2016 Jakub Alba <jakubalba@gmail.com>
 * Inspired on code by
 * Copyright (C) 2004 by Albert Astals Cid <tsdgeos@terra.es>
 * Copyright (C) 2004 by Enrico Ros <eros.kde@email.it>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include "poppler-private.h"

#include <QtCore/QByteArray>
#include <QtCore/QDebug>
#include <QtCore/QVariant>

#include <Link.h>
#include <Outline.h>
#include <PDFDocEncoding.h>
#include <UnicodeMap.h>

namespace Poppler {

namespace Debug {

    static void qDebugDebugFunction(const QString &message, const QVariant & /*closure*/)
    {
        qDebug() << message;
    }

    PopplerDebugFunc debugFunction = qDebugDebugFunction;
    QVariant debugClosure;

}

    static UnicodeMap *utf8Map = nullptr;

    void setDebugErrorFunction(PopplerDebugFunc function, const QVariant &closure)
    {
        Debug::debugFunction = function ? function : Debug::qDebugDebugFunction;
        Debug::debugClosure = closure;
    }

    static void qt5ErrorFunction(void * /*data*/, ErrorCategory /*category*/, Goffset pos, char *msg)
    {
        QString emsg;

        if (pos >= 0)
        {
            emsg = QStringLiteral("Error (%1): ").arg(pos);
        }
        else
        {
            emsg = QStringLiteral("Error: ");
        }
        emsg += QString::fromLatin1(msg);
        (*Debug::debugFunction)(emsg, Debug::debugClosure);
    }

    QString unicodeToQString(Unicode* u, int len) {
        if (!utf8Map)
        {
                GooString enc("UTF-8");
                utf8Map = globalParams->getUnicodeMap(&enc);
                utf8Map->incRefCnt();
        }

        // ignore the last character if it is 0x0
        if ((len > 0) && (u[len - 1] == 0))
        {
            --len;
        }

        GooString convertedStr;
        for (int i = 0; i < len; ++i)
        {
            char buf[8];
            const int n = utf8Map->mapUnicode(u[i], buf, sizeof(buf));
            convertedStr.append(buf, n);
        }

        return QString::fromUtf8(convertedStr.getCString(), convertedStr.getLength());
    }

    QString UnicodeParsedString(GooString *s1) {
        if ( !s1 || s1->getLength() == 0 )
            return QString();

        char *cString;
        int stringLength;
        bool deleteCString;
        if ( ( s1->getChar(0) & 0xff ) == 0xfe && ( s1->getLength() > 1 && ( s1->getChar(1) & 0xff ) == 0xff ) )
        {
            cString = s1->getCString();
            stringLength = s1->getLength();
            deleteCString = false;
        }
        else
        {
            cString = pdfDocEncodingToUTF16(s1, &stringLength);
            deleteCString = true;
        }

        QString result;
        // i = 2 to skip the unicode marker
        for ( int i = 2; i < stringLength; i += 2 )
        {
            const Unicode u = ( ( cString[i] & 0xff ) << 8 ) | ( cString[i+1] & 0xff );
            result += QChar( u );
        }
        if (deleteCString)
            delete[] cString;
        return result;
    }

    GooString *QStringToUnicodeGooString(const QString &s) {
        int len = s.length() * 2 + 2;
        char *cstring = (char *)gmallocn(len, sizeof(char));
        cstring[0] = (char)0xfe;
        cstring[1] = (char)0xff;
        for (int i = 0; i < s.length(); ++i)
        {
            cstring[2+i*2] = s.at(i).row();
            cstring[3+i*2] = s.at(i).cell();
        }
        GooString *ret = new GooString(cstring, len);
        gfree(cstring);
        return ret;
    }

    GooString *QStringToGooString(const QString &s) {
        int len = s.length();
        char *cstring = (char *)gmallocn(s.length(), sizeof(char));
        for (int i = 0; i < len; ++i)
            cstring[i] = s.at(i).unicode();
        GooString *ret = new GooString(cstring, len);
        gfree(cstring);
        return ret;
    }

    GooString *QDateTimeToUnicodeGooString(const QDateTime &dt) {
        if (!dt.isValid()) {
            return nullptr;
        }

        return QStringToUnicodeGooString(dt.toUTC().toString("yyyyMMddhhmmss+00'00'"));
    }

    static void linkActionToTocItem( ::LinkAction * a, DocumentData * doc, QDomElement * e )
    {
        if ( !a || !e )
            return;

        switch ( a->getKind() )
        {
            case actionGoTo:
            {
                // page number is contained/referenced in a LinkGoTo
                LinkGoTo * g = static_cast< LinkGoTo * >( a );
                LinkDest * destination = g->getDest();
                if ( !destination && g->getNamedDest() )
                {
                    // no 'destination' but an internal 'named reference'. we could
                    // get the destination for the page now, but it's VERY time consuming,
                    // so better storing the reference and provide the viewport on demand
                    GooString *s = g->getNamedDest();
                    QChar *charArray = new QChar[s->getLength()];
                    for (int i = 0; i < s->getLength(); ++i) charArray[i] = QChar(s->getCString()[i]);
                    QString aux(charArray, s->getLength());
                    e->setAttribute( QStringLiteral("DestinationName"), aux );
                    delete[] charArray;
                }
                else if ( destination && destination->isOk() )
                {
                    LinkDestinationData ldd(destination, nullptr, doc, false);
                    e->setAttribute( QStringLiteral("Destination"), LinkDestination(ldd).toString() );
                }
                break;
            }
            case actionGoToR:
            {
                // page number is contained/referenced in a LinkGoToR
                LinkGoToR * g = static_cast< LinkGoToR * >( a );
                LinkDest * destination = g->getDest();
                if ( !destination && g->getNamedDest() )
                {
                    // no 'destination' but an internal 'named reference'. we could
                    // get the destination for the page now, but it's VERY time consuming,
                    // so better storing the reference and provide the viewport on demand
                    GooString *s = g->getNamedDest();
                    QChar *charArray = new QChar[s->getLength()];
                    for (int i = 0; i < s->getLength(); ++i) charArray[i] = QChar(s->getCString()[i]);
                    QString aux(charArray, s->getLength());
                    e->setAttribute( QStringLiteral("DestinationName"), aux );
                    delete[] charArray;
                }
                else if ( destination && destination->isOk() )
                {
                    LinkDestinationData ldd(destination, nullptr, doc, g->getFileName() != nullptr);
                    e->setAttribute( QStringLiteral("Destination"), LinkDestination(ldd).toString() );
                }
                e->setAttribute( QStringLiteral("ExternalFileName"), g->getFileName()->getCString() );
                break;
            }
            case actionURI:
            {
                LinkURI * u = static_cast< LinkURI * >( a );
                e->setAttribute( QStringLiteral("DestinationURI"), u->getURI()->getCString() );
            }
            default: ;
        }
    }
    
    DocumentData::~DocumentData()
    {
        qDeleteAll(m_embeddedFiles);
        delete (OptContentModel *)m_optContentModel;
        delete doc;
    
        count --;
        if ( count == 0 )
        {
            utf8Map = nullptr;
            delete globalParams;
        }
      }
    
    void DocumentData::init()
    {
        m_backend = Document::CairoBackend;
        paperColor = Qt::white;
        m_hints = 0;
        m_optContentModel = nullptr;
      
        if ( count == 0 )
        {
            utf8Map = nullptr;
            globalParams = new GlobalParams();
            setErrorCallback(qt5ErrorFunction, nullptr);
        }
        count ++;
    }


    void DocumentData::addTocChildren( QDomDocument * docSyn, QDomNode * parent, GooList * items )
    {
        int numItems = items->getLength();
        for ( int i = 0; i < numItems; ++i )
        {
            // iterate over every object in 'items'
            OutlineItem * outlineItem = (OutlineItem *)items->get( i );

            // 1. create element using outlineItem's title as tagName
            QString name;
            Unicode * uniChar = outlineItem->getTitle();
            int titleLength = outlineItem->getTitleLength();
            name = unicodeToQString(uniChar, titleLength);
            if ( name.isEmpty() )
                continue;

            QDomElement item = docSyn->createElement( name );
            parent->appendChild( item );

            // 2. find the page the link refers to
            ::LinkAction * a = outlineItem->getAction();
            linkActionToTocItem( a, this, &item );

            item.setAttribute( QStringLiteral("Open"), QVariant( (bool)outlineItem->isOpen() ).toString() );

            // 3. recursively descend over children
            outlineItem->open();
            GooList * children = outlineItem->getKids();
            if ( children )
                addTocChildren( docSyn, &item, children );
        }
    }

}
