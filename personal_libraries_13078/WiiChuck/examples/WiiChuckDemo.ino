/*
 * WiiChuckDemo -- 
 *
 * 2008 Tod E. Kurt, http://thingm.com/
 *
 */

#include &lt;Wire.h&gt;
#include &quot;WiiChuck.h&quot;

int loop_cnt=0;

byte accx,accy,zbut,cbut;
int ledPin = 13;


void setup()
{
    Serial.begin(19200);
    nunchuck_setpowerpins();
    nunchuck_init(); // send the initilization handshake
    
    Serial.print(&quot;WiiChuckDemo ready\n&quot;);
}

void loop()
{
    if( loop_cnt &gt; 100 ) { // every 100 msecs get new data
        loop_cnt = 0;

        nunchuck_get_data();

        accx  = nunchuck_accelx(); // ranges from approx 70 - 182
        accy  = nunchuck_accely(); // ranges from approx 65 - 173
        zbut = nunchuck_zbutton();
        cbut = nunchuck_cbutton(); 
            
        Serial.print(&quot;accx: &quot;); Serial.print((byte)accx,DEC);
        Serial.print(&quot;\taccy: &quot;); Serial.print((byte)accy,DEC);
        Serial.print(&quot;\tzbut: &quot;); Serial.print((byte)zbut,DEC);
        Serial.print(&quot;\tcbut: &quot;); Serial.println((byte)cbut,DEC);
    }
    loop_cnt++;
    delay(1);
}