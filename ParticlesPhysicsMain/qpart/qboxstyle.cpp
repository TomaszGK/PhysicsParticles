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

    QDir pathDir {qApp->applicationDirPath()+"/styles/"};
    QString filepath {};

    filepath = pathDir.exists()?pathDir.path()+"/":"D:/Programming/GitHub/Qt/ParticlesPhysics/ParticlesPhysicsMain/styles/";
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

    for( auto &[name,sheet] : sheets ) load(docElem,sheet.tag,sheet.value);

    for( auto &[name,color] : colors ) load(docElem,color.tag,color.value);

    for( auto &[name,value] : values ) load(docElem,name.toStdString(),value);

    for( auto &[name,logic] : logics ) load(docElem,name.toStdString(),logic);


    file.close();
    return true;
}

template<typename T>
void QBoxStyle::load( const QDomElement &element , const std::string &tagName , T& value )
{
    if constexpr( std::is_same<QString,T>::value )
    {
        auto findString = element.firstChildElement(QString::fromStdString(tagName)).firstChildElement("string");

        if( !findString.isNull() )
        {
            value =  findString.text();
        }
    }
    if constexpr( std::is_same<QColor,T>::value )
    {
        auto findColor = element.firstChildElement(QString::fromStdString(tagName)).firstChildElement("color");

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
        auto findInt = element.firstChildElement(QString::fromStdString(tagName)).firstChildElement("int");

        if( !findInt.isNull() )
        {
            value = findInt.text().toInt();
        }
    }
    if constexpr( std::is_same<bool,T>::value )
    {
        auto findBool = element.firstChildElement(QString::fromStdString(tagName)).firstChildElement("bool");

        if( !findBool.isNull() )
        {
            value = findBool.text()=="true"?true:false;
        }
    }    
}
