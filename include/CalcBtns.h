#ifndef CALCBTNS_H
#define CALCBTNS_H

#include <Adafruit_GFX.h>
#include <commonDefs.h>

class CalcButton : public Adafruit_GFX_Button
{
private:
    String label="";
    inputType inType;
    byte digit;

public:
    CalcButton(inputType intype, byte Digit=0)
    {
        if(Digit>9) 
            Digit = 0;
        digit = Digit;
        inType = intype;

        switch (inType)
        {
        case inputType::clear :
            label = 'C';
            break;
        
        case inputType::equal :
            label = '=';
            break;
        
        case inputType::multiply :
            label = '*';
            break;
        
        case inputType::divide :
            label = char(47);
            break;
        
        case inputType::add :
            label = '+';
            break;
        
        case inputType::subtract :
            label = '-';
            break;

        case inputType::digit :
            label = digit;
            //sprintf(label,"%u",digit);
            break;
        }
    }

    inputType getInputType() 
    {
        return inType;
    }

    char* getLabel()
    {
        return (char*)label.c_str();
    }

    byte getDigit()
    {
        return digit;
    }
    
    ~CalcButton();
};

#endif