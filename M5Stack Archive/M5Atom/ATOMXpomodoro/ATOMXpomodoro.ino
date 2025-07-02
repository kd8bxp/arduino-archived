#include "src/M5Atom/M5Atom.h"
#include "LEDBuffer.h"

#define DEBUGx

#ifdef DEBUG
#define DURATION_MS       (25 * 200)  // (debug)5sec
#else
#define DURATION_MS       (25 * 60 * 1000) // 25min
#endif

#define IDLE_INTERVAL_MS     300
#define TIMEOVER_INTERVAL_MS 500
#define COUNT_INTERVAL_MS    100

#define ARRAY_LEN(array) (sizeof(array) / sizeof(array[0]))

// PomoController [START]----------------------------------------------------------------------
typedef enum {PM_ST_IDLE, PM_ST_COUNT, PM_ST_TIMEOVER} PM_ST_N;
class PomoController {
    public:
        PM_ST_N stat(void) {
            return stat_;
        }
        void setStat(PM_ST_N nextStat) {
            const String pm_n2str_[] = {"IDLE", "COUNTING", "TIMEOVER"};
            Serial.print("status: "); Serial.println(pm_n2str_[nextStat]);
            stat_ = nextStat;
        }
        boolean isStarted() {
            return (this->start_ms == 0) ? false : true;
        }
        unsigned long start_ms = 0;
        uint8_t disp_line=0;

    private:
        PM_ST_N stat_ = PM_ST_IDLE;
};
// PomoController [END]----------------------------------------------------------------------

LEDBuffer      m_ledbuf;
PomoController m_pomocon;

#define R_RGB 0xff0000
#define G_RGB 0x00ff00
#define TOMATO_PATTERN 4

uint32_t m_tomato_matrix[TOMATO_PATTERN][LED_HEIGHT*LED_WIDTH] = {
    {
        0,     R_RGB, R_RGB, 0,     0,
        R_RGB, R_RGB, R_RGB, R_RGB, 0,
        R_RGB, R_RGB, R_RGB, 0,     0,
        R_RGB, R_RGB, R_RGB, R_RGB, 0,
        0,     R_RGB, R_RGB, 0,     0
    },
    {
        0,     R_RGB, R_RGB, 0,     0,
        R_RGB, R_RGB, R_RGB, R_RGB, 0,
        R_RGB, R_RGB, R_RGB, G_RGB, 0,
        R_RGB, R_RGB, R_RGB, R_RGB, 0,
        0,     R_RGB, R_RGB, 0,     0
    },
    {
        0,     R_RGB, R_RGB, 0,     0,
        R_RGB, R_RGB, R_RGB, R_RGB, 0,
        R_RGB, R_RGB, R_RGB, G_RGB, G_RGB,
        R_RGB, R_RGB, R_RGB, R_RGB, 0,
        0,     R_RGB, R_RGB, 0,     0
    },
    {
        0,     R_RGB, R_RGB, 0,     0,
        R_RGB, R_RGB, R_RGB, R_RGB, 0,
        R_RGB, R_RGB, R_RGB, G_RGB, 0,
        R_RGB, R_RGB, R_RGB, R_RGB, G_RGB,
        0,     R_RGB, R_RGB, 0,     0
    },
};

void displayTimeover()
{
    m_ledbuf.clear();
    for (int h=0; h<LED_HEIGHT; h++) {
        for (int w=0; w<LED_WIDTH; w++) {
            uint8_t pt = w*LED_WIDTH + h;
            m_ledbuf.set(m_tomato_matrix[m_pomocon.disp_line][pt], pt);
        }
    }

    m_ledbuf.display();
    m_pomocon.disp_line = (m_pomocon.disp_line+1) % TOMATO_PATTERN;
}

void toggleLEDPos(uint8_t pos)
{
    static uint8_t prev_pos_m = 0;
    static uint8_t staycnt_m = 3;
    static bool    isOn_m= true;

    if (prev_pos_m != pos) {
        prev_pos_m = pos;
        isOn_m = true;
        staycnt_m = 3;
        return;
    }

    if (staycnt_m == 0) {
        isOn_m = !isOn_m;
        staycnt_m = 3;
    } else {
        if (!isOn_m) {
            m_ledbuf.set(0x0, 0x0, 0x0, pos);
        }
        staycnt_m--;
    }

}

// setup & loop [START]----------------------------------------------------------------------
void setup()
{
    M5.begin(true, false, true);
    Serial.begin(115200);

    delay(10);
    m_ledbuf.clear();
    m_ledbuf.display();
}

void loop()
{
    M5.update();

    if (m_pomocon.stat() == PM_ST_IDLE) {
        if (M5.Btn.wasPressed()) {
            m_pomocon.setStat(PM_ST_COUNT);
            m_pomocon.start_ms = 0;
        }
        delay(IDLE_INTERVAL_MS);
        return;
    }

    if (m_pomocon.stat() == PM_ST_TIMEOVER) {
        if (M5.Btn.wasPressed()) {
            m_pomocon.setStat(PM_ST_IDLE);
            m_ledbuf.clear();
            m_ledbuf.display();
        } else {
            displayTimeover();
            delay(TIMEOVER_INTERVAL_MS);
        }
        return;
    }

    if (m_pomocon.stat() == PM_ST_COUNT && m_pomocon.isStarted()) {
        if (M5.Btn.wasPressed()) {
            m_pomocon.setStat(PM_ST_IDLE);
            m_ledbuf.clear();
            m_ledbuf.display();
            return;
        }
    }

    // first count
    if (!m_pomocon.isStarted()) {
        m_pomocon.start_ms = millis();
    }

    unsigned long diff_ms;
    diff_ms    = millis() - m_pomocon.start_ms;
    // Serial.print("diff(ms): "); Serial.println(diff_ms);

    uint8_t pos;
    pos = (diff_ms * LED_MAX) / DURATION_MS;
    if (pos >= LED_MAX) {
        m_pomocon.setStat(PM_ST_TIMEOVER);
        m_pomocon.disp_line = 0;
        return;
    }

    // for debug
    {
        static uint8_t prev_pos_m = -1;
        if (pos != prev_pos_m) {
            prev_pos_m = pos;
            Serial.print("pos: "); Serial.println(pos);
        }
    }
    
    m_ledbuf.fill(0, 0, pos);
    m_ledbuf.fill(0xff0000, pos, LED_MAX);
    toggleLEDPos(pos);
    m_ledbuf.display();

    delay(COUNT_INTERVAL_MS);
}
// setup & loop [END]----------------------------------------------------------------------
