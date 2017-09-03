/**
 * BadgerHack Hello World
 * Shawn Hymel @ SparkFun Electronics
 * September 23, 2015
 * 
 * Scrolls "Hello world" across the Badger's LED array.
 * 
 * License: http://opensource.org/licenses/MIT
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <SparkFun_LED_8x7.h>
#include <Chaplex.h>

// Global variables
static byte led_pins[] = {2, 3, 4, 5, 6, 7, 8, 9}; // Pins for LEDs

void setup() {

  // Initialize LED array
  Plex.init(led_pins);

  // Clear display
  Plex.clear();
  Plex.display();
}

void loop() {

  // Scroll text 1 time
  Plex.scrollText("Hello world", 1);

  // Wait 7 seconds to let the text finish scrolling
  delay(7000);

  // Stop scrolling the text
  Plex.stopScrolling();
  delay(2000);
}

