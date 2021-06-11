// state_view.cpp

#include <stdio.h>

#include "state_view.h"

// moved to display.h
#if 0
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
#endif

#define BUTTONS_Y 120
#define BUTTON_GAP 2

uint16_t button_colors[] = {TFT_OLIVE, TFT_BLUE, TFT_RED };
const char *button_labels[] = {"Scale", "Drying", "STOP" };

StateView::StateView(Display &d, Scale &s, BME280_IF &b) :
    View(d),
    m_display(d),
    m_scale(s),
    m_bme(b),
    m_temp(0.0),
    m_humid(0.0),
    m_weight(0.0),
    m_full_weight(0.0)
{
    // TFT_eSPI &tft = m_display.get_tft();
    // tft.setRotation(ROTATION);

    this->show();

    // Set up to be told of hits to our buttons
}

// Show the static part of the view
void StateView::show()
{
    TFT_eSPI &tft = m_display.get_tft();

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

    uint16_t button_width = tft.width() / NUM_BUTTONS - (BUTTON_GAP*(NUM_BUTTONS-1));

    // Show the buttons
    for (int i = 0; i < NUM_BUTTONS; ++i)
    {
        uint16_t bx = button_width*i;
        if (i)
            bx += BUTTON_GAP;
        Button *b = new Button( Rect(bx, BUTTONS_Y, button_width, tft.height()-BUTTONS_Y),
                button_colors[i],
                button_labels[i]);
        buttons[i] = b;
        b->draw(m_display);
    }

    // Now show the state
    draw_state();
}

// Update the state we render.  Side-effect: maintain changed,
// used to know whether to update the gui from the updated values
bool StateView::update(void)
{
    bool changed = false;
    if (m_scale.get_calibrated() != m_weight)
    {
        m_weight = m_scale.get_calibrated();
        changed = true;
    }
    if (m_bme.temp() != m_temp)
    {
        m_temp = m_bme.temp();
        changed = true;
    }
    if (m_bme.humid() != m_humid)
    {
        m_humid = m_bme.humid();
        changed = true;
    }
    return changed;
}

void StateView::loop()
{
    if (update())
    {
        draw_state();
    }
}

void StateView::touch_callback(Display *d, uint16_t x, uint16_t y)
{
    // Serial.print("StateView got touch callback. x: ");
    // Serial.print(x);
    // Serial.print(", y: ");
    // Serial.println(y);

    // Serial.println("checking buttons");
    for (int i = 0; i < NUM_BUTTONS; ++i)
    {
        if (buttons[i]->pressed(x, y))
        {
            Serial.print("Button ");
            Serial.print(buttons[i]->label);
            Serial.println(" pressed");
        }
    }

    // d->calibrate();
}

// void StateView::touch_callback_func(Display *d, void *user, uint16_t x, uint16_t y)
// {
    // StateView *state_view = reinterpret_cast<*>(user);
    // state_view->touch_callback(d, x, y);
// }

// Update the state on the screen with the latest values we hold.
// Only called when the state has changed.
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

// void StateView::render()
// {
    // // TFT_eSPI &tft = m_display.get_tft();
    // draw_state();
// }

