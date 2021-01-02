#include <Adafruit_I2CDevice.h>
#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include <TouchScreen.h>
#include <commonDefs.h>
#include <CalcBtns.h>
#include <Calculator.h>

#define MINPRESSURE 200
#define MAXPRESSURE 1000

// ALL Touch panels and wiring is DIFFERENT
// copy-paste results from TouchScreen_Calibr_native.ino
const int XP=8,XM=A2,YP=A3,YM=9; //240x320 ID=0x7575
const int TS_LEFT=930,TS_RT=104,TS_TOP=937,TS_BOT=77;

MCUFRIEND_kbv tft;
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
Calculator calc;

CalcButton* pCalcBtns[4][4];
String strOnScreen="";

int pixel_x, pixel_y;     //Touch_getXY() updates global vars
bool Touch_getXY(void)
{
    TSPoint p = ts.getPoint();
    pinMode(YP, OUTPUT);      //restore shared pins
    pinMode(XM, OUTPUT);
    digitalWrite(YP, HIGH);   //because TFT control pins
    digitalWrite(XM, HIGH);
    bool pressed = (p.z > MINPRESSURE && p.z < MAXPRESSURE);
    if (pressed) {
        pixel_x = map(p.x, TS_LEFT, TS_RT, 0, tft.width()); //.kbv makes sense to me
        pixel_y = map(p.y, TS_TOP, TS_BOT, 0, tft.height());
    }
    return pressed;
}

int nBtnIindex, nBtnJindex;
bool getTouchedBtnIndexes()
{
    if(pixel_y<80)
        return false;

    if (pixel_y<140)
        nBtnIindex = 0;
    else if (pixel_y<200)
        nBtnIindex = 1;
    else if (pixel_y<260)
        nBtnIindex = 2;
    else nBtnIindex = 3;

    if (pixel_x<60)
        nBtnJindex = 0;
    else if (pixel_x<120)
        nBtnJindex = 1;
    else if (pixel_x<180)
        nBtnJindex = 2;
    else nBtnJindex = 3;

    return true;
}

void displayStrOnScreen()
{
    tft.fillRect(0,0,240,80, CYAN);
    tft.setCursor(5,35);
    tft.setTextSize(4);
    tft.setTextColor(BLACK);
    tft.print(strOnScreen);
}

void setup() {
  // put your setup code here, to run once:
    Serial.begin(9600);
    uint16_t ID = tft.readID();
    if (ID == 0xD3D3) 
        ID = 0x9486; // write-only shield

    tft.begin(ID);
    tft.setRotation(0);            //PORTRAIT
    tft.fillScreen(BLACK);

    // Draw Display Rect
    tft.fillRect(0,0,240,80, CYAN);
 
    //Init Buttons
    pCalcBtns[0][0] = new CalcButton(inputType::digit,7);
    pCalcBtns[0][0]->initButton(&tft,30,110,60,60,WHITE, BLACK, WHITE,pCalcBtns[0][0]->getLabel(),2);

    pCalcBtns[0][1] = new CalcButton(inputType::digit,8);
    pCalcBtns[0][1]->initButton(&tft,90,110,60,60,WHITE, BLACK, WHITE,pCalcBtns[0][1]->getLabel(),2);

    pCalcBtns[0][2] = new CalcButton(inputType::digit,9);
    pCalcBtns[0][2]->initButton(&tft,150,110,60,60,WHITE, BLACK, WHITE,pCalcBtns[0][2]->getLabel(),2);

    pCalcBtns[0][3] = new CalcButton(inputType::add);
    pCalcBtns[0][3]->initButton(&tft,210,110,60,60,WHITE, BLUE, WHITE,pCalcBtns[0][3]->getLabel(),2);

    pCalcBtns[1][0] = new CalcButton(inputType::digit,4);
    pCalcBtns[1][0]->initButton(&tft,30,170,60,60,WHITE, BLACK, WHITE,pCalcBtns[1][0]->getLabel(),2);

    pCalcBtns[1][1] = new CalcButton(inputType::digit,5);
    pCalcBtns[1][1]->initButton(&tft,90,170,60,60,WHITE, BLACK, WHITE,pCalcBtns[1][1]->getLabel(),2);

    pCalcBtns[1][2] = new CalcButton(inputType::digit,6);
    pCalcBtns[1][2]->initButton(&tft,150,170,60,60,WHITE, BLACK, WHITE,pCalcBtns[1][2]->getLabel(),2);

    pCalcBtns[1][3] = new CalcButton(inputType::subtract);
    pCalcBtns[1][3]->initButton(&tft,210,170,60,60,WHITE, BLUE, WHITE,pCalcBtns[1][3]->getLabel(),2);

    pCalcBtns[2][0] = new CalcButton(inputType::digit,1);
    pCalcBtns[2][0]->initButton(&tft,30,230,60,60,WHITE, BLACK, WHITE,pCalcBtns[2][0]->getLabel(),2);

    pCalcBtns[2][1] = new CalcButton(inputType::digit,2);
    pCalcBtns[2][1]->initButton(&tft,90,230,60,60,WHITE, BLACK, WHITE,pCalcBtns[2][1]->getLabel(),2);

    pCalcBtns[2][2] = new CalcButton(inputType::digit,3);
    pCalcBtns[2][2]->initButton(&tft,150,230,60,60,WHITE, BLACK, WHITE,pCalcBtns[2][2]->getLabel(),2);

    pCalcBtns[2][3] = new CalcButton(inputType::multiply);
    pCalcBtns[2][3]->initButton(&tft,210,230,60,60,WHITE, BLUE, WHITE,pCalcBtns[2][3]->getLabel(),2);

    pCalcBtns[3][0] = new CalcButton(inputType::clear);
    pCalcBtns[3][0]->initButton(&tft,30,290,60,60,WHITE, RED, WHITE,pCalcBtns[3][0]->getLabel(),2);

    pCalcBtns[3][1] = new CalcButton(inputType::digit,0);
    pCalcBtns[3][1]->initButton(&tft,90,290,60,60,WHITE, BLACK, WHITE,pCalcBtns[3][1]->getLabel(),2);

    pCalcBtns[3][2] = new CalcButton(inputType::equal);
    pCalcBtns[3][2]->initButton(&tft,150,290,60,60,WHITE, MAGENTA, WHITE,pCalcBtns[3][2]->getLabel(),2);

    pCalcBtns[3][3] = new CalcButton(inputType::divide);
    pCalcBtns[3][3]->initButton(&tft,210,290,60,60,WHITE, BLUE, WHITE,pCalcBtns[3][3]->getLabel(),2);

    for (auto &row : pCalcBtns)
        for (auto &pBtn : row)
            pBtn->drawButton();    
}

void loop() {
  // put your main code here, to run repeatedly:
    bool Touched = Touch_getXY();
    if(Touched)
    {
        getTouchedBtnIndexes();
        pCalcBtns[nBtnIindex][nBtnJindex]->drawButton(true);
        inputType inType = pCalcBtns[nBtnIindex][nBtnJindex]->getInputType();
                        
        if(inType!=inputType::clear)
        {
            if(inType==inputType::equal)
            {
                int16_t result = calc.evaluateExpression();
                strOnScreen = result;
                displayStrOnScreen();
                calc.Reset();
                calc.processNewInput(inputType::digit,result);
            } 
            else if(strOnScreen.length()<9)
            {
                if(calc.processNewInput(pCalcBtns[nBtnIindex][nBtnJindex]->getInputType(),pCalcBtns[nBtnIindex][nBtnJindex]->getDigit()))
                {
                    strOnScreen+=pCalcBtns[nBtnIindex][nBtnJindex]->getLabel();
                    displayStrOnScreen();
                }
            }
        }
        else
        {
            tft.fillRect(0,0,240,80, CYAN);
            strOnScreen="";
            calc.Reset();
        }
        
        pCalcBtns[nBtnIindex][nBtnJindex]->drawButton();
    }
}