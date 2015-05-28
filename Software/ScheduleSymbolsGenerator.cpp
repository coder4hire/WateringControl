#include "ScheduleSymbolsGenerator.h"
#include "LiquidCrystal2.h"

extern LiquidCrystal lcd;

const byte CScheduleSymbolsGenerator::iconNoPowerRestrictions[7] =
{
    B00100,
    B01000,
    B11100,
    B01000,
    B10000,
    B00000,
    B00000
};

const byte CScheduleSymbolsGenerator::emptySpaces[2][7] =
{
    {
        B00000,
        B00100,
        B00000,
        B00000,
        B00000,
        B00100,
        B00000
    },
    {
        B00000,
        B00100,
        B00000,
        B00000,
        B00000,
        B00000,
        B00000
    }
};


const byte CScheduleSymbolsGenerator::iconsChannelNums[CHANNELS_NUM][7] =
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

const byte CScheduleSymbolsGenerator::iconsWeekDays[12][7] =
{
    //--- S ---
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
        B00100,
        B00000,
        B00000,
        B00000,
        B00000,
        B00000
    },
    {
        B01111,
        B00100,
        B11110,
        B00000,
        B00000,
        B00000,
        B00000
    },
    //--- MT ---
    {
        B11011,
        B10101,
        B10101,
        B00000,
        B00000,
        B00100,
        B00000
    },
    {
        B00000,
        B00100,
        B00000,
        B00000,
        B11111,
        B00100,
        B00100
    },
    {
        B11011,
        B10101,
        B10101,
        B00000,
        B11111,
        B00100,
        B00100
    },
    //--- TF ---
    {
        B11111,
        B00100,
        B00100,
        B00000,
        B00000,
        B00100,
        B00000
    },
    {
        B00000,
        B00100,
        B00000,
        B00000,
        B11111,
        B11100,
        B10000
    },
    {
        B11111,
        B00100,
        B00100,
        B00000,
        B11111,
        B11100,
        B10000
    },
    //--- WS ---
    {
        B10101,
        B10101,
        B01010,
        B00000,
        B00000,
        B00100,
        B00000
    },
    {
        B00000,
        B00100,
        B00000,
        B00000,
        B01111,
        B00100,
        B11110
    },
    {
        B10101,
        B10101,
        B01010,
        B00000,
        B01111,
        B00100,
        B11110
    },
};

CScheduleSymbolsGenerator::CScheduleSymbolsGenerator()
{
    //ctor
    memset(currentWeekdayIcons,255,sizeof(currentWeekdayIcons));
    currentChannel=255;
}

CScheduleSymbolsGenerator::~CScheduleSymbolsGenerator()
{
    //dtor
}

void CScheduleSymbolsGenerator::Start()
{
    lcd.createChar(SSYM_NO_POWER_RESTRICTION,(uint8_t*)iconNoPowerRestrictions);
    memset(currentWeekdayIcons,255,sizeof(currentWeekdayIcons));
    currentChannel=255;
}

void CScheduleSymbolsGenerator::SetChannel(int chanNum)
{
    if(chanNum>0 && chanNum<=CHANNELS_NUM && chanNum!=currentChannel)
    {
        lcd.createChar(SSYM_CHAN,(uint8_t*)iconsChannelNums[chanNum-1]);
        currentChannel=chanNum;
    }
}

void CScheduleSymbolsGenerator::SetWeekday(byte weekDayMask)
{
    byte iconNums[4]={0,0,0,0};
    for(int i=0;i<7;i++)
    {
        if(weekDayMask & (1<<i))
        {
            iconNums[i%4+i/4] |= (1<<(i/4));
        }
    }

    for(int j=0;j<4;j++)
    {
        if(iconNums[j]!=currentWeekdayIcons[j])
        {
            lcd.createChar(SSYM_S+j,iconNums[j]>0 ? (uint8_t*)iconsWeekDays[j*3+iconNums[j]-1] :
                           (uint8_t*)emptySpaces[j==0?1:0]);
            currentWeekdayIcons[j]=iconNums[j];
        }
    }
}
