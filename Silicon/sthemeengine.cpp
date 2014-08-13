#include "sthemeengine.h"

SThemeEngine::SThemeEngine( perConf *conf )
{
    prc = conf;
}

void SThemeEngine::set(const QString & themeStr)
{
    this->theme=themeStr;
}

QString SThemeEngine::translate(const QString & theme)
{
    QString result = theme;

    int pointer=-1;

    for(int i=0;i<result.size()-2;i++)
    {
        if(result[i]=='$' && result[i+1]=='(' && pointer==-1)
            pointer=i+2;

        if(pointer!=-1 && result[i]==')')
        {
            QString replaceStr;
            if( result[pointer] == ':' )
            {
                replaceStr = defaults.Personal_Theme_Directory_Path + '/'
                           + prc->readOption(perConf::CURRENT_THEME) + '/'
                           + result.mid( pointer+1 , i-pointer-1 );
                if(!QFile(replaceStr).exists())
                {
                    replaceStr = defaults.Share_Theme_Directory_Path + '/'
                               + prc->readOption(perConf::CURRENT_THEME) + '/'
                               + result.mid( pointer+1 , i-pointer-1 );
                    if(!QFile(replaceStr).exists())
                    {
                        std::cout<<"QString SThemeEngine::translate(const QString & theme) : "
                                 <<replaceStr.toStdString()<<" No such file"<<std::endl;
                        return QString();
                    }
                }
            }
            else
            {
                SColor color;
                    color.setColor( this->readColor(result.mid(pointer-2,i-pointer+3)) );
                if(result[i+1]=='+' || result[i+1]=='-'|| result[i+1]=='*'|| result[i+1]=='/'|| result[i+1]=='~' || result[i+1]=='%')
                {
                    QString operate;
                        operate+=result[i+1];

                    for(int j=i+2;j<result.size()-2;j++)
                    {
                        if(result[j].isNumber() || result[j]=='.')
                            operate+=result[j];
                        else
                            break;
                    }

                    result.remove(i+1,operate.size());
                    color = doColorOperate( color,operate );
                    replaceStr = QString("rgba(") + QString::number(color.red()  ) + ","
                                                  + QString::number(color.green()) + ","
                                                  + QString::number(color.blue() ) + ","
                                                  + QString::number(color.alpha()) + ")";
                }
                else
                    replaceStr = QString("rgba(") + QString::number(color.red()  ) + ","
                                                  + QString::number(color.green()) + ","
                                                  + QString::number(color.blue() ) + ","
                                                  + QString::number(color.alpha()) + ")";
            }


            result.replace(pointer-2,i-pointer+3, replaceStr);
            i=pointer;
            pointer=-1;
        }
    }

    return result;
}

QColor SThemeEngine::readColor(const QString & str)
{
    if(str[0]!='$' || str[1]!='(' || str[str.size()-1]!=')')
        return QColor();

    if( str == "$(BASE)" )
        return parent.palette().color(QPalette::Base);
    if( str == "$(BACKGROUND)" )
        return parent.palette().color(QPalette::Background);
    if( str == "$(WINDOW)" )
        return parent.palette().color(QPalette::Window);
    if( str == "$(HIGHLIGHT)" )
        return parent.palette().color(QPalette::Highlight);
    if( str == "$(HIGHLIGHTED_TEXT)" )
        return parent.palette().color(QPalette::HighlightedText);
    if( str == "$(BUTTON)" )
        return parent.palette().color(QPalette::Button);
    if( str == "$(BUTTON_TEXT)" )
        return parent.palette().color(QPalette::ButtonText);
    if( str == "$(TEXT)" )
        return parent.palette().color(QPalette::Text);
    if( str == "$(TOOLTIP)" )
        return parent.palette().color(QPalette::ToolTipBase);
    if( str == "$(TOOLTIP_TEXT)" )
        return parent.palette().color(QPalette::ToolTipText);

    return QColor();
}

SColor SThemeEngine::doColorOperate(SColor color,QString operate)
{
    bool OK;
    double num=operate.mid(1,operate.size()).toDouble(&OK);

    switch(operate[0].toLatin1())
    {
        case '+':
            color=color+num;
            break;
        case '-':
            color=color-num;
            break;
        case '*':
            color=color*num;
            break;
        case '/':
            color=color/num;
            break;
        case '%':
            color.setAlpha(num);
            break;
        case '~':
            color.invert();
            break;
    }

    return color;
}
