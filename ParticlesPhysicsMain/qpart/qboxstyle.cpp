#include "qboxstyle.h"

#include <QtXml>
#include <QFile>

QBoxStyle::QBoxStyle()
{

}

bool QBoxStyle::loadStyle( QString name )
{
    QDomDocument xmlBOM;

    QFile file {name};

    if( !file.open( QIODevice::ReadOnly | QIODevice::Text ) )
    {
        return false;
    }

    if( !xmlBOM.setContent(&file) ) return false;

    file.close();
    return true;
}
