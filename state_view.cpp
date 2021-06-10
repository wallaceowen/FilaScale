// state_view.cpp

#include <stdio.h>

#include "state_view.h"

#define LANDSCAPE_ORIENTATION

#ifdef LANDSCAPE_ORIENTATION
#define ROTATION 3
#define WIDTH 320
#define HEIGHT 240
#define VALUES_X 160
#define STATE_FONT 4
#else
#define ROTATION 2
#define WIDTH 240
#define HEIGHT 320
#define VALUES_X 100
#define STATE_FONT 4
#endif


#define BORDER_THICKNESS 5
#define MARGIN BORDER_THICKNESS+2

/* State view layout:
 *-------------------------------------+
 *          F I L A S C A L E          *
 *                                     *
 * TEMP            90f 34c             *
 * Humidity        10 %                *
 * Filament       250 g                *
 *                                     *
 *-------------------------------------+
 */


StateView::StateView(Display &d) :
    m_display(d),
    m_temp(0.0),
    m_humid(0.0),
    m_weight(0.0),
    m_full_weight(0.0)
{
    TFT_eSPI &tft = m_display.get_tft();
    tft.setRotation(ROTATION);

    CallbackData cd;
    cd.cb = touch_callback_func;
    cd.user = this;
    m_display.add_callback(cd);


    // Draw our box
    tft.fillRect(0, 0, WIDTH-1, HEIGHT-1, TFT_WHITE);
    tft.fillRect(BORDER_THICKNESS, BORDER_THICKNESS, WIDTH-(BORDER_THICKNESS*2), HEIGHT-(BORDER_THICKNESS*2), TFT_BLACK);

    // Show the view name and our field names
    tft.setTextColor(TFT_GREEN);
    int line = 0;
    tft.drawString("STATE VIEW", MARGIN, tft.fontHeight(STATE_FONT)*line+MARGIN, STATE_FONT);
    ++line;
    tft.setTextColor(TFT_YELLOW);
    tft.drawString("TEMP", MARGIN, tft.fontHeight(STATE_FONT)*line+MARGIN, STATE_FONT);
    ++line;
    tft.drawString("HUMIDITY", MARGIN, tft.fontHeight(STATE_FONT)*line+MARGIN, STATE_FONT);
    ++line;
    tft.drawString("WEIGHT", MARGIN, tft.fontHeight(STATE_FONT)*line+MARGIN, STATE_FONT);

    // Show the buttons

    // Set up to be told of hits to our buttons

}

bool StateView::update_state(Scale &scale, BME280_IF &bme)
{
    bool changed = false;
    if (scale.get_calibrated() != m_weight)
    {
        m_weight = scale.get_calibrated();
        changed = true;
    }
    if (bme.temp() != m_temp)
    {
        m_temp = bme.temp();
        changed = true;
    }
    if (bme.humid() != m_humid)
    {
        m_humid = bme.humid();
        changed = true;
    }
    return changed;
}

void StateView::touch_callback(Display *d, uint16_t x, uint16_t y)
{
    Serial.println("StateView got touch callback");
}

void StateView::touch_callback_func(Display *d, void *user, uint16_t x, uint16_t y)
{
    StateView *state_view = reinterpret_cast<StateView*>(user);
    state_view->touch_callback(d, x, y);
}

void StateView::draw_state()
{
    TFT_eSPI &tft = m_display.get_tft();
    char value_buffer[38];
    tft.setTextColor(TFT_DARKGREEN);
    int line = 1;

    sprintf(value_buffer, "%4.2f", m_temp);
    int width = tft.textWidth(value_buffer, STATE_FONT);
    tft.fillRect(
            VALUES_X,
            tft.fontHeight(STATE_FONT)*line+BORDER_THICKNESS,
            width,
            tft.fontHeight(STATE_FONT)+BORDER_THICKNESS,
            TFT_WHITE);
    // tft.drawFloat(m_temp, 2, VALUES_X, tft.fontHeight(STATE_FONT)*line+BORDER_THICKNESS, STATE_FONT);
    tft.drawString(value_buffer, VALUES_X, tft.fontHeight(STATE_FONT)*line+BORDER_THICKNESS, STATE_FONT);
    ++line;

    sprintf(value_buffer, "%4.2f", m_humid);
    width = tft.textWidth(value_buffer, STATE_FONT);
    tft.fillRect(
            VALUES_X,
            tft.fontHeight(STATE_FONT)*line+BORDER_THICKNESS,
            width,
            tft.fontHeight(STATE_FONT)+BORDER_THICKNESS,
            TFT_WHITE);
    // tft.drawFloat(m_humid, 2, VALUES_X, tft.fontHeight(STATE_FONT)*line+BORDER_THICKNESS, STATE_FONT);
    tft.drawString(value_buffer, VALUES_X, tft.fontHeight(STATE_FONT)*line+BORDER_THICKNESS, STATE_FONT);
    ++line;

    sprintf(value_buffer, "%4.3f", m_weight);
    width = tft.textWidth(value_buffer, STATE_FONT);
    tft.fillRect(
            VALUES_X,
            tft.fontHeight(STATE_FONT)*line+BORDER_THICKNESS,
            width,
            tft.fontHeight(STATE_FONT)+BORDER_THICKNESS,
            TFT_WHITE);
    // tft.drawFloat(m_weight, 3, VALUES_X, tft.fontHeight(STATE_FONT)*line+BORDER_THICKNESS, STATE_FONT);
    tft.drawString(value_buffer, VALUES_X, tft.fontHeight(STATE_FONT)*line+BORDER_THICKNESS, STATE_FONT);
    ++line;
}


void StateView::render()
{
    TFT_eSPI &tft = m_display.get_tft();

    draw_state();
}

