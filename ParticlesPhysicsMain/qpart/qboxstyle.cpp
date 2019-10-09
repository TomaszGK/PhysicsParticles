#include "qboxstyle.h"

#include <QFile>

QBoxStyle::QBoxStyle()
{

}

bool QBoxStyle::loadStyle( BoxStyles style )
{
    QDomDocument xmlBOM;

    qDebug() << "loading style from file : " << styleFiles[style] << "\n";

    QFile file { qApp->applicationDirPath()+"//"+styleFiles[style] };

    if( !file.open( QIODevice::ReadOnly | QIODevice::Text ) )
    {        
        qDebug() << "Wrong file name\n";
        return false;
    }

    if( !xmlBOM.setContent(&file) )
    {
        qDebug() << "Wrong file content\n";
        return false;
    }

    auto docElem = xmlBOM.documentElement();

    buttonStyleSelected = loadString(docElem,"buttonStyleSelected");
    buttonStyleUnselected = loadString(docElem,"buttonStyleUnselected");

    cPlaneBorder = loadColor(docElem,"cPlaneBorder");
    cBackground = loadColor(docElem,"cBackground");
    cLabelColor = loadColor(docElem,"cLabelColor");
    cValue = loadColor(docElem,"cValue");
    cButtonActive = loadColor(docElem,"cButtonActive");
    cButton = loadColor(docElem,"cButton");
    cButtonLabel = loadColor(docElem,"cButtonLabel");
    cUpper = loadColor(docElem,"cUpper");
    cLower = loadColor(docElem,"cLower");
    cBigCirclePenColor = loadColor(docElem,"cBigCirclePenColor");
    cSmallCircleColor = loadColor(docElem,"cSmallCircleColor");
    cSmallCircleHookedColor = loadColor(docElem,"cSmallCircleHookedColor");

    marginLeft = loadInt(docElem,"marginLeft");
    marginRight = loadInt(docElem,"marginRight");
    marginTop = loadInt(docElem,"marginTop");
    marginBottom = loadInt(docElem,"marginBottom");
    numberOfHorizontalAxes = loadInt(docElem,"numberOfHorizontalAxes");
    numberOfVerticalAxes = loadInt(docElem,"numberOfVerticalAxes");
    planeBorderWidth = loadInt(docElem,"planeBorderWidth");

    isScalableUp = loadBool(docElem,"isScalableUp");
    isScalableDown = loadBool(docElem,"isScalableDown");

    file.close();
    return true;
}

QColor QBoxStyle::loadColor( const QDomElement& element, const QString& tagName )
{
    QColor color {0,0,0};

    auto findR = element.firstChildElement(tagName).firstChildElement("R");
    auto findG = element.firstChildElement(tagName).firstChildElement("G");
    auto findB = element.firstChildElement(tagName).firstChildElement("B");

    if( !findR.isNull() && !findG.isNull() && !findB.isNull() )
    {
      color.setRed( findR.text().toInt() );
      color.setBlue( findG.text().toInt() );
      color.setGreen( findB.text().toInt() );
    }

    return color;
}

QString QBoxStyle::loadString( const QDomElement& element , const QString& tagName )
{
    QString result {""};

    auto findString = element.firstChildElement(tagName).firstChildElement("string");

    if( !findString.isNull() )
    {
      result =  findString.text();
    }

    return result;
}

int QBoxStyle::loadInt( const QDomElement& element , const QString& tagName )
{
    int result {0};

    auto findInt = element.firstChildElement(tagName).firstChildElement("int");

    if( !findInt.isNull() )
    {
      result =  findInt.text().toInt();
    }

    return result;
}

bool QBoxStyle::loadBool( const QDomElement& element , const QString &tagName )
{
    bool result {false};

    auto findBool = element.firstChildElement(tagName).firstChildElement("bool");

    if( !findBool.isNull() )
    {
      result = findBool.text().toInt()==1?true:false;
    }

    return result;
}
