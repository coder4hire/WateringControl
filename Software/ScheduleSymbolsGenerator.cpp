#include "ScheduleSymbolsGenerator.h"
#include "LiquidCrystal2.h"

extern LiquidCrystal lcd;

byte CScheduleSymbolsGenerator::iconNoPowerRestrictions[7] =
{
    B00100,
    B01000,
    B11100,
    B01000,
    B10000,
    B00000,
    B00000
};

byte CScheduleSymbolsGenerator::emptySpace[7] =
{
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000
};


byte CScheduleSymbolsGenerator::iconsChannelNums[CHANNELS_NUM][7] =
{
    {
        B11111,
        B10011,
        B11011,
        B11011,
        B11011,
        B10001,
        B11111
    },
    {
        B11111,
        B11011,
        B10101,
        B11101,
        B11011,
        B10001,
        B11111
    },
    {
        B11111,
        B10011,
        B11101,
        B11011,
        B11101,
        B10011,
        B11111
    },
    {
        B11111,
        B10101,
        B10101,
        B10001,
        B11101,
        B11101,
        B11111
    }
};

byte CScheduleSymbolsGenerator::iconsWeekDays[12][7] =
{
    //--- ST ---
    {
        B01111,
        B00100,
        B11110,
        B00000,
        B00000,
        B00000,
        B00000
    },
    {
        B00000,
        B00000,
        B00000,
        B00000,
        B11111,
        B00100,
        B00100
    },
    {
        B01111,
        B00100,
        B11110,
        B00000,
        B11111,
        B00100,
        B00100
    },
    //--- MF ---
    {
        B11011,
        B10101,
        B10101,
        B00000,
        B00000,
        B00000,
        B00000
    },
    {
        B00000,
        B00000,
        B00000,
        B00000,
        B11111,
        B11100,
        B10000
    },
    {
        B11011,
        B10101,
        B10101,
        B00000,
        B11111,
        B11100,
        B10000
    },
    //--- TS ---
    {
        B11111,
        B00100,
        B00100,
        B00000,
        B00000,
        B00000,
        B00000
    },
    {
        B00000,
        B00000,
        B00000,
        B00000,
        B01111,
        B00100,
        B11110
    },
    {
        B11111,
        B00100,
        B00100,
        B00000,
        B01111,
        B00100,
        B11110
    },
    //--- W ---
    {
        B10101,
        B10101,
        B01010,
        B00000,
        B00000,
        B00000,
        B00000
    },
    {
        B10101,
        B10101,
        B01010,
        B00000,
        B00000,
        B00000,
        B00000
    },
    {
        B10101,
        B10101,
        B01010,
        B00000,
        B00000,
        B00000,
        B00000
    },
};

CScheduleSymbolsGenerator::CScheduleSymbolsGenerator()
{
    //ctor
}

CScheduleSymbolsGenerator::~CScheduleSymbolsGenerator()
{
    //dtor
}

void CScheduleSymbolsGenerator::Start()
{
    lcd.createChar(SSYM_NO_POWER_RESTRICTION,iconNoPowerRestrictions);
}

void CScheduleSymbolsGenerator::SetChannel(int chanNum)
{
    if(chanNum>0 && chanNum<=CHANNELS_NUM)
    {
        lcd.createChar(SSYM_CHAN,iconsChannelNums[chanNum]);
    }
}

void CScheduleSymbolsGenerator::SetWeekday(byte weekDayMask)
{
    byte iconNums[4]={0,0,0,0};
    for(int i=0;i<7;i++)
    {
        if(weekDayMask & (1<<i))
        {
            iconNums[i%4] |= (1<<(i/4));
        }
    }

    for(int j=0;j<4;j++)
    {
        lcd.createChar(SSYM_ST+j,iconNums[j]>0 ? iconsWeekDays[j*3+iconNums[j]-1] : emptySpace);
    }
}
