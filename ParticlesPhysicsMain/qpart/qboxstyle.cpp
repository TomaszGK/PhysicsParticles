#include "qboxstyle.h"

#include <type_traits>
#include <QFile>

QBoxStyle::QBoxStyle()
{

}

bool QBoxStyle::loadStyleFromFile( BoxStyles style )
{
    QDomDocument xmlBOM;

    qDebug() << "loading style from file : " << styleFiles[style] << "\n";

    QDir pathDir {qApp->applicationDirPath()+"/styles"};
    QString filepath {};

    filepath = pathDir.exists()?pathDir.path():"D:/Programming/GitHub/Qt/ParticlesPhysics/ParticlesPhysicsMain/styles/";
    filepath += styleFiles[style];

    QFile file {filepath};

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

    buttonStyleSelected = load<QString>(docElem,"buttonStyleSelected");
    buttonStyleUnselected = load<QString>(docElem,"buttonStyleUnselected");
    buttonStyleReset = load<QString>(docElem,"buttonStyleReset");

    cPlaneBorder = load<QColor>(docElem,"cPlaneBorder");
    cBackground = load<QColor>(docElem,"cBackground");
    cAxesColor = load<QColor>(docElem,"cAxesColor");
    cInnerFrameColor = load<QColor>(docElem,"cInnerFrameColor");
    cLabelColor = load<QColor>(docElem,"cLabelColor");
    cValue = load<QColor>(docElem,"cValue");
    cButtonLabel = load<QColor>(docElem,"cButtonLabel");
    cUpper = load<QColor>(docElem,"cUpper");
    cLower = load<QColor>(docElem,"cLower");
    cBigCirclePenColor = load<QColor>(docElem,"cBigCirclePenColor");
    cSmallCircleColor = load<QColor>(docElem,"cSmallCircleColor");
    cSmallCircleHookedColor = load<QColor>(docElem,"cSmallCircleHookedColor");

    marginLeft = load<int>(docElem,"marginLeft");
    marginRight = load<int>(docElem,"marginRight");
    marginTop = load<int>(docElem,"marginTop");
    marginBottom = load<int>(docElem,"marginBottom");
    numberOfHorizontalAxes = load<int>(docElem,"numberOfHorizontalAxes");
    numberOfVerticalAxes = load<int>(docElem,"numberOfVerticalAxes");
    planeBorderWidth = load<int>(docElem,"planeBorderWidth");
    buttonWidth = load<int>(docElem,"buttonWidth");
    buttonHeight = load<int>(docElem,"buttonHeight");
    buttonIndent = load<int>(docElem,"buttonIndent");

    isScalableUp = load<bool>(docElem,"isScalableUp");
    isScalableDown = load<bool>(docElem,"isScalableDown");

    file.close();
    return true;
}

template<typename T>
T QBoxStyle::load( const QDomElement &element , const QString &tagName )
{
    T value {};

    if constexpr( std::is_same<QString,T>::value )
    {
        auto findString = element.firstChildElement(tagName).firstChildElement("string");

        if( !findString.isNull() )
        {
          value =  findString.text();
        }
    }
    if constexpr( std::is_same<QColor,T>::value )
    {
        auto findColor = element.firstChildElement(tagName).firstChildElement("color");

        if( !findColor.isNull() )
        {
          QStringList colorsRGB = findColor.text().split( "," );

          if( colorsRGB.size() == 3 )
          {
              value.setRed( colorsRGB[0].toInt() );
              value.setGreen( colorsRGB[1].toInt() );
              value.setBlue( colorsRGB[2].toInt() );
          }
        }
    }
    if constexpr( std::is_same<int,T>::value )
    {
        auto findInt = element.firstChildElement(tagName).firstChildElement("int");

        if( !findInt.isNull() )
        {
            value = findInt.text().toInt();
        }
    }
    if constexpr( std::is_same<bool,T>::value )
    {
        auto findBool = element.firstChildElement(tagName).firstChildElement("bool");

        if( !findBool.isNull() )
        {
            value = findBool.text().toInt()==1?true:false;
        }
    }

    return value;
}
