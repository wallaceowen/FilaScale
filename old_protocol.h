/* protocol.cpp */

#include <Arduino.h>
#include <string.h>

#define MSGLEN 16

static bool report_weight = false;
static int32_t scale_offset = 0;
static double scale_gain = 1.0;
static Averager averager;

// If there's nothing waiting, return 0
// if there's something to read
//    If there's no room in buffer, return -1
//    If there's room, add to buffer.
//       If the char added is a \n, reset offset and
//       return 1, else return 0 (we're still adding
//       bytes to the buffer)
static int get_command(char *buffer, size_t len, size_t &offset)
{
    if (Serial.available())
    {
        if (offset == (len-2))  // leave room for terminator
            return -1;
        else
        {
            int val = Serial.read();
            if (val == '\n')
            {
                buffer[offset] = '\0';
                offset = 0;
                return 1;
            }
            else
                buffer[offset++] = val;
        }
    }
    return 0;
}

// Utility function to tokenize the received message
// so we can extract the parameters easily.
static char *get_tok(char *buffer, size_t &offset)
{
    static char tok[MSGLEN];
    char *tptr = tok;

    while (buffer[offset] && buffer[offset] == ' ')
        ++offset;
    while (buffer[offset] && buffer[offset] != ' ')
        *tptr++ = buffer[offset++];
    *tptr = '\0';

    return tok;
}

// fwd decl because it's invoked by the help function
// whose definition needs to be known by code that iterates
// over the elements of the commands array on behalf
// of show_commands.
static void show_commands();

// Send the calibrated weight message to the interface
static double calibrated_weight(int32_t w)
{
    return (w-scale_offset)*scale_gain;
}

// Send the values to the interface
static void show_weight()
{
    int32_t w = averager.average();
    Serial.print("weight ");
    Serial.println(w);
}

typedef void (*CommandFunc)(char *, size_t &);

struct Command
{
    const char *cmd;
    CommandFunc func;
    const char *desc;
};

// Report the version string
static void ver_func(char *cmd, size_t &offset)
{
    Serial.print("scale version ");
    Serial.println(version);
}

// Handle scale messages "SCALE VALUE", "SCALE ON" and "SCALE OFF"
static void scale_func(char *cmd, size_t &offset)
{
    const char *val = get_tok(cmd, offset);
    if (!strcasecmp(val, "on"))
        report_weight = true;
    if (!strcasecmp(val, "zero"))
        set_scale_zero();
    else if (!strcasecmp(val, "off"))
        report_weight = false;
    else if (!strcasecmp(val, "value"))
        show_weight();
}

// Show the commands
static void help_func(char *cmd, size_t &offset)
{
    show_commands();
}

// Add new commands here
static struct Command commands[] = {
    {"VERSION", ver_func, "query for version"},
    {"SCALE", scale_func, " SCALE ON starts sending scale values;\r\n"
                          "\t\tSCALE OFF stops;\r\n\t\tSCALE value sends it once"},
    {"HELP", help_func, "Shows the commands"},
};
#define NUM_CMDS (sizeof(commands)/sizeof(commands[0]))

static void show_commands()
{
    Serial.println("Commands:");
    for (size_t i = 0; i < NUM_CMDS; ++i)
    {
        Serial.print("\t");
        Serial.print(commands[i].cmd);
        Serial.print(": ");
        Serial.println(commands[i].desc);
    }
    Serial.println("*case is ignored.\r\n");
}

void protocol_loop(void)
{

    size_t cmd_offset = 0;
    char cmd_buffer[MSGLEN];

    for (uint16_t count = 0; true; ++count)
    {
        int result = get_command(cmd_buffer, sizeof(cmd_buffer), cmd_offset);
        if (result == -1)
        {
            Serial.print("bad command "); 
            Serial.println(cmd_buffer); 
            memset(cmd_buffer, 0, sizeof(cmd_buffer));
            cmd_offset = 0;
        }
        else if (result == 1)
        {
            bool found = false;
            for (size_t cnum = 0; cnum < NUM_CMDS; ++cnum)
            {
                size_t offset = 0;
                char *tok = get_tok(cmd_buffer, offset);
                if (!strcasecmp(commands[cnum].cmd, tok))
                {
                    found = true;
                    commands[cnum].func(cmd_buffer, offset);
                    break;
                }
            }

            if (!found)
                Serial.println("Unknown command.");
        }
    }
}

